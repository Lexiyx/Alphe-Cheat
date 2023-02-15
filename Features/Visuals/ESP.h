#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\singleton.h"
#include "..\..\SDK\IEngineTrace.h"
#include "..\Aimbot\Autowall.h"
#include "..\..\Utils\Interfaces.h"
struct TestPos
{
	Vector Pos;
	float SimulationTime;
};

enum HitboxList2
{
	HITBOX_HEAD2,
	HITBOX_NECK2,
	HITBOX_LOWER_NECK2,
	HITBOX_PELVIS2,
	HITBOX_BODY2,
	HITBOX_THORAX2,
	HITBOX_CHEST2,
	HITBOX_UPPER_CHEST2,
	HITBOX_RIGHT_THIGH2,
	HITBOX_LEFT_THIGH2,
	HITBOX_RIGHT_CALF2,
	HITBOX_LEFT_CALF2,
	HITBOX_RIGHT_FOOT2,
	HITBOX_LEFT_FOOT2,
	HITBOX_RIGHT_HAND2,
	HITBOX_LEFT_HAND2,
	HITBOX_RIGHT_UPPER_ARM2,
	HITBOX_RIGHT_FOREARM2,
	HITBOX_LEFT_UPPER_ARM2,
	HITBOX_LEFT_FOREARM2,
	HITBOX_MAX2
};

class ESP
{
public:
	void set_hitmarker_time(float time);
	void DrawHitmarker();
	void screenfx(i_game_event* event) noexcept;
	//void flags(C_BaseEntity* player);
    void Render();
	void QuakeKillCounter();
	void DrawZeusRange();
	void Ammo(C_BaseEntity* Entity);
private:
	struct
	{
		int left, top, right, bottom;
	}Box;

	Color color;
	Color ammocolor;
	Color hcolor;
	Color hbcolor;
	Color weaponcolor;
	Color namecolor;
	Color ypcolor;
	Color moneycolor;
	Color fakeduckcolor;
	Color shadow;
	int offset = 0;
	DWORD FONT;
	int offsetY;

	void BoundBox(C_BaseEntity* pEnt);
    void RenderBox(C_BaseEntity* pEnt);
	void RenderWeaponName(C_BaseEntity* pEnt);
	void RenderEntHealth(C_BaseEntity* pEnt);
	void RenderYawPitch(C_BaseEntity* pEnt);
	void RenderName(C_BaseEntity* pEnt, int iterator);
	void RenderMoney(C_BaseEntity* pEnt);
	void RenderOffScreen(C_BaseEntity* pEnt, C_BaseEntity* pLocal);

};
extern ESP g_ESP;
