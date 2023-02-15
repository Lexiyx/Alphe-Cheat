#pragma once

#include "..\Aimbot\Autowall.h"
#include "..\Aimbot\Aimbot.h"
#include "..\Aimbot\LagComp.h"
#include "..\Aimbot\EnginePrediction.h"
#include "..\..\Utils\GlobalVars.h"
#include "..\..\Utils\Math.h"
#include "..\..\SDK\ICvar.h"
#include "..\..\SDK\CPrediction.h"
#include "..\..\Menu\Menu.h"
#include "..\..\Hooks.h"
#include "..\..\Menu\Hotkeys.h"
#include "..\..\Utils\Interfaces.h"
#include "..\..\Utils\Math.h"
#include "..\Aimbot\EnginePrediction.h"
#include <iostream>
#include <algorithm>
#include "..\\..//Utils\Interfaces.h"
#include <memory>
#include <memory.h>
#include <memory.h>
#include "../../SDK/IClientState.h"
#include "../../Menu/Menu.h"
#include <Windows.h>
#include "../../Utils/xor.h"
#include "../../SDK/CHudChat.h"

#define _SOLVEY(a, b, c, d, e, f) ((c * b - d * a) / (c * f - d * e))
#define SOLVEY(...) _SOLVEY(?, ?, ?, ?, ?, ?)
#define SOLVEX(y, world, forward, right) ((world.x - right.x * y) / forward.x)
#define Square(x) ((x)*(x))

class Misc
{
public:
	int counter = 0;
	void OnPaintTraverse()
	{
		this->bulletTrace();
		this->Crosshair();
	};
	void OnCreateMove()
	{
		this->pCmd = g::pCmd;
		this->pLocal = g::pLocalEntity;

		this->DoAutostrafe();
		this->DoBhop();
		this->DoSlideWalk();
		this->DoSlowWalk();
		this->AnimClantag();
		this->DoKnifeLeftSide();
		this->LinearExtrapolations();
		this->DoDisablePostProcess();
		this->ConsoleModulate();
		this->Airstuck();
		this->Nadetracer2();
		this->Ragdollgrav();
		this->EngineCrosshair();
		this->Noflash();
		this->Legitbhop();
		this->fakeping();
		this->tombstone();
		this->walkwithmenu();
		this->Doorspam();
		this->Walkbot();
		this->antiwalkwithmenu();
		this->quickswitch();
		this->rankreveal();
		this->rainbowhud();
		this->SetLocalPlayerReady();
		this->autokillself();
	};


	void svcheats()
	{
		ConVar* sv_cheats = g_pCvar->FindVar(SteamStrings("sv_cheats"));
		if (g_Menu.Config.sv_cheats)
		{
			*(int*)((DWORD)&sv_cheats->fnChangeCallback + 0xC) = 0; // ew
			sv_cheats->SetValue(1);
			g_Menu.Config.esv = true;
		}
		if (g_Menu.Config.sv_cheats)
		{
			*(int*)((DWORD)&sv_cheats->fnChangeCallback + 0xC) = 0; // ew
			sv_cheats->SetValue(0);
			g_Menu.Config.esv = false;
		}
	}

	void ConsoleModulate()
	{
		if (!g_pEngine->Con_IsVisible() || g_Menu.menuOpened || !g_Menu.Config.ColorModulateConsole)
			return;

		static bool bShouldRecolorConsole;
		static IMaterial* cMaterial[5];
		if (!cMaterial[0] || !cMaterial[1] || !cMaterial[2] || !cMaterial[3] || !cMaterial[4])
		{
			for (MaterialHandle_t i = g_pMaterialSys->FirstMaterial(); i != g_pMaterialSys->InvalidMaterial(); i = g_pMaterialSys->NextMaterial(i))
			{
				auto pMaterial = g_pMaterialSys->GetMaterial(i);
				if (!pMaterial) continue;

				if (strstr(pMaterial->GetName(), SteamStrings("vgui_white")))
					cMaterial[0] = pMaterial;
				else if (strstr(pMaterial->GetName(), SteamStrings("800corner1")))
					cMaterial[1] = pMaterial;
				else if (strstr(pMaterial->GetName(), SteamStrings("800corner2")))
					cMaterial[2] = pMaterial;
				else if (strstr(pMaterial->GetName(), SteamStrings("800corner3")))
					cMaterial[3] = pMaterial;
				else if (strstr(pMaterial->GetName(), SteamStrings("800corner4")))
					cMaterial[4] = pMaterial;
			}
		}
		else
		{


			bShouldRecolorConsole = true;
			if (bShouldRecolorConsole)
			{
				for (int num = 0; num < 5; num++)
				{
					cMaterial[num]->ColorModulate(g_Menu.Config.ConsoleColor.red / 255.f, g_Menu.Config.ConsoleColor.green / 255.f, g_Menu.Config.ConsoleColor.blue / 255.f);
					cMaterial[num]->AlphaModulate(0.85f);
				}
			}
		}
	}

	void autokillself()
	{

		auto localhealth = g::pLocalEntity->GetHealth();
		if (g_Menu.Config.autokillself)
		{
			if (!g::pLocalEntity->IsAlive())
				return;

			if (localhealth < 100)
			{
				g_pEngine->ExecuteClientCmd(SteamStrings("kill"));
				g_pEngine->ExecuteClientCmd(SteamStrings("rs"));
			}
		}
	}

	void clearconsole()
	{

	}

