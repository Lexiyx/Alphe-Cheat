#pragma once

#include <algorithm>
#include <deque>
#include <algorithm>

#include "..\..\Menu\Menu.h"
#include "..\..\SDK\CInput.h"
#include "..\..\SDK\CEntity.h"
#include "..\..\SDK\ICvar.h"
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CModelRender.h"

//enum class FrameStage;
struct CUserCmd;
namespace Backtrack {
	void update() noexcept;
	void run(CUserCmd*) noexcept;
	void AddLatencyToNetwork(NetChannel*, float) noexcept;
	void UpdateIncomingSequences() noexcept;

	struct Record {
		Vector head;
		Vector origin;
		Vector max;
		Vector mins;
		float simulationTime;
		matrix3x4_t matrix[256];
		studiohdr_t* hdr;
	};

	extern std::deque<Record> records[65];

	struct Cvars {
		ConVar* updateRate;
		ConVar* maxUpdateRate;
		ConVar* interp;
		ConVar* interpRatio;
		ConVar* minInterpRatio;
		ConVar* maxInterpRatio;
		ConVar* maxUnlag;
	};

	extern Cvars cvars;
	//Cvars cvars;

	struct IncomingSequence
	{
		int inreliablestate;
		int sequencenr;
		float servertime;
	};

	extern std::deque<IncomingSequence>sequences;

	int timeToTicks(float time) noexcept;
	bool valid(float simtime);
	float getLerp();
	static void init() noexcept
	{
		records->clear();

		cvars.updateRate = g_pCvar->FindVar(SteamStrings("cl_updaterate"));
		cvars.maxUpdateRate = g_pCvar->FindVar(SteamStrings("sv_maxupdaterate"));
		cvars.interp = g_pCvar->FindVar(SteamStrings("cl_interp"));
		cvars.interpRatio = g_pCvar->FindVar(SteamStrings("cl_interp_ratio"));
		cvars.minInterpRatio = g_pCvar->FindVar(SteamStrings("sv_client_min_interp_ratio"));
		cvars.maxInterpRatio = g_pCvar->FindVar(SteamStrings("sv_client_max_interp_ratio"));
		cvars.maxUnlag = g_pCvar->FindVar(SteamStrings("sv_maxunlag"));
	}
}
