#include "Aimbot.h"
#include "Autowall.h"
#include "LagComp.h"
#include "..\AntiAim\AntiAim.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\SDK\ICvar.h"
#include "..\..\Utils\Math.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\Menu\Menu.h"
#include "..\..\SDK\ISurface.h"
#include "..\..\Utils\Interfaces.h"
#include "..\..\SDK\CPrediction.h"
#include "../../Utils/xor.h"
#include "../../SDK/ClientClass.h"
#include "../../SDK/IVModelInfo.h"
#include "..\Backtrack\Backtrack.h"

Aimbot g_Aimbot;
int bestHitboxint = 0;
static C_BaseEntity* OldTarget = nullptr;
void Aimbot::Autostop()
{
	if (!g_Menu.Config.Autostop)
		return;

	Vector Velocity = g::pLocalEntity->GetVelocity();

	if (Velocity.Length2D() == 0)
		return;

	static float Speed = 450.f;

	Vector Direction;
	Vector RealView;
	g_Math.VectorAngles(Velocity, Direction);
	g_pEngine->GetViewAngles(RealView);
	Direction.y = RealView.y - Direction.y;

	Vector Forward;
	g_Math.AngleVectors(Direction, &Forward);
	Vector NegativeDirection = Forward * -Speed;

	g::pCmd->forwardmove = NegativeDirection.x;
	g::pCmd->sidemove = NegativeDirection.y;
}

