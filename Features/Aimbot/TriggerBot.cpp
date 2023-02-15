#include "TriggerBot.h"
#include "../../SDK/CEntity.h"
#include "../../SDK/CInput.h"
#include "../../Utils/GlobalVars.h"
#include "../../SDK/CGlobalVarsBase.h"
#include "../../SDK/Definitions.h"
#include "../../Utils/Math.h"
#include "../../SDK/IEngineTrace.h"
#include "../../SDK/ClientClass.h"
#include "../../SDK/CInput.h"
#include "../../SDK/IEngineTrace.h"
#include "../../SDK/IVModelInfo.h"
#include "../../Menu/Menu.h"

CTriggerBot g_TriggerBot;
Vector C_BaseEntity::GetHitboxPositionxd(int Hitbox)
{
	matrix3x4_t bone_matrix[128];

	if (this->SetupBones(bone_matrix, 128, 0x00000100, 0.0f)) {

		auto studio_model = g_pModelInfo->GetStudiomodel(this->GetModel());

		if (studio_model) {

			auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(Hitbox);

			if (hitbox) {

				auto min = Vector{}, max = Vector{};

				g_Math.VectorTransform(hitbox->min, bone_matrix[hitbox->bone], min);
				g_Math.VectorTransform(hitbox->max, bone_matrix[hitbox->bone], max);

				return Vector((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f);
			}
		}
	}
	return Vector{};
}


bool m_visible;
bool C_BaseEntity::IsVisible(int bone)
{
	C_Trace tr;
	m_visible = false;


	CTraceEntity Filter(g::pLocalEntity);
	Filter.pSkip1 = g::pLocalEntity;

	g_pTrace->TraceRay(C_Ray(g::pLocalEntity->GetEyePosition(), this->GetBonePosition(bone)), MASK_SHOT, &Filter, &tr);

	if (tr.m_pEnt == this)
	{
		m_visible = true;
		return true;
	}

	return false;
}

void CTriggerBot::Move(CUserCmd* pCmd)
{
	if (g_Menu.Config.trigetbot_enable && GetAsyncKeyState(g_Menu.Config.TrigerKey))
		DoTrigger(pCmd);
}

void CTriggerBot::DoTrigger(CUserCmd* pCmd)
{
	C_BaseEntity* LocalPlayer = g::pLocalEntity;
	if (!LocalPlayer) return;
	auto LocalPlayerWeapon = LocalPlayer->GetActiveWeapon();
	auto WeaponEntity = (IClientEntity*)LocalPlayerWeapon;

	if (LocalPlayerWeapon) {
		if (LocalPlayerWeapon->GetAmmo() == 0)
			return;

		auto ClientClass = WeaponEntity->GetClientClass();
		if (ClientClass->ClassID == (int)EClassIds::CKnife ||
			ClientClass->ClassID == (int)EClassIds::CHEGrenade ||
			ClientClass->ClassID == (int)EClassIds::CDecoyGrenade ||
			ClientClass->ClassID == (int)EClassIds::CIncendiaryGrenade ||
			ClientClass->ClassID == (int)EClassIds::CSmokeGrenade ||
			ClientClass->ClassID == (int)EClassIds::CC4) {
			return;
		}
	}
	else
		return;

	Vector ViewAngles = pCmd->viewangles;
	if (g_Menu.Config.triggerbot_recoil)
		ViewAngles += LocalPlayer->GetAimPunchAngle() * 2.0f;

	Vector CrosshairForward;
	g_Math.AngleVectors(ViewAngles, &CrosshairForward);
	CrosshairForward *= 8000.f;


	Vector TraceSource = LocalPlayer->GetEyePosition();
	Vector TraceDestination = TraceSource + CrosshairForward;


	C_Trace Trace;
	CTraceEntity Filter(LocalPlayer);

	Filter.pSkip1 = LocalPlayer;

	g_pTrace->TraceRay(C_Ray(TraceSource, TraceDestination), MASK_SHOT, &Filter, &Trace);

	if (!Trace.m_pEnt)
		return;
	if (!Trace.m_pEnt->IsAlive())
		return;
	if (Trace.m_pEnt->GetHealth() <= 0 || Trace.m_pEnt->GetHealth() > 100)
		return;

	if (!g_Menu.Config.triggerbot_deadmatch) {
		if (LocalPlayer->GetTeam() == Trace.m_pEnt->GetTeam())
			return;
	}

	/*if (Shonax.TriggerBot.weapon[Shonax.number].trigetbot_smokecheck)
	{
		typedef bool(__cdecl* GoesThroughSmoke)(Vector, Vector);

		static uint32_t GoesThroughSmokeOffset = (uint32_t)Utils::FindSignature("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
		static GoesThroughSmoke GoesThroughSmokeFunction = (GoesThroughSmoke)GoesThroughSmokeOffset;

		if (GoesThroughSmokeFunction(g::pLocalEntity->GetEyePosition(), Trace.m_pEnt->GetHitboxPositionxd(6)))
			return;
	}*/

	if ((g_Menu.Config.triggerbotbox_head && Trace.hitGroup == HITGROUP_HEAD) ||
		(g_Menu.Config.triggerbotbox_chest && Trace.hitGroup == HITGROUP_CHEST) ||
		(g_Menu.Config.triggerbotbox_stomach && Trace.hitGroup == HITGROUP_STOMACH) ||
		(g_Menu.Config.triggerbotbox_arms && (Trace.hitGroup == HITGROUP_LEFTARM || Trace.hitGroup == HITGROUP_RIGHTARM)) ||
		(g_Menu.Config.triggerbotbox_legs && (Trace.hitGroup == HITGROUP_LEFTLEG || Trace.hitGroup == HITGROUP_RIGHTLEG))) {
		pCmd->buttons |= IN_ATTACK;
	}
}



