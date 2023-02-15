#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\ISurface.h"

class RemasteredEsp
{
public:
	void Render();

private:
	// Funcs
	void RenderSkeleton(C_BaseEntity* pEnt);
};

extern RemasteredEsp g_RemasteredEsp;
