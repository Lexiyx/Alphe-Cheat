#include "Backtrack.h"
#include "..\..\SDK\IVModelInfo.h"
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\IClientState.h"
#include "..\..\SDK\IClientMode.h"
#include "..\..\SDK\IVModelInfo.h"
#include "..\..\Utils\Math.h"
#include "..\..\SDK\ICvar.h"
#include "..\..\SDK\CModelRender.h"

std::deque<Backtrack::Record> Backtrack::records[65];
std::deque<Backtrack::IncomingSequence>Backtrack::sequences;
Backtrack::Cvars Backtrack::cvars;
float serverTime() noexcept
{
	static int tick;
	static CUserCmd* lastCmd;

	if (g::pCmd) {
		if (g::pLocalEntity)
			tick = g::pLocalEntity->GetTickBase();
		else
			tick++;
		lastCmd = g::pCmd;
	}
	return tick * g_pGlobalVars->intervalPerTick;
}
auto getExtraTicks() noexcept
{
	auto network = g_pEngine->GetNetChannelInfo();
	if (!network)
		return 0.f;

	return std::clamp(network->GetLatency(1) - network->GetLatency(0), 0.f, 0.2f);
}

float Backtrack::getLerp()
{
	auto ratio = std::clamp(Backtrack::cvars.interpRatio->GetFloat(), Backtrack::cvars.minInterpRatio->GetFloat(), Backtrack::cvars.maxInterpRatio->GetFloat());

	return max(Backtrack::cvars.interp->GetFloat(), (ratio / ((Backtrack::cvars.maxUpdateRate) ? Backtrack::cvars.maxUpdateRate->GetFloat() : Backtrack::cvars.updateRate->GetFloat())));
}

