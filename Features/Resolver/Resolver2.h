#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include "..\..\SDK\IClientMode.h"
#include <deque>
#define ZERO Vector(0.0f, 0.0f, 0.0f)

struct Local_data
{
	bool visualize_lag = false;

	C_AnimState* prediction_animstate = nullptr;
	C_AnimState* animstate = nullptr;

	Vector stored_real_angles = ZERO;
};
class ResolverBeta
{
public:
	Local_data local_data;
	CBaseHandle* handle = nullptr;

	bool real_server_update = false;
	bool fake_server_update = false;

	float real_simulation_time = 0.0f;
	float fake_simulation_time = 0.0f;

	float spawntime = 0.0f;
	float tickcount = 0.0f;

	float abs_angles = 0.0f;
	float pose_parameter[24];
	AnimationLayer layers[15];



	bool UseFreestandAngle[65];
	float FreestandAngle[65];

	float pitchHit[65];

	void ResolveLegit();
	void OnCreateMove();
	void FrameStage(ClientFrameStage_t stage);
	//	void Yaw1(C_BaseEntity * ent);
	float sub_59B13C30(C_AnimState* nn);
	//void FrameStage(ClientFrameStage_t stage);
	void AnimationFix(C_BaseEntity* pEnt);
private:
};
extern ResolverBeta g_ResolverBeta;