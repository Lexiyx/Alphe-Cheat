#include "LegitAntiAim.h"
#include "AntiAim.h"
#include "..\..\Menu\Menu.h"

LegitAntiAim g_LegitAntiAim;
void LegitAntiAim::LegitDesync()
{
	int finalangle;
	float MaxDelta = g_AntiAim.MaxDelta();
	finalangle = g::RealAngle.y;

	finalangle += g_Menu.Config.LegitSide ? -MaxDelta : +MaxDelta;
	g::pCmd->viewangles.y = finalangle;
	g_Menu.Config.MaxDesyncDelta = MaxDelta;
}
void LegitAntiAim::Legaa()
{
	if (g_Menu.Config.LegitAA)
	{
		if ((g::pCmd->buttons & IN_USE) || g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_LADDER || (g::pCmd->buttons & IN_ATTACK) || (g::pLocalEntity->IsNade() && g::pLocalEntity->m_fThrowTime() > 0.f) || !g::pLocalEntity->IsAlive())
			return;

		if (g::pLocalEntity->GetFlags() & FL_ONGROUND && g::pCmd->sidemove < 3 && g::pCmd->sidemove > -3) {
			static bool switch_ = false;
			if (switch_)
				g::pCmd->sidemove = 4;
			else
				g::pCmd->sidemove = -4;
			switch_ = !switch_;
		}

		g_AntiAim.SyncDesync();
		if (g::bSendPacket)
		{
			g_LegitAntiAim.LegitDesync();
		}
		else
		{
			Vector view;
			g_pEngine->GetViewAngles(view);
			g::pCmd->viewangles.y = view.y;
		}
	}
}