#pragma once
#include "algorithm"
#include "AntiAim.h"
#include "..\Aimbot\Autowall.h"
#include "..\Aimbot\Aimbot.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\Utils\Math.h"
#include "..\..\Menu\Menu.h"
#include "..\Misc\Misc.h"
#include <iostream>
#include <random>
#include <chrono>


static constexpr int MAX_CHOKE = 14;
static constexpr float TELEPORT_DISTANCE = 64.f;

class CFakeLag
{
public:
	int Fakelag_AdaptiveFactor();
	void do_fakelag(CUserCmd* cmd, C_BaseEntity* local);
};

extern CFakeLag g_Fakelag;