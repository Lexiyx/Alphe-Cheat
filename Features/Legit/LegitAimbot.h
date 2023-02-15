#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include "..\..\Utils/Math.h"
#include <deque>

struct lbyRecords
{
	int tick_count;
	float lby;
	Vector headPosition;
	Vector Angles;
	Vector Origin;
};
struct backtrackData
{
	float simtime = 0.0f;
	Vector hitboxPos;
};

class BackTrack
{
	int latest_tick = 0;
public:
	lbyRecords records[32];
	void LegitBackTrack(CUserCmd* cmd, C_BaseEntity* pLocal);
};

extern backtrackData HeadPositions[64][12];
extern BackTrack g_Backtracking;


class LegitAimbot
{
public:
	void triggerbot(CUserCmd* cmd, C_BaseEntity* local, C_BaseCombatWeapon* weapon);
	bool hit_chance(C_BaseEntity* local, CUserCmd* cmd, C_BaseCombatWeapon* weapon, C_BaseEntity* target);
	void Aim_CreateMove(CUserCmd* cmd);
	
private:
	void FindTarget();
	void GoToTarget();
	void DropTarget();
	bool EntityIsValid(int i);
	void RCS();
};
extern LegitAimbot g_LegitAimbot;