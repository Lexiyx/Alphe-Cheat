#include "SpecList.h"
#include "../Utils/xor.h"

SpecList g_SpecList;

Color list_background(33, 33, 33);
Color list_accent(222, 118, 0);

void SpecList::Draw()
{
	if (g_Menu.Config.SpecList == 0)
		return;

	int ObserverCount = 0;
	static Vector2D oldPos;
	static Vector2D mousePos;
	static bool Dragging = false;
	bool click = false;
	static int dragX = 0;
	static int dragY = 0;
	C_BaseEntity* entity = nullptr;
	C_BaseEntity* target = nullptr;
	PlayerInfo_t entityinfo;
	PlayerInfo_t targetinfo;

	int Width = 200;

	static int iWidth, iHeight;
	g_pEngine->GetScreenSize(iWidth, iHeight);

	if (GetAsyncKeyState(VK_LBUTTON))
		click = true;

	Vector2D MousePos = g_pSurface->GetMousePosition();

	if (Dragging && !click)
		Dragging = false;

	if (Dragging && click)
	{
		g_Menu.Config.SpecPos.x = MousePos.x - dragX;
		g_Menu.Config.SpecPos.y = MousePos.y - dragY;
	}

	if (g_pSurface->MouseInRegion(g_Menu.Config.SpecPos.x, g_Menu.Config.SpecPos.y, Width, 14))
		Dragging = true;
	/////////////*Menu dragging logic*/////////////
	dragX = MousePos.x - g_Menu.Config.SpecPos.x;
	dragY = MousePos.y - g_Menu.Config.SpecPos.y;
	if (g_Menu.Config.SpecPos.x < 0)
		g_Menu.Config.SpecPos.x = 0;
	if (g_Menu.Config.SpecPos.y < 0)
		g_Menu.Config.SpecPos.y = 0;
	if ((g_Menu.Config.SpecPos.x + Width) > iWidth)
		g_Menu.Config.SpecPos.x = iWidth - Width;
	if ((g_Menu.Config.SpecPos.y + 14) > iHeight)
		g_Menu.Config.SpecPos.y = iHeight - 14;
	///////////////////////////////////////////////
	g_pSurface->FilledRect(g_Menu.Config.SpecPos.x, g_Menu.Config.SpecPos.y, Width, 14, list_background);
	g_pSurface->OutlinedRect(g_Menu.Config.SpecPos.x, g_Menu.Config.SpecPos.y, Width, 14, list_accent);
	g_pSurface->DrawT(g_Menu.Config.SpecPos.x + 2, g_Menu.Config.SpecPos.y + 1, Color(255, 255, 255), g::Tahoma, false, SteamStrings("Spectators"));
	if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame())
		return;
	for (int i = 0; i < g_pEntityList->GetMaxEntities(); i++)
	{
		entity = g_pEntityList->GetClientEntity(i);
		if (!entity)
			continue;
		if (entity->IsAlive())
			continue;
		if (entity->IsDormant())
			continue;
		if (!entity->GetObserverTarget())
			continue;
		target = entity->GetObserverTarget();
		if (!target->IsPlayer() && !entity->IsPlayer()) //Entity is valid?
			continue;
		g_pEngine->GetPlayerInfo(entity->EntIndex(), &entityinfo);
		g_pEngine->GetPlayerInfo(target->EntIndex(), &targetinfo);
		if (entityinfo.szName == NULL || targetinfo.szName == NULL) //Player name is valid?
			continue;
		g_pSurface->FilledRect(g_Menu.Config.SpecPos.x, g_Menu.Config.SpecPos.y + 14 + (ObserverCount * 12), Width, 12, Color(33, 33, 33, 125));
		switch (g_Menu.Config.SpecList)
		{
		case 1:
		{
			if (g::pLocalEntity->EntIndex() == target->EntIndex())
			{
				g_pSurface->DrawT(g_Menu.Config.SpecPos.x + 2, g_Menu.Config.SpecPos.y + 14 + (ObserverCount * 12), Color(255, 255, 255), g::Tahoma, false, (std::string(entityinfo.szName) + " spectating you").c_str());
				ObserverCount++;
			}
			break;
		}
		case 2:
		{
			if (entity->GetTeam() == g::pLocalEntity->GetTeam())
			{
				g_pSurface->DrawT(g_Menu.Config.SpecPos.x + 2, g_Menu.Config.SpecPos.y + 14 + (ObserverCount * 12), Color(255, 255, 255), g::Tahoma, false, (std::string(entityinfo.szName) + " spectating " + targetinfo.szName).c_str());
				ObserverCount++;
			}
			break;
		}
		case 3:
		{
			if (entity->GetTeam() != g::pLocalEntity->GetTeam())
			{
				g_pSurface->DrawT(g_Menu.Config.SpecPos.x + 2, g_Menu.Config.SpecPos.y + 14 + (ObserverCount * 12), Color(255, 255, 255), g::Tahoma, false, (std::string(entityinfo.szName) + " spectating " + targetinfo.szName).c_str());
				ObserverCount++;
			}
			break;
		}
		case 4:
		{
			g_pSurface->DrawT(g_Menu.Config.SpecPos.x + 2, g_Menu.Config.SpecPos.y + 14 + (ObserverCount * 12), Color(255, 255, 255), g::Tahoma, false, (std::string(entityinfo.szName) + " spectating " + targetinfo.szName).c_str());
			ObserverCount++;
			break;
		}
		}
	}
}