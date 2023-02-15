#include "AntiAim.h"
#include "..\Aimbot\Autowall.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\Utils\Math.h"
#include "..\..\Menu\Menu.h"
#include "..\Misc\Misc.h"
#include "../Resolver/Resolver2.h"
#include <iostream>
#include <random>
#include <chrono>
AntiAim g_AntiAim;

//test aa
bool m_bBreakBalance = false;
bool testbool = false;
bool testside = false;

bool Swtich = false;
bool lastmove = true;

float lbyangle;
static float FinalAngle = 0.0f;
static float LastRealYaw = 0.0f;
int side = 0;
void FreeStanding()
{
	if (g::pCmd->buttons & IN_USE || (g::pCmd->buttons & IN_ATTACK2 && g::pLocalEntity->IsKnife()))
		return;
	bool bside1 = false;
	bool bside2 = false;
	bool autowalld = false;
	for (int i = 1; i <= g_pEntityList->GetHighestEntityIndex(); ++i)
	{
		if (g_pEngine->GetLocalPlayer() == i)
			continue;
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant()
			|| pPlayerEntity == g::pLocalEntity
			|| pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
			continue;

		float angToLocal = g_Math.CalcAngle(g::pLocalEntity->GetOrigin(), pPlayerEntity->GetOrigin()).y;
		Vector ViewPoint = pPlayerEntity->GetOrigin() + Vector(0, 0, g_Menu.Config.trustedmode ? 89 : 90);

		Vector2D Side1 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal))),(45 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side2 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(45 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector2D Side3 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal))),(50 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side4 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(50 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector Origin = g::pLocalEntity->GetOrigin();

		Vector2D OriginLeftRight[] = { Vector2D(Side1.x, Side1.y), Vector2D(Side2.x, Side2.y) };

		Vector2D OriginLeftRightLocal[] = { Vector2D(Side3.x, Side3.y), Vector2D(Side4.x, Side4.y) };

		for (side = 0; side < 2; side++)
		{
			Vector OriginAutowall = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 80 };
			Vector OriginAutowall2 = { ViewPoint.x + OriginLeftRightLocal[side].x,  ViewPoint.y - OriginLeftRightLocal[side].y , ViewPoint.z };

			if (g_Autowall.CanHitFloatingPoint(OriginAutowall, ViewPoint))
			{
				if (side == 0)
				{
					bside1 = true;
					FinalAngle = g_Menu.Config.trustedmode ? angToLocal + 89 : angToLocal + 90;
					g_Menu.Config.FakeSide = false;
					g::pCmd->viewangles.y += g_Menu.Config.trustedmode ? +179 : +180;
				}
				else if (side == 1)
				{
					bside2 = true;
					FinalAngle = g_Menu.Config.trustedmode ? angToLocal - 89 : angToLocal - 90;
					g_Menu.Config.FakeSide = true;
					g::pCmd->viewangles.y += g_Menu.Config.trustedmode ? +179 : +180;
				}
				autowalld = true;
			}
			else
			{
				for (int side222 = 0; side222 < 2; side222++)
				{
					Vector OriginAutowall222 = { Origin.x + OriginLeftRight[side222].x,  Origin.y - OriginLeftRight[side222].y , Origin.z + 80 };

					if (g_Autowall.CanHitFloatingPoint(OriginAutowall222, OriginAutowall2))
					{
						if (side222 == 0)
						{
							bside1 = true;
							FinalAngle = g_Menu.Config.trustedmode ? angToLocal + 89 : angToLocal + 90;
							g_Menu.Config.FakeSide = false;
						}
						else if (side222 == 1)
						{
							bside2 = true;
							FinalAngle = g_Menu.Config.trustedmode ? angToLocal - 89 : angToLocal - 90;
							g_Menu.Config.FakeSide = true;
						}
						autowalld = true;
					}
				}
			}
		}
	}

	Vector view;
	g_pEngine->GetViewAngles(view);

	if (!autowalld || (bside1 && bside2))
		g::pCmd->viewangles.y = g_Menu.Config.trustedmode ? view.y + 179 : view.y + 180;
	else
		g::pCmd->viewangles.y = FinalAngle;
}

