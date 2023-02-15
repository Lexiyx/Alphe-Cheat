#include "Autowall.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\SDK\ISurfaceData.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\SDK\bspflags.h"
#include "..\..\SDK\ICvar.h"
#include "..\..\Utils\Math.h"
#include "..\..\SDK\ClientClass.h"
#include <algorithm>

// esoterik ftw

Autowall g_Autowall;

void TraceLine(Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, C_BaseEntity* ignore, C_Trace* ptr)
{
	C_TraceFilter filter(ignore);
	g_pTrace->TraceRay(C_Ray(vecAbsStart, vecAbsEnd), mask, &filter, ptr);
}

bool Autowall::VectortoVectorVisible(Vector src, Vector point)
{
	C_Trace TraceInit;
	TraceLine(src, point, mask_solid, g::pLocalEntity, &TraceInit);
	C_Trace Trace;
	TraceLine(src, point, mask_solid, TraceInit.m_pEnt, &Trace);

	if (Trace.flFraction == 1.0f || TraceInit.flFraction == 1.0f)
		return true;

	return false;
}

float GetHitgroupDamageMult(int iHitGroup)
{
	switch (iHitGroup)
	{
	case HITGROUP_HEAD:
		return 4.f;
	case HITGROUP_STOMACH:
		return 1.25f;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		return 0.75f;
	}

	return 1.0f;
}

void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, C_Trace* tr)
{
	static DWORD dwAddress = Utils::FindPattern("client.dll", (BYTE*)"\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF0\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x81\xEC\x00\x00\x00\x00\x8B\x43\x10", "xxxxxxxxxxxxxxxxxxxxxxxx????xxx");

	if (!dwAddress)
		return;

	_asm
	{
		MOV		EAX, filter
		LEA		ECX, tr
		PUSH	ECX
		PUSH	EAX
		PUSH	mask
		LEA		EDX, vecAbsEnd
		LEA		ECX, vecAbsStart
		CALL	dwAddress
		ADD		ESP, 0xC
	}
}

bool Autowall::SimulateFireBullet(C_BaseEntity* local, C_BaseCombatWeapon* weapon, FireBulletData& data)
{
	/*data.penetrate_count = 4;
	data.trace_length = 0.0f;
	auto* wpn_data = weapon->GetCSWpnData();
	data.current_damage = (float)wpn_data->damage;
	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = wpn_data->range - data.trace_length;
		Vector End_Point = data.src + data.direction * data.trace_length_remaining;
		TraceLine(data.src, End_Point, 0x4600400B, local, &data.enter_trace);
		UTIL_ClipTraceToPlayers(data.src, End_Point * 40.f, 0x4600400B, &data.filter, &data.enter_trace);
		if (data.enter_trace.flFraction == 1.0f)
			break;
		if ((data.enter_trace.hitGroup <= 7) && (data.enter_trace.hitGroup > 0) && (local->GetTeam() != data.enter_trace.m_pEnt->GetTeam()))
		{
			data.trace_length += data.enter_trace.flFraction * data.trace_length_remaining;
			data.current_damage *= pow(wpn_data->range_modifier, data.trace_length * 0.002);
			ScaleDamage(data.enter_trace.hitGroup, data.enter_trace.m_pEnt, wpn_data->armor_ratio, data.current_damage);
			return true;
		}
		if (!HandleBulletPenetration(wpn_data, data, false, Vector(0, 0, 0)))
			break;
	}*/
	return false;
}