	/*void EdgeJump()
	{
		int old_flags = g::pLocalEntity->GetFlags();

		g_EnginePrediction.Start();
		{
			if (g_Menu.Config.EdgeJump && (old_flags & FL_ONGROUND) && !(g::pLocalEntity->GetFlags() & FL_ONGROUND))
				g::pCmd->buttons |= IN_JUMP;
		}
		g_EnginePrediction.End();
	}*/
	/*
	void DoJumpBug()
	{
		if (!g_Menu.Config.jumpbug)
			return;
		C_BaseEntity* local = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
		float max_radias = D3DX_PI * 2;
		float step = max_radias / 128;
		float xThick = 23;

		if (g_Menu.Config.jumpbug && g_pInputSystem->IsButtonDown(g_Menu.Config.jumpbug) && (local->GetFlags() & (1 << 0))) {
			g_Menu.Config.Bhop = false;
			int screenWidth, screenHeight;
			g_pEngine->GetScreenSize(screenWidth, screenHeight);
			if (unduck) {
				g_Settings.bDidJump = false;
				pCmd->buttons &= ~IN_DUCK; // duck
				pCmd->buttons |= IN_JUMP; // jump
				unduck = false;
			}
			Vector pos = local->GetOrigin();
			for (float a = 0.f; a < max_radias; a += step) {
				Vector pt;
				pt.x = (xThick * cos(a)) + pos.x;
				pt.y = (xThick * sin(a)) + pos.y;
				pt.z = pos.z;


				Vector pt2 = pt;
				pt2.z -= 6;

				trace_t fag;

				Ray_t ray;
				ray.Init(pt, pt2);

				CTraceFilter flt;
				flt.pSkip = local;
				g_pEngineTrace->TraceRay(ray, MASK_SOLID_BRUSHONLY, &flt, &fag);

				if (fag.fraction != 1.f && fag.fraction != 0.f) {
					g_Settings.bDidJump = true;
					pCmd->buttons |= IN_DUCK; // duck
					pCmd->buttons &= ~IN_JUMP; // jump
					unduck = true;
				}
			}
			for (float a = 0.f; a < max_radias; a += step) {
				Vector pt;
				pt.x = ((xThick - 2.f) * cos(a)) + pos.x;
				pt.y = ((xThick - 2.f) * sin(a)) + pos.y;
				pt.z = pos.z;

				Vector pt2 = pt;
				pt2.z -= 6;

				trace_t fag;

				Ray_t ray;
				ray.Init(pt, pt2);

				CTraceFilter flt;
				flt.pSkip = local;
				g_enginetr->TraceRay(ray, MASK_SOLID_BRUSHONLY, &flt, &fag);

				if (fag.fraction != 1.f && fag.fraction != 0.f) {
					g_Settings.bDidJump = true;
					pCmd->buttons |= IN_DUCK; // duck
					pCmd->buttons &= ~IN_JUMP; // jump
					unduck = true;
				}
			}
			for (float a = 0.f; a < max_radias; a += step) {
				Vector pt;
				pt.x = ((xThick - 20.f) * cos(a)) + pos.x;
				pt.y = ((xThick - 20.f) * sin(a)) + pos.y;
				pt.z = pos.z;

				Vector pt2 = pt;
				pt2.z -= 6;

				trace_t fag;

				Ray_t ray;
				ray.Init(pt, pt2);

				CTraceFilter flt;
				flt.pSkip = local;
				g_pEngineTrace->TraceRay(ray, MASK_SOLID_BRUSHONLY, &flt, &fag);

				if (fag.fraction != 1.f && fag.fraction != 0.f) {
					g_Settings.bDidJump = true;
					pCmd->buttons |= IN_DUCK; // duck
					pCmd->buttons &= ~IN_JUMP; // jump
					unduck = true;
				}
			}
		}



		else { g_Settings.misc.bBhopEnabled = true; }

	}
	*/
	void rainbowhud()
	{
		if (!g_Menu.Config.rainbowhud)
			return;

		g_pEngine->ExecuteClientCmd(SteamStrings("cl_hud_color 5"));
		Sleep(300);
		g_pEngine->ExecuteClientCmd(SteamStrings("cl_hud_color 6"));
		Sleep(300);
		g_pEngine->ExecuteClientCmd(SteamStrings("cl_hud_color 7"));
		Sleep(300);
		g_pEngine->ExecuteClientCmd(SteamStrings("cl_hud_color 8"));
		Sleep(300);
		g_pEngine->ExecuteClientCmd(SteamStrings("cl_hud_color 9"));
		Sleep(300);
		g_pEngine->ExecuteClientCmd(SteamStrings("cl_hud_color 2"));
		Sleep(300);
		g_pEngine->ExecuteClientCmd(SteamStrings("cl_hud_color 3"));
		Sleep(300);
		g_pEngine->ExecuteClientCmd(SteamStrings("cl_hud_color 10"));
		Sleep(300);
		g_pEngine->ExecuteClientCmd(SteamStrings("cl_hud_color 4"));
		Sleep(300);
	}



