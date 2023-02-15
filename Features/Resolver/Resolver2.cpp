#include "Resolver2.h"
#include "..\Aimbot\Aimbot.h"
#include "..\Aimbot\Autowall.h"
#include "..\Aimbot\LagComp.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\Utils\Math.h"
#include "..\..\Menu\Menu.h"
#include "..\..\Menu\config.h"
#include "../../Utils/xor.h"
#include "../AntiAim/AntiAim.h"
#include "../../SDK/ICvar.h"
#include "../ProfileChanger/ProfileChanger/MemAlloc.h"
#include "../../SDK/IClientState.h"
ResolverBeta g_ResolverBeta;

/*
my attempt at fixing desync and i was pretty successful
it can resolve static desync pretty perfectly
and can resolve some jitter desync but
it still gets rekt by other things
*/
void ResolverBeta::AnimationFix(C_BaseEntity* pEnt)
{
	//who needs structs or classes not me lol
	static float oldSimtime[65];
	static float storedSimtime[65];
	static float ShotTime[65];
	static float SideTime[65][3];
	static int LastDesyncSide[65];
	static bool Delaying[65];
	static AnimationLayer StoredLayers[64][15];
	static C_AnimState* StoredAnimState[65];
	static float StoredPosParams[65][24];
	static Vector oldEyeAngles[65];
	static float oldGoalfeetYaw[65];
	float* PosParams = (float*)((uintptr_t)pEnt + 0x2774);
	bool update = false;
	bool shot = false;

	static bool jittering[65];

	auto* AnimState = pEnt->AnimState();
	if (!AnimState || !pEnt->AnimOverlays() || !PosParams)
		return;

	auto RemapVal = [](float val, float A, float B, float C, float D) -> float
	{
		if (A == B)
			return val >= B ? D : C;
		return C + (D - C) * (val - A) / (B - A);
	};

	if (storedSimtime[pEnt->EntIndex()] != pEnt->GetSimulationTime())
	{
		jittering[pEnt->EntIndex()] = false;
		pEnt->ClientAnimations(true);
		pEnt->UpdateClientAnimation();

		memcpy(StoredPosParams[pEnt->EntIndex()], PosParams, sizeof(float) * 24);
		memcpy(StoredLayers[pEnt->EntIndex()], pEnt->AnimOverlays(), (sizeof(AnimationLayer) * pEnt->NumOverlays()));

		oldGoalfeetYaw[pEnt->EntIndex()] = AnimState->m_flGoalFeetYaw;

		if (pEnt->GetActiveWeapon() && !pEnt->IsKnifeorNade())
		{
			if (ShotTime[pEnt->EntIndex()] != pEnt->GetActiveWeapon()->GetLastShotTime())
			{
				shot = true;
				ShotTime[pEnt->EntIndex()] = pEnt->GetActiveWeapon()->GetLastShotTime();
			}
			else
				shot = false;
		}
		else
		{
			shot = false;
			ShotTime[pEnt->EntIndex()] = 0.f;
		}

		float angToLocal = g_Math.NormalizeYaw(g_Math.CalcAngle(g::pLocalEntity->GetOrigin(), pEnt->GetOrigin()).y);

		float Back = g_Math.NormalizeYaw(angToLocal);
		float DesyncFix = 0;
		float Resim = g_Math.NormalizeYaw((0.24f / (pEnt->GetSimulationTime() - oldSimtime[pEnt->EntIndex()])) * (oldEyeAngles[pEnt->EntIndex()].y - pEnt->GetEyeAngles().y));

		if (Resim > 58.f)
			Resim = 58.f;
		if (Resim < -58.f)
			Resim = -58.f;

		if (pEnt->GetVelocity().Length2D() > 0.5f && !shot)
		{
			float Delta = g_Math.NormalizeYaw(g_Math.NormalizeYaw(g_Math.CalcAngle(Vector(0, 0, 0), pEnt->GetVelocity()).y) - g_Math.NormalizeYaw(g_Math.NormalizeYaw(AnimState->m_flGoalFeetYaw + RemapVal(PosParams[11], 0, 1, -60, 60)) + Resim));

			int CurrentSide = 0;

			if (Delta < 0)
			{
				CurrentSide = 1;
				SideTime[pEnt->EntIndex()][1] = g_pGlobalVars->curtime;
			}
			else if (Delta > 0)
			{
				CurrentSide = 2;
				SideTime[pEnt->EntIndex()][2] = g_pGlobalVars->curtime;
			}

			if (LastDesyncSide[pEnt->EntIndex()] == 1)
			{
				Resim += (58.f - Resim);
				DesyncFix += (58.f - Resim);
			}
			if (LastDesyncSide[pEnt->EntIndex()] == 2)
			{
				Resim += (-58.f - Resim);
				DesyncFix += (-58.f - Resim);
			}

			if (LastDesyncSide[pEnt->EntIndex()] != CurrentSide)
			{
				Delaying[pEnt->EntIndex()] = true;

				if (.5f < (g_pGlobalVars->curtime - SideTime[pEnt->EntIndex()][LastDesyncSide[pEnt->EntIndex()]]))
				{
					LastDesyncSide[pEnt->EntIndex()] = CurrentSide;
					Delaying[pEnt->EntIndex()] = false;
				}
			}

			if (!Delaying[pEnt->EntIndex()])
				LastDesyncSide[pEnt->EntIndex()] = CurrentSide;
		}
		else if (!shot)
		{
			float Brute = UseFreestandAngle[pEnt->EntIndex()] ? g_Math.NormalizeYaw(Back + FreestandAngle[pEnt->EntIndex()]) : pEnt->GetLowerBodyYaw();

			float Delta = g_Math.NormalizeYaw(g_Math.NormalizeYaw(Brute - g_Math.NormalizeYaw(g_Math.NormalizeYaw(AnimState->m_flGoalFeetYaw + RemapVal(PosParams[11], 0, 1, -60, 60))) + Resim));

			if (Delta > 58.f)
				Delta = 58.f;
			if (Delta < -58.f)
				Delta = -58.f;

			Resim += Delta;
			DesyncFix += Delta;

			if (Resim > 58.f)
				Resim = 58.f;
			if (Resim < -58.f)
				Resim = -58.f;
		}

		float Equalized = g_Math.NormalizeYaw(g_Math.NormalizeYaw(AnimState->m_flGoalFeetYaw + RemapVal(PosParams[11], 0, 1, -60, 60)) + Resim);

		float JitterDelta = fabs(g_Math.NormalizeYaw(oldEyeAngles[pEnt->EntIndex()].y - pEnt->GetEyeAngles().y));

		if (JitterDelta >= 70.f && !shot)
			jittering[pEnt->EntIndex()] = true;

		if (pEnt != g::pLocalEntity && pEnt->GetTeam() != g::pLocalEntity->GetTeam() && (pEnt->GetFlags() & FL_ONGROUND) && g_Menu.Config.Resolver)
		{
			if (jittering[pEnt->EntIndex()])
				AnimState->m_flGoalFeetYaw = g_Math.NormalizeYaw(pEnt->GetEyeAngles().y + DesyncFix);
			else
				AnimState->m_flGoalFeetYaw = Equalized;

			pEnt->SetLowerBodyYaw(AnimState->m_flGoalFeetYaw);
		}

		StoredAnimState[pEnt->EntIndex()] = AnimState;

		oldEyeAngles[pEnt->EntIndex()] = pEnt->GetEyeAngles();

		oldSimtime[pEnt->EntIndex()] = storedSimtime[pEnt->EntIndex()];

		storedSimtime[pEnt->EntIndex()] = pEnt->GetSimulationTime();

		update = true;
	}

	pEnt->ClientAnimations(false);

	if (pEnt != g::pLocalEntity && pEnt->GetTeam() != g::pLocalEntity->GetTeam() && (pEnt->GetFlags() & FL_ONGROUND) && g_Menu.Config.Resolver)
		pEnt->SetLowerBodyYaw(AnimState->m_flGoalFeetYaw);

	AnimState = StoredAnimState[pEnt->EntIndex()];

	memcpy((void*)PosParams, &StoredPosParams[pEnt->EntIndex()], (sizeof(float) * 24));
	memcpy(pEnt->AnimOverlays(), StoredLayers[pEnt->EntIndex()], (sizeof(AnimationLayer) * pEnt->NumOverlays()));

	if (pEnt != g::pLocalEntity && pEnt->GetTeam() != g::pLocalEntity->GetTeam() && (pEnt->GetFlags() & FL_ONGROUND) && g_Menu.Config.Resolver && jittering[pEnt->EntIndex()])
		pEnt->SetAbsAngles(Vector(0, pEnt->GetEyeAngles().y, 0));
	else
		pEnt->SetAbsAngles(Vector(0, oldGoalfeetYaw[pEnt->EntIndex()], 0));
}





