#include "Nightmode.h"
#include "..\..\Utils\GlobalVars.h"
#include "..\..\Utils\Math.h"
#include "..\..\SDK\ICvar.h"
#include "..\..\Menu\Menu.h"
#include "..\..\Hooks.h"
#include "../../Utils/xor.h"
Nightmode g_Nightmode;

void Nightmode::Run()
{
	/*if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame()) {
		return;
	}
	bool update = false;
	bool runonce = false;

	if (g_Menu.Config.updateworld || g_Menu.Config.Nightmode)
		update = true;

	static ConVar*r_DrawSpecificStaticProp;
	static ConVar*mat_ambient_light_r;
	static ConVar*mat_ambient_light_g;
	static ConVar*mat_ambient_light_b;
	if (false)//(update && g_Menu.Config.Nightmode)
	{
		if (!r_DrawSpecificStaticProp)
			r_DrawSpecificStaticProp = g_pCvar->FindVar(SteamStrings("r_DrawSpecificStaticProp"));
		r_DrawSpecificStaticProp->SetValue(0);
		for (MaterialHandle_t i = g_pMaterialSys->FirstMaterial(); i != g_pMaterialSys->InvalidMaterial(); i = g_pMaterialSys->NextMaterial(i))
		{
			IMaterial* pMaterial = g_pMaterialSys->GetMaterial(i);
			if (!pMaterial)
				continue;
			if (g_Menu.Config.preset != 0)
			{
				if (strstr(pMaterial->GetTextureGroupName(), SteamStrings("models/props")) || strstr(pMaterial->GetTextureGroupName(), SteamStrings("StaticProp")))
				{
					pMaterial->ColorModulate(g_Menu.Config.PropColor.red / 255.f, g_Menu.Config.PropColor.green / 255.f, g_Menu.Config.PropColor.blue / 255.f);
				}
			}
			else
			{
				if (strstr(pMaterial->GetTextureGroupName(), SteamStrings("World")) || strstr(pMaterial->GetTextureGroupName(), SteamStrings("StaticProp")))
				{
					pMaterial->ColorModulate(1.0f, 1.0f, 1.0f);
					//pMaterial->ColorModulate(g_Menu.Config.PropColor.red / 255.f, g_Menu.Config.PropColor.green / 255.f, g_Menu.Config.PropColor.blue / 255.f);
				}
			}
		}
		g_Menu.Config.updateworld = false;
		update = false;
		runonce = false;
	}
	else if(false)//(!g_Menu.Config.Nightmode)
	{
		if (!runonce)
		{
			for (MaterialHandle_t i = g_pMaterialSys->FirstMaterial(); i != g_pMaterialSys->InvalidMaterial(); i = g_pMaterialSys->NextMaterial(i))
			{
				IMaterial* pMaterial = g_pMaterialSys->GetMaterial(i);
				if (!pMaterial)
					continue;
				if (g_Menu.Config.preset != 0)
				{
					if (strstr(pMaterial->GetTextureGroupName(), SteamStrings("models/props")) || strstr(pMaterial->GetTextureGroupName(), SteamStrings("StaticProp")))
					{
						pMaterial->ColorModulate(1.0f, 1.0f, 1.0f);
					}
				}
				else
				{
					if (strstr(pMaterial->GetTextureGroupName(), SteamStrings("World")) || strstr(pMaterial->GetTextureGroupName(), SteamStrings("StaticProp")))
					{
						pMaterial->ColorModulate(1.0f, 1.0f, 1.0f);
					}
				}
				runonce = true;
			}
		}
	}
		
		ConVar* mat_force_tonemap_scale = g_pCvar->FindVar(SteamStrings("mat_force_tonemap_scale"));

		if (false)//(g_Menu.Config.ProperNightmode)
		{
			if (mat_force_tonemap_scale->GetFloat() != 0.2f)
			{
				if(g_Menu.Config.esv)
				g_pEngine->ExecuteClientCmd(SteamStrings("mat_force_tonemap_scale 0.2"));
			}

			static float storedCurtime = g_pGlobalVars->curtime;

			double timeSoFar = abs(g_pGlobalVars->curtime - storedCurtime);

			srand(time(NULL));

		}
		else if (false)//(!g_Menu.Config.ProperNightmode && mat_force_tonemap_scale->GetFloat() != 1.f)
		{
			if (g_Menu.Config.esv)
			g_pEngine->ExecuteClientCmd(SteamStrings("mat_force_tonemap_scale 1"));
		}

	//if (g_Menu.Config.AntiUntrusted)
		//return;

	if (!g_Menu.Config.updateworld && g_Menu.Config.AmbientLight)
	{
		if (!mat_ambient_light_r || !mat_ambient_light_g || !mat_ambient_light_b)
		{
			mat_ambient_light_r = g_pCvar->FindVar(SteamStrings("mat_ambient_light_r"));
			mat_ambient_light_g = g_pCvar->FindVar(SteamStrings("mat_ambient_light_g"));
			mat_ambient_light_b = g_pCvar->FindVar(SteamStrings("mat_ambient_light_b"));
		}

		mat_ambient_light_r->SetValue(g_Menu.Config.AmbientLightColor.red / 255.f);
		mat_ambient_light_g->SetValue(g_Menu.Config.AmbientLightColor.green / 255.f);
		mat_ambient_light_b->SetValue(g_Menu.Config.AmbientLightColor.blue / 255.f);
	}*/
}