	void Airstuck()
	{
		//auto cmd = g::pCmd;

		if (g_Menu.Config.Airstuck && GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.Airstuckkey)))
		{
			if (g::pLocalEntity->IsAlive())
			{
				g::pCmd->command_number = INT_MAX;
				g_pGlobalVars->tickcount = INT_MAX;
			}

		}

	}

	void Walkbot()
	{
		Vector velocity = g::pLocalEntity->GetVelocity(); velocity.z = 0;
		float speed = velocity.Length();
		if (!g_Menu.Config.Walkbot)
		{
			return;
		}

		g_pEngine->ExecuteClientCmd(SteamStrings("-right"));
		g_pEngine->ExecuteClientCmd(SteamStrings("-left"));
		g_pEngine->ExecuteClientCmd(SteamStrings("+forward"));

		if (speed < 100)
		{
			g_pEngine->ExecuteClientCmd(SteamStrings("+right"));

		}

		if (speed < 50)
		{
			g_pEngine->ExecuteClientCmd(SteamStrings("-right"));
			g_pEngine->ExecuteClientCmd(SteamStrings("+left"));

			if (speed > 120)
				return;
		}

	}

	void MovementFix(Vector& oldang)
	{
		Vector vMovements(g::pCmd->forwardmove, g::pCmd->sidemove, 0.f);

		if (vMovements.Length2D() == 0)
			return;

		Vector vRealF, vRealR;
		Vector aRealDir = g::pCmd->viewangles;
		aRealDir.Clamp();

		g_Math.AngleVectors(aRealDir, &vRealF, &vRealR, nullptr);
		vRealF[2] = 0;
		vRealR[2] = 0;

		VectorNormalize(vRealF);
		VectorNormalize(vRealR);

		Vector aWishDir = oldang;
		aWishDir.Clamp();

		Vector vWishF, vWishR;
		g_Math.AngleVectors(aWishDir, &vWishF, &vWishR, nullptr);

		vWishF[2] = 0;
		vWishR[2] = 0;

		VectorNormalize(vWishF);
		VectorNormalize(vWishR);

		Vector vWishVel;
		vWishVel[0] = vWishF[0] * g::pCmd->forwardmove + vWishR[0] * g::pCmd->sidemove;
		vWishVel[1] = vWishF[1] * g::pCmd->forwardmove + vWishR[1] * g::pCmd->sidemove;
		vWishVel[2] = 0;

		float a = vRealF[0], b = vRealR[0], c = vRealF[1], d = vRealR[1];
		float v = vWishVel[0], w = vWishVel[1];

		float flDivide = (a * d - b * c);
		float x = (d * v - b * w) / flDivide;
		float y = (a * w - c * v) / flDivide;

		g::pCmd->forwardmove = x;
		g::pCmd->sidemove = y;
	}
	void EngineCrosshair()
	{
		static auto showSpread = g_pCvar->FindVar(SteamStrings("weapon_debug_spread_show"));
		showSpread->SetValue(g_Menu.Config.ForceCrosshair && !g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer())->IsScoped() ? 3 : 0);
	}

	void AsusProps()
	{
		bool update = false;
		if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame() || !g::pLocalEntity || !g::pLocalEntity->IsAlive())
		{
			return;
		}
		static ConVar* r_DrawSpecificStaticProp;
		if (g_Menu.Config.updateworld)
			update = true;
		if (false)//(update && g_Menu.Config.asuswalls)
		{
			for (MaterialHandle_t i = g_pMaterialSys->FirstMaterial(); i != g_pMaterialSys->InvalidMaterial(); i = g_pMaterialSys->NextMaterial(i))
			{
				IMaterial* pMaterial = g_pMaterialSys->GetMaterial(i);
				if (!pMaterial)
					continue;
				if (strstr(pMaterial->GetTextureGroupName(), SteamStrings("World")) || strstr(pMaterial->GetTextureGroupName(), SteamStrings("StaticProp")))
				{
					if (strstr(pMaterial->GetTextureGroupName(), SteamStrings("StaticProp textures")))
					{

					}
				}
			}
			g_Menu.Config.updateworld = false;
			update = false;
		}

	}

	void Crosshair()
	{
		if (!g::pLocalEntity)
			return;

		if (g::pLocalEntity->IsScoped() && g_Menu.Config.NoScope && g::pLocalEntity->IsAlive())
		{
			int Height, Width;
			g_pEngine->GetScreenSize(Width, Height);

			Vector punchAngle = g::pLocalEntity->GetAimPunchAngle();

			float x = Width / 2;
			float y = Height / 2;
			int dy = Height / 90;
			int dx = Width / 90;
			x -= (dx * (punchAngle.y));
			y += (dy * (punchAngle.x));

			Vector2D screenPunch = { x, y };

			g_pSurface->Line(0, Height / 2, Width, Height / 2, Color(0, 0, 0, 255));
			g_pSurface->Line(Width / 2, 0, Width / 2, Height, Color(0, 0, 0, 255));
		}

		static bool init = false;
		static bool init2 = false;

		if (g_Menu.Config.Crosshair && g_Menu.Config.Aimbot)
		{
			if (g::pLocalEntity->IsScoped())
			{
				if (init2 && g_Menu.Config.esv)
				{
					g_pEngine->ExecuteClientCmd(SteamStrings("weapon_debug_spread_show 0"));
					g_pEngine->ExecuteClientCmd(SteamStrings("crosshair 0"));
				}
				init2 = false;
			}
			else
			{
				if (!init2 && g_Menu.Config.esv)
				{
					g_pEngine->ExecuteClientCmd(SteamStrings("weapon_debug_spread_show 3"));
					g_pEngine->ExecuteClientCmd(SteamStrings("crosshair 1"));
				}
				init2 = true;
			}

			init = false;
		}
		else
		{
			if (!init && g_Menu.Config.esv)
			{
				g_pEngine->ExecuteClientCmd(SteamStrings("weapon_debug_spread_show 0"));
				g_pEngine->ExecuteClientCmd(SteamStrings("crosshair 1"));
			}
			init = true;
		}
	}

	void bulletTrace()
	{
		if (!g_pEngine->IsInGame() || !g::pLocalEntity)
			return;

		if (g_Menu.Config.BulletTracers)
		{
			for (unsigned int i = 0; i < trace_logs.size(); i++)
			{
				auto& current = trace_logs.at(i);

				auto* shooter = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(trace_logs[i].userid));


				bool is_local_player = shooter == g::pLocalEntity;
				bool is_teammate = g::pLocalEntity->GetTeam() == shooter->GetTeam() && !is_local_player;


				Color bullet_tracer_color;

				if (is_local_player && g_Menu.Config.monkey) // if the shooter is local player
				{

					Color localclr(g_Menu.Config.BulletTracerLocalColor.red, g_Menu.Config.BulletTracerLocalColor.green, g_Menu.Config.BulletTracerLocalColor.blue);

					if (g_Menu.Config.Impacts)
					{
						switch (g_Menu.Config.ImpactType)
						{
						case 0:
						{
							g_pDebugOverlay->AddBoxOverlay(trace_logs[i].position, Vector(-2, -2, -2), Vector(2, 2, 2), QAngle(0, 0, 0), localclr.red, localclr.green, localclr.blue, 127, 2);
							break;
						}
						case 1:
						{
							g_pDebugOverlay->AddSphereOverlay(trace_logs[i].position, 5, 5, 5, localclr.red, localclr.green, localclr.blue, 127, 2);
							break;
						}
						}
					}

					g_pDebugOverlay->AddLineOverlay(trace_logs[i].start, trace_logs[i].position, localclr.red, localclr.green, localclr.blue, true, 2);

				}
				else if (!is_teammate && g_Menu.Config.monkey2)
				{
					Color enemyclr(g_Menu.Config.BulletTracerEnemyColor.red, g_Menu.Config.BulletTracerEnemyColor.green, g_Menu.Config.BulletTracerEnemyColor.blue);

					if (g_Menu.Config.Impacts)
					{
						switch (g_Menu.Config.ImpactType)
						{
						case 0:
						{
							g_pDebugOverlay->AddBoxOverlay(trace_logs[i].position, Vector(-2, -2, -2), Vector(2, 2, 2), QAngle(0, 0, 0), enemyclr.red, enemyclr.green, enemyclr.blue, 127, 2);
							break;
						}
						case 1:
						{
							g_pDebugOverlay->AddSphereOverlay(trace_logs[i].position, 5, 5, 5, enemyclr.red, enemyclr.green, enemyclr.blue, 127, 2);
							break;
						}
						}
					}

					g_pDebugOverlay->AddLineOverlay(trace_logs[i].start, trace_logs[i].position, enemyclr.red, enemyclr.green, enemyclr.blue, true, 2);
				}

				trace_logs.erase(trace_logs.begin() + i);
			}
		}
	}


	/*void fakePrime()
	{
		static bool lastState = false;

		if (g_Menu.Config.fakeprime != lastState) {
			lastState = g_Menu.Config.fakeprime;

			if (DWORD oldProtect; VirtualProtect(memory.fakePrime, 1, PAGE_EXECUTE_READWRITE, &oldProtect)) {
				constexpr uint8_t patch[]{ 0x74, 0xEB };
				*memory.fakePrime = patch[g_Menu.Config.fakeprime];
				VirtualProtect(memory.fakePrime, 1, oldProtect, nullptr);
			}
		}
	}
	*/
	void Nadetracer2()
	{
		if (!g::pLocalEntity)
			return;
		if (!g::pLocalEntity->IsAlive())
			return;
		if (!g_Menu.Config.Nadetracer2)
			return;


		static bool init = false;
		static bool init2 = false;

		if (g_Menu.Config.Nadetracer2 && g_Menu.Config.esv)
		{
			g_pEngine->ExecuteClientCmd(SteamStrings("cl_grenadepreview 1"));
		}

	}

	void tombstone() const {
		if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame() || !g::pLocalEntity || !g::pLocalEntity->IsAlive())
			return; static ConVar* metal = g_pCvar->FindVar(SteamStrings("cl_phys_show_active"));
		metal->SetValue(g_Menu.Config.tombstone);
	}

	void SetLocalPlayerReady()
	{
		if (!g_Menu.Config.Autoaccept || g_pEngine->IsInGame() || g_pEngine->IsConnected())
			return;

		static auto SetLocalPlayerReadyFn = reinterpret_cast<bool(__stdcall*)(const char*)>(Utils::FindSignature(SteamStrings("client.dll"), SteamStrings("55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12")));

		HWND Hwnd;
		if ((Hwnd = FindWindow(NULL, SteamStrings("Valve001"))) && GetForegroundWindow() == Hwnd) // Gets the csgo window and checks if the active window is csgos window // EDIT: Changed from Valve001 to Counter-Strike: Global Offensive.
		{
			RECT lprect;
			GetClientRect(Hwnd, &lprect); // Gets the resolution of the window
			SendMessage(Hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(lprect.right / 2, lprect.bottom / 2)); // Moves the mouse into the middle of the window
			mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // click 
		}
		else if ((Hwnd = FindWindow(NULL, SteamStrings("Counter-Strike: Global Offensive"))) && GetForegroundWindow() == Hwnd) // Gets the csgo window and checks if the active window is csgos window // EDIT: Changed from Valve001 to Counter-Strike: Global Offensive.
		{
			RECT lprect;
			GetClientRect(Hwnd, &lprect); // Gets the resolution of the window
			SendMessage(Hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(lprect.right / 2, lprect.bottom / 2)); // Moves the mouse into the middle of the window
			mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // click 
		}
		if (SetLocalPlayerReadyFn)
			SetLocalPlayerReadyFn(""); // Calling with "" because then it accepts everytime. Calling it with "deferred" dont always work.
	}


	void walkwithmenu()
	{
		if (!g::pLocalEntity || !g::pLocalEntity->IsAlive())
			return;

		if (!g_Menu.Config.walkwithmenu)
			return;
		if (!g_Menu.menuOpened)
			return;

		if (GetAsyncKeyState('W'))
		{
			g_pEngine->ExecuteClientCmd(SteamStrings("+forward"));
		}

		if (GetAsyncKeyState('A'))
		{
			g_pEngine->ExecuteClientCmd(SteamStrings("+moveleft"));
		}

		if (GetAsyncKeyState('S'))
		{
			g_pEngine->ExecuteClientCmd(SteamStrings("+back"));
		}

		if (GetAsyncKeyState('D'))
		{
			g_pEngine->ExecuteClientCmd(SteamStrings("+moveright"));
		}

		if (GetAsyncKeyState(VK_SPACE))
		{
			g_pEngine->ExecuteClientCmd(SteamStrings("+jump"));
		}



	}

	void antiwalkwithmenu()
	{
		if (!g_Menu.Config.walkwithmenu)
			return;
		if (!g::pLocalEntity || !g::pLocalEntity->IsAlive())
			return;

		if (!GetAsyncKeyState('W'))
		{
			g_pEngine->ExecuteClientCmd(SteamStrings("-forward"));
		}

		if (!GetAsyncKeyState('A'))
		{
			g_pEngine->ExecuteClientCmd(SteamStrings("-moveleft"));
		}

		if (!GetAsyncKeyState('S'))
		{
			g_pEngine->ExecuteClientCmd(SteamStrings("-back"));
		}

		if (!GetAsyncKeyState('D'))
		{
			g_pEngine->ExecuteClientCmd(SteamStrings("-moveright"));
		}

		if (!GetAsyncKeyState(VK_SPACE))
		{
			g_pEngine->ExecuteClientCmd(SteamStrings("-jump"));
		}

	}

	void Ragdollgrav()
	{
		if (!g_Menu.Config.esv)
			return;

		if (g_Menu.Config.Ragdollgrav)
		{
			g_pEngine->ExecuteClientCmd(SteamStrings("cl_ragdoll_gravity -500"));
		}

		else if (!g_Menu.Config.Ragdollgrav)
		{
			g_pEngine->ExecuteClientCmd(SteamStrings("cl_ragdoll_gravity 800"));
		}
	}


	void FixedBombPlant(CUserCmd* cmd)
	{
		if (!(cmd->buttons & IN_USE) && !(cmd->buttons & IN_ATTACK))
			return;
		if (!g::pLocalEntity || !g::pLocalEntity->IsAlive() || !g_Menu.Config.quickerplant)
			return;

		C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)g::pLocalEntity->GetActiveWeapon();
		auto isbomb = pWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_C4;

		if (!pWeapon || !isbomb || g::pLocalEntity->InBombZone() && g::pLocalEntity->GetFlags() & FL_ONGROUND)
			return;//m_bInBombZone

		//keep antiaiming/dont run 10m into site
		cmd->buttons &= ~IN_USE;
		cmd->buttons &= ~IN_ATTACK;
		//jk
	}

	void roll_fov(CViewSetup* pSetup)
	{
		if (g_pEngine->IsInGame() && g::pLocalEntity && g::pLocalEntity->GetActiveWeapon())
		{
			auto ViewModel = reinterpret_cast<C_BaseEntity*>(g_pEntityList->GetClientEntityFromHandle(g::pLocalEntity->m_hViewModel()));
			if (ViewModel)
			{
				auto eyeAng = pSetup->angles;
				eyeAng.z -= g_Menu.Config.roll;
				ViewModel->SetAbsAngles(eyeAng);
			}
		}
	}
	void ThirdPerson()
	{
		bool enable = GetKeyState(g_Menu.Config.ThirdpersonHotkey);
		C_BaseEntity* local_player = g::pLocalEntity;
		/* return if local is nullptr */
		if (g::pLocalEntity == nullptr)
			return;

		/* check if we are connected */
		if (!g_pEngine->IsConnected() && !g_pEngine->IsInGame())
			return;

		/* vec angles */
		static Vector vecAngles;

		/* pointer to localplayer */
		C_BaseEntity* localplayer = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

		/* get view angles */
		g_pEngine->GetViewAngles(vecAngles);
		//g::LastAngle = vecAngles;
		if (g_Menu.Config.Thirdpersonondeath && !g::pLocalEntity->IsAlive() && g::pLocalEntity->GetObserverMode() == 4)
		{
			g::pLocalEntity->SetObserverMode(5);
		}

		/* if we have clicked the key */
		if (g_Menu.Config.thirdperson_alive && enable && g::pLocalEntity->IsAlive())
		{
			/* if we are not in thirdperson */
			if (!g_GameInput->m_fCameraInThirdPerson)
			{
				/* getting correct distance */
				auto GetCorrectDistance = [&localplayer](float ideal_distance) -> float
				{
					/* vector for the inverse angles */
					Vector inverseAngles;
					g_pEngine->GetViewAngles(inverseAngles);

					/* inverse angles by 180 */
					inverseAngles.x *= -1.f, inverseAngles.y += 180.f;

					/* vector for direction */
					Vector direction;
					g_Math.AngleVectors(inverseAngles, &direction);

					/* ray, trace & filters */
					C_Ray ray;
					C_Trace trace;
					CTraceFilter filter;

					/* dont trace local player */
					filter.pSkip = g::pLocalEntity;

					/* create ray */
					ray.Init(localplayer->GetVecOrigin() + localplayer->GetViewOffset(), (localplayer->GetVecOrigin() + localplayer->GetViewOffset()) + (direction * ideal_distance));

					/* trace ray */
					g_pTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);

					/* return the ideal distance */
					return (ideal_distance * trace.flFraction) - 10.f;
				};

				/* change the distance from player to camera */
				vecAngles.z = GetCorrectDistance(120);

				/* make player thirdperson */
				g_GameInput->m_fCameraInThirdPerson = true;

				/* set camera view */
				g_GameInput->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, vecAngles.z);
			}


		}
		else
		{
			/* set player to firstperson */
			g_GameInput->m_fCameraInThirdPerson = false;

			/* return to default view */
			//g_GameInput->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, 0);
		}
	}