float flAngleMod(float flAngle)
{
	return((360.0f / 65536.0f) * ((int32_t)(flAngle * (65536.0f / 360.0f)) & 65535));
}
float ApproachAngle(float target, float value, float speed)
{
	target = flAngleMod(target);
	value = flAngleMod(value);

	float delta = target - value;

	// Speed is assumed to be positive
	if (speed < 0)
		speed = -speed;

	if (delta < -180)
		delta += 360;
	else if (delta > 180)
		delta -= 360;

	if (delta > speed)
		value += speed;
	else if (delta < -speed)
		value -= speed;
	else
		value = target;

	return value;
}
/*


*/
float ResolverBeta::sub_59B13C30(C_AnimState* nn)
{
	if (nn->pBaseEntity != g::pLocalEntity)
	{
		float v1;  // xmm0_4
		float v2;  // xmm1_4
		float v3;  // xmm0_4
		int v4;    // eax
		float v5;  // xmm4_4
		float v6;  // xmm2_4
		float v7;  // xmm0_4
		int v8;	   // eax
		float v10; // [esp+0h] [ebp-Ch]
		float v11; // [esp+4h] [ebp-8h]

		v1 = nn->m_flFeetSpeedForwardsOrSideWays;
		v2 = 1.0;
		v10 = 0.0;
		v11 = v1;
		if (v1 <= 1.f)
		{
			v4 = v10;
			if (v1 >= 0.0)
				v4 = v11;
			v3 = v4;
		}
		else
			v3 = 1.f;

		v5 = nn->m_fDuckAmount;
		v6 = ((nn->m_flStopToFullRunningFraction * -0.30000001f) - 0.19999999f) * v3 + 1.0f;
		if (v5 > 0.0)
		{
			v7 = nn->m_flFeetSpeedUnknownForwardOrSideways;
			v11 = 0.0;
			v10 = v7;
			if (v7 <= 1.0)
			{
				v8 = v11;
				if (v7 >= 0.0)
					v8 = v10;
				v2 = v8;
			}
			v6 = v6 + (float)((float)(v2 * v5) * (float)(0.5f - v6));
		}

		return (nn->max_yaw * v6);
	}
	/*else
		return /*get_max_desync_delta(cheat::main::local()); (cheat::features::antiaimbot.max_delta - .1f);//((*(float*)((uintptr_t)nn + 0x334))  v6);*/
}
void HandleBackUpResolve(C_BaseEntity* pEnt) {



	if (pEnt->GetTeam() == g::pLocalEntity->GetTeam())
		return;


	auto animstate = pEnt->AnimState();

	const auto player_animation_state = pEnt->AnimState();

	float flMaxYawModifier = 0.f;
	flMaxYawModifier = player_animation_state->pad10[516] * flMaxYawModifier;
	float flMinYawModifier = player_animation_state->pad10[512] * flMaxYawModifier;

	float newFeetYaw = 1.f;

	auto eyeYaw = player_animation_state->m_flEyeYaw;

	auto lbyYaw = player_animation_state->m_flGoalFeetYaw;

	float eye_feet_delta = fabs(eyeYaw - lbyYaw);


	if (!player_animation_state)
		return;

	float m_flLastClientSideAnimationUpdateTimeDelta = fabs(player_animation_state->m_iLastClientSideAnimationUpdateFramecount - player_animation_state->m_flLastClientSideAnimationUpdateTime);

	auto v28 = 0.f;

	if (player_animation_state->m_flFeetSpeedForwardsOrSideWays >= 0.0f)
	{
		v28 = fminf(player_animation_state->m_flFeetSpeedForwardsOrSideWays, 0.0f);
	}
	else
	{
		v28 = 0x3F800000;
	}
	float v49 = 0.0f;
	v49 = ((player_animation_state->m_flStopToFullRunningFraction * -0.30000001) - 0.19999999) * v49;

	float flYawModifier = v49 + 1.0;

	if (player_animation_state->m_fDuckAmount > 0.0)
	{
		float v53 = 0.0f;

		if (player_animation_state->m_flFeetSpeedUnknownForwardOrSideways >= 0.0)
		{
			v53 = fminf(player_animation_state->m_flFeetSpeedUnknownForwardOrSideways, 1.0);
		}
		else
		{
			v53 = 0.0f;
		}
	}

	if (eye_feet_delta <= flMaxYawModifier)
	{
		if (flMinYawModifier > eye_feet_delta)
		{
			newFeetYaw = fabs(flMinYawModifier) + eyeYaw;
		}
	}
	else
	{
		newFeetYaw = eyeYaw - fabs(flMaxYawModifier);
	}


	float v136 = fmod(newFeetYaw, 360.0);

	if (v136 > 180.0)
	{
		v136 = v136 - 360.0;
	}

	if (v136 < 180.0)
	{
		v136 = v136 + 360.0;
	}

	v28 = v49++;

	{                                             // inlined max_desync_delta
		float v9 = fabs(animstate->m_iLastClientSideAnimationUpdateFramecount - animstate->m_flLastClientSideAnimationUpdateTime);
		float speedfraction = 0.0;
		if (animstate->m_flFeetSpeedForwardsOrSideWays < 0.0)
			speedfraction = 0.0;
		else
			speedfraction = animstate->m_flFeetSpeedForwardsOrSideWays;

		float v2 = (animstate->m_flStopToFullRunningFraction * -0.30000001 - 0.19999999) * speedfraction;
		float v18 = v2;
		float v3 = v2 + 1.0;
		float v23 = v3;
		if (animstate->m_fDuckAmount > 0.0)
		{
			float v29 = 0.0;
			if (animstate->m_flFeetSpeedUnknownForwardOrSideways < 0.0)
				v29 = 0.0;
			else
				v29 = fminf((animstate->m_flFeetSpeedUnknownForwardOrSideways), 0x3F800000);
		}

		if (g::pLocalEntity)
		{
			for (int i = 1; i < 64; i++)
			{

				if (pEnt)// dormant
				{
					float v28 = pEnt->GetEyeAngles().y == 0.0 ? -58 : 58;
					if (v28)
						return;
					float v27 = pEnt->GetEyeAngles().y == 0.0 ? -89 : 89;
					if (v27)
						return;
					float v26 = pEnt->GetEyeAngles().y == 0.0 ? -79 : 79;
					if (v26)
						return;
					float v25 = pEnt->GetEyeAngles().y == 0.0 ? -125 : 125;
					if (v25)
						return;
					float v24 = pEnt->GetEyeAngles().y == 0.0 ? -78 : 78;
					if (v24)
						return;
				}
				float v8 = 0;
				float v7 = 0;
				float v6 = 0;
				for (size_t i = 0; i < pEnt->GetNumAnimOverlays(); i++)
				{
					auto animLayer = pEnt->GetAnimOverlay4(i);
					if (!animLayer)
						continue;
					if (pEnt->GetSequenceActivity(animLayer->m_nSequence) == 979);
					auto v6 = pEnt->GetLowerBodyYaw();
				}
				float v20 = (animstate->m_vVelocityX) * v23;
				float a1 = (animstate->m_vVelocityY) * v23;
				float v30 = 0.0;
				float eye_angles_y = animstate->m_flEyeYaw;
				float goal_feet_yaw = animstate->m_flGoalFeetYaw;
				float v22 = fabs(eye_angles_y - goal_feet_yaw);
				if (v20 < v22)
				{
					float v11 = fabs(v20);
					v30 = eye_angles_y - v11;
				}
				else if (a1 > v22)
				{
					float v12 = fabs(a1);
					v30 = v12 + eye_angles_y;
				}
				float v36 = std::fmodf((v30), 360.0);
				if (v36 > 180.0)
					v36 = v36 - 360.0;
				if (v36 < 180.0)
					v36 = v36 + 360.0;
				animstate->m_flGoalFeetYaw = v36;
				if (g::MissedShots[pEnt->EntIndex()] > 2)
				{
					int v19 = g::MissedShots[pEnt->EntIndex()] % 4;
					switch (v19)
					{
					case 0:
						animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw + 52.0;
						break;
					case 1:
						animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw - 52.0;
						break;
					case 2:
						animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw - 30.0;
						break;
					case 3:
						animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw + 30.0;
						break;
					default:
						return;




						player_animation_state->m_flGoalFeetYaw = v136;
					}
				}
			}

		}
	}
}



