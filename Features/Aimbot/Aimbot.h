#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include <deque>

#define TIME_TO_TICKS( dt )		( (int)( 0.5 + (float)(dt) / g_pGlobalVars->intervalPerTick ) )
#define TICKS_TO_TIME( t )		( g_pGlobalVars->intervalPerTick *( t ) )
struct AimbotData_t
{
	AimbotData_t(C_BaseEntity* player, const int& idx)
	{
		this->pPlayer = player;
		this->index = idx;
	}
	C_BaseEntity* pPlayer;
	int					index;
};
struct LayerRecord
{
	LayerRecord()
	{
		m_nOrder = 0;
		m_nSequence = 0;
		m_flWeight = 0.f;
		m_flCycle = 0.f;
	}

	LayerRecord(const LayerRecord& src)
	{
		m_nOrder = src.m_nOrder;
		m_nSequence = src.m_nSequence;
		m_flWeight = src.m_flWeight;
		m_flCycle = src.m_flCycle;
	}

	uint32_t m_nOrder;
	uint32_t m_nSequence;
	float_t m_flWeight;
	float_t m_flCycle;
};
struct LagRecord
{
	LagRecord()
	{
		m_iPriority = -1;

		m_flSimulationTime = -1.f;
		m_vecOrigin.Init();
		m_angAngles.Init();
		m_vecMins.Init();
		m_vecMax.Init();
		m_bMatrixBuilt = false;
	}

	bool operator==(const LagRecord& rec)
	{
		return m_flSimulationTime == rec.m_flSimulationTime;
	}

	void SaveRecord(C_BaseEntity* player);

	matrix3x4_t	matrix[128];
	bool m_bMatrixBuilt;
	Vector m_vecHeadSpot;
	float m_iTickCount;

	// For priority/other checks
	int32_t m_iPriority;
	Vector  m_vecVelocity;
	float_t m_flPrevLowerBodyYaw;
	std::array<float_t, 24> m_arrflPrevPoseParameters;
	Vector  m_vecLocalAimspot;
	bool    m_bNoGoodSpots;

	// For backtracking
	float_t m_flSimulationTime;
	int32_t m_nFlags;
	Vector m_vecOrigin;	   // Server data, change to this for accuracy
	Vector m_vecAbsOrigin;
	Vector m_angAngles;
	Vector m_vecMins;
	Vector m_vecMax;
	std::array<float_t, 24> m_arrflPoseParameters;
	std::array<LayerRecord, 15> m_LayerRecords;
};

class Aimbot
{
public:
	std::deque<LagRecord> m_LagRecord[64]; // All records
	std::pair<LagRecord, LagRecord> m_RestoreLagRecord[64]; // Used to restore/change
	std::deque<LagRecord> backtrack_records; // Needed to select records based on menu option.
	LagRecord current_record[64]; // Used for various other actions where we need data of the current lag compensated player
	inline void ClearHistory()
	{
		for (int i = 0; i < 64; i++)
			m_LagRecord[i].clear();
	}

	matrix3x4_t Matrix[65][128];
	int Sequence;
	Vector multipoint(C_BaseEntity* entity, int option);
	float accepted_inaccuracy(C_BaseCombatWeapon* weapon);
	void RemoveBadRecords(int Idx, std::deque<LagRecord>& records);
	int GetPriorityLevel(C_BaseEntity* player, LagRecord* lag_record);
	void FrameUpdatePostEntityThink();
	bool StartLagCompensation(C_BaseEntity* player);
	void FinishLagCompensation(C_BaseEntity* player);
	bool IsTickValid(int tick);
	void ProcessCMD(int iTargetIdx, CUserCmd* usercmd);
	bool FindViableRecord(C_BaseEntity* player, LagRecord* record);
	void RageBacktrack(C_BaseEntity* target, CUserCmd* usercmd, Vector aimpoint);
	void FakelagFix(C_BaseEntity* player);
	void OnCreateMove();
	int bestEntDmg;
	int targetID = 0;

private:
	bool Backtrack[65];
	bool ShotBacktrack[65];
	std::vector<Vector> GetMultiplePointsForHitbox(C_BaseEntity* local, C_BaseEntity* entity, int iHitbox, matrix3x4_t BoneMatrix[128]);
	void RCS();
	//Vector GetMultiplePointsForHitbox(C_BaseEntity* local, C_BaseEntity* entity, int iHitbox, matrix3x4_t BoneMatrix[128]);
	bool HitChance(C_BaseEntity* pEnt, C_BaseCombatWeapon* pWeapon, Vector Angle, Vector Point, int chance);
	Vector Hitscan(C_BaseEntity* pEnt, int option);
	void Autostop();

	std::vector<AimbotData_t>	Entities;
};
extern Aimbot g_Aimbot;