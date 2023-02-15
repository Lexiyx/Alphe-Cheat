#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include "..\..\SDK\IEngineTrace.h"

struct FireBulletData
{
	FireBulletData(const Vector& eyePos, C_BaseEntity* entity) : src(eyePos), filter(entity)
	{
	}

	Vector          src;
	C_Trace         enter_trace;
	Vector          direction;
	C_TraceFilter   filter;
	float           trace_length;
	float           trace_length_remaining;
	float           current_damage;
	int             penetrate_count;
};
class Autowall
{
public:
	struct Autowall_Return_Info
	{
		int damage;
		int hitgroup;
		int penetration_count;
		bool did_penetrate_wall;
		float thickness;
		Vector end;
		C_BaseEntity* hit_entity;
	};
	struct Autowall_Info
	{
		Vector start;
		Vector end;
		Vector current_position;
		Vector direction;

		ITraceFilter* filter;
		C_Trace enter_trace;

		float thickness;
		float current_damage;
		int penetration_count;
	};
	void ScaleDamage(C_BaseEntity* entity, WeaponInfo_t* weapon_info, int hitgroup, float& current_damage);
	bool VectortoVectorVisible(Vector src, Vector point);
	//bool CanHitFloatingPoint(const Vector& point, const Vector& source, C_BaseEntity* pEnt);
	//bool CanWallbang(float& dmg);
	bool SimulateFireBullet(C_BaseEntity* local, C_BaseCombatWeapon* weapon, FireBulletData& data);
	bool IsBreakableEntity(C_BaseEntity* e);
	bool TraceToExit(C_Trace* enter_trace, Vector start, Vector dir, C_Trace* exit_trace);
	bool handle_bullet_penetration(WeaponInfo_t* info, Autowall_Info& data);
	bool HandleBulletPenetration(WeaponInfo_t* wpn_data, FireBulletData& data, bool extracheck, Vector point);
	float Damage(const Vector& point);
	bool CanHitFloatingPoint(const Vector& point, const Vector& source);
	Autowall_Return_Info CalculateDamage(Vector start, Vector end, C_BaseEntity* from_entity = nullptr, C_BaseEntity* to_entity = nullptr, int specific_hitgroup = -1);
private:
};
extern Autowall g_Autowall;