void DesyncFreeStanding()
{
	if (!g::pLocalEntity->IsAlive() || g::pCmd->buttons & IN_USE || (g::pCmd->buttons & IN_ATTACK2 && g::pLocalEntity->IsKnife()))
		return;
	static float FinalAngle;
	bool bside1 = false;
	bool bside2 = false;
	bool autowalld = false;
	for (int i = 1; i <= g_pEntityList->GetHighestEntityIndex(); ++i)
	{
		if (g_pEngine->GetLocalPlayer() == i)
			continue;
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant()
			|| pPlayerEntity == g::pLocalEntity
			|| pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
			continue;

		float angToLocal = g_Math.CalcAngle(g::pLocalEntity->GetOrigin(), pPlayerEntity->GetOrigin()).y;
		Vector ViewPoint = pPlayerEntity->GetOrigin() + Vector(0, 0, g_Menu.Config.trustedmode ? 89 : 90);

		Vector2D Side1 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal))),(45 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side2 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(45 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector2D Side3 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal))),(50 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side4 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(50 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector Origin = g::pLocalEntity->GetOrigin();

		Vector2D OriginLeftRight[] = { Vector2D(Side1.x, Side1.y), Vector2D(Side2.x, Side2.y) };

		Vector2D OriginLeftRightLocal[] = { Vector2D(Side3.x, Side3.y), Vector2D(Side4.x, Side4.y) };

		for (int side = 0; side < 2; side++)
		{
			Vector OriginAutowall = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 80 };
			Vector OriginAutowall2 = { ViewPoint.x + OriginLeftRightLocal[side].x,  ViewPoint.y - OriginLeftRightLocal[side].y , ViewPoint.z };

			if (g_Autowall.CanHitFloatingPoint(OriginAutowall, ViewPoint))
			{
				if (side == 0)
				{
					bside1 = true;
					FinalAngle = g_Menu.Config.trustedmode ? angToLocal + 179 : angToLocal + 180;
					g_Menu.Config.FakeSide = false;
				}
				else if (side == 1)
				{
					bside2 = true;
					FinalAngle = g_Menu.Config.trustedmode ? angToLocal - 179 : angToLocal - 180;
					g_Menu.Config.FakeSide = true;
				}
				autowalld = true;
			}
			else
			{
				for (int side222 = 0; side222 < 2; side222++)
				{
					Vector OriginAutowall222 = { Origin.x + OriginLeftRight[side222].x,  Origin.y - OriginLeftRight[side222].y , Origin.z + 80 };

					if (g_Autowall.CanHitFloatingPoint(OriginAutowall222, OriginAutowall2))
					{
						if (side222 == 0)
						{
							bside1 = true;
							FinalAngle = g_Menu.Config.trustedmode ? angToLocal + 179 : angToLocal + 180;
							g_Menu.Config.FakeSide = false;
						}
						else if (side222 == 1)
						{
							bside2 = true;
							FinalAngle = g_Menu.Config.trustedmode ? angToLocal - 179 : angToLocal - 180;
							g_Menu.Config.FakeSide = true;
						}
						autowalld = true;
					}
				}
			}
		}
	}

	Vector view;
	g_pEngine->GetViewAngles(view);

	if (!autowalld || (bside1 && bside2))
		g::pCmd->viewangles.y = view.y + g_Menu.Config.trustedmode ? (g_Menu.Config.FakeSide ? -89 : 89) : (g_Menu.Config.FakeSide ? -90 : 90);
	else
		g::pCmd->viewangles.y = FinalAngle;
}
void FreeStanding2()
{
	if (g::pCmd->buttons & IN_USE || (g::pCmd->buttons & IN_ATTACK2 && g::pLocalEntity->IsKnife()))
		return;

	static float FinalAngle;
	bool bside1 = false;
	bool bside2 = false;
	bool autowalld = false;
	for (int i = 1; i <= g_pEntityList->GetHighestEntityIndex(); ++i)
	{
		if (g_pEngine->GetLocalPlayer() == i)
			continue;
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant()
			|| pPlayerEntity == g::pLocalEntity
			|| pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
			continue;

		float angToLocal = g_Math.CalcAngle(g::pLocalEntity->GetOrigin(), pPlayerEntity->GetOrigin()).y;
		Vector ViewPoint = pPlayerEntity->GetOrigin() + Vector(0, 0, g_Menu.Config.trustedmode ? 89 : 90);

		Vector2D Side1 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal))),(45 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side2 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(45 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector2D Side3 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal))),(50 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side4 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(50 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector Origin = g::pLocalEntity->GetOrigin();

		Vector2D OriginLeftRight[] = { Vector2D(Side1.x, Side1.y), Vector2D(Side2.x, Side2.y) };

		Vector2D OriginLeftRightLocal[] = { Vector2D(Side3.x, Side3.y), Vector2D(Side4.x, Side4.y) };

		for (int side = 0; side < 2; side++)
		{
			Vector OriginAutowall = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 80 };
			Vector OriginAutowall2 = { ViewPoint.x + OriginLeftRightLocal[side].x,  ViewPoint.y - OriginLeftRightLocal[side].y , ViewPoint.z };

			if (g_Autowall.CanHitFloatingPoint(OriginAutowall, ViewPoint))
			{
				if (side == 0)
				{
					bside1 = true;
					FinalAngle = g_Menu.Config.trustedmode ? angToLocal + 89 : angToLocal + 90;
					g_Menu.Config.FakeSide = false;
				}
				else if (side == 1)
				{
					bside2 = true;
					FinalAngle = g_Menu.Config.trustedmode ? angToLocal - 89 : angToLocal - 90;
					g_Menu.Config.FakeSide = true;
				}
				autowalld = true;
			}
			else
			{
				for (int side222 = 0; side222 < 2; side222++)
				{
					Vector OriginAutowall222 = { Origin.x + OriginLeftRight[side222].x,  Origin.y - OriginLeftRight[side222].y , Origin.z + 80 };

					if (g_Autowall.CanHitFloatingPoint(OriginAutowall222, OriginAutowall2))
					{
						if (side222 == 0)
						{
							bside1 = true;
							FinalAngle = g_Menu.Config.trustedmode ? angToLocal + 89 : angToLocal + 90;
						}
						else if (side222 == 1)
						{
							bside2 = true;
							FinalAngle = g_Menu.Config.trustedmode ? angToLocal - 89 : angToLocal - 90;
						}
						autowalld = true;
					}
				}
			}
		}
	}

	Vector view;
	g_pEngine->GetViewAngles(view);

	if (!autowalld || (bside1 && bside2))
		g::pCmd->viewangles.y = view.y + g_Menu.Config.trustedmode ? 179 : 180;
	else
		g::pCmd->viewangles.y = FinalAngle;
}


