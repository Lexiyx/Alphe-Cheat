
#include "..\Features.h"
#include "..\..\SDK\SDKinc.h"
int* m_nPredictionRandomSeed;
int* m_pSetPredictionPlayer;
float flOldCurtime;
float flOldFrametime;
EnginePrediction g_EnginePrediction;
void set_button_states(int buttons) {
	// last buttons = player buttons
	int* buttons_last = reinterpret_cast<int*>(uintptr_t(g::pLocalEntity) + 0x1018);
	int buttons_changed = buttons ^ *buttons_last;
	// player last buttons = last buttons
	*reinterpret_cast<int*>(uintptr_t(g::pLocalEntity) + 0x100C) = *buttons_last;
	*buttons_last = buttons;
	// button pressed
	*reinterpret_cast<int*>(uintptr_t(g::pLocalEntity) + 0x1010) = buttons & buttons_changed;
	// button released
	*reinterpret_cast<int*>(uintptr_t(g::pLocalEntity) + 0x1014) = buttons_changed & ~buttons;
}
void EnginePrediction::Start()
{
	if (!g_Menu.Config.Aimbot)
		return;

	*m_nPredictionRandomSeed = MD5_PseudoRandom(g::pCmd->command_number) & 0x7FFFFFFF;
	*m_pSetPredictionPlayer = (uintptr_t)g::pLocalEntity;

	flOldCurtime = g_pGlobalVars->curtime;
	flOldFrametime = g_pGlobalVars->frametime;

	g_pGlobalVars->curtime = g::pLocalEntity->GetTickBase() * g_pGlobalVars->intervalPerTick;
	g_pGlobalVars->frametime = g_pGlobalVars->intervalPerTick;

	//Here we're doing CBasePlayer::UpdateButtonState // NOTE: hard to tell when offsets changed, think of more longterm solution or just dont do this.
	CMoveData moveData;
	moveData.m_nButtons = g::pCmd->buttons;
	set_button_states(g::pCmd->buttons);

	if (g::pCmd->impulse)
		*reinterpret_cast<char*>(uintptr_t(g::pLocalEntity) + 0x34) = g::pCmd->impulse;

	g_pMovement->StartTrackPredictionErrors(g::pLocalEntity);

	memset(&moveData, 0, sizeof(CMoveData));
	g_pMoveHelper->SetHost(g::pLocalEntity);
	g_pPrediction->SetupMove(g::pLocalEntity, g::pCmd, g_pMoveHelper, &moveData);
	g_pMovement->ProcessMovement(g::pLocalEntity, &moveData);
	g_pPrediction->FinishMove(g::pLocalEntity, g::pCmd, &moveData);
}

void EnginePrediction::End()
{
	if (!g_Menu.Config.Aimbot)
		return;

	g_pMoveHelper->SetHost(nullptr);

	*m_nPredictionRandomSeed = -1;
	m_pSetPredictionPlayer = nullptr;

	g_pGlobalVars->curtime = flOldCurtime;
	g_pGlobalVars->frametime = flOldFrametime;
}