bool Backtrack::valid(float simtime)
{
	auto network = g_pEngine->GetNetChannelInfo();
	if (!network)
		return false;

	auto deadTime = static_cast<int>(serverTime() - Backtrack::cvars.maxUnlag->GetFloat());
	if (simtime < deadTime)
		return false;

	auto delta = std::clamp(network->GetLatency(0) + network->GetLatency(1) + getLerp(), 0.f, Backtrack::cvars.maxUnlag->GetFloat()) - (serverTime() - simtime);
	return std::fabsf(delta) <= 0.2f;
}
void Backtrack::update() noexcept
{
	if (!g_Menu.Config.PosBacktrack || !g::pLocalEntity || !g::pLocalEntity->IsAlive()) {
		for (auto& record : records)
			record.clear();

		return;
	}


	for (int i = 1; i <= g_pEngine->GetMaxClients(); i++) {
		auto entity = g_pEntityList->GetClientEntity(i);
		if (!entity || entity == g::pLocalEntity || entity->IsDormant() || !entity->IsAlive()) {
			records[i].clear();
			continue;
		}

		if (!records[i].empty() && (records[i].front().simulationTime == entity->GetSimulationTime()))
			continue;

		Record record{ };
		if (const model_t* mod = entity->GetModel(); mod)
			record.hdr = g_pModelInfo->GetStudiomodel(mod);
		record.head = entity->GetBonePosition(8);
		record.origin = entity->GetAbsOrigin();
		record.simulationTime = entity->GetSimulationTime();
		record.mins = entity->GetCollideable()->OBBMins();
		record.max = entity->GetCollideable()->OBBMaxs();
		entity->SetupBones(record.matrix, 256, 0x7FF00, g_pGlobalVars->curtime);
		records[i].push_front(record);

		while (records[i].size() > 3 && records[i].size() > static_cast<size_t>(timeToTicks(static_cast<float>(200 /*BackTrackTimeLimit*/) / 1000.f + getExtraTicks())))
			records[i].pop_back();

		if (auto invalid = std::find_if(std::cbegin(records[i]), std::cend(records[i]), [](const Record& rec) { return !valid(rec.simulationTime); }); invalid != std::cend(records[i]))
			records[i].erase(invalid, std::cend(records[i]));
	}

}
constexpr auto radiansToDegrees = [](float radians) constexpr noexcept { return radians * 180.0f / static_cast<float>(M_PI); };
Vector calculateRelativeAngle(const Vector& source, const Vector& destination, const Vector& viewAngles) noexcept
{
	Vector delta = destination - source;
	Vector angles(radiansToDegrees(atan2f(-delta.z, std::hypotf(delta.x, delta.y))) - viewAngles.x, radiansToDegrees(atan2f(delta.y, delta.x)) - viewAngles.y, radiansToDegrees(atan2f(delta.y, delta.x)) - viewAngles.z);
	angles.Normalize();
	return angles;
}
void Backtrack::run(CUserCmd* cmd) noexcept
{
	if (!g_Menu.Config.PosBacktrack)
		return;

	if (g::pCmd->buttons & IN_ATTACK)
	{

		if (!g::pLocalEntity)
			return;

		auto localPlayerEyePosition = g::pLocalEntity->GetEyePosition();

		auto bestFov{ 255.f };
		C_BaseEntity* bestTarget{ };
		int bestTargetIndex{ };
		Vector bestTargetHead{ };
		int bestRecord{ };

		const auto aimPunch = g::pLocalEntity->GetAimPunchAngle();

		for (int i = 1; i <= g_pEngine->GetMaxClients(); i++) {
			auto entity = g_pEntityList->GetClientEntity(i);
			if (!entity || entity == g::pLocalEntity || entity->IsDormant() || !entity->IsAlive())
				continue;

			auto head = entity->GetBonePosition(8);

			auto angle = calculateRelativeAngle(localPlayerEyePosition, head, cmd->viewangles + aimPunch);
			auto fov = std::hypotf(angle.x, angle.y);
			if (fov < bestFov) {
				bestFov = fov;
				bestTarget = entity;
				bestTargetIndex = i;
				bestTargetHead = head;
			}
		}

		if (bestTarget) {
			if (records[bestTargetIndex].size() <= 3)
				return;

			bestFov = 255.f;

			for (size_t i = 0; i < records[bestTargetIndex].size(); i++) {
				auto& record = records[bestTargetIndex][i];
				if (!valid(record.simulationTime))
					continue;

				auto angle = calculateRelativeAngle(localPlayerEyePosition, record.head, cmd->viewangles + aimPunch);
				auto fov = std::hypotf(angle.x, angle.y);
				if (fov < bestFov) {
					bestFov = fov;
					bestRecord = i;
				}
			}
		}

		if (bestRecord) {
			auto record = records[bestTargetIndex][bestRecord];
			cmd->tick_count = timeToTicks(record.simulationTime + getLerp());
		}
	}
}

void Backtrack::AddLatencyToNetwork(NetChannel* network, float latency) noexcept
{
	for (auto& sequence : sequences)
	{
		if (serverTime() - sequence.servertime >= latency)
		{
			network->m_nInReliableState = sequence.inreliablestate;
			network->m_nInSequenceNr = sequence.sequencenr;
			break;
		}
	}
}

void Backtrack::UpdateIncomingSequences() noexcept
{
	static int lastIncomingSequenceNumber = 0;

	if (!g_Menu.Config.Extbt_value)
		return;

	if (!g::pLocalEntity)
		return;

	auto network = g_pClientState->m_net_channel;
	if (!network)
		return;

	if (network->m_nInSequenceNr != lastIncomingSequenceNumber)
	{
		lastIncomingSequenceNumber = network->m_nInSequenceNr;

		IncomingSequence sequence{ };
		sequence.inreliablestate = network->m_nInReliableState;
		sequence.sequencenr = network->m_nInSequenceNr;
		sequence.servertime = serverTime();
		sequences.push_front(sequence);
	}

	while (sequences.size() > 2048)
		sequences.pop_back();
}

int Backtrack::timeToTicks(float time) noexcept
{
	return static_cast<int>(0.5f + time / g_pGlobalVars->intervalPerTick);
}