#define PI_F	((float)(M_PI))

	void onheadblockbot(CUserCmd* cmd) {
		if (cmd->sidemove != 0 || cmd->forwardmove != 0 || !g_Menu.Config.blockbotonhead || !(GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.BlockBotKey))))
			return;

		C_BaseEntity* local = g::pLocalEntity;
		C_BaseEntity* closest_teammate = nullptr;

		if (!local)
			return;


		float dist = 245.f;
		for (int i = 1; i < 64; i++)
		{
			auto current_ent = g_pEntityList->GetClientEntity(i);
			if (!current_ent || current_ent == local)
				continue;

			if (!current_ent->IsAlive() || current_ent->IsDormant() || current_ent->GetTeam() != local->GetTeam())
				continue;

			float current_dist = local->GetAbsOrigin().DistTo(current_ent->GetAbsOrigin());
			if (current_dist < dist) {
				dist = current_dist;
				closest_teammate = current_ent;
			}
		}

		if (!closest_teammate)
			return;

		Vector entPos = closest_teammate->GetAbsOrigin();
		Vector ownPos = local->GetAbsOrigin();

		/*if (entPos.z < ownPos.z)
		{
			Vector angles = g_Math.CalcAngle(ownPos, entPos);

			float dog = closest_teammate->GetVelocity().Length2D();
			float me = local->GetVelocity().Length2D();
			angles.y -= g::pLocalEntity->GetEyeAngles().y;
			g_Math.NormalizeAngles(angles);//(angles);
			if (angles.y < 0.0f)
				cmd->sidemove = dog * 1.8;
			else if (angles.y > 0.0f)
				cmd->sidemove = -dog * 1.8;
		}
		else if(entPos.z > ownPos.z)
		{*/
		float yaw = cmd->viewangles.y;
		Vector VecForward = ownPos - entPos;

		Vector translatedVelocity = Vector(
			(float)(VecForward.x * cos(yaw / 180 * PI_F) + VecForward.y * sin(yaw / 180 * PI_F)),
			(float)(VecForward.y * cos(yaw / 180 * PI_F) - VecForward.x * sin(yaw / 180 * PI_F)),
			VecForward.z
		);

		cmd->forwardmove = -translatedVelocity.x * 20.f;
		cmd->sidemove = translatedVelocity.y * 20.f;
	}


	void acpectratio(float numbero)
	{
		if (!g_Menu.Config.esv)
			return;
		ConVar* yessoftware = g_pCvar->FindVar(SteamStrings("r_aspectratio"));
		*(int*)((DWORD)&yessoftware->fnChangeCallback + 0xC) = 0;
		yessoftware->SetValue(numbero);
	}

	void SetName(const char* name)
	{
		ConVar* nameConvar = g_pCvar->FindVar(SteamStrings("name"));
		*(int*)((DWORD)&nameConvar->fnChangeCallback + 0xC) = 0;
		nameConvar->SetValueChar(name);
	}

	Color getHealthColor(C_BaseEntity* Entity)
	{
		int red, green, blue, alpha;
		int health = Entity->GetHealth();
		red = 255 - (health * 2.55);
		green = health * 2.55;
		blue = 0;
		return Color(red, green, blue, 255);
	}

	static void SetClanTag(std::string text, std::string text2)
	{
		if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected())
			return;
		Hooks::SetClanTag(text.c_str(), text.c_str());
	}

	static void Marquee(std::string& clantag)
	{
		std::string temp = clantag;
		clantag.erase(0, 1);
		clantag += temp[0];
	}

	static void AnimClantag()
	{
		static bool clreset = false;
		if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame() || !g::pLocalEntity)
		{
			//g_Menu.Config.ClanTagMode = 0;
			if (clreset) {
				g_pEngine->ExecuteClientCmd(SteamStrings("cl_clanid 0"));
				clreset = false;
			}
		}

		if (g_Menu.Config.ClanTagMode == 0)
		{
			if (clreset)
			{
				g_pEngine->ExecuteClientCmd(SteamStrings("cl_clanid 0"));
				clreset = false;
			}
		}

		if (g_Menu.Config.ClanTagMode == 1)
		{
			clreset = true;
			static float oldTime;
			if (g_pEngine->IsConnected())
			{
				if (g_pGlobalVars->curtime - oldTime >= 0.25f)
				{
					SetClanTag(g_Menu.Config.ClantagText, g_Menu.Config.ClantagText);
					oldTime = g_pGlobalVars->curtime;
				}
			}
		}
		else if (g_Menu.Config.ClanTagMode == 2)
		{
			clreset = true;
			// \u0020
			static std::string cur_clantag = SteamStrings("Alpheus \u000F \u000F \u000F \u000F \u000F \u000F \u000F \u000F \u000F");
			static float oldTime;
			int threshold = 35;
			if (g_pEngine->IsConnected())
			{
				if (g_pGlobalVars->tickcount - oldTime >= threshold)
				{
					Marquee(cur_clantag);
					SetClanTag(cur_clantag, cur_clantag);
					oldTime = g_pGlobalVars->tickcount;
				}
			}
		}
		else if (g_Menu.Config.ClanTagMode == 3)
		{
			clreset = true;
			// \u0020
			static std::string cur_clantag = SteamStrings("gamesense \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020");
			static float oldTime;
			int threshold = 35;
			if (g_pEngine->IsConnected())
			{
				if (g_pGlobalVars->tickcount - oldTime >= threshold)
				{
					Marquee(cur_clantag);
					SetClanTag(cur_clantag, cur_clantag);
					oldTime = g_pGlobalVars->tickcount;
				}
			}
		}
		else if (g_Menu.Config.ClanTagMode == 4)
		{
			clreset = true;
			// \u0020
			static std::string cur_clantag = SteamStrings("fatality \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020");
			static float oldTime;

			if (g_pEngine->IsConnected())
			{
				if (g_pGlobalVars->curtime - oldTime >= 0.3f)
				{
					Marquee(cur_clantag);
					SetClanTag(cur_clantag, cur_clantag);
					oldTime = g_pGlobalVars->curtime;
				}
			}
		}
		else if (g_Menu.Config.ClanTagMode == 5)
		{
			clreset = true;
			// \u0020
			static std::string cur_clantag = SteamStrings(" AIMWARE V5");
			static float oldTime;

			if (g_pEngine->IsConnected())
			{
				if (g_pGlobalVars->curtime - oldTime >= 0.35f)
				{
					Marquee(cur_clantag);
					SetClanTag(cur_clantag, cur_clantag);
					oldTime = g_pGlobalVars->curtime;
				}
			}
		}
		else if (g_Menu.Config.ClanTagMode == 6)
		{
			clreset = true;
			static float oldTime;
			int threshold = 35;
			if (g_pEngine->IsConnected())
			{
				if (g_pGlobalVars->tickcount - oldTime >= threshold)
				{
					SetClanTag(g_Menu.Config.Threat, g_Menu.Config.Threat);
					oldTime = g_pGlobalVars->tickcount;
				}
			}
		}
		else if (g_Menu.Config.ClanTagMode == 7)
		{
			clreset = true;
			// \u0020
			static std::string cur_clantag = SteamStrings("               \u0016 \u0016 \u0016 \u0016 \u0016 \u0016 \u0016 \u0016");
			static float oldTime;
			int threshold = 5;
			if (g_pEngine->IsConnected())
			{
				if (g_pGlobalVars->tickcount - oldTime >= threshold)
				{
					Marquee(cur_clantag);
					SetClanTag(cur_clantag, cur_clantag);
					oldTime = g_pGlobalVars->tickcount;
				}
			}
		}
	}

	static bool Misc::RandomBool()
	{
		return 0 + (rand() % (1 - 0 + 1)) == 1;
	}