void HandleHits(C_BaseEntity* pEnt)
{
	auto NetChannel = g_pEngine->GetNetChannelInfo();

	if (!NetChannel)
		return;

	static float predTime[65];
	static bool init[65];

	if (g::Shot[pEnt->EntIndex()])
	{
		if (init[pEnt->EntIndex()])
		{
			g_ResolverBeta.pitchHit[pEnt->EntIndex()] = pEnt->GetEyeAngles().x;
			predTime[pEnt->EntIndex()] = g_pGlobalVars->curtime + NetChannel->GetAvgLatency(FLOW_INCOMING) + NetChannel->GetAvgLatency(FLOW_OUTGOING) + TICKS_TO_TIME(1) + TICKS_TO_TIME(g_pClientState->m_net_channel->m_nChokedPackets);
			init[pEnt->EntIndex()] = false;
		}

		if (g_pGlobalVars->curtime > predTime[pEnt->EntIndex()] && !g::Hit[pEnt->EntIndex()])
		{
			g::MissedShots[pEnt->EntIndex()] += 1;
			g::Shot[pEnt->EntIndex()] = false;
		}
		else if (g_pGlobalVars->curtime <= predTime[pEnt->EntIndex()] && g::Hit[pEnt->EntIndex()])
			g::Shot[pEnt->EntIndex()] = false;

	}
	else
		init[pEnt->EntIndex()] = true;

	g::Hit[pEnt->EntIndex()] = false;
}
using namespace std;
void ResolverBeta::ResolveLegit()
{
	/*auto final_feet_yaw = 0.0f;
	auto old_feet_yaw = 0.0f;
	auto eye_lby_delta = 0.0f;
	for (int i = 1; i < g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* entity = g_pEntityList->GetClientEntity(i);

		if (!entity
			|| !g::pLocalEntity
			|| !entity->IsAlive()
			|| entity == g::pLocalEntity
			|| entity->GetTeam() == g::pLocalEntity->GetTeam())
			continue;

		PlayerInfo_t player_info;
		interfaces::engine->get_player_info(entity->EntIndex(), &player_info);

		if (player_info.fakeplayer) // skip bots
			continue;

		auto anim_state = entity->AnimState();
		if (!anim_state)
			return;

		final_feet_yaw[entity->EntIndex()] = clamp(Math::NormalizeYaw(anim_state->m_flGoalFeetYaw), -180.f, 180.f);

		old_feet_yaw[entity->EntIndex()] = std::clamp(math::normalize_viewangles_yaw_float(old_feet_yaw[entity->EntIndex()]), -180.f, 180.f);

		eye_lby_delta[entity->EntIndex()] = math::normalize_viewangles_yaw_float(entity->GetEyeAngles().y - entity->GetLowerBodyYaw());

		if (fabs(eye_lby_delta[entity->EntIndex()]) > 35.f && entity->GetVelocity().Length2D() <= 2.f)
		{
			final_feet_yaw[entity->index()] += std::clamp(eye_lby_delta[entity->index()], -58.f, 58.f);
			old_feet_yaw[entity->index()] = final_feet_yaw[entity->index()];
		}

		anim_state->goal_feet_yaw = final_feet_yaw[entity->index()];
	}*/
}
void ResolverBeta::OnCreateMove() // cancer v2
{
	if (!g_Menu.Config.Resolver)
		return;

	if (!g::pLocalEntity->GetActiveWeapon() || g::pLocalEntity->IsKnifeorNade())
		return;


	for (int i = 1; i < 64; i++)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant()
			|| pPlayerEntity == g::pLocalEntity
			|| pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
		{
			UseFreestandAngle[i] = false;
			continue;
		}

		if (abs(pPlayerEntity->GetVelocity().Length2D()) > 29.f)
			UseFreestandAngle[pPlayerEntity->EntIndex()] = false;

		if (abs(pPlayerEntity->GetVelocity().Length2D()) <= 29.f && !UseFreestandAngle[pPlayerEntity->EntIndex()])
		{
			bool Autowalled = false, HitSide1 = false, HitSide2 = false;

			float angToLocal = g_Math.CalcAngle(g::pLocalEntity->GetOrigin(), pPlayerEntity->GetOrigin()).y;
			Vector ViewPoint = g::pLocalEntity->GetOrigin() + Vector(0, 0, 90);

			Vector2D Side1 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal))),(45 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
			Vector2D Side2 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(45 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

			Vector2D Side3 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal))),(50 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
			Vector2D Side4 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(50 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

			Vector Origin = pPlayerEntity->GetOrigin();

			Vector2D OriginLeftRight[] = { Vector2D(Side1.x, Side1.y), Vector2D(Side2.x, Side2.y) };

			Vector2D OriginLeftRightLocal[] = { Vector2D(Side3.x, Side3.y), Vector2D(Side4.x, Side4.y) };
			for (int side = 0; side < 2; side++)
			{
				Vector OriginAutowall = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 90 };
				Vector OriginAutowall2 = { ViewPoint.x + OriginLeftRightLocal[side].x,  ViewPoint.y - OriginLeftRightLocal[side].y , ViewPoint.z };

				if (g_Autowall.CanHitFloatingPoint(OriginAutowall, ViewPoint))
				{
					if (side == 0)
					{
						HitSide1 = true;
						FreestandAngle[pPlayerEntity->EntIndex()] = 90;
					}
					else if (side == 1)
					{
						HitSide2 = true;
						FreestandAngle[pPlayerEntity->EntIndex()] = -90;
					}

					Autowalled = true;
				}
				else
				{
					for (int side222 = 0; side222 < 2; side222++)
					{
						Vector OriginAutowall222 = { Origin.x + OriginLeftRight[side222].x,  Origin.y - OriginLeftRight[side222].y , Origin.z + 90 };

						if (g_Autowall.CanHitFloatingPoint(OriginAutowall222, OriginAutowall2))
						{
							if (side222 == 0)
							{
								HitSide1 = true;
								FreestandAngle[pPlayerEntity->EntIndex()] = 90;
							}
							else if (side222 == 1)
							{
								HitSide2 = true;
								FreestandAngle[pPlayerEntity->EntIndex()] = -90;
							}

							Autowalled = true;
						}
					}
				}
			}

			if (Autowalled)
			{
				if (HitSide1 && HitSide2)
					UseFreestandAngle[pPlayerEntity->EntIndex()] = false;
				else
					UseFreestandAngle[pPlayerEntity->EntIndex()] = true;
			}
		}
	}
}

