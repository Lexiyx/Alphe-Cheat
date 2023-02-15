#include "LegitAimbot.h"
#include "../Aimbot/Aimbot.h"
#include "../../Menu/Menu.h"
#include "../../Menu/config.h"
#include "../Visuals/ESP.h"
#include "..\..\SDK\IVModelInfo.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\Menu\Hotkeys.h"
#include "..\Aimbot\LagComp.h"

float m_fbestfov = 20.0f;
int m_ibesttargetlegit = -1;

float deltaTime = 0.0f;
float curAimTime = 0.0f;
LegitAimbot g_LegitAimbot;

BackTrack g_Backtracking;
backtrackData HeadPositions[64][12];

Vector GetHitboxPosition(C_BaseEntity* pEntity, int Hitbox)
{
	matrix3x4_t matrix[128];


	if (!pEntity->SetupBones2(matrix, 128, 0x00000100, pEntity->GetSimulationTime()))
		return Vector(0, 0, 0);

	studiohdr_t* hdr = g_pModelInfo->GetStudiomodel(pEntity->GetModel());
	mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

	mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

	if (!hitbox)
		return Vector(0, 0, 0);

	Vector vMin, vMax, vCenter, sCenter;
	g_Math.VectorTransform(hitbox->min, matrix[hitbox->bone], vMin);
	g_Math.VectorTransform(hitbox->max, matrix[hitbox->bone], vMax);
	vCenter = (vMin + vMax) * 0.5f;
	return vCenter;
}

Vector angle_vector(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(M_PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(M_PI));

	auto sp = sin(meme.x / 180.f * static_cast<float>(M_PI));
	auto cp = cos(meme.x / 180.f * static_cast<float>(M_PI));

	return Vector(cp * cy, cp * sy, -sp);
}

float distance_point_to_line(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x * Dir.x + Dir.y * Dir.y + Dir.z * Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}
void BackTrack::LegitBackTrack(CUserCmd* cmd, C_BaseEntity* pLocal)
{
	if (g_Menu.Config.LegitBacktrack && g_Menu.Config.LegitAimbot)
	{
		int bestTargetIndex = -1;
		float bestFov = FLT_MAX;
		PlayerInfo_t info;
		if (!pLocal->IsAlive())
			return;

		for (int i = 0; i < 33; i++)
		{
			auto entity = static_cast<C_BaseEntity*>(g_pEntityList->GetClientEntity(i));

			if (!entity || !pLocal)
				continue;

			if (entity == pLocal)
				continue;

			if (!g_pEngine->GetPlayerInfo(i, &info))
				continue;

			if (entity->IsDormant())
				continue;

			if (entity->GetTeam() == pLocal->GetTeam())
				continue;

			if (entity->IsAlive())
			{

				float simtime = entity->GetSimulationTime();
				Vector hitboxPos = GetHitboxPosition(entity, 0);

				HeadPositions[i][cmd->command_number % 12] = backtrackData{ simtime, hitboxPos };
				Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->GetViewPunchAngle() * 2.f));
				float FOVDistance = distance_point_to_line(hitboxPos, pLocal->GetEyePosition(), ViewDir);

				if (bestFov > FOVDistance)
				{
					bestFov = FOVDistance;
					bestTargetIndex = i;
				}
			}
		}

		if (bestTargetIndex != -1)
		{
			float bestTarGetSimulationTime = 0.0f;
			float tempFloat = FLT_MAX;
			Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->GetAimPunchAngle() * 2.f));
			for (int t = 0; t < 12; ++t)
			{
				float tempFOVDistance = distance_point_to_line(HeadPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
				if (tempFloat > tempFOVDistance && HeadPositions[bestTargetIndex][t].simtime > pLocal->GetSimulationTime() - 1)
				{
					tempFloat = tempFOVDistance;
					bestTarGetSimulationTime = HeadPositions[bestTargetIndex][t].simtime;
				}
			}
			if (!g::pLocalEntity->IsKnifeorNade())
			{
				if (cmd->buttons & IN_ATTACK)
				{
					cmd->tick_count = TIME_TO_TICKS(bestTarGetSimulationTime);
				}
			}
		}
	}
}
void LegitAutomaticWeapons()
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
void LegitAimbot::Aim_CreateMove(CUserCmd* cmd)
{
	DropTarget();

	static float oldServerTime = g::pLocalEntity->GetTickBase() * g_pGlobalVars->intervalPerTick;
	float serverTime = g::pLocalEntity->GetTickBase() * g_pGlobalVars->intervalPerTick;
	deltaTime = serverTime - oldServerTime;
	oldServerTime = serverTime;

	auto weapon = g::pLocalEntity->GetActiveWeapon();
	auto localPlayer = reinterpret_cast<C_BaseEntity*>(g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer()));

	if (g_Menu.menuOpened)
		return;
	if (g::pLocalEntity->IsKnifeorNade())
		return;
	if (!g::pLocalEntity->GetActiveWeapon())
		return;

	if (g::pLocalEntity->GetActiveWeapon()->IsKnifeorNade() || g::pLocalEntity->GetActiveWeapon()->GetAmmo() == 0)
		return;

	if (g_Menu.Config.RecoilControlSystem)
		RCS();

	if (!g_Menu.Config.LegitAimbot)
		return;

	if ((!(g::pCmd->buttons & IN_ATTACK) || m_ibesttargetlegit == -1))
		FindTarget();

	if (m_ibesttargetlegit != -1 && GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.LegitAimKey)))
		GoToTarget();

	if (g_Menu.Config.AutomaticWeapons && !g::pLocalEntity->IsKnifeorNade())
		LegitAutomaticWeapons();
}

