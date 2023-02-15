#pragma once
#include "Menu.h"
#include "..\Utils\GlobalVars.h"
#include "..\SDK\ISurface.h"

class KeyVisualizer
{
public:
	void Draw();

private:

};

void KeyVisualizer::Draw()
{
	static int iWidth, iHeight;
	g_pEngine->GetScreenSize(iWidth, iHeight);

	if (g_Menu.Config.KeyVisualizer)
	{
		int center_x = iWidth / 2;
		int offset_y = 50;
		Color pressed(150, 150, 150);
		Color normal(50, 50, 50);
		// Mouse 1 Box
		if (GetAsyncKeyState(0x01))
		{
			g_pSurface->FilledRect(center_x - 65, offset_y, 40, 40, pressed);
			g_pSurface->DrawT(center_x - 45, offset_y + 14, Color(255, 255, 255), g::SansSerif, true, "M1");
		}
		else
		{
			g_pSurface->FilledRect(center_x - 65, offset_y, 40, 40, normal);
			g_pSurface->DrawT(center_x - 45, offset_y + 14, Color(255, 255, 255), g::SansSerif, true, "M1");
		}
		// W Box
		if (GetAsyncKeyState(0x57))
		{
			g_pSurface->FilledRect(center_x - 20, offset_y, 40, 40, pressed);
			g_pSurface->DrawT(center_x, offset_y + 14, Color(255, 255, 255), g::SansSerif, true, "W");
		}
		else
		{
			g_pSurface->FilledRect(center_x - 20, offset_y, 40, 40, normal);
			g_pSurface->DrawT(center_x, offset_y + 14, Color(255, 255, 255), g::SansSerif, true, "W");
		}
		// Mouse 2 Box
		if (GetAsyncKeyState(0x02))
		{
			g_pSurface->FilledRect(center_x + 25, offset_y, 40, 40, pressed);
			g_pSurface->DrawT(center_x + 45, offset_y + 14, Color(255, 255, 255), g::SansSerif, true, "M2");
		}
		else
		{
			g_pSurface->FilledRect(center_x + 25, offset_y, 40, 40, normal);
			g_pSurface->DrawT(center_x + 45, offset_y + 14, Color(255, 255, 255), g::SansSerif, true, "M2");
		}
		// NEW LINE
		offset_y = offset_y + 45;
		// A Box
		if (GetAsyncKeyState(0x41))
		{
			g_pSurface->FilledRect(center_x - 65, offset_y, 40, 40, pressed);
			g_pSurface->DrawT(center_x - 45, offset_y + 14, Color(255, 255, 255), g::SansSerif, true, "A");
		}
		else
		{
			g_pSurface->FilledRect(center_x - 65, offset_y, 40, 40, normal);
			g_pSurface->DrawT(center_x - 45, offset_y + 14, Color(255, 255, 255), g::SansSerif, true, "A");
		}
		// S Box
		if (GetAsyncKeyState(0x53))
		{
			g_pSurface->FilledRect(center_x - 20, offset_y, 40, 40, pressed);
			g_pSurface->DrawT(center_x, offset_y + 14, Color(255, 255, 255), g::SansSerif, true, "S");
		}
		else
		{
			g_pSurface->FilledRect(center_x - 20, offset_y, 40, 40, normal);
			g_pSurface->DrawT(center_x, offset_y + 14, Color(255, 255, 255), g::SansSerif, true, "S");
		}
		// D Box
		if (GetAsyncKeyState(0x44))
		{
			g_pSurface->FilledRect(center_x + 25, offset_y, 40, 40, pressed);
			g_pSurface->DrawT(center_x + 45, offset_y + 14, Color(255, 255, 255), g::SansSerif, true, "D");
		}
		else
		{
			g_pSurface->FilledRect(center_x + 25, offset_y, 40, 40, normal);
			g_pSurface->DrawT(center_x + 45, offset_y + 14, Color(255, 255, 255), g::SansSerif, true, "D");
		}
		// NEW LINE
		offset_y = offset_y + 45;
		// Shift Box
		if (GetAsyncKeyState(0xA0))
		{
			g_pSurface->FilledRect(center_x - 65, offset_y, 40, 40, pressed);
			g_pSurface->DrawT(center_x - 45, offset_y + 14, Color(255, 255, 255), g::SansSerif, true, "Shift");
		}
		else
		{
			g_pSurface->FilledRect(center_x - 65, offset_y, 40, 40, normal);
			g_pSurface->DrawT(center_x - 45, offset_y + 14, Color(255, 255, 255), g::SansSerif, true, "Shift");
		}
		// Space Box
		if (GetAsyncKeyState(0x20))
		{
			g_pSurface->FilledRect(center_x - 20, offset_y, 40, 40, pressed);
			g_pSurface->DrawT(center_x, offset_y + 14, Color(255, 255, 255), g::SansSerif, true, "Space");
		}
		else
		{
			g_pSurface->FilledRect(center_x - 20, offset_y, 40, 40, normal);
			g_pSurface->DrawT(center_x, offset_y + 14, Color(255, 255, 255), g::SansSerif, true, "Space");
		}
		// Ctrl Box
		if (GetAsyncKeyState(0xA2))
		{
			g_pSurface->FilledRect(center_x + 25, offset_y, 40, 40, pressed);
			g_pSurface->DrawT(center_x + 45, offset_y + 14, Color(255, 255, 255), g::SansSerif, true, "Ctrl");
		}
		else
		{
			g_pSurface->FilledRect(center_x + 25, offset_y, 40, 40, normal);
			g_pSurface->DrawT(center_x + 45, offset_y + 14, Color(255, 255, 255), g::SansSerif, true, "Ctrl");
		}
	}
}
extern KeyVisualizer g_KeyVisualizer;