void AimFix(C_BaseEntity* entity)
{
	if (!entity)
		return;

	auto old_curtime = g_pGlobalVars->curtime;
	auto old_frametime = g_pGlobalVars->frametime;
	auto old_fraction = entity->AnimState()->m_flUnknownFraction = 0.f;

	g_pGlobalVars->curtime = entity->GetSimulationTime();
	g_pGlobalVars->frametime = g_pGlobalVars->intervalPerTick;

	AnimationLayer backup_layers[15];
	std::memcpy(backup_layers, entity->AnimOverlays(), (sizeof(AnimationLayer) * 15));

	if (entity->AnimState())
		entity->AnimState()->m_iLastClientSideAnimationUpdateFramecount = g_pGlobalVars->framecount - 1;

	entity->GetClientSideAnimation() = true;
	entity->UpdateClientAnimation();
	entity->GetClientSideAnimation() = false;

	std::memcpy(entity->AnimOverlays(), backup_layers, (sizeof(AnimationLayer) * 15));

	g_pGlobalVars->curtime = old_curtime;
	g_pGlobalVars->frametime = old_frametime;

	entity->AnimState()->m_flUnknownFraction = old_fraction;

	entity->SetAbsAngles(Vector(0, entity->AnimState()->m_flGoalFeetYaw, g::RealAngle.z));
}
void ApplyLocalPlayer() {
	auto animstate = g::pLocalEntity->AnimState();
	if (!animstate)
		return;

	const auto backup_frametime = g_pGlobalVars->frametime;
	const auto backup_curtime = g_pGlobalVars->curtime;

	animstate->m_flGoalFeetYaw = g::RealAngle.y;

	if (animstate->m_iLastClientSideAnimationUpdateFramecount == g_pGlobalVars->framecount)
		animstate->m_iLastClientSideAnimationUpdateFramecount -= 1.f;

	g_pGlobalVars->frametime = g_pGlobalVars->intervalPerTick;
	g_pGlobalVars->curtime = g::pLocalEntity->GetSimulationTime();

	g::pLocalEntity->EFlags() &= ~0x1000;
	g::pLocalEntity->GetAbsVelocity() = g::pLocalEntity->GetVelocity();

	static float angle = animstate->m_flGoalFeetYaw;

	animstate->m_flFeetYawRate = 0.f;

	AnimationLayer backup_layers[13];
	if (g::pLocalEntity->GetSimulationTime() != g::pLocalEntity->GetOldSimulationTime())
	{
		std::memcpy(backup_layers, g::pLocalEntity->AnimOverlays(),
			(sizeof(AnimationLayer) * g::pLocalEntity->GetNumAnimOverlays()));

		g::pLocalEntity->GetClientSideAnimation() = true;
		g::pLocalEntity->UpdateAnimationState(animstate, g::FakeAngle);
		g::pLocalEntity->UpdateClientSideAnimation();
		g::pLocalEntity->GetClientSideAnimation() = false;
		angle = animstate->m_flGoalFeetYaw;

		std::memcpy(g::pLocalEntity->AnimOverlays(), backup_layers,
			(sizeof(AnimationLayer) * g::pLocalEntity->GetNumAnimOverlays()));
	}
	animstate->m_flGoalFeetYaw = angle;
	g_pGlobalVars->curtime = backup_curtime;
	g_pGlobalVars->frametime = backup_frametime;
}

