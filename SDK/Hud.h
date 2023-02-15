#pragma once
#include "..\Hooks.h"
#include "..\Utils\Interfaces.h"

class Hud
{
	enum PanelMode
	{
		PANEL_SHOW = 1,
		PANEL_FADE_OUT = 2
	};

	struct CPanel2D
	{
		void ShowAlert(const char* text, const int mode)
		{
			using Fn = void(__thiscall*)(CPanel2D*, const char*, int);
			static Fn fn = (Fn)Utils::FindSignature("client.dll", "55 8B EC A1 ? ? ? ? 83 EC 08 56 8B F1 57 A8 01 75 26 8B 0D ? ? ? ? 83 C8 01 A3 ? ? ? ? 68 ? ? ? ? 8B 01 FF 90 ? ? ? ? 66 A3 ? ? ? ? A1");

			return fn(this, text, mode);
		}

		void HidePanel()
		{
			using Fn = void(__thiscall*)(CPanel2D*, bool);
			static Fn fn = (Fn)Utils::FindSignature("client.dll", "E8 ? ? ? ? 5F 5E 5B 8B E5 5D C2 04 00 8B D3 B9 ? ? ? ? E8 ? ? ? ? 85 C0 0F 85 ? ? ? ? 8B 44 24 14");

			return fn(this, false);
		}
	};

	struct CHudElement
	{
		CPanel2D* GetPanel2D()
		{
			return reinterpret_cast<CPanel2D*>(uintptr_t(this) - 0x14);
		}

		const char* m_pName;
	};

	struct CCSGO_HudUniqueAlerts : public CHudElement
	{

	};

	struct CHud
	{
		template<class T>
		static T* FindHudElement(const char* name)
		{
			static auto pThis = *reinterpret_cast<DWORD * *> (Utils::FindSignature("client.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1); //(Utilities::Memory::FindPatternV2("client.dll", "B9 ? ? ? ? 68 ? ? ? ? E8 ? ? ? ? 89 46 24") + 1);

			static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(Utils::FindSignature("client.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
			return (T*)find_hud_element(pThis, name);
		}
	};
};

extern Hud g_Hud;