bool NextLbyUpdate()
{
	static float next_lby_update_time = 0;
	float curtime = g_pGlobalVars->curtime;

	auto animstate = g::pLocalEntity->AnimState();
	if (!animstate)
		return false;

	if (!(g::pLocalEntity->GetFlags() & FL_ONGROUND))
		return false;

	if (animstate->speed_2d > 0.1)
		next_lby_update_time = curtime + 0.22f;

	if (next_lby_update_time < curtime)
	{
		next_lby_update_time = curtime + 1.1f;
		return true;
	}

	return false;
}
float randnum(int Min, int Max)
{
	return ((rand() % (Max - Min)) + Min);
}
//auto client_state = *reinterpret_cast<uintptr_t*>(uintptr_t(GetModuleHandle("engine.dll")) + 0x57E854);
float desyncdelta()
{
	if (!g::pLocalEntity)
		return 0.f;
	auto state = g::pLocalEntity->AnimState();

	auto v42 = -g_Menu.Config.trustedmode ? 359.0f : 360.0f;
	auto v43 = state->m_flGoalFeetYaw;
	auto v129 = -g_Menu.Config.trustedmode ? 359.0f : 360.0f;

	if (v43 >= -g_Menu.Config.trustedmode ? 359.0f : 360.0f) {
		v42 = std::fminf(v43, g_Menu.Config.trustedmode ? 359.0f : 360.0f);
		v129 = v42;
	}

	auto v44 = state->m_flEyeYaw - v42;
	auto v45 = state->m_flEyeYaw;
	auto v135 = std::fmodf(v44, g_Menu.Config.trustedmode ? 359.0f : 360.0f);
	auto v46 = v135;

	if (v45 <= v129) {
		if (v135 <= g_Menu.Config.trustedmode ? -179.0f : -180.0f)
			v46 = v135 + g_Menu.Config.trustedmode ? 359.0f : 360.0f;
	}
	else if (v135 >= g_Menu.Config.trustedmode ? 179.0f : 180.0f) {
		v46 = v135 - g_Menu.Config.trustedmode ? 359.0f : 360.0f;
	}

	auto v48 = 0.0f;
	auto v47 = state->m_flFeetSpeedUnknownForwardOrSideways;

	if (v47 >= 0.0f)
		v48 = std::fmodf(v47, 1.0f);
	else
		v48 = 0.0f;

	auto v49 = (float)((float)(state->m_flStopToFullRunningFraction * -0.3f) - 0.2f) * v48;
	auto v50 = state->m_fDuckAmount;
	auto v51 = v49 + 1.0f;

	if (v50 > 0.0f) {
		auto v52 = state->m_flFeetSpeedUnknownForwardOrSideways;
		auto v53 = 0.0f;

		if (v52 >= 0.0f)
			v53 = std::fmodf(v52, 1.0f);
		else
			v53 = 0.0f;

		v51 = v51 + (float)((float)(v50 * v53) * (float)(0.5f - v51));
	}

	return *(float*)((std::uintptr_t)state + 0x334) * v51;
}