void LegitAimbot::FindTarget()
{
	m_fbestfov = g_Menu.Config.LegitAimFov;

	for (int i = 1; i < g_pEntityList->GetHighestEntityIndex(); i++)
	{
		auto Entity = g_pEntityList->GetClientEntity(i);

		if (!Entity->is_valid(g::pLocalEntity))
			continue;

		if (g_Menu.Config.LegitAimHitbox == 0)
		{
			Vector viewangles = g::pCmd->viewangles + g::pLocalEntity->GetAimPunchAngle() * 2.f;
			float fov = g_Math.GetFov(g::pCmd->viewangles, g_Math.CalcAngle(g::pLocalEntity->GetEyePosition(), Entity->GetBonePosition(2)));
			if (fov < m_fbestfov)
			{
				m_fbestfov = fov;
				m_ibesttargetlegit = i;
			}
		}
		else if (g_Menu.Config.LegitAimHitbox == 1)
		{
			Vector viewangles = g::pCmd->viewangles + g::pLocalEntity->GetAimPunchAngle() * 2.f;
			float fov = g_Math.GetFov(g::pCmd->viewangles, g_Math.CalcAngle(g::pLocalEntity->GetEyePosition(), Entity->GetBonePosition(6)));
			if (fov < m_fbestfov)
			{
				m_fbestfov = fov;
				m_ibesttargetlegit = i;
			}
		}
		else if (g_Menu.Config.LegitAimHitbox == 2)
		{
			Vector viewangles = g::pCmd->viewangles + g::pLocalEntity->GetAimPunchAngle() * 2.f;
			float fov = g_Math.GetFov(g::pCmd->viewangles, g_Math.CalcAngle(g::pLocalEntity->GetEyePosition(), Entity->GetBonePosition(0)));
			if (fov < m_fbestfov)
			{
				m_fbestfov = fov;
				m_ibesttargetlegit = i;
			}
		}
	}
}

void LegitAimbot::GoToTarget()
{
	auto Entity = g_pEntityList->GetClientEntity(m_ibesttargetlegit);

	if (!Entity)
		return;

	int hitbox;

	switch (g_Menu.Config.LegitAimHitbox)
	{
	case 0: //head
		hitbox = HITBOX_HEAD;
		break;
	case 1: //chest
		hitbox = HITBOX_UPPER_CHEST;
		break;
	case 2: //pelvis
		hitbox = HITBOX_PELVIS;
		break;
	default:
		break;
	}

	Vector predicted = Entity->GetPredicted(Entity->GetBonePosition(hitbox));

	Vector dst = g_Math.CalcAngle(g::pLocalEntity->GetEyePosition(), predicted);
	Vector src = g::pCmd->viewangles;



	dst.x -= g::pLocalEntity->GetAimPunchAngle().x * (g_Menu.Config.RCS_X / 50.f);
	dst.y -= g::pLocalEntity->GetAimPunchAngle().y * (g_Menu.Config.RCS_Y / 50.f);


	Vector delta = dst - src;

	delta.Clamp();

	if (!delta.IsZero())
	{
		float finalTime = delta.Length() / (g_Menu.Config.AimSpeed); //5

		curAimTime += deltaTime;

		if (curAimTime > finalTime)
			curAimTime = finalTime;

		float percent = curAimTime / finalTime;

		delta *= percent;
		dst = src + delta;
	}

	g_pEngine->SetViewAngles(dst.Clamp());
}

void LegitAimbot::DropTarget()
{
	if (!EntityIsValid(m_ibesttargetlegit))
	{
		m_ibesttargetlegit = -1;
		curAimTime = 0.f;
	}
}

bool LegitAimbot::EntityIsValid(int i)
{
	auto Entity = g_pEntityList->GetClientEntity(i);

	if (!Entity->is_valid())
		return false;

	int hitbox;

	switch (g_Menu.Config.LegitAimHitbox)
	{
	case 0: //head
		hitbox = HITBOX_HEAD;
		break;
	case 1: //chest
		hitbox = HITBOX_UPPER_CHEST;
		break;
	case 2: //pelvis
		hitbox = HITBOX_PELVIS;
		break;
	default:
		break;
	}

	if (g_Math.GetFov(g::pCmd->viewangles, g_Math.CalcAngle(g::pLocalEntity->GetEyePosition(), Entity->GetBonePosition(hitbox))) > g_Menu.Config.LegitAimFov * 0.1)
		return false;

	return true;
}

Vector m_oldangle;

void LegitAimbot::RCS()
{
	if (g::pCmd->buttons & IN_ATTACK)
	{
		Vector view_angle;
		g_pEngine->GetViewAngles(view_angle);
		int shots_fired = g::pLocalEntity->GetShotsFired();
		Vector punch = g::pLocalEntity->GetAimPunchAngle() * 2;
		if (shots_fired > 1) {
			g::pCmd->viewangles = (view_angle - punch);
		}
	}
}
