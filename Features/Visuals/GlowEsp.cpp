#include "GlowEsp.h"
#include "..\..\Menu\Menu.h"
#include "..\..\SDK\CGlowObjectManager.h"
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CEntity.h"
#include "..\..\SDK\ClientClass.h"

GlowEsp g_GlowEsp;

void GlowEsp::RunGlow()
{
	if (!g::pLocalEntity)
		return;

	if (g_Menu.Config.EnemyGlow || g_Menu.Config.TeamGlow || g_Menu.Config.LocalGlow)
	{
		for (auto i = 0; i < g_GlowManager->GetSize(); i++)
		{
			GlowObjectDefinition_t& GlowObject = g_GlowManager->m_GlowObjectDefinitions[i];
			C_BaseEntity* GlowEntitiy = reinterpret_cast<C_BaseEntity*>(GlowObject.m_pEntity);

			if (!GlowEntitiy)
				continue;

			if (GlowObject.IsUnused())
				continue;

			bool IsLocal = GlowEntitiy == g::pLocalEntity;
			bool IsTeam = g::pLocalEntity->GetTeam() == GlowEntitiy->GetTeam() && !IsLocal;
			bool IsEnemy = GlowEntitiy->GetTeam() != g::pLocalEntity->GetTeam();
			//bool IsWeapon = EntityFlags->isWeapon;
			//bool IsWeapon = GlowEntitiy == g::weapon;

			GlowObject.m_bFullBloomRender = false;

			if (IsLocal)
			{
				if (g_Menu.Config.LocalGlow && g_Menu.Config.IsThirdperson) {
					int class_id = GlowEntitiy->GetClientClass()->ClassID;

					switch (class_id)
					{
					default:
						GlowObject.m_flAlpha = 0.0f;
						break;
					case EClassIds::CCSPlayer:
						GlowObject.m_nGlowStyle = g_Menu.Config.LocalGlowMode;
						GlowObject.m_flAlpha = g_Menu.Config.LocalGlowOpacity / 255.f;
						break;
					}

					GlowObject.m_flRed = g_Menu.Config.LocalGlowColor.red / 255.0f;
					GlowObject.m_flGreen = g_Menu.Config.LocalGlowColor.green / 255.0f;
					GlowObject.m_flBlue = g_Menu.Config.LocalGlowColor.blue / 255.0f;
					GlowObject.m_bRenderWhenOccluded = true;
					GlowObject.m_bRenderWhenUnoccluded = false;
				}
			}
			else if (!IsTeam)
			{
				if (g_Menu.Config.EnemyGlow) {
					int class_id = GlowEntitiy->GetClientClass()->ClassID;


					switch (class_id)
					{
					default:
						GlowObject.m_flAlpha = 0.0f;
						break;
					case EClassIds::CCSPlayer:
						GlowObject.m_nGlowStyle = g_Menu.Config.EnemyGlowMode;
						GlowObject.m_flAlpha = g_Menu.Config.EnemyGlowOpacity / 255.f;
						break;
					}

					GlowObject.m_flRed = g_Menu.Config.EnemyGlowColor.red / 255.0f;
					GlowObject.m_flGreen = g_Menu.Config.EnemyGlowColor.green / 255.0f;
					GlowObject.m_flBlue = g_Menu.Config.EnemyGlowColor.blue / 255.0f;
					GlowObject.m_bRenderWhenOccluded = true;
					GlowObject.m_bRenderWhenUnoccluded = false;
				}
			}
			else if (IsTeam)
			{
				if (g_Menu.Config.TeamGlow) {
					int class_id = GlowEntitiy->GetClientClass()->ClassID;


					switch (class_id)
					{
					default:
						GlowObject.m_flAlpha = 0.0f;
						break;
					case EClassIds::CCSPlayer:
						GlowObject.m_nGlowStyle = g_Menu.Config.TeamGlowMode;;
						GlowObject.m_flAlpha = g_Menu.Config.TeamGlowOpacity / 255.f;
						break;
					}

					GlowObject.m_flRed = g_Menu.Config.TeamGlowColor.red / 255.0f;
					GlowObject.m_flGreen = g_Menu.Config.TeamGlowColor.green / 255.0f;
					GlowObject.m_flBlue = g_Menu.Config.TeamGlowColor.blue / 255.0f;
					GlowObject.m_bRenderWhenOccluded = true;
					GlowObject.m_bRenderWhenUnoccluded = false;
				}
			}
		}
	}
}