bool TraceToExitalt(Vector& end, C_Trace& tr, Vector start, Vector vEnd, C_Trace* trace)
{
	typedef bool(__fastcall* TraceToExitFn)(Vector&, C_Trace&, float, float, float, float, float, float, C_Trace*);
	static TraceToExitFn oTraceToExit = (TraceToExitFn)Utils::FindSignature("client.dll", "55 8B EC 83 EC 30 F3 0F 10 75");

	if (!oTraceToExit)
		return false;

	return oTraceToExit(end, tr, start.x, start.y, start.z, vEnd.x, vEnd.y, vEnd.z, trace);
}
bool Autowall::IsBreakableEntity(C_BaseEntity* e)
{
	if (!e || !e->EntIndex())
		return false;

	static auto is_breakable_fn = reinterpret_cast<bool(__thiscall*)(C_BaseEntity*)>(
		Utils::FindSignature("client.dll", "55 8B EC 51 56 8B F1 85 F6 74 68"));

	const auto result = is_breakable_fn(e);
	auto class_id = e->GetClientClass()->ClassID;
	if (!result &&
		(class_id == 10 ||
			class_id == 31 ||
			(class_id == 11 && e->GetCollideable()->GetSolid() == 1)))
		return true;

	return result;
}
bool Autowall::TraceToExit(C_Trace* enter_trace, Vector start, Vector dir, C_Trace* exit_trace)
{
	Vector end;
	float distance = 0.f;
	signed int distance_check = 25;
	int first_contents = 0;
	if (!exit_trace)
		return false;

	do
	{
		distance += 3.5f;
		end = start + dir * distance;

		if (!first_contents) first_contents = g_pTrace->GetPointContents(end, MASK_SHOT | CONTENTS_GRATE, NULL);

		int point_contents = g_pTrace->GetPointContents(end, MASK_SHOT | CONTENTS_GRATE, NULL);

		if (!(point_contents & (MASK_SHOT_HULL | CONTENTS_HITBOX)) || point_contents & CONTENTS_HITBOX && point_contents != first_contents)
		{
			Vector new_end = end - (dir * 4.f);

			C_Ray ray;
			ray.Init(end, new_end);

			g_pTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, nullptr, exit_trace);

			if (exit_trace->startSolid && exit_trace->surface.flags & SURF_HITBOX)
			{
				TraceLine(end, start, MASK_SHOT_HULL | CONTENTS_HITBOX, exit_trace->m_pEnt, exit_trace);

				if (exit_trace->DidHit() && !exit_trace->startSolid) return true;

				continue;
			}

			if (exit_trace->DidHit() && !exit_trace->startSolid)
			{
				if (enter_trace->surface.flags & SURF_NODRAW || !(exit_trace->surface.flags & SURF_NODRAW)) {
					if (exit_trace->plane.normal.Dot(dir) <= 1.f)
						return true;

					continue;
				}

				if (IsBreakableEntity(enter_trace->m_pEnt)
					&& IsBreakableEntity(exit_trace->m_pEnt))
					return true;

				continue;
			}

			if (exit_trace->surface.flags & SURF_NODRAW)
			{
				if (IsBreakableEntity(enter_trace->m_pEnt)
					&& IsBreakableEntity(exit_trace->m_pEnt))
				{
					return true;
				}
				else if (!(enter_trace->surface.flags & SURF_NODRAW))
				{
					continue;
				}
			}

			if ((!enter_trace->m_pEnt
				|| enter_trace->m_pEnt->EntIndex() == 0)
				&& (IsBreakableEntity(enter_trace->m_pEnt)))
			{
				exit_trace = enter_trace;
				exit_trace->end = start + dir;
				return true;
			}

			continue;
		}

		distance_check--;
	} while (distance_check);

	return false;
}
bool Autowall::HandleBulletPenetration(WeaponInfo_t* wpn_data, FireBulletData& data, bool extracheck, Vector point)
{
	/*surfacedata_t* enter_surface_data = g_pSurfaceData->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
	data.trace_length += data.enter_trace.flFraction * data.trace_length_remaining;
	data.current_damage *= pow(wpn_data->range_modifier, (data.trace_length * 0.002));
	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
		data.penetrate_count = 0;
	if (data.penetrate_count <= 0)
		return false;
	Vector dummy;
	C_Trace trace_exit;
	if (!TraceToExitalt(dummy, data.enter_trace, data.enter_trace.end, data.direction, &trace_exit))
		return false;
	surfacedata_t* exit_surface_data = g_pSurfaceData->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;
	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;
	if (((data.enter_trace.contents & contents_grate) != 0) || (enter_material == 89) || (enter_material == 71))
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)combined_penetration_modifier = 2.0f;
	}
	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->penetration) * 1.25f);
	float thickness = VectorLength(trace_exit.end - data.enter_trace.end);
	if (extracheck)
		if (!VectortoVectorVisible(trace_exit.end, point))
			return false;
	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;
	float lost_damage = fmaxf(0.0f, v35 + thickness);
	if (lost_damage > data.current_damage)
		return false;
	if (lost_damage >= 0.0f)
		data.current_damage -= lost_damage;
	if (data.current_damage < 1.0f)
		return false;
	data.src = trace_exit.end;
	data.penetrate_count--;

	return true;*/

	C_Trace trace_exit;
	surfacedata_t* enter_surface_data = g_pSurfaceData->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;

	float enter_surf_penetration_modifier = enter_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.18f;
	float compined_penetration_modifier = 0.f;
	bool solid_surf = ((data.enter_trace.contents >> 3) & CONTENTS_SOLID);
	bool light_surf = ((data.enter_trace.surface.flags >> 7) & SURF_LIGHT);

	if
		(
			data.penetrate_count <= 0
			|| (!data.penetrate_count && !light_surf && !solid_surf && enter_material != CHAR_TEX_GLASS && enter_material != CHAR_TEX_GRATE)
			|| wpn_data->fPenetration <= 0.f
			|| !TraceToExit(&data.enter_trace, data.enter_trace.end, data.direction, &trace_exit)
			&& !(g_pTrace->GetPointContents(data.enter_trace.end, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL) & (MASK_SHOT_HULL | CONTENTS_HITBOX))
			)
	{
		return false;
	}

	surfacedata_t* exit_surface_data = g_pSurfaceData->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;
	float exit_surf_penetration_modifier = exit_surface_data->game.flPenetrationModifier;

	if (enter_material == CHAR_TEX_GLASS || enter_material == CHAR_TEX_GRATE) {
		compined_penetration_modifier = 3.f;
		final_damage_modifier = 0.08f;
	}
	else if (light_surf || solid_surf)
	{
		compined_penetration_modifier = 1.f;
		final_damage_modifier = 0.18f;
	}
	else {
		compined_penetration_modifier = (enter_surf_penetration_modifier + exit_surf_penetration_modifier) * 0.5f;
		final_damage_modifier = 0.18f;
	}

	if (enter_material == exit_material)
	{
		if (exit_material == CHAR_TEX_CARDBOARD || exit_material == CHAR_TEX_WOOD)
			compined_penetration_modifier = 3.f;
		else if (exit_material == CHAR_TEX_PLASTIC)
			compined_penetration_modifier = 2.0f;
	}

	float thickness = (trace_exit.end - data.enter_trace.end).LengthSqr();
	float modifier = max(0.f, 1.f / compined_penetration_modifier);

	if (extracheck) {
		static auto VectortoVectorVisible = [&](Vector src, Vector point) -> bool {
			C_Trace TraceInit;
			TraceLine(src, point, MASK_SOLID, g::pLocalEntity, &TraceInit);
			C_Trace Trace;
			TraceLine(src, point, MASK_SOLID, TraceInit.m_pEnt, &Trace);

			if (Trace.flFraction == 1.0f || TraceInit.flFraction == 1.0f)
				return true;

			return false;
		};
		if (!VectortoVectorVisible(trace_exit.end, point))
			return false;
	}
	float lost_damage = max(((modifier * thickness) / 24.f) + ((data.current_damage * final_damage_modifier) + (max(3.75f / wpn_data->fPenetration, 0.f) * 3.f * modifier)), 0.f);

	if (lost_damage > data.current_damage)
		return false;

	if (lost_damage > 0.f)
		data.current_damage -= lost_damage;

	if (data.current_damage < 1.f)
		return false;

	data.src = trace_exit.end;
	data.penetrate_count--;

	return true;
}

