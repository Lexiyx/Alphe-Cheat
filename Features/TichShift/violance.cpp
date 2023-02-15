#include "violance.h"
#include "../../Hooks.h"
#include "../../features/Features.h"
#include <string>
#include <intrin.h>
#include "../../SDK/IBaseClientDll.h"
#include "../../SDK/IVEngineClient.h"
#include "../../SDK/CPrediction.h"
#include "../../SDK/CEntity.h"
#include "../../Menu/Menu.h"

CTickbase g_Violance;
bool CTickbase::CanProcessPacket(CUserCmd* m_pCmd)
{
	m_nChokeLimit = 14;
	m_nShift = 0;
	m_iMaxProcessTicks++;
	m_bEnabled = g_Menu.Config.Doubletap;

	C_BaseCombatWeapon* m_pWeapon = reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntity(g::pLocalEntity->GetActiveWeaponIndex()));
	if (!m_bEnabled || !m_pWeapon || m_pWeapon->IsKnifeorNade() || (int)m_pWeapon->GetItemDefinitionIndex() == 64)
	{
		if (!g_Menu.Config.Doubletap)
			m_bSwap = false;

		return true;
	}

	if (m_bSwap)
	{
		if (m_iMaxProcessTicks >= 12 || g_pClientState->m_choked_commands >= 5)
		{
			m_nChokeLimit = 1;
			m_nShift = 9;

			if (g_Menu.Config.Doubletap)
			{
				if (CanShift(12) || (!CanShift() && m_bNextShift))
					m_nShift = 12;
				else if (CanShift())
					m_nShift = 0;
			}
			else
				m_bNextShift = false;

			return true;
		}
		else
		{
			m_nShift = 0;
			m_bEnabled = false;

			return false;
		}
	}
	else
	{
		m_iMaxProcessTicks = 0;
		m_nShift = 0;
		m_bEnabled = false;
		m_pCmd->tick_count = INT_MAX;
		m_bSwap = true;

		return true;
	}
}
bool CTickbase::CanShift(int m_nShiftedTicks)
{
	if (m_bProcessPacket)
		return false;

	C_BaseCombatWeapon* m_pWeapon = reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntity(g::pLocalEntity->GetActiveWeaponIndex()));
	if (!m_pWeapon)
		return false;

	if (g::pLocalEntity->GetFlags() & FL_ATCONTROLS)
		return false;

	if (m_pWeapon->GetAmmo() != 0)
		return false;

	if (m_pWeapon->IsKnifeorNade() || (int)m_pWeapon->GetItemDefinitionIndex() == 64)
		return false;

	int m_nTickbase = 0;
	if (m_bEnabled && m_nShift > 0)
		m_nTickbase = g::pLocalEntity->GetTickBase() - m_nShift + 1;

	if (m_nShiftedTicks > 0)
		m_nTickbase = g::pLocalEntity->GetTickBase() - m_nShiftedTicks;

	float m_flPlayerTime = m_nTickbase * g_pGlobalVars->intervalPerTick;
	if (m_flPlayerTime < min(g::pLocalEntity->GetNextAttack(), m_pWeapon->GetNextPrimaryAttack()))
		return false;

	return true;
}
void CTickbase::DoubleTap(CUserCmd* m_pCmd, bool* m_pbSendPacket)
{
	C_BaseCombatWeapon* m_pWeapon = reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntity(g::pLocalEntity->GetActiveWeaponIndex()));
	if (!m_pWeapon)
		return;

	if (!(m_pCmd->buttons & IN_ATTACK) || !CanShift() || m_nShift <= 0 || !m_bEnabled)
		return;

	if (g_pClientState->m_choked_commands < min(m_nChokeLimit, 14))
		*m_pbSendPacket = false;

	m_ShotData.m_nCmd = m_pCmd->command_number;
	m_ShotData.m_nShift = m_nShift;
	m_ShotData.m_nTickbase = g::pLocalEntity->GetTickBase();
}
void CTickbase::ShiftTickbase(CUserCmd* m_pCmd, bool m_bSendPacket)
{
	if (!m_bSendPacket)
	{
		m_bProcessPacket = (m_pCmd->buttons & IN_ATTACK) && CanShift();
		return;
	}

	m_ShotData.m_nTickbase = 0;
	if (!m_bEnabled)
		return;

	if (m_bProcessPacket || ((m_pCmd->buttons & IN_ATTACK) && CanShift()))
	{
		m_ShotData.m_nTickbase = -m_nShift;

		if (m_nShift == 12)
			m_bNextShift = false;
		else if (!m_nShift)
			m_bNextShift = true;
	}

	m_bProcessPacket = false;
	m_bSwap = false;
}

void CTickbase::ResetData()
{
	m_ShotData.m_nTickbase = 0;
	m_iMaxProcessTicks = 0;
	m_bSwap = false;
}