float AntiAim::MaxDelta() {


	auto animstate = uintptr_t(g::pLocalEntity->AnimState());

	float duckammount = *(float*)(animstate + 0xA4);
	float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));

	float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));

	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001f) - 0.19999999f) * speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;

	if (duckammount > 0) {
		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));
	}

	unk3 = *(float*)(animstate + 0x334) * unk2;

	return unk3;
}
void UpDownRand()
{
	if (!g::pLocalEntity->IsAlive())
		return;

	bool upDownRnd = Misc::RandomBool();
	if (upDownRnd)
		g::pCmd->viewangles.x = g_Menu.Config.trustedmode ? 89.f : 90.f;
	else
		g::pCmd->viewangles.x = g_Menu.Config.trustedmode ? -89.f : -90.f;
}

void nospread()
{
	if (!g::pLocalEntity->IsAlive())
		return;
	bool upDownRnd = Misc::RandomBool();
	if (upDownRnd)
		g::pCmd->viewangles.x = 540;
	else
		g::pCmd->viewangles.x = -540;
}

void corruption()
{
	if (!g::pLocalEntity->IsAlive())
		return;
	long currentTime_ms = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	static long timeStamp = currentTime_ms;

	timeStamp = currentTime_ms;

	switch (timeStamp % 8)
	{
	case 1: g::pCmd->viewangles.y += 45;  break;
	case 2: g::pCmd->viewangles.y -= 45;  break;
	case 3: g::pCmd->viewangles.y += 50;  break;
	case 4: g::pCmd->viewangles.y -= 50;  break;
	case 5: g::pCmd->viewangles.y += 55;  break;
	case 6: g::pCmd->viewangles.y -= 55;  break;
	case 7: g::pCmd->viewangles.y += 58;  break;
	case 8: g::pCmd->viewangles.y -= 58;  break;
	}
}