float Autowall::Damage(const Vector& point)
{
	auto data = FireBulletData(g::pLocalEntity->GetEyePosition(), g::pLocalEntity);

	Vector angles;
	angles = g_Math.CalcAngle(data.src, point);
	g_Math.AngleVectors(angles, &data.direction);
	VectorNormalize(data.direction);

	if (SimulateFireBullet(g::pLocalEntity, g::pLocalEntity->GetActiveWeapon(), data))
		return data.current_damage;

	return 0.f;
}

bool Autowall::CanHitFloatingPoint(const Vector& point, const Vector& source) // ez
{
	if (!g::pLocalEntity)
		return false;

	FireBulletData data = FireBulletData(source, g::pLocalEntity);

	Vector angles = g_Math.CalcAngle(data.src, point);
	g_Math.AngleVectors(angles, &data.direction);
	VectorNormalize(data.direction);

	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)g::pLocalEntity->GetActiveWeapon();

	if (!pWeapon)
		return false;

	data.penetrate_count = 1;
	data.trace_length = 0.0f;

	WeaponInfo_t* weaponData = pWeapon->GetCSWpnData();

	if (!weaponData)
		return false;

	data.current_damage = (float)weaponData->iDamage;
	data.trace_length_remaining = weaponData->fRange - data.trace_length;
	Vector end = data.src + (data.direction * data.trace_length_remaining);
	TraceLine(data.src, end, mask_shot | contents_hitbox, g::pLocalEntity, &data.enter_trace);

	if (VectortoVectorVisible(data.src, point) || HandleBulletPenetration(weaponData, data, true, point))
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Autowall::ScaleDamage(C_BaseEntity* entity, WeaponInfo_t* weapon_info, int hitgroup, float& current_damage)
{
	//Cred. to N0xius for reversing this.
	//TODO: _xAE^; look into reversing this yourself sometime

	bool hasHeavyArmor = false;
	int armorValue = entity->ArmorValue();

	//Fuck making a new function, lambda beste. ~ Does the person have armor on for the hitbox checked?
	auto IsArmored = [&entity, &hitgroup]()-> bool
	{
		C_BaseEntity* targetEntity = entity;
		switch (hitgroup)
		{
		case HITGROUP_HEAD:
			return targetEntity->HasHelmet();
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			return true;
		default:
			return false;
		}
	};

	switch (hitgroup)
	{
	case HITGROUP_HEAD:
		current_damage *= hasHeavyArmor ? 2.f : 4.f; //Heavy Armor does 1/2 damage
		break;
	case HITGROUP_STOMACH:
		current_damage *= 1.25f;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		current_damage *= 0.75f;
		break;
	default:
		break;
	}

	if (armorValue > 0 && IsArmored())
	{
		float bonusValue = 1.f, armorBonusRatio = 0.5f, armorRatio = weapon_info->fArmorRatio / 2.f;

		//Damage gets modified for heavy armor users
		if (hasHeavyArmor)
		{
			armorBonusRatio = 0.33f;
			armorRatio *= 0.5f;
			bonusValue = 0.33f;
		}

		auto NewDamage = current_damage * armorRatio;

		if (hasHeavyArmor)
			NewDamage *= 0.85f;

		if (((current_damage - (current_damage * armorRatio)) * (bonusValue * armorBonusRatio)) > armorValue)
			NewDamage = current_damage - (armorValue / armorBonusRatio);

		current_damage = NewDamage;
	}
}
bool Autowall::handle_bullet_penetration(WeaponInfo_t* info, Autowall_Info& data) {
	C_Trace trace_exit;
	surfacedata_t* enter_surface_data = g_pSurfaceData->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;

	float enter_surf_penetration_modifier = enter_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float compined_penetration_modifier = 0.f;
	bool solid_surf = ((data.enter_trace.contents >> 3) & CONTENTS_SOLID);
	bool light_surf = ((data.enter_trace.surface.flags >> 7) & SURF_LIGHT);

	if (data.penetration_count <= 0
		|| (!data.penetration_count && !light_surf && !solid_surf && enter_material != CHAR_TEX_GLASS && enter_material != CHAR_TEX_GRATE)
		|| info->fPenetration <= 0.f
		|| !TraceToExit(&data.enter_trace, data.enter_trace.end, data.direction, &trace_exit)
		&& !(g_pTrace->GetPointContents(data.enter_trace.end, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL) & (MASK_SHOT_HULL | CONTENTS_HITBOX)))
		return false;

	surfacedata_t* exit_surface_data = g_pSurfaceData->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;
	float exit_surf_penetration_modifier = exit_surface_data->game.flPenetrationModifier;

	if (enter_material == CHAR_TEX_GLASS || enter_material == CHAR_TEX_GRATE) {
		compined_penetration_modifier = 3.f;
		final_damage_modifier = 0.05f;
	}

	else if (light_surf || solid_surf) {
		compined_penetration_modifier = 1.f;
		final_damage_modifier = 0.16f;
	}
	else {
		compined_penetration_modifier = (enter_surf_penetration_modifier + exit_surf_penetration_modifier) * 0.5f;
		final_damage_modifier = 0.16f;
	}

	if (enter_material == exit_material) {
		if (exit_material == CHAR_TEX_CARDBOARD || exit_material == CHAR_TEX_WOOD)
			compined_penetration_modifier = 3.f;
		else if (exit_material == CHAR_TEX_PLASTIC)
			compined_penetration_modifier = 2.0f;
	}

	float thickness = (trace_exit.end - data.enter_trace.end).LengthSqr();
	float modifier = fmaxf(0.f, 1.f / compined_penetration_modifier);

	float lost_damage = fmaxf(
		((modifier * thickness) / 24.f) //* 0.041666668
		+ ((data.current_damage * final_damage_modifier)
			+ (fmaxf(3.75 / info->fPenetration, 0.f) * 3.f * modifier)), 0.f);

	if (lost_damage > data.current_damage)
		return false;

	if (lost_damage > 0.f)
		data.current_damage -= lost_damage;

	if (data.current_damage < 1.f)
		return false;

	data.current_position = trace_exit.end;
	data.penetration_count--;

	return true;
}
Autowall::Autowall_Return_Info Autowall::CalculateDamage(Vector start, Vector end, C_BaseEntity* from_entity, C_BaseEntity* to_entity, int specific_hitgroup)
{
	// default values for return info, in case we need to return abruptly
	Autowall_Return_Info return_info;
	return_info.damage = -1;
	return_info.hitgroup = -1;
	return_info.hit_entity = nullptr;
	return_info.penetration_count = 4;
	return_info.thickness = 0.f;
	return_info.did_penetrate_wall = false;

	Autowall_Info autowall_info;
	autowall_info.penetration_count = 4;
	autowall_info.start = start;
	autowall_info.end = end;
	autowall_info.current_position = start;
	autowall_info.thickness = 0.f;

	// direction 
	g_Math.AngleVectors(g_Math.CalcAngle(start, end), &autowall_info.direction);

	// attacking entity
	if (!from_entity)
		from_entity = g::pLocalEntity;
	if (!from_entity)
		return return_info;

	auto filter_player = cTraceFilterOneEntity();
	filter_player.pEntity = to_entity;



	auto filter_local = CTraceFilter();
	filter_local.pSkip = from_entity;

	// setup filters
	if (to_entity)
		autowall_info.filter = &filter_player;
	else
		autowall_info.filter = &filter_player;
	// weapon
	auto weapon = reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntity(from_entity->GetActiveWeaponIndex()));
	if (!weapon)
		return return_info;

	// weapon data
	auto weapon_info = weapon->GetCSWpnData();
	if (!weapon_info)
		return return_info;

	// client class
	auto weapon_client_class = reinterpret_cast<C_BaseEntity*>(weapon)->GetClientClass();
	if (!weapon_client_class)
		return return_info;

	// weapon range
	float range = min(weapon_info->fRange, (start - end).Length());
	end = start + (autowall_info.direction * range);
	autowall_info.current_damage = weapon_info->iDamage;

	while (autowall_info.current_damage > 0 && autowall_info.penetration_count > 0)
	{
		return_info.penetration_count = autowall_info.penetration_count;
		//g_pIVDebugOverlay->AddLineOverlay(g::pLocalEntity->GetEyePosition(), end, 0, 255, 0, true, 0.1);
		TraceLine(autowall_info.current_position, end, MASK_SHOT | CONTENTS_GRATE, from_entity, &autowall_info.enter_trace);
		UTIL_ClipTraceToPlayers(autowall_info.current_position, autowall_info.current_position + autowall_info.direction * 40.f, MASK_SHOT | CONTENTS_GRATE, autowall_info.filter, &autowall_info.enter_trace);

		const float distance_traced = (autowall_info.enter_trace.end - start).Length();
		autowall_info.current_damage *= pow(weapon_info->fRangeModifier, (distance_traced / 500.f));

		/// if reached the end
		if (autowall_info.enter_trace.flFraction == 1.f)
		{
			if (to_entity && specific_hitgroup != 0)
			{
				ScaleDamage(to_entity, weapon_info, specific_hitgroup, autowall_info.current_damage);

				return_info.damage = autowall_info.current_damage;
				return_info.hitgroup = specific_hitgroup;
				return_info.end = autowall_info.enter_trace.end;
				return_info.hit_entity = to_entity;
			}
			else
			{
				return_info.damage = autowall_info.current_damage;
				return_info.hitgroup = -1;
				return_info.end = autowall_info.enter_trace.end;
				return_info.hit_entity = nullptr;
			}

			break;
		}
		// if hit an entity
		if (autowall_info.enter_trace.hitGroup > 0 && autowall_info.enter_trace.hitGroup <= 7 && autowall_info.enter_trace.m_pEnt)
		{
			// checkles gg
			if ((to_entity && autowall_info.enter_trace.m_pEnt != to_entity) ||
				(autowall_info.enter_trace.m_pEnt->GetTeam() == from_entity->GetTeam()))
			{
				return_info.damage = -1;
				return return_info;
			}

			if (specific_hitgroup != -1)
				ScaleDamage(autowall_info.enter_trace.m_pEnt, weapon_info, specific_hitgroup, autowall_info.current_damage);
			else
				ScaleDamage(autowall_info.enter_trace.m_pEnt, weapon_info, autowall_info.enter_trace.hitGroup, autowall_info.current_damage);

			// fill the return info
			return_info.damage = autowall_info.current_damage;
			return_info.hitgroup = autowall_info.enter_trace.hitGroup;
			return_info.end = autowall_info.enter_trace.end;
			return_info.hit_entity = autowall_info.enter_trace.m_pEnt;

			break;
		}

		// break out of the loop retard

		if (!handle_bullet_penetration(weapon_info, autowall_info))
			break;

		return_info.did_penetrate_wall = true;
	}

	return_info.penetration_count = autowall_info.penetration_count;

	return return_info;
}