static void create_state(C_AnimState* state, C_BaseEntity* e)
{
	using Fn = void(__thiscall*)(C_AnimState*, C_BaseEntity*);
	static auto fn = reinterpret_cast <Fn> (Utils::FindSignature(SteamStrings("client.dll"), SteamStrings("55 8B EC 56 8B F1 B9 ? ? ? ? C7 46")));

	fn(state, e);
}

static void update_state(C_AnimState* state, const Vector& angles)
{
	using Fn = void(__vectorcall*)(void*, void*, float, float, float, void*);
	static auto fn = reinterpret_cast <Fn> (Utils::FindSignature(SteamStrings("client.dll"), SteamStrings("55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24")));

	fn(state, nullptr, 0.0f, angles[1], angles[0], nullptr);
}

static void reset_state(C_AnimState* state)
{
	using Fn = void(__thiscall*)(C_AnimState*);
	static auto fn = reinterpret_cast <Fn> (Utils::FindSignature(SteamStrings("client.dll"), SteamStrings("56 6A 01 68 ? ? ? ? 8B F1")));

	fn(state);
}
int C_BaseEntity::animlayer_count()
{
	if (!this) //-V704
		return 0;

	return *(int*)((DWORD)this + 0x298C);
}
static int find_in_datamap(datamap_t* map, const char* name)
{
	while (map)
	{
		for (auto i = 0; i < map->dataNumFields; ++i)
		{
			if (!map->dataDesc[i].fieldName)
				continue;

			if (!strcmp(name, map->dataDesc[i].fieldName))
				return map->dataDesc[i].fieldOffset[TD_OFFSET_NORMAL];

			if (map->dataDesc[i].fieldType == 10)
			{
				if (map->dataDesc[i].td)
				{
					unsigned int offset;

					if (offset = find_in_datamap(map->dataDesc[i].td, name))
						return offset;
				}
			}
		}

		map = map->baseMap;
	}

	return 0;
}
uint32_t& C_BaseEntity::m_fEffects()
{
	static auto m_fEffects = find_in_datamap(GetPredDescMap(), SteamStrings("m_fEffects"));
	return *(uint32_t*)(uintptr_t(this) + m_fEffects);
}
uint32_t& C_BaseEntity::m_iMostRecentModelBoneCounter()
{
	static auto invalidate_bone_cache = Utils::FindSignature(SteamStrings("client.dll"), SteamStrings("80 3D ?? ?? ?? ?? ?? 74 16 A1 ?? ?? ?? ?? 48 C7 81"));
	static auto most_recent_model_bone_counter = *(uintptr_t*)(invalidate_bone_cache + 0x1B);

	return *(uint32_t*)((uintptr_t)this + most_recent_model_bone_counter);
}
float& C_BaseEntity::m_flLastBoneSetupTime()
{
	static auto invalidate_bone_cache = Utils::FindSignature(SteamStrings("client.dll"), SteamStrings("80 3D ?? ?? ?? ?? ?? 74 16 A1 ?? ?? ?? ?? 48 C7 81"));
	static auto last_bone_setup_time = *(uintptr_t*)(invalidate_bone_cache + 0x11);

	return *(float*)((uintptr_t)this + last_bone_setup_time);
}
void C_BaseEntity::invalidate_bone_cache()
{
	if (!this) //-V704
		return;

	m_flLastBoneSetupTime() = -FLT_MAX;
	m_iMostRecentModelBoneCounter() = UINT_MAX;
}
bool C_BaseEntity::setup_bones_fixed(matrix3x4_t* matrix, int mask)
{
	if (!this) //-V704
		return false;

	auto setuped = false;

	auto backup_value = *(uint8_t*)((uintptr_t)this + 0x274);
	*(uint8_t*)((uintptr_t)this + 0x274) = 0;

	auto backup_effects = m_fEffects();
	m_fEffects() |= 8;

	auto animstate = AnimState();
	auto previous_weapon = animstate ? animstate->pLastActiveWeapon : nullptr;

	if (previous_weapon)
		animstate->pLastActiveWeapon = animstate->pActiveWeapon;

	auto backup_abs_origin = GetAbsOrigin();

	if (this != g::pLocalEntity)
		SetAbsOrigin(GetOrigin());

	g::setuping_bones = true;
	invalidate_bone_cache();

	SetupBones(matrix, matrix ? 128 : -1, mask, GetSimulationTime());

	g::setuping_bones = false;

	if (this != g::pLocalEntity)
		SetAbsOrigin(backup_abs_origin);

	if (previous_weapon)
		animstate->pLastActiveWeapon = previous_weapon;

	m_fEffects() = backup_effects;
	*(uint8_t*)((uintptr_t)this + 0x274) = backup_value;

	return setuped;
}
void ResolverBeta::FrameStage(ClientFrameStage_t stage)
{
	if (!g::pLocalEntity || !g_pEngine->IsInGame())
		return;

	for (int i = 1; i < 32; i++)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity)
			continue;
		if (pPlayerEntity->IsDormant())
			continue;

		if (stage == FRAME_RENDER_START)
		{
			HandleHits(pPlayerEntity);
			AnimationFix(pPlayerEntity);

			*reinterpret_cast<int*>(uintptr_t(pPlayerEntity) + 0xA30) = g_pGlobalVars->framecount;
			*reinterpret_cast<int*>(uintptr_t(pPlayerEntity) + 0xA28) = 0;
		}

		if (stage == FRAME_NET_UPDATE_END)
		{
			/*if (!g_GameRules->IsFreezeTime())
				ApplyLocalPlayer(); //Prob broken, need to fix*/

			static auto r_jiggle_bones = g_pCvar->FindVar(SteamStrings("r_jiggle_bones"));
			if (r_jiggle_bones->GetInt() > 0)
				r_jiggle_bones->SetValue(0);

			for (int i = 1; i < 32; i++) {
				auto entity = g_pEntityList->GetClientEntity(i);
				if (g::pLocalEntity && entity != nullptr && entity->IsPlayer() && entity != g::pLocalEntity && entity->IsAlive()) {
					const auto var_map = reinterpret_cast<uintptr_t>(entity) + 36;

					for (auto index = 0; index < *reinterpret_cast<int*>(var_map + 20); index++)
						*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(var_map) + index * 12) = 0;
				}
			}
		}
	}
}