#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include "..\..\SDK\IClientMode.h"
#include <deque>

class ResolverGamma
{ /// ???
public:
	bool UseFreestandAngle[65];
	float FreestandAngle[65];

	float pitchHit[65];

	void OnCreateMoves();
	//void Yaw1(C_BaseEntity* ent);
	void FrameStages(ClientFrameStage_t stage);
private:
	void AnimationFixs(C_BaseEntity* pEnt);
};
extern ResolverGamma g_ResolverGamma;