private:
	CUserCmd* pCmd;
	C_BaseEntity* pLocal;

	void DoBhop() const
	{
		if (!g_Menu.Config.Bhop)
			return;
		if (!g::pLocalEntity->IsAlive())
			return;
		static bool bLastJumped = false;
		static bool bShouldFake = false;
		if (!bLastJumped && bShouldFake)
		{
			bShouldFake = false;
			pCmd->buttons |= IN_JUMP;
		}
		else if (pCmd->buttons & IN_JUMP)
		{
			if (pLocal->GetFlags() & FL_ONGROUND)
				bShouldFake = bLastJumped = true;
			else
			{
				pCmd->buttons &= ~IN_JUMP;
				bLastJumped = false;
			}
		}
		else
			bShouldFake = bLastJumped = false;
	}

	void DoSlideWalk()
	{
		if (!g_Menu.Config.SlideWalk)
			return;

		if (g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_NOCLIP || g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_LADDER)
			return;

		if (pCmd->forwardmove > 0)
		{
			pCmd->buttons |= IN_BACK;
			pCmd->buttons &= ~IN_FORWARD;
		}

		if (pCmd->forwardmove < 0)
		{
			pCmd->buttons |= IN_FORWARD;
			pCmd->buttons &= ~IN_BACK;
		}

		if (pCmd->sidemove < 0)
		{
			pCmd->buttons |= IN_MOVERIGHT;
			pCmd->buttons &= ~IN_MOVELEFT;
		}

		if (pCmd->sidemove > 0)
		{
			pCmd->buttons |= IN_MOVELEFT;
			pCmd->buttons &= ~IN_MOVERIGHT;
		}
	}



	void DoKnifeLeftSide()
	{
		if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected() || !g::pLocalEntity)
			return;

		if (!g_Menu.Config.KnifeLeftSide)
			return;

		ConVar* cl_righthand = g_pCvar->FindVar(SteamStrings("cl_righthand"));

		if (g::pLocalEntity->IsKnifeorNade() && !g::pLocalEntity->IsNade())
			cl_righthand->SetValue(0);
		else
			cl_righthand->SetValue(1);
	}

	template<class T, class U>
	T fine(T in, U low, U high)
	{
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;
	}
	void rankreveal()
	{
		if (!g_Menu.Config.revealranks || !(g::pCmd->buttons & IN_SCORE))
			return;
		g_pClientDll->DispatchUserMessage(50, 0, 0, nullptr);
	}

	void DoAutostrafe() const
	{
		Vector velocity = g::pLocalEntity->GetVelocity(); velocity.z = 0;
		float speed = velocity.Length();

		if (!g_Menu.Config.AutoStrafe)
			return;

		if (g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_NOCLIP || g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_LADDER)
			return;

		if (speed < 40 && g_Menu.Config.Jumpscout && g::pLocalEntity->IsScout())
			return;

		if (g::pLocalEntity->IsAlive() && !(g::pLocalEntity->GetFlags() & FL_ONGROUND) && GetAsyncKeyState(VK_SPACE) && !g_Menu.Config.WASD)
		{
			pCmd->forwardmove = (10000.f / g::pLocalEntity->GetVelocity().Length2D() > 450.f) ? 450.f : 10000.f / g::pLocalEntity->GetVelocity().Length2D();
			pCmd->sidemove = (pCmd->mousedx != 0) ? (pCmd->mousedx < 0.0f) ? -450.f : 450.f : (pCmd->command_number % 2) == 0 ? -450.f : 450.f;
		}

		else if (g_Menu.Config.WASD)
		{
			if (g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_NOCLIP || g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_LADDER)
				return;

			if (speed < 40 && g_Menu.Config.Jumpscout && g::pLocalEntity->IsScout())
				return;
			if (!g::pLocalEntity->IsAlive())
				return;

			if (!(g::pLocalEntity->GetFlags() & FL_ONGROUND))
			{
				if (GetAsyncKeyState('W'))
				{

					pCmd->forwardmove = (10000.f / g::pLocalEntity->GetVelocity().Length2D() > 450.f) ? 450.f : 10000.f / g::pLocalEntity->GetVelocity().Length2D();
					pCmd->sidemove = (pCmd->mousedx != 0) ? (pCmd->mousedx < 0.0f) ? -450.f : 450.f : (pCmd->command_number % 2) == 0 ? -450.f : 450.f;

				}
				else if (GetAsyncKeyState('A')) //a
				{
					pCmd->sidemove = (-1550.f * 5) / g::pLocalEntity->GetVelocity().Length2D();
					pCmd->forwardmove = (pCmd->command_number % 2) == 0 ? 450.f : -450.f;
					if (pCmd->sidemove < -450.f)
						pCmd->sidemove = -450.f;
				}
				else if (GetAsyncKeyState('D')) //d
				{
					pCmd->sidemove = (1550.f * 5) / g::pLocalEntity->GetVelocity().Length2D();
					pCmd->forwardmove = (pCmd->command_number % 2) == 0 ? -450.f : 450.f;
					if (pCmd->sidemove > 450.f)
						pCmd->sidemove = 450.f;
				}
				else if (GetAsyncKeyState('S')) //s
				{
					pCmd->forwardmove = (-1550.f * 5) / g::pLocalEntity->GetVelocity().Length2D();
					pCmd->sidemove = (pCmd->command_number % 2) == 0 ? 450.f : -450.f;
					if (pCmd->forwardmove < -450.f)
						pCmd->forwardmove = -450.f;
				}
				else //default
				{
					pCmd->forwardmove = (10000.f / g::pLocalEntity->GetVelocity().Length2D() > 450.f) ? 450.f : 10000.f / g::pLocalEntity->GetVelocity().Length2D();
					pCmd->sidemove = (pCmd->mousedx != 0) ? (pCmd->mousedx < 0.0f) ? -450.f : 450.f : (pCmd->command_number % 2) == 0 ? -450.f : 450.f;
				}
			}

		}
	}

	int lag_comp_break(bool mode) const
	{
		Vector velocity = g::pLocalEntity->GetVelocity(); velocity.z = 0;
		float speed = velocity.Length();

		if (speed > 0.f) {
			auto distance_per_tick = speed * g_pGlobalVars->intervalPerTick;
			int choked_ticks = std::ceilf(65.f / distance_per_tick);
			return mode ? std::min<int>(choked_ticks, g_Menu.Config.FakeLagOnGround) : std::min<int>(choked_ticks, g_Menu.Config.FakeLagInAir);

		}
		return 1;
	}
	void fakeping()
	{
		ConVar* net_fakelag = g_pCvar->FindVar(SteamStrings("net_fakelag"));
		if (net_fakelag && (g_Menu.Config.Extbt_value > 0))
			net_fakelag->SetValue(g_Menu.Config.Extbt_value);
	}
	void quickswitch()
	{
		static bool dontqs = false;
		static bool quickswitching = false;
		if (!g_Menu.Config.quickswitch)
			return;
		if (!g::pCmd->buttons && IN_ATTACK)
		{
			dontqs = false;
		}

		if (g::pLocalEntity->IsAwp() && g::pCmd->buttons & IN_ATTACK && !quickswitching && !dontqs)
		{
			g_pEngine->ExecuteClientCmd(SteamStrings("slot3"));

			quickswitching = true;

		}

		if (quickswitching && g::pLocalEntity->IsKnife())
		{
			g_pEngine->ExecuteClientCmd(SteamStrings("slot1"));

			quickswitching = false;
			if (g::pCmd->buttons & IN_ATTACK)
			{
				dontqs = true;
			}
		}
	}

	void DoFakeLag() const
	{
		if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame())
			return;
		if (!g::pLocalEntity->IsAlive())
			return;
		if (g::pLocalEntity->IsNade())
			return;
		if (g::isfakeducking || g::IsFakewalking)
			return;
		if (g_pEngine->IsVoiceRecording())
			return;
		if (g::pCmd->buttons & IN_USE)
			return;
		//^^^ Actual returns are in fakelag.cpp
		int fakelagVal;

		if (g::pLocalEntity->GetFlags() & FL_ONGROUND)
			fakelagVal = g_Menu.Config.BreakLagComp ? lag_comp_break(true) : g_Menu.Config.FakeLagOnGround;
		else
			fakelagVal = g_Menu.Config.BreakLagComp ? lag_comp_break(false) : g_Menu.Config.FakeLagInAir;

		auto NetChannel = g_pClientState->m_net_channel; //mby problem?

		if (!NetChannel)
			return;

		static float maxSpeed = 320.f;
		static float Acceleration = 5.5f;
		float maxAccelspeed = Acceleration * maxSpeed * g_pGlobalVars->intervalPerTick;

		float TicksToStop = g::pLocalEntity->GetVelocity().Length() / maxAccelspeed;

		static bool Hitinit = false;
		bool SkipTick = false;
		//static bool canHit = false;
		bool HitCheck = false;

		bool canHit = false;
		static bool init = false;
		static bool stop = true;
		static bool stop2 = true;
		bool skip = false;

		if (g_Menu.Config.FakeLagOnPeek)
		{
			for (int i = 1; i < 64; ++i)
			{
				C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

				if (!pPlayerEntity
					|| !pPlayerEntity->IsAlive()
					|| pPlayerEntity->IsDormant()
					|| pPlayerEntity == g::pLocalEntity
					|| pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
					continue;
				Vector EnemyHead = { pPlayerEntity->GetOrigin().x, pPlayerEntity->GetOrigin().y, (pPlayerEntity->GetHitboxPosition(0, g_Aimbot.Matrix[pPlayerEntity->EntIndex()]).z + 10.f) };

				Vector Head = { g::pLocalEntity->GetOrigin().x, g::pLocalEntity->GetOrigin().y, (g::pLocalEntity->GetHitboxPosition(0, g_Aimbot.Matrix[pPlayerEntity->EntIndex()]).z + 10.f) };
				Vector HeadExtr = (Head + (g::pLocalEntity->GetVelocity() * TIME_TO_TICKS(14)));
				Vector OriginExtr = ((g::pLocalEntity->GetOrigin() + (g::pLocalEntity->GetVelocity() * TIME_TO_TICKS(14))) + Vector(0, 0, 8));

				if (fabs(g::pLocalEntity->GetVelocity().Length2D()) > .1f && (g_Autowall.CanHitFloatingPoint(HeadExtr, EnemyHead) || g_Autowall.CanHitFloatingPoint(OriginExtr, EnemyHead)))
				{
					if (!Hitinit)
					{
						canHit = true;
						Hitinit = true;
					}
					HitCheck = true;
				}
			}

			if (!HitCheck)
				Hitinit = false;

			if (canHit)
			{
				g::bSendPacket = true;
				SkipTick = true;
				g::LagPeek = true;
				canHit = false;
			}

		}
		else
		{
			init = false;
			stop = true;
			stop2 = true;
			g::LagPeek = false;

			g::bSendPacket = (NetChannel->m_nChokedPackets >= fakelagVal);
		}
	}


	void LinearExtrapolations()
	{
		if (g_Menu.Config.Aimbot && g_Menu.Config.FakeLagPrediction)
		{
			auto m_local = g::pLocalEntity;
			if (m_local && m_local->IsAlive()) {
				for (int i = 1; i < 64; i++)
				{
					auto m_entity = g_pEntityList->GetClientEntity(i);
					if (m_entity) {

						float simtime_delta = m_entity->GetSimulationTime() - m_entity->GetOldSimulationTime();
						int choked_ticks = fine(TIME_TO_TICKS(simtime_delta), 1, 15);
						Vector lastOrig;

						if (lastOrig.Length() != m_entity->GetOrigin().Length())
							lastOrig = m_entity->GetOrigin();

						float delta_distance = (m_entity->GetOrigin() - lastOrig).LengthSqr();
						if (delta_distance > 4096.f)
						{
							Vector velocity_per_tick = m_entity->GetVelocity() * g_pGlobalVars->intervalPerTick;
							auto new_origin = m_entity->GetOrigin() + (velocity_per_tick * choked_ticks);
							m_entity->SetAbsOrigin(new_origin);
						}
					}

				}
			}
		}
	}

	void Doorspam()
	{


		if (!g_Menu.Config.Doorspam)
			return;
		if (!GetKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.Doorspamkey)))
			return;

		static bool spam = false;
		static float time = 0;
		if (g_pGlobalVars->curtime > time) {

			g::pCmd->buttons &= IN_USE;

			time = g_pGlobalVars->curtime + 0.2f;
		}
		if (fabs(time - g_pGlobalVars->curtime) > 0.3f)
			time = g_pGlobalVars->curtime;

	}


	void Legitbhop()
	{
		if (!g_Menu.Config.Legitbhop)
			return;

		{
			static int hops_restricted = 0;
			static int hops_hit = 0;
			if (!(g::pCmd->buttons & IN_JUMP)
				|| (g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_LADDER))
				return;
			if (!(g::pLocalEntity->GetFlags() & FL_ONGROUND))
			{
				g::pCmd->buttons &= ~IN_JUMP;
				hops_restricted = 0;
			}
			else if ((rand() % 100 > g_Menu.Config.hophitchance			//chance of hitting first hop is always the same, the 2nd part is that so it always doesn't rape your speed
				&& hops_restricted < g_Menu.Config.Hoplimit				//the same amount, it can be made a constant if you want to or can be removed, up to you
				|| (g_Menu.Config.MaxHops > 0							//force fuck up after certain amount of hops to look more legit, you could add variance to this and
					&& hops_hit > g_Menu.Config.MaxHops)))				//everything but fuck off that's too much customisation in my opinion, i only added this one because prof told me to
			{
				g::pCmd->buttons &= ~IN_JUMP;
				hops_restricted++;
				hops_hit = 0;
			}
			else
				hops_hit++;
		}
	}

	void Noflash()
	{
		if (g_Menu.Config.NoFlash)
		{
			g::pLocalEntity->SetFlashDuration(0);
		}

	}
	inline float FastSqrt1(float x)
	{
		unsigned int i = *(unsigned int*)&x;
		i += 127 << 23;
		i >>= 1;
		return *(float*)&i;
	}
