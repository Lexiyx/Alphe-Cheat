#include "SDKinc.h"
#include "Drops+.h"
#include "../Features/Visuals/ESP.h"
#include <string.h>
#include "../Utils/xor.h"

Drops _drops;

bool Drops::transf(const Vector& in, Vector& out)
{
	static ptrdiff_t ptrViewMatrix;
	if (!ptrViewMatrix)
	{//                                                          
		ptrViewMatrix = static_cast<ptrdiff_t>(Utils::FindSignature(SteamStrings("client.dll"), SteamStrings("0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9")));
		ptrViewMatrix += 0x3;
		ptrViewMatrix = *reinterpret_cast<uintptr_t*>(ptrViewMatrix);
		ptrViewMatrix += 176;
	}
	const matrix3x4& worldToScreen = g_pEngine->MatrixWorldToScreenMatrix(); // matrix



	int ScrW, ScrH;

	g_pEngine->GetScreenSize(ScrW, ScrH);

	float w = worldToScreen[3][0] * in[0] + worldToScreen[3][1] * in[1] + worldToScreen[3][2] * in[2] + worldToScreen[3][3];
	out.z = 0; // 0 poniewaz z nie jest nam potrzebne | uzywamy tylko wysokosci i szerokosci (x,y)
	if (w > 0.01)
	{
		float inverseWidth = 1 / w; // inverse na 1 pozycje ekranu
		out.x = (ScrW / 2) + (0.5 * ((worldToScreen[0][0] * in[0] + worldToScreen[0][1] * in[1] + worldToScreen[0][2] * in[2] + worldToScreen[0][3]) * inverseWidth) * ScrW + 0.5);
		out.y = (ScrH / 2) - (0.5 * ((worldToScreen[1][0] * in[0] + worldToScreen[1][1] * in[1] + worldToScreen[1][2] * in[2] + worldToScreen[1][3]) * inverseWidth) * ScrH + 0.5);
		return true;
	}
	return false;
}