static bool DesyncSwitch = false;
void AntiAim::SyncDesync()
{
	if (!g::pLocalEntity->IsAlive() || (g::pCmd->buttons & IN_ATTACK && !g_Menu.Config.FakeDuckKey) || (g::pCmd->buttons & IN_ATTACK2 && g::pLocalEntity->IsKnife())
		|| (g::offakelag && !g::pLocalEntity->IsNotDT() && !g::pLocalEntity->IsKnife()) || g::pCmd->buttons & IN_USE || g_pEngine->IsVoiceRecording())
		return;

	auto NetChannel = g_pClientState->m_net_channel;

	if (!NetChannel)
		return;
	int iChoke = 1;
	int fakelagVal = 1;
	if (!NetChannel || g_Menu.Config.FakeLagType != 0 || !(g::pLocalEntity->GetFlags() & FL_ONGROUND))
		return;

	iChoke = fakelagVal;
	auto velocity = g::pLocalEntity->GetVelocity();;
	auto speed = velocity.Length();
	auto distance_per_tick = speed * g_pGlobalVars->intervalPerTick;
	if (velocity.Length2D() > 0)
		iChoke = std::min<int>(std::ceilf(DISTANCE / distance_per_tick), fakelagVal);
	else
		iChoke = fakelagVal;

	if (NetChannel->m_nChokedPackets < iChoke)
		g::bSendPacket = false;
	else
		g::bSendPacket = true;
}
float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}
float yaw(int version)
{
	float yaw;
	yaw = 0.0f;
	switch (version)
	{
	case 0:
		yaw = g_Menu.Config.trustedmode ? 88.99 : 90;
		break;
	case 1:
		yaw = g_Menu.Config.trustedmode ? 179 : 180;
		break;
	case 2:
		yaw = g_pGlobalVars->curtime * 425;
		break;
	case 3:
		yaw = 180 + (RandomFloat(0, 50) - 25);
		break;
	case 4:
	{
		Vector angles; g_pEngine->GetViewAngles(angles);
		yaw = angles.y;
		break;
	}
	case 5:
		yaw = g::pLocalEntity->GetLowerBodyYaw();
		break;
	case 6:
		//freestanding
		break;
	case 7:
	{
		constexpr auto maxRange = 90.0f;
		constexpr auto angleAdditive = 180.0f;
		yaw = angleAdditive - maxRange / 2.f + std::fmodf(g_pGlobalVars->curtime * 60, maxRange);
		break;
	}
	}
	return yaw;

}
void AntiAim::YawMode()
{
	if (!g::pLocalEntity->IsAlive())
		return;
	Vector view;
	g_pEngine->GetViewAngles(view);
	switch (g_Menu.Config.YawMode)
	{
	case 0:
		FreeStanding();
		break;
	case 1:
		FreeStanding2();
		break;
	case 2:
		g::pCmd->viewangles.y = freestand();
		break;
	}
}
void AntiAim::Desync()
{
	if (!g::pLocalEntity->IsAlive())
		return;
	static bool switch_ = false;
	static bool switch_s = false;
	static float test = 0.f;
	Vector view;
	g_pEngine->GetViewAngles(view);

	if (g_Menu.Config.DesyncAngle)
	{
		if (g::bSendPacket && g::pLocalEntity->AnimState())
		{
			YawMode(); //FreeStanding
			if (g_Menu.Config.DesyncStyle == 0) //Static
			{
				LastRealYaw = g::pCmd->viewangles.y;
			}
			if (g_Menu.Config.DesyncStyle == 1) //Jitter
			{
				switch_ = !switch_;
				if (switch_)
					g::pCmd->viewangles.y = FinalAngle - g_Menu.Config.JitterRange;
				else
					g::pCmd->viewangles.y = FinalAngle + g_Menu.Config.JitterRange;

				LastRealYaw = FinalAngle;
			}
		}
		else
		{
			if (g_Menu.Config.DesyncMode == 0) //Static
				g::pCmd->viewangles.y = g_Menu.Config.FakeSide ? LastRealYaw - 120.f : LastRealYaw + 120.f;
			if (g_Menu.Config.DesyncMode == 1) //Automatic
				g::pCmd->viewangles.y = g_Menu.Config.FakeSide ? LastRealYaw + g::pLocalEntity->get_max_desync_delta_aimware() : LastRealYaw - g::pLocalEntity->get_max_desync_delta_aimware();
			if (g_Menu.Config.DesyncMode == 2) //Calculated
				g::pCmd->viewangles.y = g_Menu.Config.FakeSide ? LastRealYaw + g::pLocalEntity->get_max_desync_delta() : LastRealYaw - g::pLocalEntity->get_max_desync_delta();
			if (g_Menu.Config.DesyncMode == 3) //Custom static
				g::pCmd->viewangles.y = g_Menu.Config.FakeSide ? LastRealYaw - g_Menu.Config.DesyncCustomStatic : LastRealYaw + g_Menu.Config.DesyncCustomStatic;

		}
	}
}
float AntiAim::freestand() {
	auto local = g::pLocalEntity;

	float b, r, l;

	if (!local)
		return 0.0f;

	Vector src, dst, forward, right, up;
	C_Trace tr;
	C_TraceFilter filter(g::pLocalEntity);

	Vector va;
	g_pEngine->GetViewAngles(va);

	va.x = 0.0f;

	g_Math.AngleVectors(va, &forward, &right, &up);


	src = local->GetEyePosition();
	dst = src + forward * 384.0f;

	g_pTrace->TraceRay(C_Ray(src, dst), 0x46004003, &filter, &tr);

	b = (tr.end - tr.start).Length();

	g_pTrace->TraceRay(C_Ray(src + right * 35.0f, dst + right * 35.0f), 0x46004003, &filter, &tr);

	r = (tr.end - tr.start).Length();

	g_pTrace->TraceRay(C_Ray(src - right * 35.0f, dst - right * 35.0f), 0x46004003, &filter, &tr);

	l = (tr.end - tr.start).Length();

	if (l > r)
		return va.y - 90.0f;
	else if (r > l)
		return va.y + 90.0f;
	else if (b > r || b > l)
		return va.y - 180.0f;

	return 0.0f;
}
float updateTime;
float lastUpdate;
float wasmoving;
bool performBreak;