#define square1( x ) ( x * x )
	void DoSlowWalk()
	{
		g::IsSlowwalking = false;
		/*auto velocity = g::pLocalEntity->GetVelocity().Length2D();
		auto weapon = g::pLocalEntity->GetActiveWeapon(); 	if (!weapon) return;
		float min_speed = (float)(FastSqrt(Square(g::pCmd->forwardmove) + Square(g::pCmd->sidemove) + Square(g::pCmd->upmove)));
		float kys = g_Menu.Config.SlowWalkAmount / min_speed;*/

		if (g_Menu.Config.SlowWalk) {
			if (g_Menu.Config.SlowWalkAmount <= 0.f || !GetAsyncKeyState(VK_SHIFT))
				return;

			float min_speed = (float)(FastSqrt1(square1(g::pCmd->forwardmove) + square1(g::pCmd->sidemove) + square1(g::pCmd->upmove)));
			if (min_speed <= 0.f)
				return;

			if (g::pCmd->buttons & IN_DUCK)
				g_Menu.Config.SlowWalkAmount *= 2.94117647f;

			if (min_speed <= g_Menu.Config.SlowWalkAmount)
				return;

			float speed = g_Menu.Config.SlowWalkAmount / min_speed;

			g::pCmd->forwardmove *= speed;
			g::pCmd->sidemove *= speed;
			g::pCmd->upmove *= speed;
			g::IsSlowwalking = true;
		}
	}

	void DoDisablePostProcess()
	{
		if (!g_Menu.Config.esv)
			return;
		ConVar* post_process = g_pCvar->FindVar(SteamStrings("mat_postprocess_enable"));
		*(int*)((DWORD)&post_process->fnChangeCallback) = 1;
		if (!g_Menu.Config.DisablePostProcess)
			post_process->SetValue(1);
		else
			post_process->SetValue(0);
	}

	void AntiClientCrasher()
	{
		if (!g_Menu.Config.AntiClientCrasher)
			return;

		ConVar* cl_invites_only_friends = g_pCvar->FindVar(SteamStrings("cl_invites_only_friends"));
		ConVar* cl_invites_only_mainmenu = g_pCvar->FindVar(SteamStrings("cl_invites_only_mainmenu"));

		if (cl_invites_only_friends->GetInt() != g_Menu.Config.AntiClientCrasher || cl_invites_only_mainmenu->GetInt() != g_Menu.Config.AntiClientCrasher)
		{
			cl_invites_only_friends->SetValue(g_Menu.Config.AntiClientCrasher);
			cl_invites_only_mainmenu->SetValue(g_Menu.Config.AntiClientCrasher);
		}
	}
};
extern Misc g_Misc;