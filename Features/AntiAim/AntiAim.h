#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"
static constexpr float DISTANCE = 64.f;
class AntiAim
{
public:

	enum LbyMode
	{
		LBY_REAL,
		LBY_NORMAL,
		LBY_MAXDELTA,
		LBY_MAXDELTAMULT,
		LBY_MAXDELTAADD,
		LBY_ADD,
		LBY_SPIN
	};

	enum DesyncMode
	{
		FAKE_STATIC,
		FAKE_JITTER,
		FAKE_RANDOM_MAX
	};
	float MaxDelta();
	void SyncDesync();
	void YawMode();
	void Desync();
	float freestand();
	void OnCreateMove();
	//float MaxDelta(C_BaseEntity* pEnt);
	//void OnCreateMove(C_AnimState* animstate);
	//void LegitAA(CUserCmd* cmd, bool& bSendPacket);
};
extern AntiAim g_AntiAim;