void LbyBreaker() {
	float Velocity = g::pLocalEntity->GetVelocity().Length2D();
	float MaxDelta = g_AntiAim.MaxDelta();

	int Add = g_Menu.Config.LbyAdd;

	if (!g::pLocalEntity->IsAlive())
		return;

	if (!performBreak && !g::bSendPacket) {
		if (Velocity > 0.4f && (g::pLocalEntity->GetFlags() & FL_ONGROUND)) {
			lastUpdate = g_pGlobalVars->curtime;
			wasmoving = true;
			m_bBreakBalance = true;
		}
		else {
			if (wasmoving && g_pGlobalVars->curtime - lastUpdate > 0.22f && (g::pLocalEntity->GetFlags() & FL_ONGROUND)) {
				wasmoving = false;
				performBreak = true;
				m_bBreakBalance = false;
			}
			else if (g_pGlobalVars->curtime - lastUpdate > 1.1f && (g::pLocalEntity->GetFlags() & FL_ONGROUND)) {
				performBreak = true;
				m_bBreakBalance = false;
			}
		}
	}
}
void LBYBreaker()
{
	if (g_Menu.Config.LbyBreaker)
	{
		int Add = g_Menu.Config.LbyAdd;

		Vector view;
		g_pEngine->GetViewAngles(view);
		float finalby = view.y;

		LbyBreaker();
		if (performBreak && !g::bSendPacket)
		{
			if (g_Menu.Config.SendPacketOnBreak)
				g::bSendPacket = true;
			else
				g::bSendPacket = false;
			g::pCmd->viewangles.y += g_Menu.Config.FakeSide ? finalby - Add : finalby + Add;
			lastUpdate = g_pGlobalVars->curtime;
			performBreak = false;
			return;
		}
	}

	if (g_Menu.Config.StaticLby)
	{
		if (fabsf(g::pCmd->forwardmove) < 2.2f && fabsf(g::pCmd->sidemove) < 2.2f)
		{
			if (!g::pCmd->buttons && IN_DUCK)
				g::pCmd->sidemove = g::pCmd->command_number % 2 ? -1.10f : 1.10f;
			else
				g::pCmd->sidemove = g::pCmd->command_number % 2 ? -3.25f : 3.25f;
		}
	}

}
void AntiAim::OnCreateMove()
{
	if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected() || !g::pLocalEntity || !g::pLocalEntity->IsAlive())
		return;

	Vector view;
	g_pEngine->GetViewAngles(view);

	if (!g_Menu.Config.Antiaim || g::pCmd->buttons & IN_USE || g::pCmd->buttons & IN_ATTACK)
	{
		g::pCmd->viewangles = view;
		return;
	}

	if (!g::pLocalEntity->GetActiveWeapon() || (g::pLocalEntity->IsNade() && g::pLocalEntity->m_fThrowTime() > 0.f))
		return;


	if (g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_LADDER)
		return;

	C_BaseCombatWeapon* weapon = g::pLocalEntity->GetActiveWeapon();
	if (weapon && weapon->GetItemDefinitionIndex() != ItemDefinitionIndex::WEAPON_REVOLVER)
	{
		float flServerTime = g::pLocalEntity->GetTickBase() * g_pGlobalVars->intervalPerTick;
		bool canShoot = (weapon->GetNextPrimaryAttack() <= flServerTime);
		if (canShoot && (g::pCmd->buttons & IN_ATTACK))
			return;
	}

	Swtich = !Swtich;

	switch (g_Menu.Config.PitchMode)
	{
	case 0:
		//Down
		g::pCmd->viewangles.x = g_Menu.Config.trustedmode ? 89 : 90;
		break;
	case 1:
		//Nazi Down
		g::pCmd->viewangles.x = g_Menu.Config.trustedmode ? 88 : 90;
		break;
	case 2:
		//Half-Down
		g::pCmd->viewangles.x = 45;
		break;
	case 3:
		//Zero
		g::pCmd->viewangles.x = 0;
		break;
	case 4:
		//Up
		g::pCmd->viewangles.x = g_Menu.Config.trustedmode ? -89 : -90;
		break;
	case 5:
		//Jitter Up/Down
		UpDownRand();
		break;
	case 6:
		//Custom
		g::pCmd->viewangles.x = g_Menu.Config.CustomPitchValue - g_Menu.Config.trustedmode ? 89 : 90;
		break;
	case 7:
		//nospread
		g::pCmd->viewangles.x = -540;
		break;
	case 8:
		//nospread
		nospread();
		break;
	case 9:
		g::pCmd->viewangles.x = view.x;
		break;
	default:
		break;
	}


	if (g::pLocalEntity->GetFlags() & FL_ONGROUND && g::pCmd->sidemove < 2 && g::pCmd->sidemove > -2) {
		static bool switch_ = false;
		if (switch_)
			g::pCmd->sidemove = 2;
		else
			g::pCmd->sidemove = -2;
		switch_ = !switch_;
	}
	SyncDesync();
	LBYBreaker();
	Desync();
}
