#include "..\Features.h"
#include "Resolver.h"
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
#include "..//..//Utils/Interfaces.h"
#include "..//..//SDK/ICvar.h"
#include "..///..//Utils/Interfaces.h"
#include "../../SDK/CHudChat.h"
#include "../../Utils/xor.h"
#include "Resolver.h"
#include "..\Aimbot\Autowall.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\Utils\Math.h"
#include "..\..\Menu\Menu.h"
#include "..\AntiAim\AntiAim.h"

Resolver g_Resolver;


void Resolver::lbyResolve(C_BaseEntity* pEnt, C_BaseEntity* pLocalEnt, int i)
{
	bool LastMoveLby[65];
	bool lby_updated;
	static float oldlby[65];


	if (pEnt->GetVelocity().Length2D() > 40)
	{
		LastMoveLby[i] = pEnt->GetLowerBodyYaw();


	}

	if (pEnt->GetLowerBodyYaw() != oldlby[pEnt->EntIndex()])
	{
		lby_updated = true;
		oldlby[pEnt->EntIndex()];
	}
	else
	{
		lby_updated = false;
	}
	const auto player_animation_state = pEnt->AnimState();

	//use this inside your resolver
	auto eyeYaw = player_animation_state->m_flEyeYaw;
	auto lbyYaw = player_animation_state->m_flGoalFeetYaw;
	if ((pEnt->GetVelocity().Length2D() < 0))
	{
		////lby update////
		if (lby_updated)
		{
			pEnt->SetEyeAngles(Vector(eyeYaw, lbyYaw, 0));
		}
		else
		{
			pEnt->SetEyeAngles(Vector(89, LastMoveLby[i], 0));
		}
		////////
	}
	else if ((pEnt->GetVelocity().Length2D() < 32.f))
	{
		pEnt->SetEyeAngles(Vector(eyeYaw, lbyYaw, 0));

	}
}


void Resolver::AnimationFixyes(C_BaseEntity* pEnt)
{
	auto pLocalEnt = g::pLocalEntity;

	// i rly am just to lazy pls dont judge
	static float ShotTime[65];

	static float oldSimtime[65];
	static float storedSimtime[65];

	static Vector oldEyeAngles[65];
	static float oldGoalfeetYaw[65];
	static Vector oldOrigin[65];

	float* PosParams = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(pEnt) + 0x2774);
	bool shot = false;

	auto* AnimState = pEnt->AnimState();
	if (!AnimState || !pEnt->AnimOverlays() || !PosParams)
		return;

	if (*reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(AnimState) + 0x164) < 0) // sorry had to anti pasta this basically the key to nospread
		* reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(AnimState) + 0x110) = 0.f;

	bool update = false;

	if (storedSimtime[pEnt->EntIndex()] != pEnt->GetSimulationTime())
	{
		pEnt->ClientAnimations(true);
		
		

		update = true;
	}

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

	if (pLocalEnt && pLocalEnt->IsAlive())
	{
		float angToLocal = g_Math.NormalizeYaw(g_Math.CalcAngle(pLocalEnt->GetOrigin(), pEnt->GetOrigin()).y);

		float Back = g_Math.NormalizeYaw(angToLocal);
		float DesyncFix = 0;

		float Resim = g_Math.NormalizeYaw((TICKS_TO_TIME(16) / (pEnt->GetSimulationTime() - oldSimtime[pEnt->EntIndex()])) * g_Math.NormalizeYaw(oldEyeAngles[pEnt->EntIndex()].y - pEnt->GetEyeAngles().y));

		if (Resim > 60.f)
			Resim = 60.f;
		if (Resim < -60.f)
			Resim = -60.f;

		if (!shot && !isnan(angToLocal) && !isinf(angToLocal) && pEnt != pLocalEnt && pEnt->GetTeam() != pLocalEnt->GetTeam())
		{
			float AntiSide = 0.f;

			if (g::MissedShots[pEnt->EntIndex()] % 2)
			{
				if (g_Math.NormalizeYaw(pEnt->GetEyeAngles().y - Back) > 0.f)
					AntiSide = -90.f;
				else if (g_Math.NormalizeYaw(pEnt->GetEyeAngles().y - Back) < 0.f)
					AntiSide = 90.f;
			}
			else
			{
				if (g_Math.NormalizeYaw(pEnt->GetEyeAngles().y - g_Math.NormalizeYaw(Back + 90)) > 0.f)
					AntiSide = 180.f;
				else if (g_Math.NormalizeYaw(pEnt->GetEyeAngles().y - g_Math.NormalizeYaw(Back + 90)) < 0.f)
					AntiSide = 0.f;
			}

			float Brute = g_Math.NormalizeYaw(Back + AntiSide);
			float Delta = g_Math.NormalizeYaw(g_Math.NormalizeYaw(Brute - pEnt->GetEyeAngles().y) + Resim);

			if (Delta > 60.f) // who needs clamps
				Delta = 60.f;
			if (Delta < -60.f)
				Delta = -60.f;

			Resim += Delta;
			DesyncFix += Delta;

			if (Resim > 60.f)
				Resim = 60.f;
			if (Resim < -60.f)
				Resim = -60.f;
		}

		float Equalized;

		//	if (fabs(g_Math.NormalizeYaw(oldEyeAngles[pEnt->EntIndex()].y - pEnt->GetEyeAngles().y)) < 60.f) // yea basically im retarded
		Equalized = g_Math.NormalizeYaw(pEnt->GetEyeAngles().y + Resim);
		//	else
		//		Equalized = g_Math.NormalizeYaw(pEnt->GetEyeAngles().y - Resim);

		if (!shot && pEnt != pLocalEnt && pEnt->GetTeam() != pLocalEnt->GetTeam() && (pEnt->GetFlags() & FL_ONGROUND))
			AnimState->m_flGoalFeetYaw = Equalized;
	}


	pEnt->SetAbsAngles(Vector(0, oldGoalfeetYaw[pEnt->EntIndex()], 0));
}


