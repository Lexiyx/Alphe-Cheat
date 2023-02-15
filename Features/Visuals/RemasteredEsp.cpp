#include "RemasteredEsp.h"
#include "..\Aimbot\Aimbot.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\Utils\Math.h"
#include "..\Aimbot\LagComp.h"
#include "..\..\Menu\Menu.h"
#include "..\Misc\Misc.h"

RemasteredEsp g_RemasteredEsp;

void RemasteredEsp::RenderSkeleton(C_BaseEntity* pEnt)
{
	Vector Hitboxes[19];
	Vector2D HitboxW2S[19];

	for (int Hitbox = 0; Hitbox < 19; Hitbox++)
	{
		Hitboxes[Hitbox] = pEnt->GetHitboxPosition(Hitbox, g_Aimbot.Matrix[pEnt->EntIndex()]);
		Hitbox == 15 ? Utils::WorldToScreen(Hitboxes[Hitbox] + Vector(0, 0, 3), HitboxW2S[Hitbox]) : Utils::WorldToScreen(Hitboxes[Hitbox], HitboxW2S[Hitbox]);
	}

	// Spine
	g_pSurface->Line(HitboxW2S[HITBOX_HEAD].x, HitboxW2S[HITBOX_HEAD].y, HitboxW2S[HITBOX_UPPER_CHEST].x, HitboxW2S[HITBOX_UPPER_CHEST].y, Color(255, 255, 255));
	g_pSurface->Line(HitboxW2S[HITBOX_UPPER_CHEST].x, HitboxW2S[HITBOX_UPPER_CHEST].y, HitboxW2S[HITBOX_LOWER_CHEST].x, HitboxW2S[HITBOX_LOWER_CHEST].y, Color(255, 255, 255));
	g_pSurface->Line(HitboxW2S[HITBOX_LOWER_CHEST].x, HitboxW2S[HITBOX_LOWER_CHEST].y, HitboxW2S[HITBOX_THORAX].x, HitboxW2S[HITBOX_THORAX].y, Color(255, 255, 255));
	g_pSurface->Line(HitboxW2S[HITBOX_THORAX].x, HitboxW2S[HITBOX_THORAX].y, HitboxW2S[HITBOX_BELLY].x, HitboxW2S[HITBOX_BELLY].y, Color(255, 255, 255));
	g_pSurface->Line(HitboxW2S[HITBOX_BELLY].x, HitboxW2S[HITBOX_BELLY].y, HitboxW2S[HITBOX_PELVIS].x, HitboxW2S[HITBOX_PELVIS].y, Color(255, 255, 255));

	// Right Leg
	g_pSurface->Line(HitboxW2S[HITBOX_PELVIS].x, HitboxW2S[HITBOX_PELVIS].y, HitboxW2S[HITBOX_RIGHT_THIGH].x, HitboxW2S[HITBOX_RIGHT_THIGH].y, Color(255, 255, 255));
	g_pSurface->Line(HitboxW2S[HITBOX_RIGHT_THIGH].x, HitboxW2S[HITBOX_RIGHT_THIGH].y, HitboxW2S[HITBOX_RIGHT_CALF].x, HitboxW2S[HITBOX_RIGHT_CALF].y, Color(255, 255, 255));
	g_pSurface->Line(HitboxW2S[HITBOX_RIGHT_CALF].x, HitboxW2S[HITBOX_RIGHT_CALF].y, HitboxW2S[HITBOX_RIGHT_FOOT].x, HitboxW2S[HITBOX_RIGHT_FOOT].y, Color(255, 255, 255));

	// Right Arm
	g_pSurface->Line(HitboxW2S[HITBOX_UPPER_CHEST].x, HitboxW2S[HITBOX_UPPER_CHEST].y, HitboxW2S[HITBOX_RIGHT_UPPER_ARM].x, HitboxW2S[HITBOX_RIGHT_UPPER_ARM].y, Color(255, 255, 255));
	g_pSurface->Line(HitboxW2S[HITBOX_RIGHT_UPPER_ARM].x, HitboxW2S[HITBOX_RIGHT_UPPER_ARM].y, HitboxW2S[HITBOX_RIGHT_FOREARM].x, HitboxW2S[HITBOX_RIGHT_FOREARM].y, Color(255, 255, 255));
	g_pSurface->Line(HitboxW2S[HITBOX_RIGHT_FOREARM].x, HitboxW2S[HITBOX_RIGHT_FOREARM].y, HitboxW2S[HITBOX_RIGHT_HAND].x, HitboxW2S[HITBOX_RIGHT_HAND].y, Color(255, 255, 255));

	// Left Leg
	g_pSurface->Line(HitboxW2S[HITBOX_PELVIS].x, HitboxW2S[HITBOX_PELVIS].y, HitboxW2S[HITBOX_LEFT_THIGH].x, HitboxW2S[HITBOX_LEFT_THIGH].y, Color(255, 255, 255));
	g_pSurface->Line(HitboxW2S[HITBOX_LEFT_THIGH].x, HitboxW2S[HITBOX_LEFT_THIGH].y, HitboxW2S[HITBOX_LEFT_CALF].x, HitboxW2S[HITBOX_LEFT_CALF].y, Color(255, 255, 255));
	g_pSurface->Line(HitboxW2S[HITBOX_LEFT_CALF].x, HitboxW2S[HITBOX_LEFT_CALF].y, HitboxW2S[HITBOX_LEFT_FOOT].x, HitboxW2S[HITBOX_LEFT_FOOT].y, Color(255, 255, 255));

	// Left Arm
	g_pSurface->Line(HitboxW2S[HITBOX_UPPER_CHEST].x, HitboxW2S[HITBOX_UPPER_CHEST].y, HitboxW2S[HITBOX_LEFT_UPPER_ARM].x, HitboxW2S[HITBOX_LEFT_UPPER_ARM].y, Color(255, 255, 255));
	g_pSurface->Line(HitboxW2S[HITBOX_LEFT_UPPER_ARM].x, HitboxW2S[HITBOX_LEFT_UPPER_ARM].y, HitboxW2S[HITBOX_LEFT_FOREARM].x, HitboxW2S[HITBOX_LEFT_FOREARM].y, Color(255, 255, 255));
	g_pSurface->Line(HitboxW2S[HITBOX_LEFT_FOREARM].x, HitboxW2S[HITBOX_LEFT_FOREARM].y, HitboxW2S[HITBOX_LEFT_HAND].x, HitboxW2S[HITBOX_LEFT_HAND].y, Color(255, 255, 255));
}
void RemasteredEsp::Render()
{
	if (!g::pLocalEntity)
		return;

	for (int i = 1; i < g_pEntityList->GetHighestEntityIndex(); i++)
	{
		C_BaseEntity* pEnt = g_pEntityList->GetClientEntity(i);

		if (!pEnt
			|| !pEnt->IsAlive()
			|| pEnt == g::pLocalEntity
			|| pEnt->GetTeam() == g::pLocalEntity->GetTeam()
			|| pEnt->IsDormant()
			|| !pEnt->IsPlayer())
			continue;

		Vector HeadPosition = pEnt->GetHitboxPosition(HITBOX_HEAD, g_Aimbot.Matrix[pEnt->EntIndex()]);
		Vector2D ScreenHeadPosition;
		Utils::WorldToScreen(HeadPosition + Vector(0, 0, 5), ScreenHeadPosition);
		Vector EntOrigin = pEnt->GetOrigin();
		Vector LocalOrigin = g::pLocalEntity->GetOrigin();

		float EntDistance = sqrtf((EntOrigin.x - LocalOrigin.x) * (EntOrigin.x - LocalOrigin.x) + (EntOrigin.y - LocalOrigin.y) * (EntOrigin.y - LocalOrigin.y) + (EntOrigin.z - LocalOrigin.z) * (EntOrigin.z - LocalOrigin.z)) * 0.01905f;

		int EspWidth = 60 / (EntDistance / 5);

		PlayerInfo_t pInfo;
		g_pEngine->GetPlayerInfo(pEnt->EntIndex(), &pInfo);

		if (g_Menu.Config.Esp == 1 && g::pLocalEntity->IsAlive())
		{
			if (g_Menu.Config.Name)
			{
				// Name
				RECT TextSize = g_pSurface->GetTextSizeRect(g::Tahoma, pInfo.szName);
				g_pSurface->DrawT(ScreenHeadPosition.x, ScreenHeadPosition.y - 20, Color(255, 255, 255), g::ReEspText, true, pInfo.szName);
			}
			if (g_Menu.Config.SkeletonEsp)
			{
				// Extra
				RenderSkeleton(pEnt);
			}
		}
		else if (g_Menu.Config.Esp == 2 && !g::pLocalEntity->IsAlive())
		{
			if (g_Menu.Config.Name)
			{
				// Name
				RECT TextSize = g_pSurface->GetTextSizeRect(g::Tahoma, pInfo.szName);
				g_pSurface->DrawT(ScreenHeadPosition.x, ScreenHeadPosition.y - 20, Color(255, 255, 255), g::ReEspText, true, pInfo.szName);
			}
			if (g_Menu.Config.SkeletonEsp)
			{
				// Extra
				RenderSkeleton(pEnt);
			}
		}
	}
}