bool Aimbot::HitChance(C_BaseEntity* pEnt, C_BaseCombatWeapon* pWeapon, Vector Angle, Vector Point, int chance)
{
	if (chance == 0 || g_Menu.Config.Hitchance == 0)
		return true;

	if (Backtrack[pEnt->EntIndex()] || ShotBacktrack[pEnt->EntIndex()]) // doing this bec im lazy
	{
		float Velocity = g::pLocalEntity->GetVelocity().Length();

		if (Velocity <= (g::pLocalEntity->GetActiveWeapon()->GetCSWpnData()->fMaxSpeedAlt * .34f))
			Velocity = 0.0f;

		float SpreadCone = pWeapon->GetAccuracyPenalty() * 256.0f / M_PI + pWeapon->GetCSWpnData()->fMaxSpeed * Velocity / 3000.0f; // kmeth https://github.com/DankPaster/kmethdude
		float a = (Point - g::pLocalEntity->GetEyePosition()).Length();
		float b = sqrt(tan(SpreadCone * M_PI / 180.0f) * a);
		if (2.2f > b) return true;
		return (chance <= ((2.2f / fmax(b, 2.2f)) * 100.0f));
	}

	float Seeds = (g_Menu.Config.Hitchance == 1) ? 356.f : 256.f;

	Angle -= (g::pLocalEntity->GetAimPunchAngle() * g_pCvar->FindVar(SteamStrings("weapon_recoil_scale"))->GetFloat());

	Vector forward, right, up;

	g_Math.AngleVectors(Angle, &forward, &right, &up);

	int Hits = 0, neededHits = (Seeds * (chance / 100.f));

	float weapSpread = pWeapon->GetSpread(), weapInaccuracy = pWeapon->GetInaccuracy();

	for (int i = 0; i < Seeds; i++)
	{
		float Inaccuracy = g_Math.RandomFloat(0.f, 1.f) * weapInaccuracy;
		float Spread = g_Math.RandomFloat(0.f, 1.f) * weapSpread;

		Vector spreadView((cos(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Inaccuracy) + (cos(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Spread), (sin(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Inaccuracy) + (sin(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Spread), 0), direction;
		direction = Vector(forward.x + (spreadView.x * right.x) + (spreadView.y * up.x), forward.y + (spreadView.x * right.y) + (spreadView.y * up.y), forward.z + (spreadView.x * right.z) + (spreadView.y * up.z)).Normalize();

		Vector viewanglesSpread, viewForward;

		g_Math.VectorAngles(direction, up, viewanglesSpread);
		g_Math.NormalizeAngles(viewanglesSpread);

		g_Math.AngleVectors(viewanglesSpread, &viewForward);
		viewForward.NormalizeInPlace();

		viewForward = g::pLocalEntity->GetEyePosition() + (viewForward * pWeapon->GetCSWpnData()->fRange);

		C_Trace Trace;

		g_pTrace->ClipRayToEntity(C_Ray(g::pLocalEntity->GetEyePosition(), viewForward), mask_shot | contents_grate, pEnt, &Trace);

		if (Trace.m_pEnt == pEnt)
			Hits++;

		if (((Hits / Seeds) * 100.f) >= chance)
			return true;

		if ((Seeds - i + Hits) < neededHits)
			return false;
	}

	return false;
}

bool ShouldBaim(C_BaseEntity* pEnt)
{
	static float oldSimtime[65];
	static float storedSimtime[65];

	static float ShotTime[65];
	static float NextShotTime[65];
	static bool BaimShot[65];

	if (storedSimtime[pEnt->EntIndex()] != pEnt->GetSimulationTime())
	{
		oldSimtime[pEnt->EntIndex()] = storedSimtime[pEnt->EntIndex()];
		storedSimtime[pEnt->EntIndex()] = pEnt->GetSimulationTime();
	}

	float simDelta = storedSimtime[pEnt->EntIndex()] - oldSimtime[pEnt->EntIndex()];

	bool Shot = false;

	if (pEnt->GetActiveWeapon() && !pEnt->IsKnifeorNade())
	{
		if (ShotTime[pEnt->EntIndex()] != pEnt->GetActiveWeapon()->GetLastShotTime())
		{
			Shot = true;
			BaimShot[pEnt->EntIndex()] = false;
			ShotTime[pEnt->EntIndex()] = pEnt->GetActiveWeapon()->GetLastShotTime();
		}
		else
			Shot = false;
	}
	else
	{
		Shot = false;
		ShotTime[pEnt->EntIndex()] = 0.f;
	}

	if (Shot)
	{
		NextShotTime[pEnt->EntIndex()] = pEnt->GetSimulationTime() + pEnt->FireRate();

		if (simDelta >= pEnt->FireRate())
			BaimShot[pEnt->EntIndex()] = true;
	}

	if (BaimShot[pEnt->EntIndex()])
	{
		if (pEnt->GetSimulationTime() >= NextShotTime[pEnt->EntIndex()])
			BaimShot[pEnt->EntIndex()] = false;
	}

	if (g_Menu.Config.BaimPitch && BaimShot[pEnt->EntIndex()] && !(pEnt->GetFlags() & FL_ONGROUND))
		return true;

	if (g_Menu.Config.BaimInAir && !(pEnt->GetFlags() & FL_ONGROUND))
		return true;

	return false;
}


void AutoRevolver()
{
	if (!g_Menu.Config.AutoRevolver)
		return;

	C_BaseCombatWeapon* weapon = g::pLocalEntity->GetActiveWeapon();
	if (!weapon || weapon->GetItemDefinitionIndex() != ItemDefinitionIndex::WEAPON_REVOLVER)
		return;

	static float delay = 0.f;
	if (delay < 0.15f)//0.20f magical but dumps ground
	{
		delay += g_pGlobalVars->intervalPerTick;
		g::pCmd->buttons |= IN_ATTACK;
	}
	else
		delay = 0.f;
}
int global_hitcahnce;
int global_head;
int global_body;
int global_hitscan;
int min_dmg;
int wal_dmg;
void AutomaticWeapons()
{
	C_BaseCombatWeapon* pWeapon = g::pLocalEntity->GetActiveWeapon();
	if (!pWeapon)
		return;

	if (pWeapon->GetAmmo() == 0)
		return;

	static bool WasFiring = false;
	WeaponInfo_t* WeaponInfo = pWeapon->GetCSWpnData();
	if (!WeaponInfo->bFullAuto)
	{
		if (g::pCmd->buttons & IN_ATTACK)
		{
			if (WasFiring)
			{
				g::pCmd->buttons &= ~IN_ATTACK;
			}
		}
		WasFiring = g::pCmd->buttons & IN_ATTACK ? true : false;
	}
}
float Aimbot::accepted_inaccuracy(C_BaseCombatWeapon* weapon)
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player) return 0;

	if (!weapon) return 0;
	if (weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_TASER) return 0;

	float inaccuracy = weapon->GetInaccuracy();
	if (inaccuracy == 0) inaccuracy = 0.0000001;
	inaccuracy = 1 / inaccuracy;
	return inaccuracy;
}
void Aimbot::RemoveBadRecords(int Idx, std::deque<LagRecord>& records)
{
	auto& m_LagRecords = records; // Should use rbegin but can't erase
	for (auto lag_record = m_LagRecords.begin(); lag_record != m_LagRecords.end(); lag_record++)
	{
		if (!IsTickValid(TIME_TO_TICKS(lag_record->m_flSimulationTime)))
		{
			m_LagRecords.erase(lag_record);
			if (!m_LagRecords.empty())
				lag_record = m_LagRecords.begin();
			else break;
		}
	}
}
bool IsPlayerValid(C_BaseEntity* player)
{
	if (!player)
		return false;

	if (!player->IsPlayer())
		return false;

	if (player == g::pLocalEntity)
		return false;

	if (player->IsDormant())
		return false;

	if (!player->IsAlive())
		return false;

	if (player->GetTeam() == g::pLocalEntity->GetTeam())
		return false;

	if (player->IsImmune())
		return false;

	return true;
}
int Aimbot::GetPriorityLevel(C_BaseEntity* player, LagRecord* lag_record)
{
	int priority = 0;

	if (lag_record->m_flPrevLowerBodyYaw != player->GetLowerBodyYaw())
	{
		lag_record->m_angAngles.y = player->GetLowerBodyYaw();
		priority = 3;
	}

	if ((player->m_flPoseParameter()[1] > (0.85f) && lag_record->m_arrflPrevPoseParameters[1] <= (0.85f)) || (player->m_flPoseParameter()[1] <= (0.85f) && lag_record->m_arrflPrevPoseParameters[1] > (0.85f)))
		priority = 1;

	lag_record->m_flPrevLowerBodyYaw = player->GetLowerBodyYaw();
	lag_record->m_arrflPrevPoseParameters = player->m_flPoseParameter();

	return priority;
}
void Aimbot::FrameUpdatePostEntityThink()
{
	static auto sv_unlag = g_pCvar->FindVar("sv_unlag");
	if (g_pGlobalVars->maxClients <= 1 || !sv_unlag->GetBool())
	{
		ClearHistory();
		return;
	}

	for (int i = 1; i <= 64; i++)
	{
		C_BaseEntity* player = g_pEntityList->GetClientEntity(i);

		auto& lag_records = this->m_LagRecord[i];

		if (!IsPlayerValid(player))
		{
			if (lag_records.size() > 0)
				lag_records.clear();

			continue;
		}

		int32_t ent_index = player->EntIndex();
		float_t sim_time = player->GetSimulationTime();

		LagRecord cur_lagrecord;

		RemoveBadRecords(ent_index, lag_records);

		if (lag_records.size() > 0)
		{
			auto& tail = lag_records.back();

			if (tail.m_flSimulationTime == sim_time)
				continue;
		}
		cur_lagrecord.SaveRecord(player); // first let's create the record

		if (!lag_records.empty()) // apply specific stuff that is needed
		{
			auto& temp_lagrecord = lag_records.back();
			int32_t priority_level = GetPriorityLevel(player, &temp_lagrecord);

			cur_lagrecord.m_iPriority = priority_level;
			cur_lagrecord.m_flPrevLowerBodyYaw = temp_lagrecord.m_flPrevLowerBodyYaw;
			cur_lagrecord.m_arrflPrevPoseParameters = temp_lagrecord.m_arrflPrevPoseParameters;

			if (priority_level == 3)
				cur_lagrecord.m_angAngles.y = temp_lagrecord.m_angAngles.y;
		}

		lag_records.emplace_back(cur_lagrecord);
	}
}
void LagRecord::SaveRecord(C_BaseEntity* player)
{
	m_vecOrigin = player->GetOrigin();
	m_vecAbsOrigin = player->GetAbsOrigin();
	m_angAngles = player->GetEyeAngles();
	m_flSimulationTime = player->GetSimulationTime();
	m_vecMins = player->GetCollideable()->OBBMins();
	m_vecMax = player->GetCollideable()->OBBMaxs();
	m_nFlags = player->m_fFlags();
	m_vecVelocity = player->GetVelocity();

	int layerCount = player->GetNumAnimOverlays();
	for (int i = 0; i < layerCount; i++)
	{
		AnimationLayer* currentLayer = player->GetAnimOverlay(i);
		m_LayerRecords[i].m_nOrder = currentLayer->m_nOrder;
		m_LayerRecords[i].m_nSequence = currentLayer->m_nSequence;
		m_LayerRecords[i].m_flWeight = currentLayer->m_flWeight;
		m_LayerRecords[i].m_flCycle = currentLayer->m_flCycle;
	}

	m_arrflPoseParameters = player->m_flPoseParameter();

	m_iTickCount = g_pGlobalVars->tickcount;
	m_vecHeadSpot = player->GetBonePosition(8);
}
bool Aimbot::StartLagCompensation(C_BaseEntity* player)
{
	enum
	{
		// Only try to awall the "best" records, otherwise fail.
		TYPE_BEST_RECORDS,
		// Only try to awall the newest and the absolute best record.
		TYPE_BEST_AND_NEWEST,
		// Awall everything (fps killer)
		TYPE_ALL_RECORDS,
	};

	backtrack_records.clear();

	auto& m_LagRecords = this->m_LagRecord[player->EntIndex()];
	m_RestoreLagRecord[player->EntIndex()].second.SaveRecord(player);

	LagRecord newest_record = LagRecord();
	for (auto it : m_LagRecords)
	{
		if (it.m_flSimulationTime > newest_record.m_flSimulationTime)
			newest_record = it;

		if (it.m_iPriority >= 1)
			backtrack_records.emplace_back(it);
	}

	switch (g_Menu.Config.posbacktrk_option)
	{
	case TYPE_BEST_RECORDS:
	{
		for (auto it : m_LagRecords)
		{
			if (it.m_iPriority >= 1 || (it.m_vecVelocity.Length2D() > 10.f))
				backtrack_records.emplace_back(it);
		}
		break;
	}
	case TYPE_BEST_AND_NEWEST:
	{
		LagRecord newest_record = LagRecord();
		for (auto it : m_LagRecords)
		{
			if (it.m_flSimulationTime > newest_record.m_flSimulationTime)
				newest_record = it;

			if (it.m_iPriority >= 1)
				backtrack_records.emplace_back(it);
		}
		backtrack_records.emplace_back(newest_record);
		break;
	}
	case TYPE_ALL_RECORDS:
		backtrack_records = m_LagRecords;
		break;
	}

	backtrack_records.emplace_back(newest_record);

	std::sort(backtrack_records.begin(), backtrack_records.end(), [](LagRecord const& a, LagRecord const& b) { return a.m_iPriority > b.m_iPriority; });
	return backtrack_records.size() > 0;
}
void Aimbot::FinishLagCompensation(C_BaseEntity* player)
{
	int idx = player->EntIndex();

	player->invalidate_bone_cache();

	player->GetCollideable()->OBBMins() = m_RestoreLagRecord[idx].second.m_vecMins;
	player->GetCollideable()->OBBMaxs() = m_RestoreLagRecord[idx].second.m_vecMax;

	player->SetAbsAngles(Vector(0, m_RestoreLagRecord[idx].second.m_angAngles.y, 0));
	player->SetAbsOrigin(m_RestoreLagRecord[idx].second.m_vecOrigin);

	player->SetFlags(m_RestoreLagRecord[idx].second.m_nFlags);

	int layerCount = player->GetNumAnimOverlays();
	for (int i = 0; i < layerCount; ++i)
	{
		AnimationLayer* currentLayer = player->GetAnimOverlay(i);
		currentLayer->m_nOrder = m_RestoreLagRecord[idx].second.m_LayerRecords[i].m_nOrder;
		currentLayer->m_nSequence = m_RestoreLagRecord[idx].second.m_LayerRecords[i].m_nSequence;
		currentLayer->m_flWeight = m_RestoreLagRecord[idx].second.m_LayerRecords[i].m_flWeight;
		currentLayer->m_flCycle = m_RestoreLagRecord[idx].second.m_LayerRecords[i].m_flCycle;
	}

	player->m_flPoseParameter() = m_RestoreLagRecord[idx].second.m_arrflPoseParameters;
}
template<class T, class U>
T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}
bool Aimbot::IsTickValid(int tick)
{
	// better use polak's version than our old one, getting more accurate results

	INetChannelInfo* nci = g_pEngine->GetNetChannelInfo();

	static auto sv_maxunlag = g_pCvar->FindVar("sv_maxunlag");

	if (!nci || !sv_maxunlag)
		return false;

	float correct = clamp(nci->GetLatency(FLOW_OUTGOING) + g_LagComp.LerpTime(), 0.f, sv_maxunlag->GetFloat());

	float deltaTime = correct - (g_pGlobalVars->curtime - TICKS_TO_TIME(tick));

}
static  C_BaseEntity* GetEntityByIndex(int index)
{
	return static_cast<C_BaseEntity*>(g_pEntityList->GetClientEntity(index));
}
static C_BaseEntity* GetPlayerByIndex(int i)
{
	return static_cast<C_BaseEntity*>(GetEntityByIndex(i));
}
float LerpTime() {

	/*static ConVar* updaterate = g_pCvar->FindVar(SteamStrings("cl_updaterate"));
	static ConVar* minupdate = g_pCvar->FindVar(SteamStrings("sv_minupdaterate"));
	static ConVar* maxupdate = g_pCvar->FindVar(SteamStrings("sv_maxupdaterate"));
	static ConVar* lerp = g_pCvar->FindVar(SteamStrings("cl_interp"));
	static ConVar* cmin = g_pCvar->FindVar(SteamStrings("sv_client_min_interp_ratio"));
	static ConVar* cmax = g_pCvar->FindVar(SteamStrings("sv_client_max_interp_ratio"));
	static ConVar* ratio = g_pCvar->FindVar(SteamStrings("cl_interp_ratio"));

	float lerpurmom = lerp->GetFloat(), maxupdateurmom = maxupdate->GetFloat(),
		ratiourmom = ratio->GetFloat(), cminurmom = cmin->GetFloat(), cmaxurmom = cmax->GetFloat();
	int updaterateurmom = updaterate->GetInt(),
		sv_maxupdaterate = maxupdate->GetInt(), sv_minupdaterate = minupdate->GetInt();

	if (sv_maxupdaterate && sv_minupdaterate)
		updaterateurmom = maxupdateurmom;

	if (ratiourmom == 0)
		ratiourmom = 1.0f;

	if (cmin && cmax && cmin->GetFloat() != 1)
		ratiourmom = std::clamp(ratiourmom, cminurmom, cmaxurmom);

	return max(lerpurmom, ratiourmom / updaterateurmom);*/

	static auto cl_interp = g_pCvar->FindVar(SteamStrings("cl_interp")); //-V807
	static auto cl_interp_ratio = g_pCvar->FindVar(SteamStrings("cl_interp_ratio"));
	static auto sv_client_min_interp_ratio = g_pCvar->FindVar(SteamStrings("sv_client_min_interp_ratio"));
	static auto sv_client_max_interp_ratio = g_pCvar->FindVar(SteamStrings("sv_client_max_interp_ratio"));
	static auto cl_updaterate = g_pCvar->FindVar(SteamStrings("cl_updaterate"));
	static auto sv_minupdaterate = g_pCvar->FindVar(SteamStrings("sv_minupdaterate"));
	static auto sv_maxupdaterate = g_pCvar->FindVar(SteamStrings("sv_maxupdaterate"));

	auto updaterate = clamp(cl_updaterate->GetFloat(), sv_minupdaterate->GetFloat(), sv_maxupdaterate->GetFloat());
	auto lerp_ratio = clamp(cl_interp_ratio->GetFloat(), sv_client_min_interp_ratio->GetFloat(), sv_client_max_interp_ratio->GetFloat());

	return clamp(lerp_ratio / updaterate, cl_interp->GetFloat(), 1.0f);
}
void Aimbot::ProcessCMD(int iTargetIdx, CUserCmd* usercmd)
{
	if (g_Menu.Config.PosBacktrack)
	{
		LagRecord recentLR = m_RestoreLagRecord[iTargetIdx].first;
		if (!IsTickValid(TIME_TO_TICKS(recentLR.m_flSimulationTime)))
			usercmd->tick_count = TIME_TO_TICKS(GetPlayerByIndex(iTargetIdx)->GetSimulationTime() + LerpTime());
		else
			usercmd->tick_count = TIME_TO_TICKS(recentLR.m_flSimulationTime + LerpTime());
	}
	//g::pCmd->buttons |= IN_ATTACK;
}
bool Aimbot::FindViableRecord(C_BaseEntity* player, LagRecord* record)
{
	auto& m_LagRecords = this->m_LagRecord[player->EntIndex()];

	// Ran out of records to check. Go back.
	if (backtrack_records.empty())
	{
		return false;
	}

	LagRecord
		recentLR = *backtrack_records.begin(),
		prevLR;

	// Should still use m_LagRecords because we're checking for LC break.
	auto iter = std::find(m_LagRecords.begin(), m_LagRecords.end(), recentLR);
	auto idx = std::distance(m_LagRecords.begin(), iter);
	if (0 != idx) prevLR = *std::prev(iter);

	// Saving first record for processcmd.
	m_RestoreLagRecord[player->EntIndex()].first = recentLR;

	if (!IsTickValid(TIME_TO_TICKS(recentLR.m_flSimulationTime)))
	{
		backtrack_records.pop_front();
		return backtrack_records.size() > 0; // RET_NO_RECORDS true false
	}

	// Remove a record...
	backtrack_records.pop_front();

	if ((0 != idx) && (recentLR.m_vecOrigin - prevLR.m_vecOrigin).LengthSqr() > 4096.f)
	{
		FakelagFix(player);
		// Bandage fix so we "restore" to the lagfixed player.
		m_RestoreLagRecord[player->EntIndex()].second.SaveRecord(player);
		*record = m_RestoreLagRecord[player->EntIndex()].second;

		// Clear so we don't try to bt shit we can't
		backtrack_records.clear();

		return true; // Return true so we still try to aimbot.
	}
	else
	{
		player->invalidate_bone_cache();

		player->GetCollideable()->OBBMins() = recentLR.m_vecMins;
		player->GetCollideable()->OBBMaxs() = recentLR.m_vecMax;

		player->SetAbsAngles(Vector(0, recentLR.m_angAngles.y, 0));
		player->SetAbsOrigin(recentLR.m_vecOrigin);

		player->SetFlags(recentLR.m_nFlags);

		int layerCount = player->GetNumAnimOverlays();
		for (int i = 0; i < layerCount; ++i)
		{
			AnimationLayer* currentLayer = player->GetAnimOverlay(i);
			currentLayer->m_nOrder = recentLR.m_LayerRecords[i].m_nOrder;
			currentLayer->m_nSequence = recentLR.m_LayerRecords[i].m_nSequence;
			currentLayer->m_flWeight = recentLR.m_LayerRecords[i].m_flWeight;
			currentLayer->m_flCycle = recentLR.m_LayerRecords[i].m_flCycle;
		}

		player->m_flPoseParameter() = recentLR.m_arrflPoseParameters;

		*record = recentLR;
		return true;
	}
}
void Aimbot::RageBacktrack(C_BaseEntity* target, CUserCmd* usercmd, Vector aimpoint)
{
	auto firedShots = g::pLocalEntity->GetShotsFired();
	if (g_Menu.Config.PosBacktrack)
	{
		if (StartLagCompensation(target))
		{
			LagRecord cur_record;
			auto& m_LagRecords = this->m_LagRecord[target->EntIndex()];
			while (FindViableRecord(target, &cur_record))
			{
				auto iter = std::find(m_LagRecords.begin(), m_LagRecords.end(), cur_record);
				if (iter == m_LagRecords.end())
					continue;

				if (iter->m_bNoGoodSpots)
				{
					// Already awalled from same spot, don't try again like a dumbass.
					if (iter->m_vecLocalAimspot == g::pLocalEntity->GetEyePosition())
						continue;
					else
						iter->m_bNoGoodSpots = false;
				}

				if (!iter->m_bMatrixBuilt)
				{
					if (!target->SetupBones2(iter->matrix, 128, 256, iter->m_flSimulationTime))
						continue;
					iter->m_bMatrixBuilt = true;
				}
				if (!aimpoint.Is_Valid())
				{
					FinishLagCompensation(target);
					iter->m_bNoGoodSpots = true;
					iter->m_vecLocalAimspot = g::pLocalEntity->GetEyePosition();
					continue;
				}

				this->current_record[target->EntIndex()] = *iter;
				break;
			}
			FinishLagCompensation(target);
			ProcessCMD(target->EntIndex(), usercmd);
		}
	}
}
void Aimbot::FakelagFix(C_BaseEntity* player)
{
	auto& lag_records = this->m_LagRecord[player->EntIndex()];

	if (lag_records.size() > 2)
	{
		bool bChocked = TIME_TO_TICKS(player->GetSimulationTime() - lag_records.back().m_flSimulationTime) > 1;
		bool bInAir = false;

		if (!(player->GetFlags() & FL_ONGROUND) || !(lag_records.back().m_nFlags & FL_ONGROUND))
			bInAir = true;

		if (bChocked)
		{
			player->SetOrigin(lag_records.back().m_vecOrigin);
			player->SetAbsOrigin(lag_records.back().m_vecAbsOrigin);
			player->SetVelocity(lag_records.back().m_vecVelocity);
			player->SetFlags(lag_records.back().m_nFlags);
		}

		Vector data_origin = player->GetOrigin();
		Vector data_velocity = player->GetVelocity();
		int data_flags = player->GetFlags();

		if (bChocked)
		{
			//SimulateMovement(data_velocity, data_origin, player, data_flags, bInAir);

			player->GetVecOrigin() = data_origin;
			player->SetAbsOrigin(data_origin);
			player->GetVelocity() = data_velocity;

			player->m_fFlags() &= 0xFFFFFFFE;
			auto penultimate_record = *std::prev(lag_records.end(), 2);
			if ((lag_records.back().m_nFlags & FL_ONGROUND) && (penultimate_record.m_nFlags & FL_ONGROUND))
				player->m_fFlags() |= 1;
			if (*(float*)((uintptr_t)player->GetAnimOverlay(0) + 0x138) > 0.f)
				player->m_fFlags() |= 1;
		}
	}
}
void Aimbot::OnCreateMove()
{
	if (!g::pLocalEntity || !g_pEngine->IsInGame() || !g_pEngine->IsConnected())
		return;

	Vector Aimpoint = { 0,0,0 };
	C_BaseEntity* Target = nullptr;

	int tempDmg = 0;
	static bool shot = false;
	static float oldTime;
	if (g_Menu.Config.AutomaticWeapons && !g::pLocalEntity->IsKnifeorNade())
		AutomaticWeapons();
	if (!g_Menu.Config.Aimbot)
		return;
	AutoRevolver();
	C_BaseCombatWeapon* weapon = g::pLocalEntity->GetActiveWeapon();
	static ConVar* weaponRecoilScale = g_pCvar->FindVar(SteamStrings("weapon_recoil_scale"));
	Vector aimPunch = g::pLocalEntity->GetAimPunchAngle() * weaponRecoilScale->GetFloat();
	for (int i = 1; i <= 64; ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant())
		{
			g_LagComp.ClearRecords(i);
			g::Shots[i] = 0;
			g::MissedShots[i] = 0;
			continue;
		}
		if (!weapon)
			continue;
		if (pPlayerEntity == g::pLocalEntity)
			continue;

		g_LagComp.StoreRecord(pPlayerEntity);

		if (!g_Menu.Config.FriendlyFire && pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
			continue;

		if (pPlayerEntity->IsImmune())
			continue;

		g::EnemyEyeAngs[i] = pPlayerEntity->GetEyeAngles();

		if (g_LagComp.PlayerRecord[i].size() == 0 || !g::pLocalEntity->IsAlive())
			continue;

		global_head = g_Menu.Config.HeadScale;
		global_body = g_Menu.Config.BodyScale;
		if (weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_AWP)
		{
			global_hitcahnce = g_Menu.Config.AwpHC;
			global_hitscan = g_Menu.Config.awpglobal_hitscan;
			min_dmg = g_Menu.Config.AwpMD;
			wal_dmg = g_Menu.Config.AwpAWDM;
		}
		else
		{
			if (weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_SSG08)
			{
				global_hitcahnce = g_Menu.Config.ScoutHC;
				global_hitscan = g_Menu.Config.scoutglobal_hitscan;
				min_dmg = g_Menu.Config.ScoutMD;
				wal_dmg = g_Menu.Config.ScoutAWDM;

			}
			else
			{
				if (weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_SCAR20 || weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_G3SG1)
				{
					global_hitcahnce = g_Menu.Config.AutoHC;
					global_hitscan = g_Menu.Config.autoglobal_hitscan;
					min_dmg = g_Menu.Config.AutoMD;
					wal_dmg = g_Menu.Config.AutoAWDM;
				}
				else
				{
					if (weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_DEAGLE || weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_REVOLVER)
					{
						global_hitcahnce = g_Menu.Config.DeagleHC;
						global_hitscan = g_Menu.Config.deagleglobal_hitscan;
						min_dmg = g_Menu.Config.DeagleMD;
						wal_dmg = g_Menu.Config.DeagleAWDM;
					}
					if (weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_HKP2000 || weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_P250
						|| weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_ELITE || weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_FIVESEVEN
						|| weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_GLOCK || weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_USP_SILENCER
						|| weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_CZ75A || weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_TEC9)
					{
						global_hitcahnce = g_Menu.Config.PistolHC;
						global_hitscan = g_Menu.Config.pistolglobal_hitscan;
						min_dmg = g_Menu.Config.PistolMD;
						wal_dmg = g_Menu.Config.PistolAWDM;
					}
					else
					{
						global_hitcahnce = g_Menu.Config.RifleHC;
						global_hitscan = g_Menu.Config.rifleglobal_hitscan;
						min_dmg = g_Menu.Config.RifleMD;
						wal_dmg = g_Menu.Config.RifleAWDM;
					}
				}
			}
		}
		bestEntDmg = 0;
		Vector Hitbox = multipoint(pPlayerEntity, global_hitscan);
		if (Hitbox != Vector(0, 0, 0) && tempDmg <= bestEntDmg)
		{
			Aimpoint = Hitbox;
			Target = pPlayerEntity;
			targetID = Target->EntIndex();
			tempDmg = bestEntDmg;
		}
	}
	if (!g::pLocalEntity->IsAlive() || !weapon || g::pLocalEntity->IsKnifeorNade()
		|| weapon->GetAmmo() == 0 || g_GameRules->IsFreezeTime())
	{
		shot = false;
		return;
	}
	if (shot)
		shot = false;

	float flServerTime = g::pLocalEntity->GetTickBase() * g_pGlobalVars->intervalPerTick;
	bool canShoot = (weapon->GetNextPrimaryAttack() <= flServerTime);
	if (Target)
	{
		//RageBacktrack(Target, g::pCmd, Aimpoint); //FIXME
		g::pCmd->tick_count = Backtrack::timeToTicks(Target->GetSimulationTime() + Backtrack::getLerp());

		g::TargetIndex = targetID;
		float SimulationTime = 0.f;

		if (Backtrack[targetID])
			SimulationTime = g_LagComp.PlayerRecord[targetID].at(0).SimTime;
		else
			SimulationTime = g_LagComp.PlayerRecord[targetID].at(g_LagComp.PlayerRecord[targetID].size() - 1).SimTime;

		if (ShotBacktrack[targetID])
			SimulationTime = g_LagComp.PlayerRecord[targetID].at(g_LagComp.ShotTick[targetID]).SimTime;

		Vector Angle = g_Math.CalcAngle(g::pLocalEntity->GetEyePosition(), Aimpoint);

		Vector view;
		g_pEngine->GetViewAngles(view);

		static int MinimumVelocity = 0;

		if (g_Menu.Config.AutoScope)
		{
			if (weapon)
			{
				std::string WeaponName = weapon->GetName();
				if (weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_SSG08 ||
					weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_AWP ||
					weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_G3SG1 ||
					weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_SCAR20)
				{
					if (!g::pLocalEntity->IsScoped())
					{
						g::pCmd->buttons |= IN_ATTACK2;
					}
				}
			}
		}

		if (weapon && weapon->GetItemDefinitionIndex() != ItemDefinitionIndex::WEAPON_TASER)
		{
			if (g::pLocalEntity->IsKnifeorNade())
				return;

			if (!g_Menu.Config.AutoStopMode == 0)
			{
				MinimumVelocity = g::pLocalEntity->GetActiveWeapon()->GetCSWpnData()->fMaxSpeedAlt * .34f;
			}
			else if (!g_Menu.Config.AutoStopMode == 1)
			{
				MinimumVelocity = 0;
			}
			else if (!g_Menu.Config.AutoStopMode == 2)
			{
				MinimumVelocity = g::pLocalEntity->GetActiveWeapon()->GetCSWpnData()->fMaxSpeedAlt / g_Menu.Config.customstop * 0.1;
			}

			bool shouldstop = !g_Menu.Config.StopInBetweenShots ? true : canShoot;

			if (g::pLocalEntity->GetVelocity().Length() >= MinimumVelocity && shouldstop && !GetAsyncKeyState(VK_SPACE))
				Autostop();
		}
		if (!(g::pCmd->buttons & IN_ATTACK) && canShoot && HitChance(Target, g::pLocalEntity->GetActiveWeapon(), Angle, Aimpoint, g_Menu.Config.HitchanceValue))
		{
			if (!Backtrack[targetID] && !ShotBacktrack[targetID])
				g::Shot[targetID] = true;

			g::bSendPacket = true;
			shot = true;
			g::Shots[Target->EntIndex()] += 1;
			g::pCmd->viewangles = (Angle - aimPunch); //RCS
			if (!g_Menu.Config.SilentAim)
				g_pEngine->SetViewAngles(Angle);
			if (accepted_inaccuracy(weapon) > global_hitcahnce)
			{
				g::pCmd->buttons |= IN_ATTACK;
			}
		}
	}

	if (g_Menu.Config.quickswitch)
	{
		static bool dontqs = false, quickswitching = false;

		if (!shot)
			dontqs = false;

		if (g::pLocalEntity->IsAwp() && shot && !quickswitching && !dontqs)
		{
			g_pEngine->ExecuteClientCmd(SteamStrings("slot3"));
			quickswitching = true;
		}

		if (quickswitching && g::pLocalEntity->IsKnife())
		{
			g_pEngine->ExecuteClientCmd(SteamStrings("slot1"));
			quickswitching = false;
			if (shot)
				dontqs = true;
		}
	}
}
inline float fastsqrt(float x)
{
	unsigned int i = *(unsigned int*)&x;
	i += 127 << 23;
	i >>= 1;
	return *(float*)&i;
}
#define square( x ) ( x * x )
void ClampMovement(CUserCmd* pCommand, float fMaxSpeed)
{
	if (fMaxSpeed <= 0.f)
		return;
	float fSpeed = (float)(fastsqrt(square(pCommand->forwardmove) + square(pCommand->sidemove) + square(pCommand->upmove)));
	if (fSpeed <= 0.f)
		return;
	if (pCommand->buttons & IN_DUCK)
		fMaxSpeed *= 2.94117647f; // TO DO: Maybe look trough the leaked sdk for an exact value since this is straight out of my ass...
	if (fSpeed <= fMaxSpeed)
		return;
	float fRatio = fMaxSpeed / fSpeed;
	pCommand->forwardmove *= fRatio;
	pCommand->sidemove *= fRatio;
	pCommand->upmove *= fRatio;
}
std::vector<Vector> Aimbot::GetMultiplePointsForHitbox(C_BaseEntity* local, C_BaseEntity* entity, int iHitbox, matrix3x4_t BoneMatrix[128])
{
	auto VectorTransform_Wrapper = [](const Vector& in1, const matrix3x4_t& in2, Vector& out)
	{
		auto VectorTransform = [](const float* in1, const matrix3x4_t& in2, float* out)
		{
			auto DotProducts = [](const float* v1, const float* v2)
			{
				return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
			};
			out[0] = DotProducts(in1, in2[0]) + in2[0][3];
			out[1] = DotProducts(in1, in2[1]) + in2[1][3];
			out[2] = DotProducts(in1, in2[2]) + in2[2][3];
		};
		VectorTransform(&in1.x, in2, &out.x);
	};


	const auto pModel = entity->GetModel();
	auto pStudioHdr = g_pModelInfo->GetStudiomodel(pModel);

	mstudiohitboxset_t* set = pStudioHdr->GetHitboxSet(0);
	mstudiobbox_t* hitbox = set->GetHitbox(iHitbox);

	std::vector<Vector> vecArray;

	Vector max;
	Vector min;
	VectorTransform_Wrapper(hitbox->max, BoneMatrix[hitbox->bone], max);
	VectorTransform_Wrapper(hitbox->min, BoneMatrix[hitbox->bone], min);

	auto center = (min + max) * 0.5f;

	Vector CurrentAngles = g_Math.CalcAngle(center, local->GetEyePosition());

	Vector Forward;
	g_Math.AngleVectors(CurrentAngles, &Forward);

	Vector Right = Forward.Cross(Vector(0, 0, 1));
	Vector Left = Vector(-Right.x, -Right.y, Right.z);

	Vector Top = Vector(0, 0, 1);
	Vector Bot = Vector(0, 0, -1);

	switch (iHitbox) {
	case 0://head
		for (auto i = 0; i < 4; ++i)
			vecArray.emplace_back(center);

		vecArray[1] += Top * (hitbox->radius * global_head * 0.01);
		vecArray[2] += Right * (hitbox->radius * global_head * 0.01);
		vecArray[3] += Left * (hitbox->radius * global_head * 0.01);
		break;
	default://rest

		for (auto i = 0; i < 3; ++i)
			vecArray.emplace_back(center);

		vecArray[1] += Right * (hitbox->radius * global_body * 0.01);
		vecArray[2] += Left * (hitbox->radius * global_body * 0.01);
		break;
	}
	return vecArray;
}
Vector Aimbot::multipoint(C_BaseEntity* entity, int option)
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player) return Vector(0, 0, 0);

	Vector vector_best_point = Vector(0, 0, 0);
	PlayerRecords pPlayerEntityRecord = g_LagComp.PlayerRecord[entity->EntIndex()].at(0);
	int minDamage = min_dmg;

	auto matrix = g_Aimbot.Matrix[entity->EntIndex()];

	Backtrack[entity->EntIndex()] = false;
	ShotBacktrack[entity->EntIndex()] = false;
	//auto matrixsssss = g_LagComp->Matrix[entity->GetIndex()];
	int mixedhitboxes[] = {
		HITBOX_HEAD,
		HITBOX_NECK,
		HITBOX_PELVIS,
		HITBOX_LOWER_CHEST,
		HITBOX_UPPER_CHEST,
		HITBOX_BELLY,
		HITBOX_THORAX,
		HITBOX_RIGHT_UPPER_ARM,
		HITBOX_RIGHT_FOREARM,
		HITBOX_LEFT_UPPER_ARM,
		HITBOX_LEFT_FOREARM,
		HITBOX_LEFT_CALF,
		HITBOX_RIGHT_CALF,
		HITBOX_LEFT_THIGH,
		HITBOX_RIGHT_THIGH };// mixed hitscan
	int headbodyhitboxes[] = {
		HITBOX_HEAD,
		HITBOX_NECK,
		HITBOX_PELVIS,
		HITBOX_LOWER_CHEST,
		HITBOX_UPPER_CHEST,
		HITBOX_LOWER_CHEST,
		HITBOX_RIGHT_UPPER_ARM,
		HITBOX_RIGHT_FOREARM,
		HITBOX_LEFT_UPPER_ARM,
		HITBOX_LEFT_FOREARM }; // head-body hitscan
	int bodyhitboxes[] = {
		HITBOX_PELVIS,
		HITBOX_LOWER_CHEST,
		HITBOX_UPPER_CHEST,
		HITBOX_LOWER_CHEST,
		HITBOX_RIGHT_UPPER_ARM,
		HITBOX_RIGHT_FOREARM,
		HITBOX_LEFT_UPPER_ARM,
		HITBOX_LEFT_FOREARM }; // body hitscan
	int headhitboxes[] = {
		HITBOX_HEAD,
		HITBOX_NECK };  //head hitscan
	int mixedbaimhitboxes[] =
	{
		HITBOX_PELVIS,
		HITBOX_LOWER_CHEST,
		HITBOX_UPPER_CHEST

	};// mixed hitscan

	if (GetAsyncKeyState(g_Menu.Config.fbaimkey)) //baim key
	{
		for (int i = 0; i < ARRAYSIZE(mixedbaimhitboxes); i++)
		{
			for (auto point : GetMultiplePointsForHitbox(local_player, entity, mixedbaimhitboxes[i], matrix))
			{
				int damage = g_Autowall.CalculateDamage(local_player->GetEyePosition(), point, local_player, entity).damage;
				if (damage > minDamage && damage > wal_dmg)
				{
					bestHitboxint = mixedbaimhitboxes[i];
					minDamage = damage;
					vector_best_point = point;

					if (minDamage >= entity->GetHealth())
						return vector_best_point;
				}
			}
		}
	}
	else
	{
		switch (option)
		{
		case 0:
		{
			for (int i = 0; i < ARRAYSIZE(headhitboxes); i++)
			{
				for (auto point : GetMultiplePointsForHitbox(local_player, entity, headhitboxes[i], matrix))
				{
					int damage = g_Autowall.CalculateDamage(local_player->GetEyePosition(), point, local_player, entity).damage;
					if (damage > minDamage && damage > wal_dmg)
					{
						bestHitboxint = headhitboxes[i];
						minDamage = damage;
						vector_best_point = point;

						if (minDamage >= entity->GetHealth())
							return vector_best_point;
					}
				}
			}
		}
		break;
		case 1:
		{
			for (int i = 0; i < ARRAYSIZE(bodyhitboxes); i++)
			{
				for (auto point : GetMultiplePointsForHitbox(local_player, entity, bodyhitboxes[i], matrix))
				{
					int damage = g_Autowall.CalculateDamage(local_player->GetEyePosition(), point, local_player, entity).damage;
					if (damage > minDamage && damage > wal_dmg)
					{
						bestHitboxint = bodyhitboxes[i];
						minDamage = damage;
						vector_best_point = point;

						if (minDamage >= entity->GetHealth())
							return vector_best_point;
					}
				}
			}
		}
		break;
		case 2:
		{
			for (int i = 0; i < ARRAYSIZE(headbodyhitboxes); i++)
			{
				for (auto point : GetMultiplePointsForHitbox(local_player, entity, headbodyhitboxes[i], matrix))
				{

					int damage = g_Autowall.CalculateDamage(local_player->GetEyePosition(), point, local_player, entity).damage;
					if (damage > minDamage && damage > wal_dmg)
					{
						bestHitboxint = headbodyhitboxes[i];
						minDamage = damage;
						vector_best_point = point;

						if (minDamage >= entity->GetHealth())
							return vector_best_point;
					}

				}

			}
		}
		break;
		case 3:
		{
			for (int i = 0; i < ARRAYSIZE(mixedhitboxes); i++)
			{
				for (auto point : GetMultiplePointsForHitbox(local_player, entity, mixedhitboxes[i], matrix))
				{
					int damage = g_Autowall.CalculateDamage(local_player->GetEyePosition(), point, local_player, entity).damage;
					if (damage > minDamage && damage > wal_dmg)
					{
						bestHitboxint = mixedhitboxes[i];
						minDamage = damage;
						vector_best_point = point;

						if (minDamage >= entity->GetHealth())
							return vector_best_point;
					}
				}
			}
		}
		break;
		}

	}
	return vector_best_point;
}