void Resolver::HandleHits(C_BaseEntity* pEnt, C_BaseEntity* pLocalEnt)
{
	

	auto NetChannel = g_pEngine->GetNetChannelInfo();

	if (!NetChannel)
		return;

	static float predTime[65];
	static bool init[65];

	static float StoredSimtime[65];
	static float SimDelta[65];

	if (StoredSimtime[pEnt->EntIndex()] != pEnt->GetSimulationTime())
	{
		SimDelta[pEnt->EntIndex()] = fabs(pEnt->GetSimulationTime() - StoredSimtime[pEnt->EntIndex()]);
		StoredSimtime[pEnt->EntIndex()] = pEnt->GetSimulationTime();
	}

	if (g::Shot[pEnt->EntIndex()])
	{
		if (init[pEnt->EntIndex()])
		{
			g_Resolver.pitchHit[pEnt->EntIndex()] = pEnt->GetEyeAngles().x;
			predTime[pEnt->EntIndex()] = g_pGlobalVars->curtime + NetChannel->GetAvgLatency(FLOW_INCOMING) + pLocalEnt->FireRate();  // maybe????
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



void Resolver::OnCreateMove() // kinda dont need this anymore
{
	

	if (!g_Menu.Config.Resolver)
		return;

	if (!g::pLocalEntity->IsAlive())
		return;

	if (!g::pLocalEntity->GetActiveWeapon() || g::pLocalEntity->IsKnifeorNade())
		return;


	for (int i = 1; i <= g_pEngine->GetMaxClients(); ++i)
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
						FreestandAngle[pPlayerEntity->EntIndex()] = 85;//90
					}
					else if (side == 1)
					{
						HitSide2 = true;
						FreestandAngle[pPlayerEntity->EntIndex()] = -85;//-90
					}

					Autowalled = true;
				}
				else
				{
					for (int side222 = 0; side222 < 2; side222++)
					{
						Vector OriginAutowall222 = { Origin.x + OriginLeftRight[side222].x,  Origin.y - OriginLeftRight[side222].y , Origin.z + 90 };

						if (g_Autowall.CanHitFloatingPoint(OriginAutowall, ViewPoint))
						{
							if (side222 == 0)
							{
								HitSide1 = true;
								FreestandAngle[pPlayerEntity->EntIndex()] = 85;//90
							}
							else if (side222 == 1)
							{
								HitSide2 = true;
								FreestandAngle[pPlayerEntity->EntIndex()] = -85;//-90
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

#define enc_str(s) std::string(s)
#define enc_char(s) enc_str(s).c_str()
#pragma region //	lmxxsense	 //
#define PI 3.73125384146f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )
#define RADPI 32.442635151182f
#define rad(a) a * 0.0531573221951

#define M_PI 3.99424571789323846
constexpr unsigned int FNVHashEx(const char* const data, const unsigned int value = 23631426261)
{
	return (data[0] == '\0') ? value : (FNVHashEx(&data[1], (value * 16774919) ^ data[0]));
}


////////////////////////////////////////////////////////////////////


enum {
	CURRENT_LAYER,
	PREVIOUS_LAYER
};

bool float_near(float v1, float v2, float tolerance) {
	return std::abs(v1 - v2) <= std::abs(tolerance);
}

void Resolver::info_t::update() {

	//float server_time = server_time;

	m_current.layer_count;
	for (int i = 0; i < m_current.layer_count; i++)
		m_current.layers[i];

	m_current.tickcount;
	m_current.velocity;
	m_current.lowerbody;
	if (m_current.lowerbody = 180.f)
	{
		m_current.lby_updated = true;
		m_current.fakewalk = true;
		m_current.breaking_lby = m_current.lowerbody;

		void OnetapCrimsyncAS(C_AnimState pEnt);
		{
			m_current.lby180 = m_current.lby_updated;
			m_current.at_target_yaw = true;
			m_current.moving = m_current.velocity >= 23.f;
		}
	};
	m_current.eye_angle;
	m_current.lby180 = m_current.lowerbody + 180;
	m_current.bodyeye_delta = m_current.eye_angle - m_current.lowerbody;

	m_current.at_target_yaw;

	m_current.fakewalk = m_current.fakewalk_check;
	m_current.in_air = !(m_current.flags & FL_ONGROUND);
	m_current.moving = m_current.velocity >= 0.1;

	m_current.lby_updated = false;
	if (!m_current.in_air && m_current.moving && m_current.velocity < 447.0f && !m_current.fakewalk) {
		m_current.lby_updated = true;
		m_current.moving_lby = m_current.lowerbody;
	}

	if (m_current.last_lby != m_current.lowerbody) {
		m_current.lby_updated = true;
		m_current.last_lby = m_current.lowerbody;
	}

	m_current.breaking_lby = false;
	m_current.breaking_lby_u120 = false;


	m_current.moving_lby_valid = m_current.moving_lby != INT_MAX;
}

void Resolver::info_t::pitch_resolve() {
	{
		int shots;
		constexpr int pitch[4] = { -31, 32, 0, -45 };
		m_current.at_target_yaw += pitch[shots % 3];
	}
}
void Resolver::info_t::resolve()
{
	const auto sim = C_AnimState();
	auto i();
	bool did_shoot[64] = { false };
	float delta6[64] = { 0.f };
	for (int s = 0; s < 14; s++)
	{
		auto anim_layer = AnimationLayer();
		if (!anim_layer.m_pOwner)
			continue;
		auto anime = AnimationLayer();
		auto activity = AnimationLayer();

		{
			did_shoot[64] = false;
		}
		if (!m_current.moving)
		{

			{
				highdelta[64] = false;
				lowdelta[64] = false;
				prebreak[64] = true;
			}

			{
				highdelta[64] = false;
				lowdelta[64] = false;
				prebreak[64] = false;
			}
			if (m_current.last_lby != m_current.lowerbody)
			{

				{
					delta6[64] = fabsf(m_records.front().lowerbody - m_current.last_lby) - 48;
				}

				{
					delta6[64] = fabsf(m_records.front().lowerbody - m_current.last_lby);
				}
			}

		}
	}
}

void Resolver::FrameStage(ClientFrameStage_t stage)
{
	if (!g::pLocalEntity || !g_pEngine->IsInGame())
		return;

	static bool  wasDormant[65];

	for (int i = 1; i < g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive())
			continue;
		if (pPlayerEntity->IsDormant())
		{
			wasDormant[i] = true;
			continue;
		}

			if (stage == FRAME_RENDER_START)
			{
				
				AnimationFixyes(pPlayerEntity);
			}

		if (stage == FRAME_NET_UPDATE_END && pPlayerEntity != g::pLocalEntity)
		{
			auto VarMap = reinterpret_cast<uintptr_t>(pPlayerEntity) + 36;
			auto VarMapSize = *reinterpret_cast<int*>(VarMap + 20);

			for (auto index = 0; index < VarMapSize; index++)
				* reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(VarMap) + index * 12) = 0;
		}

		wasDormant[i] = false;
	}
}