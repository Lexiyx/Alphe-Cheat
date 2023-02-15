#include "glovechanger.h"

#include <Windows.h>
#include "../../SDK/CEntity.h"
#include "../../Utils/GlobalVars.h"
#include "../../SDK/Definitions.h"
#include "../../SDK/ClientClass.h"
#include "../../SDK/IVModelInfo.h"
#include "../../SDK/ClientClass.h"
#include "../../SDK/IBaseClientDll.h"
#include "../../Menu/Menu.h"
#include "../../Utils/xor.h"
#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
typedef void(*RecvVarProxyFn)(const CRecvProxyData* pData, void* pStruct, void* pOut);
RecvVarProxyFn fnSequenceProxyFnN = nullptr;
RecvVarProxyFn oRecvnModelIndexX;
int modelindex = 0;
int bh;
int sporty;
int slick;
int handwarp;
int motor;
int specialist;
int bhhydra;

void Hooked_RecvProxy_GloveChanger(CRecvProxyData* pData, void* pStruct, void* pOut)
{
	// Get the knife view model id's
	bh = g_pModelInfo->GetModelIndex((SteamStrings("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl")));
	sporty = g_pModelInfo->GetModelIndex((SteamStrings("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl")));
	slick = g_pModelInfo->GetModelIndex((SteamStrings("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl")));
	handwarp = g_pModelInfo->GetModelIndex((SteamStrings("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl")));
	motor = g_pModelInfo->GetModelIndex((SteamStrings("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl")));
	specialist = g_pModelInfo->GetModelIndex((SteamStrings("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl")));
	bhhydra = g_pModelInfo->GetModelIndex((SteamStrings("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl")));



	if (g_Menu.Config.Skinchanger.glovesenabled && g::pLocalEntity)
	{

		if (g::pLocalEntity->IsAlive() && (
			pData->m_Value.m_Int == bh ||
			pData->m_Value.m_Int == sporty ||
			pData->m_Value.m_Int == slick ||
			pData->m_Value.m_Int == handwarp ||
			pData->m_Value.m_Int == motor ||
			pData->m_Value.m_Int == specialist ||
			pData->m_Value.m_Int == bhhydra))
		{
			// Set whatever ghove we want
			if (g_Menu.Config.Skinchanger.gloves == 0)
				pData->m_Value.m_Int = bh;
			else if (g_Menu.Config.Skinchanger.gloves == 9)
				pData->m_Value.m_Int = sporty;
			else if (g_Menu.Config.Skinchanger.gloves == 6)
				pData->m_Value.m_Int = slick;
			else if (g_Menu.Config.Skinchanger.gloves == 7)
				pData->m_Value.m_Int = handwarp;
			else if (g_Menu.Config.Skinchanger.gloves == 1)
				pData->m_Value.m_Int = motor;
			else if (g_Menu.Config.Skinchanger.gloves == 2)
				pData->m_Value.m_Int = specialist;
			else if (g_Menu.Config.Skinchanger.gloves == 5)
				pData->m_Value.m_Int = bhhydra;
		}
	}

	// Carry on the to original proxy
	oRecvnModelIndexX(pData, pStruct, pOut);
}
void gloveload()
{
	ClientClass* pClass = g_pClientDll->GetAllClasses();
	while (pClass)
	{
		const char* pszName = pClass->pRecvTable->pNetTableName;
		if (!strcmp(pszName, ("DT_SmokeGrenadeProjectile")))
		{
			for (int i = 0; i < pClass->pRecvTable->nProps; i++)
			{
				RecvProp* pProp = &(pClass->pRecvTable->pProps[i]);
				const char* name = pProp->pVarName;
				if (!strcmp(name, ("m_bDidSmokeEffect")))
				{
					//fnNoSmoke = (RecvVarProxyFn)pProp->m_ProxyFn;
					//pProp->m_ProxyFn = NoSmoke;
				}

			}
		}
		else if (!strcmp(pszName, ("DT_BaseViewModel")))
		{
			for (int i = 0; i < pClass->pRecvTable->nProps; i++)
			{
				RecvProp* pProp = &(pClass->pRecvTable->pProps[i]);
				const char* name = pProp->pVarName;
				if (!strcmp(name, ("m_nModelIndex")))
				{
					oRecvnModelIndexX = (RecvVarProxyFn)pProp->ProxyFn;
					pProp->ProxyFn = (RecvVarProxyFn)Hooked_RecvProxy_GloveChanger;
				}
			}
		}
		pClass = pClass->pNext;
	}
}
void glovechanger()
{
	const auto pLocal = g::pLocalEntity;

	if (!pLocal)
		return;

	if (g_Menu.Config.Skinchanger.glovesenabled)
	{
		gloveload();
		/*PlayerInfo_s localPlayerInfo;
		if (pLocal && pLocal->GetActiveWeapon() && pLocal->GetActiveWeaponIndex())
		{
			if (g_pEngine->GetPlayerInfo(g_pEngine->GetLocalPlayer(), &localPlayerInfo))
			{
				DWORD* hMyWearables = (DWORD*)((size_t)pLocal->GetWearables()); //m_hMyWearables
																				//GetWearables
				if (hMyWearables)
				{
					if (!g_pEntityList->GetClientEntityFromHandle((DWORD)hMyWearables[0]))
					{
						static ClientClass* pClass;

						if (!pClass)
							pClass = g_pClientDll->GetAllClasses();
						while (pClass)
						{
							if (pClass->ClassID == 52)
								break;
							pClass = pClass->pNext;
						}

						int iEntry = g_pEntityList->GetHighestEntityIndex() + 1;
						int iSerial = RandomInt(0x0, 0xFFF);

						pClass->pCreateFn(iEntry, iSerial);
						hMyWearables[0] = iEntry | (iSerial << 16);

						C_BaseEntity* pEnt = (C_BaseEntity*)g_pEntityList->GetClientEntityFromHandle((DWORD)hMyWearables[0]);

						if (pEnt)
						{
							if (g_Menu.Config.Skinchanger.gloves == 1)
								modelindex = g_pModelInfo->GetModelIndex((SteamStrings("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl")));
							else if (g_Menu.Config.Skinchanger.gloves == 2)
								modelindex = g_pModelInfo->GetModelIndex((SteamStrings("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl")));
							else if (g_Menu.Config.Skinchanger.gloves == 3)
								modelindex = g_pModelInfo->GetModelIndex((SteamStrings("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl")));
							else if (g_Menu.Config.Skinchanger.gloves == 4)
								modelindex = g_pModelInfo->GetModelIndex((SteamStrings("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl")));
							else if (g_Menu.Config.Skinchanger.gloves == 5)
								modelindex = g_pModelInfo->GetModelIndex((SteamStrings("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl")));
							else if (g_Menu.Config.Skinchanger.gloves == 6)
								modelindex = g_pModelInfo->GetModelIndex((SteamStrings("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl")));
							else if (g_Menu.Config.Skinchanger.gloves == 7)
								modelindex = g_pModelInfo->GetModelIndex((SteamStrings("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl")));

							int ItemDefinitionIndex;
							if (g_Menu.Config.Skinchanger.gloves == 1)
							{
								ItemDefinitionIndex = 5027;
							}
							else if (g_Menu.Config.Skinchanger.gloves == 2)
							{
								ItemDefinitionIndex = 5030;
							}
							else if (g_Menu.Config.Skinchanger.gloves == 3)
							{
								ItemDefinitionIndex = 5031;
							}
							else if (g_Menu.Config.Skinchanger.gloves == 4)
							{
								ItemDefinitionIndex = 5032;
							}
							else if (g_Menu.Config.Skinchanger.gloves == 5)
							{
								ItemDefinitionIndex = 5033;
							}
							else if (g_Menu.Config.Skinchanger.gloves == 6)
							{
								ItemDefinitionIndex = 5034;
							}
							else if (g_Menu.Config.Skinchanger.gloves == 7)
							{
								ItemDefinitionIndex = 5035;
							}
							else
								ItemDefinitionIndex = 0;

							int paintkit;
							if (g_Menu.Config.Skinchanger.gloves == 1)
							{
								switch (g_Menu.Config.Skinchanger.skingloves)
								{
								case 0:
									paintkit = 10006;
									break;
								case 1:
									paintkit = 10007;
									break;
								case 2:
									paintkit = 10008;
									break;
								case 3:
									paintkit = 10039;
									break;
								default:
									paintkit = 0; break;
								}
							}
							else if (g_Menu.Config.Skinchanger.gloves == 2)
							{
								switch (g_Menu.Config.Skinchanger.skingloves)
								{
								case 0:
									paintkit = 10038; break;
								case 1:
									paintkit = 10037; break;
								case 2:
									paintkit = 10018; break;
								case 3:
									paintkit = 10019; break;
								case 4:
									paintkit = 10048; break;
								case 5:
									paintkit = 10047; break;
								case 6:
									paintkit = 10045; break;
								case 7:
									paintkit = 10046; break;
								default:
									paintkit = 0; break;
								}
							}
							else if (g_Menu.Config.Skinchanger.gloves == 3)
							{
								switch (g_Menu.Config.Skinchanger.skingloves)
								{
								case 0:
									paintkit = 10013; break;
								case 1:
									paintkit = 10015; break;
								case 2:
									paintkit = 10016; break;
								case 3:
									paintkit = 10040; break;
								case 4:
									paintkit = 10043; break;
								case 5:
									paintkit = 10044; break;
								case 6:
									paintkit = 10041; break;
								case 7:
									paintkit = 10042; break;
								default:
									paintkit = 0; break;
								}
							}
							else if (g_Menu.Config.Skinchanger.gloves == 4)
							{
								switch (g_Menu.Config.Skinchanger.skingloves)
								{
								case 0:
									paintkit = 10009; break;
								case 1:
									paintkit = 10010; break;
								case 2:
									paintkit = 10021; break;
								case 3:
									paintkit = 10036; break;
								case 4:
									paintkit = 10053; break;
								case 5:
									paintkit = 10054; break;
								case 6:
									paintkit = 10055; break;
								case 7:
									paintkit = 10056; break;
								default:
									paintkit = 0; break;
								}
							}
							else if (g_Menu.Config.Skinchanger.gloves == 5)
							{
								switch (g_Menu.Config.Skinchanger.skingloves)
								{
								case 0:
									paintkit = 10024; break;
								case 1:
									paintkit = 10026; break;
								case 2:
									paintkit = 10027; break;
								case 3:
									paintkit = 10028; break;
								case 4:
									paintkit = 10050; break;
								case 5:
									paintkit = 10051; break;
								case 6:
									paintkit = 10052; break;
								case 7:
									paintkit = 10049; break;
								default:
									paintkit = 0; break;
								}
							}
							else if (g_Menu.Config.Skinchanger.gloves == 6)
							{
								switch (g_Menu.Config.Skinchanger.skingloves)
								{
								case 0:
									paintkit = 10030; break;
								case 1:
									paintkit = 10033; break;
								case 2:
									paintkit = 10034; break;
								case 3:
									paintkit = 10035; break;
								case 4:
									paintkit = 10061; break;
								case 5:
									paintkit = 10062; break;
								case 6:
									paintkit = 10063; break;
								case 7:
									paintkit = 10064; break;
								default:
									paintkit = 0; break;
								}
							}
							else if (g_Menu.Config.Skinchanger.gloves == 7)
							{
								switch (g_Menu.Config.Skinchanger.skingloves)
								{
								case 0:
									paintkit = 10057; break;
								case 1:
									paintkit = 10058; break;
								case 2:
									paintkit = 10059; break;
								case 3:
									paintkit = 10060; break;
								default:
									paintkit = 0; break;
								}
							}
							*(int*)((DWORD)pEnt + 0x2FAA) = ItemDefinitionIndex;
							*(int*)((DWORD)pEnt + 0x2FC0) = -1;
							*(int*)((DWORD)pEnt + 0x2FAC) = 4;
							*(int*)((DWORD)pEnt + 0x2FC8) = localPlayerInfo.xuidLow;
							*(float*)((DWORD)pEnt + 0x31C0) = g_Menu.Config.Skinchanger.glovewear;
							*(int*)((DWORD)pEnt + 0x31BC) = 0;
							*(int*)((DWORD)pEnt + 0x31C4) = -1;
							*(int*)((DWORD)pEnt + 0x31B8) = paintkit;

							pEnt->SetModelIndex(modelindex);
							pEnt->PreDataUpdate(0);
						}
					}
				}

			}
		}*/
	}

}