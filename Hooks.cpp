#include <thread>
#include <comdef.h>
#include "Hooks.h"
#include "Utils\Utils.h"
#include "Features\Features.h"
#include "Menu\Menu.h"
#include "SDK\Hitboxes.h"
#include "libloaderapi.h"
#include "SDK/IMaterial.h"
#include "SDK/ICvar.h"
#include "SDK/CGlowObjectManager.h"
#include "Utils/Math.h"
#include "Utils\hitsounds.h"
#include "Features/Knife/skinchanger.h"
#include "Features/Knife/glovechanger.h"
#include "SDK/CHudChat.h"
#include "Utils\xor.h"
#include "Utils\Interfaces.h"
#include "Features/Knife/Knife.h"
#include "Features/Visuals/grenadetracer.h"
#include "Features/Visuals/grenadeparth.h"
#include "Menu/SpecList.h"
#include "Features/AntiAim/LegitAntiAim.h"
#include "Features/AntiAim/FakeLag.h"
#include "Features/Resolver/Resolver2.h"
#include "Menu/Config.h"
#include "SDK\CInput.h"
#include "Features/Aimbot/TriggerBot.h"
#include "Features/Misc/Logger.h"
#include "Features/ProfileChanger/ProtoUtils/Protobuffs.h"
#include "Features/TichShift/violance.h"
#include "Features/Logger/Logger_sec.h"
#include "Core/Core.h"
#include <intrin.h>
#include "SDK/SteamAPI.h"
#include "SDK/IGameConsole.h"
#include "Features\Backtrack\Backtrack.h"

Misc     g_Misc;
Hooks    g_Hooks;
Event    g_Event;

HookManager::NewHookedCreateMove::fn oNewHookedCreateMove = nullptr;
HookManager::CreateMove::fn oCreateMove = nullptr;
HookManager::WriteDeltaBuff::fn oWriteDeltafunct = nullptr;
HookManager::FrameStageNotify::fn oFrameStageNotify = nullptr;
HookManager::OverrideView::fn oOverrideView = nullptr;
HookManager::GetViewmodelFov::fn oGetViewmodelFov = nullptr;
HookManager::LockCursor::fn oLockCursor = nullptr;
HookManager::PaintTraverse::fn  oPaintTraverse = nullptr;
HookManager::DrawModelExecute::fn oDrawModelExecute = nullptr;
HookManager::SceneEnd::fn oSceneEnd = nullptr;
HookManager::EmitSound::fn oEmitSound = nullptr;
HookManager::SendDatagram::fn oSendDatagram = nullptr;
HookManager::hkGCSendMessage::GCRetrieveMessage ohkGCSendMessage = nullptr;
HookManager::hkGCRetrieveMessage::GCSendMessage ohkGCRetrieveMessage = nullptr;

int screenSizeX, screenCenterX;
int screenSizeY, screenCenterY;
static bool hooked = false;
static bool bInitializedDrawManager = false;
matrix3x4_t mat[126];
__declspec(naked) void __stdcall CreateMoveFix(int sequence_number, float input_sample_frametime, bool active)
{
	__asm
	{
		push ebx
		push esp
		push dword ptr[esp + 20]
		push dword ptr[esp + 0Ch + 8]
		push dword ptr[esp + 10h + 4]
		call HookManager::NewHookedCreateMove::hook
		pop ebx
		retn 0Ch
	}
}
void Hooks::Init()
{
	// Get window handle
	while (!(g_Hooks.hCSGOWindow = FindWindowA(SteamStrings("Valve001"), nullptr)))
	{
		using namespace std::literals::chrono_literals;
		std::this_thread::sleep_for(50ms);
	}

	interfaces::Init();                         // Get interfaces
	g_pNetvars = std::make_unique<NetvarTree>();// Get netvars after getting interfaces as we use them

	if (g_Hooks.hCSGOWindow)        // Hook WNDProc to capture mouse / keyboard input
		g_Hooks.pOriginalWNDProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(g_Hooks.hCSGOWindow, GWLP_WNDPROC,
			reinterpret_cast<LONG_PTR>(g_Hooks.WndProc)));
	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize MH_Initialize.");

	const auto createmove_t = reinterpret_cast<void*>(HookManager::get_virtual(g_pClientMode, vtable_indexes::createMove));
	const auto newhookedcreateMove_t = reinterpret_cast<void*>(HookManager::get_virtual(g_pClientDll, vtable_indexes::createMoveFix));
	const auto writedeltabuffer_t = reinterpret_cast<bool*>(HookManager::get_virtual(g_pClientDll, 24));
	const auto getviewmodelfov_t = reinterpret_cast<void*>(HookManager::get_virtual(g_pClientMode, vtable_indexes::viewmodelFov));
	const auto framestagenotify_t = reinterpret_cast<void*>(HookManager::get_virtual(g_pClientDll, vtable_indexes::frameStage));
	const auto overrideview_t = reinterpret_cast<void*>(HookManager::get_virtual(g_pClientMode, vtable_indexes::view));
	const auto lockcursor_t = reinterpret_cast<void*>(HookManager::get_virtual(g_pSurface, vtable_indexes::lockCursor));
	const auto drawmodelexecute_t = reinterpret_cast<void*>(HookManager::get_virtual(g_pModelRender, vtable_indexes::dme));
	const auto sceneend_t = reinterpret_cast<void*>(HookManager::get_virtual(g_pRenderView, vtable_indexes::sceneEnd));
	const auto emitsound_t = reinterpret_cast<void*>(HookManager::get_virtual(g_EngineSound, vtable_indexes::emitSound));
	const auto gamecoordinator_send_t = reinterpret_cast<void*>(HookManager::get_virtual(g_SteamGameCoordinator, vtable_indexes::send_message));
	const auto gamecoordinator_recive_t = reinterpret_cast<void*>(HookManager::get_virtual(g_SteamGameCoordinator, vtable_indexes::retrieve_message));
	const auto painttraverse_t = reinterpret_cast<void*>(HookManager::get_virtual(g_pPanel, vtable_indexes::paint));

	if (MH_CreateHook(framestagenotify_t, &HookManager::FrameStageNotify::hook, reinterpret_cast<void**>(&oFrameStageNotify)) != MH_OK)
		throw std::runtime_error("failed to initialize FrameStageNotify. (outdated index?)");
	if (MH_CreateHook(writedeltabuffer_t, &HookManager::WriteDeltaBuff::hook, reinterpret_cast<void**>(&oWriteDeltafunct)) != MH_OK)
		throw std::runtime_error("failed to initialize WriteDeltaBuff. (outdated index?)");
	if (MH_CreateHook(newhookedcreateMove_t, &CreateMoveFix, reinterpret_cast<void**>(&oNewHookedCreateMove)) != MH_OK)
		throw std::runtime_error("failed to initialize CreateMoveFix. (outdated index?)");
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	if (MH_CreateHook(createmove_t, &HookManager::CreateMove::hook, reinterpret_cast<void**>(&oCreateMove)) != MH_OK)
		throw std::runtime_error("failed to initialize CreateMove. (outdated index?)");
	if (MH_CreateHook(overrideview_t, &HookManager::OverrideView::hook, reinterpret_cast<void**>(&oOverrideView)) != MH_OK)
		throw std::runtime_error("failed to initialize OverrideView. (outdated index?)");
	if (MH_CreateHook(getviewmodelfov_t, &HookManager::GetViewmodelFov::hook, reinterpret_cast<void**>(&oGetViewmodelFov)) != MH_OK)
		throw std::runtime_error("failed to initialize GetViewmodelFov. (outdated index?)");
	if (MH_CreateHook(lockcursor_t, &HookManager::LockCursor::hook, reinterpret_cast<void**>(&oLockCursor)) != MH_OK)
		throw std::runtime_error("failed to initialize LockCursor. (outdated index?)");
	if (MH_CreateHook(drawmodelexecute_t, &HookManager::DrawModelExecute::hook, reinterpret_cast<void**>(&oDrawModelExecute)) != MH_OK)
		throw std::runtime_error("failed to initialize DrawModelExecute. (outdated index?)");
	if (MH_CreateHook(sceneend_t, &HookManager::SceneEnd::hook, reinterpret_cast<void**>(&oSceneEnd)) != MH_OK)
		throw std::runtime_error("failed to initialize SceneEnd. (outdated index?)");
	if (MH_CreateHook(emitsound_t, &HookManager::EmitSound::hook, reinterpret_cast<void**>(&oEmitSound)) != MH_OK)
		throw std::runtime_error("failed to initialize EmitSound. (outdated index?)");
	if (MH_CreateHook(gamecoordinator_send_t, &HookManager::hkGCSendMessage::hook, reinterpret_cast<void**>(&ohkGCSendMessage)) != MH_OK)
		throw std::runtime_error("failed to initialize GCSendMessage. (outdated index?)");
	if (MH_CreateHook(gamecoordinator_recive_t, &HookManager::hkGCRetrieveMessage::hook, reinterpret_cast<void**>(&ohkGCRetrieveMessage)) != MH_OK)
		throw std::runtime_error("failed to initialize GCRetrieveMessage. (outdated index?)");
	if (MH_CreateHook(painttraverse_t, &HookManager::PaintTraverse::hook, reinterpret_cast<void**>(&oPaintTraverse)) != MH_OK)
		throw std::runtime_error("failed to initialize PaintTraverse. (outdated index?)");

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks.");
#ifdef RELEASE
	write.SendClientHello();
	write.SendMatchmakingClient2GCHello();
#endif
	//CreateFonts();
	g_Event.Init();

	//Setting up chat filter for later use (Damage log)
	ConVar* con_filter_enable = g_pCvar->FindVar("con_filter_enable");
	con_filter_enable->SetValue(2);
	ConVar* con_filter_text = g_pCvar->FindVar("con_filter_text");
	con_filter_text->SetValueChar("[Alpheus]");
	//-------------------------------------------------------------
	g_pEngine->ExecuteClientCmd("toggleconsole");
	g_pEngine->ExecuteClientCmd("clear");
	g_pEngine->ExecuteClientCmd("echo [Alpheus] Alpheus has been loaded! Release date: 2023.02.05");//21.11.21
	Beep(100, 500);
}
struct BulletImpact_t
{
	float			flImpactTime;
	Vector			vecImpactPos;
	Color			color;
	C_BaseEntity* pPlayer;

	__forceinline BulletImpact_t()
	{
		vecImpactPos = { 0.0f, 0.0f, 0.0f };
		flImpactTime = 0.0f;
		color = Color(255, 255, 255, 255);
		pPlayer = nullptr;
	}

	__forceinline BulletImpact_t(C_BaseEntity* player, Vector pos, float time, Color col = Color(255, 255, 255, 255))
	{
		pPlayer = player;
		flImpactTime = time;
		vecImpactPos = pos;
		color = col;
	}
};

std::deque<BulletImpact_t> Impacts;
std::vector<trace_info> trace_logs;
struct HitMarker_t
{
	float expireTime;
	int damage;
};
std::vector<HitMarker_t> HitMarkers;
bool reload_fonts()
{
	static int old_width, old_height;
	int width, height;
	g_pEngine->GetScreenSize(width, height);

	if (width != old_width || height != old_height)
	{
		old_width = width;
		old_height = height;
		return true;
	}

	return false;
}
void Installfonts()
{
	static auto create_font = [](char* name, int size, int weight, DWORD flags) -> unsigned long {
		unsigned long font = g_pSurface->FontCreate();
		g_pSurface->SetFontGlyphSet(font, name, size, weight, 0, 0, flags);
		return font;
	};
	if (reload_fonts())
	{
		g::Tahoma = create_font(u8"NEW ACADEMY", 12, 700, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
		g::TahomaSmall = create_font(SteamStrings(u8"Tahoma"), 12, 700, FONTFLAG_DROPSHADOW);
		//g::EspSmallText = create_font(SteamStrings("Small Fonts"), 10, 400, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
		g::EspSmallText = create_font(SteamStrings(u8"NEW ACADEMY"), 10, 400, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
		g::WeaponIcons = create_font(SteamStrings(u8"AstriumWep"), 24, 400, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
		g::Icons = create_font(SteamStrings(u8"Aurora"), 24, 400, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
		g::Logo = create_font(SteamStrings(u8"Ethnocentric"), 72, 600, FONTFLAG_NONE | FONTFLAG_DROPSHADOW);
		//g::SansSerif = create_font(SteamStrings("Thoma"), 12, 500, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
		g::SansSerif = create_font(SteamStrings(u8"NEW ACADEMY"), 12, 500, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
		g::SansSerifBold = create_font(SteamStrings(u8"MS Sans Serif"), 12, 600, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
		g::Indicators = create_font(SteamStrings(u8"Verdana"), 30, 10000, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
		g::ToolTip = create_font(SteamStrings(u8"Verdana"), 20, 10000, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
		g::Velocity = create_font(SteamStrings(u8"Verdana"), 50, 700, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
		g::Watermark = create_font(SteamStrings(u8"Verdana"), 50, 10000, FONTFLAG_ANTIALIAS);
		g::KillCounter = create_font(SteamStrings(u8"Tahoma"), 24, 700, FONTFLAG_DROPSHADOW);
		g::AAarrows = create_font(SteamStrings(u8"Arrows"), 12, 1000, FONTFLAG_OUTLINE);
		g::x88Font = create_font(SteamStrings(u8"Arial"), 18, 1100, FONTFLAG_DROPSHADOW);
		g::ReEspText = create_font(SteamStrings(u8"Tahoma"), 12, 400, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);
		g::IndicatorFont = create_font(SteamStrings(u8"Verdana"), 24, 900, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	}
}
void Hooks::Restore()
{
	Utils::Log(SteamStrings("Unhooking in progress..."));
	MH_Uninitialize();
	MH_DisableHook(MH_ALL_HOOKS);
	SetWindowLongPtr(g_Hooks.hCSGOWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(g_Hooks.pOriginalWNDProc));
	g_pNetvars.reset();   /* Need to reset by-hand, global pointer so doesnt go out-of-scope */
	Utils::Log(SteamStrings("Unhooking succeded!"));
}
void UpdateAngleAA(CUserCmd* pCmd)
{
	if (g::bSendPacket)
		g::RealAngle = pCmd->viewangles;
	else
		g::FakeAngle = pCmd->viewangles;
}
EGCResultxd __fastcall HookManager::hkGCSendMessage::hook(void* ecx, void*, uint32_t unMsgType, const void* pubData, uint32_t cubData)
{
	EGCResult status;
#ifdef RELEASE
	bool sendMessage = write.PreSendMessage(unMsgType, const_cast<void*>(pubData), cubData);

#if DEBUG
	FEATURES::MISC::InGameLogger::Log send;
	send.color_line.PushBack("[Alpheus] ", Color(190, 0, 255, 255));
	send.color_line.PushBack("[<-] Message sent to GC", Color(255, 255, 255, 255));
	FEATURES::MISC::in_game_logger.AddLog(send);
#endif

	if (!sendMessage)
		return k_EGCResultOKxd;
#endif

	return ohkGCSendMessage(ecx, unMsgType, const_cast<void*>(pubData), cubData);
}
EGCResultxd __fastcall HookManager::hkGCRetrieveMessage::hook(void* ecx, void*, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)
{
	auto status = ohkGCRetrieveMessage(ecx, punMsgType, pubDest, cubDest, pcubMsgSize);

	if (status == k_EGCResultOK)
	{

		void* thisPtr = nullptr;
		__asm mov thisPtr, ebx;
		auto oldEBP = *reinterpret_cast<void**>((uint32_t)_AddressOfReturnAddress() - 4);

		uint32_t messageType = *punMsgType & 0x7FFFFFFF;
#ifdef RELEASE
		write.ReceiveMessage(thisPtr, oldEBP, messageType, pubDest, cubDest, pcubMsgSize);
#endif
#if DEBUG
		FEATURES::MISC::InGameLogger::Log received;
		received.color_line.PushBack("[Alpheus] ", Color(190, 0, 255, 255));
		received.color_line.PushBack("[->] Message received from GC!", Color(255, 255, 255, 255));
		FEATURES::MISC::in_game_logger.AddLog(received);
#endif
	}
	return status;
}
void Hooks::HookPlayers()
{
	static bool Init[(32 + 1)];
	static bool Hooked[(32 + 1)];

	for (int i = 1; i < g_pEntityList->GetHighestEntityIndex(); i++)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant())
		{
			if (Hooked[i])
				g_Hooks.pPlayerHook[i]->Unhook(vtable_indexes::extraBonePro);

			Hooked[i] = false;
			continue;
		}

		if (!Init[i])
		{
			g_Hooks.pPlayerHook[i] = std::make_unique<ShadowVTManager>();
			Init[i] = true;
		}

		if (Hooked[i])
			g_Hooks.pPlayerHook[i]->Unhook(vtable_indexes::extraBonePro);

		if (!Hooked[i])
		{
			g_Hooks.pPlayerHook[i]->Setup(pPlayerEntity);
			g_Hooks.pPlayerHook[i]->Hook(vtable_indexes::extraBonePro, Hooks::DoExtraBonesProcessing);

			Hooked[i] = true;
		}
	}
}

void Hooks::SetClanTag(const char* tag, const char* name)
{
	static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(((DWORD)Utils::FindPattern(SteamStrings("engine.dll"), (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x30", "xxxxxxxxx????xxxxxx")));
	pSetClanTag(tag, name);
}
void __fastcall HookManager::EmitSound::hook(void* ecx, void* edx, void* filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const vec3_t* pOrigin, const vec3_t* pDirection, vec3_t* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, StartSoundParams_t& params) {

	if (!strcmp(pSoundEntry, "UIPanorama.popup_accept_match_beep")) {

		g_Misc.SetLocalPlayerReady();
	}
	oEmitSound(ecx, edx, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, flAttenuation, nSeed, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, params);

}
void  Hooks::MessageSend()
{
#ifdef RELEASE
	write.SendClientHello();
	write.SendMatchmakingClient2GCHello();
#endif
}
void __stdcall HookManager::NewHookedCreateMove::hook(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
{
	oNewHookedCreateMove(g_pClientDll, sequence_number, input_sample_frametime, active);
	CUserCmd* m_pcmd = g_GameInput->GetUserCmd_S(sequence_number);
	CVerifiedUserCmd* verified = g_GameInput->GetVerifiedUserCmd(sequence_number);

	if (!m_pcmd)
		return;

	if (!m_pcmd->command_number)
		return;

	bSendPacket = g::bSendPacket;
	verified->m_cmd = *m_pcmd;
	verified->m_crc = m_pcmd->GetChecksum();
}
DWORD retAddr = NULL;
void** searchPtr(void** addr, void* value)
{
	while (*addr != value) addr++;
	return addr;
}
__declspec(naked) void __fastcall createMove_End() noexcept
{
	__asm
	{
		mov bl, g::bSendPacket
		jmp retAddr
	}
}
bool __stdcall HookManager::CreateMove::hook(float sample_frametime, CUserCmd* pCmd) {
	auto result = oCreateMove(g_pClientMode, sample_frametime, pCmd);

	if (!pCmd || !pCmd->command_number)
		return result;

	g::pCmd = pCmd;
	g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	g::bSendPacket = true;
	if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected())
		return result;
	//HookPlayers();
	g::OriginalView = g::pCmd->viewangles;
	Backtrack::UpdateIncomingSequences(); //NEEDTOFIX
	g_Misc.OnCreateMove();
	if (g::pLocalEntity->IsAlive() && !g_GameRules->IsFreezeTime())
	{

		g_EnginePrediction.Start();
		g_Misc.onheadblockbot(pCmd);
		g_Misc.FixedBombPlant(pCmd);
		g_Fakelag.do_fakelag(pCmd, g::pLocalEntity);
		g_ResolverBeta.OnCreateMove();
		g_AntiAim.OnCreateMove();
		g_LegitAntiAim.Legaa();
		g_Aimbot.OnCreateMove();
		Backtrack::run(pCmd);//NEEDTOFIX
		g_TriggerBot.Move(pCmd);
		g_LegitAimbot.Aim_CreateMove(pCmd);
		g_EnginePrediction.End();
		g_Backtracking.LegitBackTrack(pCmd, g::pLocalEntity);

		if (g_Menu.Config.GrenadePrediction)
			GrenadePrediction::get().Tick(pCmd->buttons);

		if (g_Menu.Config.crouchcooldown)
			g::pCmd->buttons |= IN_BULLRUSH;

		if (g_Menu.Config.FakeDuck && GetAsyncKeyState(g_Menu.Config.FakeDuckKey)) {
			int choked_goal = g_Menu.Config.FakeLagValue / 2;
			bool should_crouch = g_pClientState->m_choked_commands >= choked_goal;
			if (g::pLocalEntity->GetFlags() & FL_ONGROUND)
			{
				g::pCmd->buttons |= IN_BULLRUSH;
				if (should_crouch)
					g::pCmd->buttons |= IN_DUCK;
				else
					g::pCmd->buttons &= ~IN_DUCK;
			}
		}
	}
	else
	{
		Vector view;
		g_pEngine->GetViewAngles(view);
		g::pCmd->viewangles = view;
		g::bSendPacket = true;
	}
	/*if (g::pLocalEntity && g::pLocalEntity->IsAlive())
	{
		Backtrack::run(pCmd);//NEEDTOFIX
	}*/

	if (g_Menu.Config.namestealer)
	{
		for (int i = 1; i < g_pEntityList->GetHighestEntityIndex(); i++)
		{
			C_BaseEntity* player = g_pEntityList->GetClientEntity(i);
			if (!player)
				continue;
			if (player == g::pLocalEntity)
				continue;
			PlayerInfo_t pInfo;
			g_pEngine->GetPlayerInfo(i, &pInfo);
			if (pInfo.ishltv)
				continue;
			std::string name = "  ";
			name += pInfo.szName;
			name += " ";
			char* tekst = &name[0];
			g_Misc.SetName(tekst);

		}
	}
	g_Misc.MovementFix(g::OriginalView);
	g_Math.Clamp(g::pCmd->viewangles);
	UpdateAngleAA(pCmd);

	retAddr = (DWORD)_ReturnAddress();

	void** p = searchPtr((void**)&p, (void*)retAddr);
	*p = createMove_End;
	return false;
}
#define net_frame_backup 64
#define net_frame_mask (net_frame_backup-1)
int __fastcall HookManager::SendDatagram::hook(NetChannel* network, void* edx, void* datagram) //This is... i almost gave up :/
{
	if (datagram)
		return oSendDatagram(network, edx, datagram);

	int instate = network->m_nInReliableState;
	int insequencenr = network->m_nInSequenceNr;

	int ret = oSendDatagram(network, edx, datagram);

	network->m_nInReliableState = instate;
	network->m_nInSequenceNr = insequencenr;
	return ret;
}
void __fastcall HookManager::SceneEnd::hook(void* ecx, void* edx)
{
	if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame() || !g::pLocalEntity)
		return;

	static auto linegoesthrusmoke = Utils::FindPattern(SteamStrings("client.dll"), (PBYTE)"\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx");

	if (g_Menu.Config.NoSmoke) {
		std::vector<const char*> vistasmoke_wireframe = { SteamStrings("particle/vistasmokev1/vistasmokev1_smokegrenade"), };

		for (auto mat_s : vistasmoke_wireframe)
		{
			IMaterial* mat = g_pMaterialSys->FindMaterial(mat_s, SteamStrings("Other textures"));
			mat->IncrementReferenceCount();
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
		}

		static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);
		*(int*)(smokecout) = 0;
	}
	static IMaterial* ChamsMaterial = nullptr;
	static IMaterial* skeetchams = g_pMaterialSys->FindMaterial(SteamStrings("skeetchams"), SteamStrings("Model Textures"));
	bool bFound = false;
	auto pVar = skeetchams->FindVar("$envmaptint", &bFound);
	if (bFound)
		(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, g_Menu.Config.Glowoverlaycolor.red / 255.f, g_Menu.Config.Glowoverlaycolor.green / 255.f, g_Menu.Config.Glowoverlaycolor.blue / 255.f);

	static IMaterial* glowOverlay = g_pMaterialSys->FindMaterial(SteamStrings("glowOverlay"), SteamStrings("Model Textures"));
	bool bFound2 = false;
	auto pVar2 = glowOverlay->FindVar("$envmaptint", &bFound2);
	if (bFound2)
		(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar2 + 44))((uintptr_t)pVar2, g_Menu.Config.EnemyVisChamsColor.red / 255.f, g_Menu.Config.EnemyVisChamsColor.green / 255.f, g_Menu.Config.EnemyVisChamsColor.blue / 255.f);

	static IMaterial* testnigger = g_pMaterialSys->FindMaterial(SteamStrings("testnigger"), SteamStrings("Model Textures"));
	bool bFound3 = false;
	auto pVar3 = testnigger->FindVar("$envmaptint", &bFound3);
	if (bFound3)
		(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar3 + 44))((uintptr_t)pVar3, g_Menu.Config.PearlColor.red / 255.f, g_Menu.Config.PearlColor.green / 255.f, g_Menu.Config.PearlColor.blue / 255.f);

	static IMaterial* glow_armsrace = g_pMaterialSys->FindMaterial(SteamStrings("dev/glow_armsrace"), SteamStrings("Model Textures"));
	bool bFound4 = false;
	auto pVar4 = glow_armsrace->FindVar("$envmaptint", &bFound4);
	if (bFound4)
		(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar4 + 44))((uintptr_t)pVar4, g_Menu.Config.Glowoverlaycolor.red / 255.f, g_Menu.Config.Glowoverlaycolor.green / 255.f, g_Menu.Config.Glowoverlaycolor.blue / 255.f);

	static IMaterial* tester = g_pMaterialSys->FindMaterial(SteamStrings("models/inventory_items/trophy_majors/crystal_clear"), SteamStrings("Model Textures"));
	bool bFound5 = false;
	auto pVar5 = tester->FindVar("$envmaptint", &bFound5);
	if (bFound5)
		(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar5 + 44))((uintptr_t)pVar5, g_Menu.Config.Glowoverlaycolor.red / 255.f, g_Menu.Config.Glowoverlaycolor.green / 255.f, g_Menu.Config.Glowoverlaycolor.blue / 255.f);


	switch (g_Menu.Config.ChamsMaterial)
	{
	case 0:
		ChamsMaterial = g_pMaterialSys->FindMaterial(SteamStrings("debug/debugambientcube"), SteamStrings("Model Textures"));
		ChamsMaterial->IncrementReferenceCount();;
		break;

	case 1:
		ChamsMaterial = g_pMaterialSys->FindMaterial(SteamStrings("debug/debugdrawflat"), SteamStrings("Model textures"));
		ChamsMaterial->IncrementReferenceCount();
		break;
	case 2:
		ChamsMaterial = g_pMaterialSys->FindMaterial(SteamStrings("simple_ignorez_reflective"), SteamStrings("Model Textures"));
		ChamsMaterial->IncrementReferenceCount();
		break;
	case 3:
		ChamsMaterial = g_pMaterialSys->FindMaterial(SteamStrings("chamsAnimated"), SteamStrings("Model textures"));
		ChamsMaterial->IncrementReferenceCount();
		break;
	case 4:
		ChamsMaterial = g_pMaterialSys->FindMaterial(SteamStrings("models/inventory_items/dogtags/dogtags_lightray"), SteamStrings("Model textures"));
		ChamsMaterial->IncrementReferenceCount();
		break;
	case 5:
		ChamsMaterial = g_pMaterialSys->FindMaterial(SteamStrings("dev/glow_armsrace"), SteamStrings("Model textures"));
		ChamsMaterial->IncrementReferenceCount();
		break;
	case 6:
		ChamsMaterial = g_pMaterialSys->FindMaterial(SteamStrings("skeetchams"), SteamStrings("Model textures"));
		ChamsMaterial->IncrementReferenceCount();
		break;
	case 7:
		ChamsMaterial = g_pMaterialSys->FindMaterial(SteamStrings("glowOverlay"), SteamStrings("Model textures"));
		ChamsMaterial->IncrementReferenceCount();
		break;
	case 8:
		ChamsMaterial = g_pMaterialSys->FindMaterial(SteamStrings("testnigger"), SteamStrings("Model textures"));
		ChamsMaterial->IncrementReferenceCount();
		break;
	case 9:
		ChamsMaterial = g_pMaterialSys->FindMaterial(SteamStrings("models/inventory_items/dogtags/dogtags_outline"), SteamStrings("Model textures"));
		ChamsMaterial->IncrementReferenceCount();
		break;
	case 10://t1
		ChamsMaterial = g_pMaterialSys->FindMaterial(SteamStrings("models/inventory_items/wildfire_gold/wildfire_gold_detail"), SteamStrings("Model textures"));
		ChamsMaterial->IncrementReferenceCount();
		break;
	case 11://t2
		ChamsMaterial = g_pMaterialSys->FindMaterial(SteamStrings("models/inventory_items/trophy_majors/crystal_clear"), SteamStrings("Model textures"));
		ChamsMaterial->IncrementReferenceCount();
		break;
	case 12://t3
		ChamsMaterial = g_pMaterialSys->FindMaterial(SteamStrings("models/inventory_items/trophy_majors/velvet"), SteamStrings("Model textures"));
		ChamsMaterial->IncrementReferenceCount();
		break;

	default:
		break;
	}




	if (ChamsMaterial && !ChamsMaterial->IsErrorMaterial())
	{
		for (int i = 1; i < g_pEntityList->GetHighestEntityIndex(); i++)
		{
			C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);
			if (!pPlayerEntity
				|| !pPlayerEntity->IsAlive()
				|| pPlayerEntity->IsDormant()
				|| !pPlayerEntity->IsPlayer()
				|| !g::pLocalEntity)
				continue;

			if (pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam()) // Team
			{
				if (pPlayerEntity == g::pLocalEntity) //Local
				{
					if (g_Menu.Config.DesyncChams && g_GameInput->m_fCameraInThirdPerson && !g_GameRules->IsFreezeTime() && (g_Menu.Config.Antiaim || g_Menu.Config.LegitAA))
					{
						Vector OrigAng;
						g_pPrediction->GetViewAngles(OrigAng);
						g_pPrediction->SetViewAngles(Vector(0, g::FakeAngle.y, 0));
						float VisColor[3] = { g_Menu.Config.DesyncChamsColor.red / 255.f, g_Menu.Config.DesyncChamsColor.green / 255.f, g_Menu.Config.DesyncChamsColor.blue / 255.f };
						ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
						ChamsMaterial->AlphaModulate(g_Menu.Config.DesyncChamsTransparency / 255.f);
						g_pModelRender->ForcedMaterialOverride(ChamsMaterial);
						g_pRenderView->SetColorModulation(VisColor);
						pPlayerEntity->DrawModel(0x1, pPlayerEntity->GetModelInstance());
						g_pPrediction->SetViewAngles(OrigAng);
						g_pModelRender->ForcedMaterialOverride(nullptr);
					}

					float VisColor[3] = { g_Menu.Config.LocalVisChamsColor.red / 255.f, g_Menu.Config.LocalVisChamsColor.green / 255.f, g_Menu.Config.LocalVisChamsColor.blue / 255.f };
					float XQZColor[3] = { g_Menu.Config.LocalXQZChamsColor.red / 255.f, g_Menu.Config.LocalXQZChamsColor.green / 255.f, g_Menu.Config.LocalXQZChamsColor.blue / 255.f };

					if (g_Menu.Config.LocalChams)
					{
						Vector OrigAng;
						OrigAng = g::RealAngle;
						g::pLocalEntity->SetEyeAngles(g::RealAngle);
						ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
						ChamsMaterial->AlphaModulate(1.f);
						g_pModelRender->ForcedMaterialOverride(ChamsMaterial);
						g_pRenderView->SetBlend(1.0f);
						g_pRenderView->SetColorModulation(VisColor);
						pPlayerEntity->DrawModel(0x1, pPlayerEntity->GetModelInstance());
						g::pLocalEntity->SetEyeAngles(OrigAng);
						g_pModelRender->ForcedMaterialOverride(nullptr);
					}
				}
				else
				{
					float VisColor[3] = { g_Menu.Config.TeamVisChamsColor.red / 255.f, g_Menu.Config.TeamVisChamsColor.green / 255.f, g_Menu.Config.TeamVisChamsColor.blue / 255.f };
					float XQZColor[3] = { g_Menu.Config.TeamXQZChamsColor.red / 255.f, g_Menu.Config.TeamXQZChamsColor.green / 255.f, g_Menu.Config.TeamXQZChamsColor.blue / 255.f };

					if (g_Menu.Config.TeamChams)
					{
						if (g_Menu.Config.TeamXQZChams)
						{
							ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
							ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
							ChamsMaterial->AlphaModulate(1.f);
							g_pModelRender->ForcedMaterialOverride(ChamsMaterial);
							g_pRenderView->SetColorModulation(XQZColor);
							pPlayerEntity->DrawModel(0x1, pPlayerEntity->GetModelInstance());
							g_pModelRender->ForcedMaterialOverride(nullptr);
						}

						ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
						ChamsMaterial->AlphaModulate(1.f);
						g_pModelRender->ForcedMaterialOverride(ChamsMaterial);
						g_pRenderView->SetColorModulation(VisColor);
						pPlayerEntity->DrawModel(0x1, pPlayerEntity->GetModelInstance());
						g_pModelRender->ForcedMaterialOverride(nullptr);
					}
				}
			}
			else // Enemy
			{
				float VisColor[3] = { g_Menu.Config.EnemyVisChamsColor.red / 255.f, g_Menu.Config.EnemyVisChamsColor.green / 255.f, g_Menu.Config.EnemyVisChamsColor.blue / 255.f };
				float XQZColor[3] = { g_Menu.Config.EnemyXQZChamsColor.red / 255.f, g_Menu.Config.EnemyXQZChamsColor.green / 255.f, g_Menu.Config.EnemyXQZChamsColor.blue / 255.f };

				if (g_Menu.Config.EnemyChams)
				{
					if (g_Menu.Config.EnemyXQZChams)
					{
						ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, g_Menu.Config.EnemyWireFrame);
						ChamsMaterial->AlphaModulate(1.f);
						g_pModelRender->ForcedMaterialOverride(ChamsMaterial);
						g_pRenderView->SetColorModulation(XQZColor);
						g_pRenderView->SetBlend(g_Menu.Config.EnemyChamsTransparency / 255.f);
						pPlayerEntity->DrawModel(0x1, pPlayerEntity->GetModelInstance());
						g_pModelRender->ForcedMaterialOverride(nullptr);
					}

					ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
					ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, g_Menu.Config.EnemyWireFrame);
					ChamsMaterial->AlphaModulate(1.f);
					g_pModelRender->ForcedMaterialOverride(ChamsMaterial);
					g_pRenderView->SetColorModulation(VisColor);
					g_pRenderView->SetBlend(g_Menu.Config.EnemyChamsTransparency / 255.f);
					pPlayerEntity->DrawModel(0x1, pPlayerEntity->GetModelInstance());
					g_pModelRender->ForcedMaterialOverride(nullptr);
				}
			}


			float LagColor[3] = { g_Menu.Config.ShowLagColor.red / 255.f, g_Menu.Config.ShowLagColor.green / 255.f, g_Menu.Config.ShowLagColor.blue / 255.f };

			if (pPlayerEntity == g::pLocalEntity && g_Menu.Config.ShowLag && g::pLocalEntity->GetVelocity().Length2D() > 5)
			{
				static IMaterial* material = g_pMaterialSys->FindMaterial(SteamStrings("dev/glow_armsrace"), SteamStrings("Model Textures"));

				if (!material)
					return;

				static auto last_networked_origin = Vector(0, 0, 0);

				if (g::bSendPacket)
				{
					last_networked_origin = g::pLocalEntity->GetAbsOrigin();
				}

				g::pLocalEntity->SetAbsOrigin(last_networked_origin);
				material->AlphaModulate(100.f / 255.f);
				g_pModelRender->ForcedMaterialOverride(material);
				g_pRenderView->SetColorModulation(LagColor);
				g::pLocalEntity->DrawModel(0x1, g::pLocalEntity->GetModelInstance());
				g_pModelRender->ForcedMaterialOverride(nullptr);
				g::pLocalEntity->SetAbsOrigin(g::pLocalEntity->GetAbsOrigin());
			}
		}
	}
	oSceneEnd(ecx, edx);
}


void __fastcall Hooks::DoExtraBonesProcessing(void* ECX, void* EDX, void* unkn1, void* unkn2, void* unkn3, void* unkn4, CBoneBitList& unkn5, void* unkn6)
{
	C_BaseEntity* pPlayerEntity = (C_BaseEntity*)ECX;

	if (!pPlayerEntity || pPlayerEntity == nullptr)
		return;
	if (!pPlayerEntity || pPlayerEntity->IsDormant() || !pPlayerEntity->AnimState())
		return;

	auto oDoExtraBonesProcessing = g_Hooks.pPlayerHook[pPlayerEntity->EntIndex()]->GetOriginal<ExtraBoneProcess_t>(vtable_indexes::extraBonePro);

	float Backup = pPlayerEntity->AnimState()->m_flUnknownFraction;
	pPlayerEntity->AnimState()->m_flUnknownFraction = 0;

	oDoExtraBonesProcessing(ECX, unkn1, unkn2, unkn3, unkn4, unkn5, unkn6);

	pPlayerEntity->AnimState()->m_flUnknownFraction = Backup;
}
# pragma region NEED TO FIX SETUPBONES
bool RebuiltSetupBones(C_BaseEntity* m_pPlayer)
{
	float m_flCurtime = g_pGlobalVars->curtime;
	float m_flFrametime = g_pGlobalVars->frametime;
	int m_iNextSimTick = m_pPlayer->GetSimulationTime() / g_pGlobalVars->intervalPerTick + 1;
	int m_nFrames = g_pGlobalVars->framecount;
	int m_nTicks = g_pGlobalVars->tickcount;

	g_pGlobalVars->curtime = m_pPlayer->GetSimulationTime();
	g_pGlobalVars->frametime = g_pGlobalVars->intervalPerTick;;
	g_pGlobalVars->framecount = m_iNextSimTick;
	g_pGlobalVars->tickcount = m_iNextSimTick;

	auto m_fBackupEffects = m_pPlayer->EFlags();

	m_pPlayer->EFlags() |= 8;

	m_pPlayer->EFlags() = m_fBackupEffects;

	g_pGlobalVars->curtime = m_flCurtime;
	g_pGlobalVars->frametime = m_flFrametime;
	g_pGlobalVars->framecount = m_nFrames;
	g_pGlobalVars->tickcount = m_nTicks;
	return true;
}
void InvalidateBoneCache(C_BaseEntity* m_pPlayer, int m_iBoneCounter)
{
	*(size_t*)((uintptr_t)m_pPlayer + 0x2924) = -FLT_MAX;
	*(size_t*)((uintptr_t)m_pPlayer + 0x2690) = m_iBoneCounter - 1;
}

void FixLocalBones(C_BaseEntity* m_pPlayer, bool m_bSwap, float m_flPoseParam, bool* m_pbResult, bool* m_pbResult2)
{
	if (!m_pPlayer->IsAlive())
		return;

	AnimationLayer* m_pAnimLayers = m_pPlayer->AnimOverlays();
	float m_flBackupCycle = m_pAnimLayers[3].m_flCycle;
	float m_flBackupWeight = m_pAnimLayers[3].m_flWeight;

	if ((m_pPlayer->EFlags() & FL_ONGROUND) && m_flPoseParam < 0.1f)
	{
		if (m_pPlayer->GetSequenceActivity(m_pAnimLayers[3].m_nSequence) == 979)
		{
			m_pAnimLayers[3].m_flCycle = 0.f;
			m_pAnimLayers[3].m_flWeight = 0.f;
		}
	}

	if (m_bSwap)
	{
		m_pPlayer->SetAbsAngles(m_pPlayer->GetVecOrigin());
		*m_pbResult = RebuiltSetupBones(m_pPlayer);
		m_pPlayer->SetAbsAngles(m_pPlayer->GetVecOrigin());
	}

	*m_pbResult2 = RebuiltSetupBones(m_pPlayer);

	m_pAnimLayers[3].m_flCycle = m_flBackupCycle;
	m_pAnimLayers[3].m_flWeight = m_flBackupWeight;
}
#pragma endregion

bool resetMaterials = true;

void __fastcall HookManager::DrawModelExecute::hook(void* ecx, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix)
{
	const char* ModelName = g_pModelInfo->GetModelName((model_t*)info.pModel);
	const std::string strModelName = g_pModelInfo->GetModelName((model_t*)info.pModel);
	if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame())
		return;
	g_GlowEsp.RunGlow();

	if (strstr(ModelName, SteamStrings("models/player/")) && g_Menu.Config.EnemyChamsTransparency < 255) //Enemy model transparency
	{
		C_BaseEntity* pEnt = g_pEntityList->GetClientEntity(info.index);
		if (g::pLocalEntity &&
			pEnt &&
			pEnt != g::pLocalEntity &&
			pEnt->IsAlive() &&
			pEnt->GetTeam() != g::pLocalEntity->GetTeam())
		{
			g_pRenderView->SetBlend(g_Menu.Config.EnemyChamsTransparency / 255.f);
		}
	}

	if (strstr(ModelName, SteamStrings("models/player/")) && g_Menu.Config.TeamChamsTransparency < 255) //Team model transparency
	{
		C_BaseEntity* pEnt = g_pEntityList->GetClientEntity(info.index);
		if (g::pLocalEntity &&
			pEnt &&
			pEnt != g::pLocalEntity &&
			pEnt->IsAlive() &&
			pEnt->GetTeam() == g::pLocalEntity->GetTeam())
		{
			g_pRenderView->SetBlend(g_Menu.Config.TeamChamsTransparency / 255.f);
		}
	}
	if (strstr(ModelName, SteamStrings("models/player/")) && g_Menu.Config.LocalTransparency < 255) //Local model transparency
	{
		C_BaseEntity* pEnt = g_pEntityList->GetClientEntity(info.index);
		if (g::pLocalEntity &&
			pEnt &&
			pEnt == g::pLocalEntity &&
			pEnt->IsAlive() &&
			pEnt->GetTeam() == g::pLocalEntity->GetTeam())
		{
			g_pRenderView->SetBlend(g_Menu.Config.LocalTransparency / 255.f);
		}
	}
	if (strstr(ModelName, SteamStrings("models/player/")) && g::pLocalEntity && g::pLocalEntity->IsScoped()) //Blend player while scoped
	{
		C_BaseEntity* pEnt = g_pEntityList->GetClientEntity(info.index);
		if (g::pLocalEntity && pEnt && pEnt == g::pLocalEntity)
			g_pRenderView->SetBlend(g_Menu.Config.scopeblend / 255.f);
	}
	if (strstr(ModelName, SteamStrings("models/player/")) && (g_Menu.Config.LegitBacktrack || g_Menu.Config.PosBacktrack) && g_Menu.Config.BacktrackingChams != 0) //Backtrack chams (on enemy)
	{
		C_BaseEntity* pEnt = g_pEntityList->GetClientEntity(info.index);
		if (g::pLocalEntity &&
			pEnt &&
			pEnt != g::pLocalEntity &&
			pEnt->IsAlive() &&
			pEnt->GetTeam() != g::pLocalEntity->GetTeam())
		{
			if (!g_LagComp.PlayerRecord[info.index].empty() && pEnt->GetVelocity().Length2D() > 5)
			{
				float OLDCOLORMODULATION[3];
				g_pRenderView->GetColorModulation(OLDCOLORMODULATION);
				if (g_Menu.Config.BacktrackingChams != 4)
				{
					for (int i = 0; i < g_LagComp.PlayerRecord[info.index].size(); i++)
					{
						switch (g_Menu.Config.BacktrackingChams)
						{
						case 0: break;
						case 1:
						{
							float BTCOLOR[3] = { (i * 21.25f) / 255.f, (255 - (i * 21.25f)) / 255.f, 1.f };
							g_pRenderView->SetColorModulation(BTCOLOR);
							g_pRenderView->SetBlend(0.05f);
							break;
						}
						case 2:
						{
							float BTCOLOR[3] = { (255 - (i * 21.25f)) / 255.f, (255 - (i * 21.25f)) / 255.f, (255 - (i * 21.25f)) / 255.f };
							g_pRenderView->SetColorModulation(BTCOLOR);
							g_pRenderView->SetBlend(0.05f);
							break;
						}
						case 3:
						{
							float BTCOLOR[3] = { g_Menu.Config.EnemyVisChamsColor.red / 255.f, g_Menu.Config.EnemyVisChamsColor.green / 255.f, g_Menu.Config.EnemyVisChamsColor.blue / 255.f };
							g_pRenderView->SetColorModulation(BTCOLOR);
							g_pRenderView->SetBlend((i * 21.25f) / 255.f);
							break;
						}
						}
						oDrawModelExecute(ecx, context, state, info, g_LagComp.PlayerRecord[info.index].at(i).Matrix);
					}
				}
				else
				{
					float BTCOLOR[3] = { g_Menu.Config.BacktrackChamsColor.red / 255.f, g_Menu.Config.BacktrackChamsColor.green / 255.f, g_Menu.Config.BacktrackChamsColor.blue / 255.f };;
					g_pRenderView->SetColorModulation(BTCOLOR);
					g_pRenderView->SetBlend(0.5f);
					oDrawModelExecute(ecx, context, state, info, g_LagComp.PlayerRecord[info.index].at(0).Matrix);
				}

				g_pRenderView->SetColorModulation(OLDCOLORMODULATION);
				g_pRenderView->SetBlend(1.f);
				oDrawModelExecute(ecx, context, state, info, matrix);
			}
		}
	}
	if (strstr(ModelName, SteamStrings("weapons/v")) && !strstr(ModelName, SteamStrings("arms")) && g_Menu.Config.WeaponChams) //Local weapon model chams
	{

		if (g::pLocalEntity->IsScoped() && !g_Menu.Config.chamsthruscope)
			return oDrawModelExecute(ecx, context, state, info, matrix);
		// ->		

		static IMaterial* material;
		switch (g_Menu.Config.WeaponChamsMode)
		{
		case 0:
		{
			material = g_pMaterialSys->FindMaterial(SteamStrings("debug/debugambientcube"), SteamStrings("Model Textures"));
			material->IncrementReferenceCount();
			break;
		}
		case 1:
		{
			material = g_pMaterialSys->FindMaterial(SteamStrings("models/inventory_items/dogtags/dogtags_lightray"), SteamStrings("Model textures"));
			material->IncrementReferenceCount();
			break;
		}
		case 2:
		{
			material = g_pMaterialSys->FindMaterial(SteamStrings("dev/glow_armsrace"), SteamStrings("Model textures"));
			material->IncrementReferenceCount();
			break;
		}
		case 3:
		{
			material = g_pMaterialSys->FindMaterial(SteamStrings("skeetchams"), SteamStrings("Model textures"));
			material->IncrementReferenceCount();
			break;
		}
		case 4:
		{
			material = g_pMaterialSys->FindMaterial(SteamStrings("glowOverlay"), SteamStrings("Model textures"));
			material->IncrementReferenceCount();
			break;
		}
		case 5:
		{
			material = g_pMaterialSys->FindMaterial(SteamStrings("testnigger"), SteamStrings("Model textures"));
			material->IncrementReferenceCount();
			break;
		}
		case 6:
		{
			material = g_pMaterialSys->FindMaterial(SteamStrings("models/inventory_items/dogtags/dogtags_outline"), SteamStrings("Model textures"));
			material->IncrementReferenceCount();
			break;
		}
		}

		if (material)
		{
			material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
			material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
			float WEAPONCOLOR[3] = { g_Menu.Config.WeaponChamsColor.red / 255.f, g_Menu.Config.WeaponChamsColor.green / 255.f, g_Menu.Config.WeaponChamsColor.blue / 255.f };
			g_pRenderView->SetColorModulation(WEAPONCOLOR);
			g_pRenderView->SetBlend(g_Menu.Config.WeaponChamsTransparency / 255.f);
			g_pModelRender->ForcedMaterialOverride(material);
		}
	}

	if (strstr(ModelName, "arms") && g_Menu.Config.ArmsChams)
	{
		static IMaterial* material;
		switch (g_Menu.Config.ArmsChamsMode) //Player arms chams material
		{
		case 0:
		{
			material = g_pMaterialSys->FindMaterial(SteamStrings("debug/debugambientcube"), SteamStrings("Model Textures"));
			material->IncrementReferenceCount();
			break;
		}
		case 1:
		{
			material = g_pMaterialSys->FindMaterial(SteamStrings("debug/debugambientcube"), SteamStrings("Model Textures"));
			material->IncrementReferenceCount();
			break;
		}
		case 2:
		{
			material = g_pMaterialSys->FindMaterial(SteamStrings("dev/glow_armsrace"), SteamStrings("Model textures"));
			material->IncrementReferenceCount();
			break;
		}
		case 3:
		{
			material = g_pMaterialSys->FindMaterial(SteamStrings("skeetchams"), SteamStrings("Model textures"));
			material->IncrementReferenceCount();
			break;
		}
		case 4:
		{
			material = g_pMaterialSys->FindMaterial(SteamStrings("glowOverlay"), SteamStrings("Model textures"));
			material->IncrementReferenceCount();
			break;
		}
		case 5:
		{
			material = g_pMaterialSys->FindMaterial(SteamStrings("testnigger"), SteamStrings("Model textures"));
			material->IncrementReferenceCount();
			break;
		}
		case 6:
		{
			material = g_pMaterialSys->FindMaterial(SteamStrings("models/inventory_items/dogtags/dogtags_outline"), SteamStrings("Model textures"));
			material->IncrementReferenceCount();
			break;
		}
		}
		if (material)
		{
			material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
			material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, g_Menu.Config.ArmsWireFrame);
			float ARMSCOLOR[3] = { g_Menu.Config.ArmsChamsColor.red / 255.f, g_Menu.Config.ArmsChamsColor.green / 255.f, g_Menu.Config.ArmsChamsColor.blue / 255.f };
			g_pRenderView->SetColorModulation(ARMSCOLOR);
			g_pRenderView->SetBlend(g_Menu.Config.ArmsChamsTransparency / 255.f);
			g_pModelRender->ForcedMaterialOverride(material);
		}
	}

	oDrawModelExecute(ecx, context, state, info, matrix);
}
void __fastcall HookManager::FrameStageNotify::hook(void* ecx, void* edx, ClientFrameStage_t curStage)
{
	const auto client = SteamStrings("client.dll");
	static auto backtrackInit = (Backtrack::init(), false);
	if (g_pEngine->IsInGame() && g_pEngine->IsConnected() && g::pLocalEntity)
	{
		if (g::knifeapply)
		{
			KnifeApplyCallbk();
			g::knifeapply = false;
		}
		if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		{

			if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected())
			{
				g::pLocalEntity = nullptr;
				return oFrameStageNotify(edx, curStage);
			}

			skinchanger();
		}
		if (curStage == FRAME_NET_UPDATE_END)
		{
			if (g_pEngine->IsInGame() && g_pEngine->IsConnected() && g_Menu.Config.PosBacktrack)
				g_Aimbot.FrameUpdatePostEntityThink();
		}
		if (curStage == FRAME_RENDER_START)
		{
			if (g_pEngine->IsInGame() && g_pEngine->IsConnected())
			{
				if (!hooked && g_pClientState)
				{
					NetChannel* netchannel = g_pClientState->m_net_channel;
					static void* oldPointer = nullptr;
					if (oldPointer != netchannel && netchannel && g::pLocalEntity)
					{
						oldPointer = netchannel;

						const auto senddatagram_t = reinterpret_cast<void*>(HookManager::get_virtual(netchannel, vtable_indexes::SendDatagram));
						if (MH_CreateHook(senddatagram_t, &HookManager::SendDatagram::hook, reinterpret_cast<void**>(&oSendDatagram)) != MH_OK)
							throw std::runtime_error("failed to initialize SendDatagram. (outdated index?)");
						if (MH_EnableHook(senddatagram_t) != MH_OK)
							throw std::runtime_error("failed to hook SendDatagram in FRAME_RENDER_START.");

						//Mapváltás után semmi sem mûködik, + backtrack; FIXELVE
						//g_pGlobalVars = **reinterpret_cast<CGlobalVarsBase***>((*reinterpret_cast<uintptr_t**>(g_pClientDll))[0] + 0x1F);
						//g_pMoveHelper = **reinterpret_cast<IMoveHelper***>((Utils::FindSignature(client, "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 0x2));
						//g_GlowManager = *reinterpret_cast<CGlowObjectManager**>(Utils::FindSignature(client, "0F 11 05 ? ? ? ? 83 C8 01") + 3);//0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00 Fix 20220526
						//g_GameInput = *(CInput**)(Utils::FindSignature(client, "B9 ? ? ? ? 8B 40 38 FF D0 84 C0 0F 85") + 1); //B9 ? ? ? ? F3 0F 11 04 24 FF 50 10 Fix 20220526
						//g_GameRules = **reinterpret_cast<CCSGameRules***>(Utils::FindSignature(client, "A1 ? ? ? ? 85 C0 0F 84 ? ? ? ? 80 B8 ? ? ? ? ? 74 7A") + 0x1);//A1 ? ? ? ? 8B 0D ? ? ? ? 6A 00 68 ? ? ? ? C6 Fix 20220526
						g_pGlobalVars->tickcount = 0; //<- ?
						hooked = true;
					}
				}
				else
					hooked = false;
			}
			Backtrack::update();//NEEDTOFIX
			if (g_GameInput->m_fCameraInThirdPerson)
				g_pPrediction->SetLocalViewAngles(g::RealAngle); //Local real yaw visualization (tp fix)
		}
		g_ResolverBeta.FrameStage(curStage);
	}

	oFrameStageNotify(ecx, curStage);
}


C_BaseEntity* UTIL_PlayerByIndex(int index)
{

	typedef C_BaseEntity* (__fastcall* PlayerByIndex)(int);
	static PlayerByIndex UTIL_PlayerByIndex = (PlayerByIndex)Utils::FindSignature(SteamStrings("server.dll"), SteamStrings("85 C9 7E 2A A1"));

	if (!UTIL_PlayerByIndex)
		return false;

	return UTIL_PlayerByIndex(index);
}

void hit()
{
	if (!g::pLocalEntity || !g_pEngine->IsInGame())
		return;

	if (!g::pLocalEntity->IsAlive())
		return;

	static int width = 0;
	static int height = 0;
	if (height == 0 || width == 0)
		g_pEngine->GetScreenSize(width, height);

	float alpha = 0.f;

	for (size_t i = 0; i < HitMarkers.size(); i++)
	{
		float timeDiff = HitMarkers[i].expireTime - g_pGlobalVars->curtime;

		if (timeDiff < 0.f)
		{
			HitMarkers.erase(HitMarkers.begin() + i);
			continue;
		}

		std::string dmg = "-";
		dmg += std::to_string(HitMarkers[i].damage).c_str();

		int moveDist = 24;

		float ratio = 1.f - (timeDiff / g_Menu.Config.ExpireTime);
		alpha = (timeDiff / g_Menu.Config.ExpireTime) * 2.f;
		if (g_Menu.Config.ShowDmg)
			g_pSurface->DrawT(width / 2 + 6 + ratio * moveDist / 2, height / 2 + 6 + ratio * moveDist, Color(255, 255, 255, 255), g::Tahoma, false, dmg.c_str());
	}

	if (HitMarkers.size() > 0)
	{
		int lineSize = (int)g_Menu.Config.MarkerSize;
		int screenSizeX, screenCenterX;
		int screenSizeY, screenCenterY;
		g_pEngine->GetScreenSize(screenSizeX, screenSizeY);
		screenCenterX = screenSizeX / 2;
		screenCenterY = screenSizeY / 2;
		g_pSurface->Line(screenCenterX - lineSize, screenCenterY - lineSize, screenCenterX - (lineSize / 4), screenCenterY - (lineSize / 4), Color(255, 255, 255, 255));
		g_pSurface->Line(screenCenterX - lineSize, screenCenterY + lineSize, screenCenterX - (lineSize / 4), screenCenterY + (lineSize / 4), Color(255, 255, 255, 255));
		g_pSurface->Line(screenCenterX + lineSize, screenCenterY + lineSize, screenCenterX + (lineSize / 4), screenCenterY + (lineSize / 4), Color(255, 255, 255, 255));
		g_pSurface->Line(screenCenterX + lineSize, screenCenterY - lineSize, screenCenterX + (lineSize / 4), screenCenterY - (lineSize / 4), Color(255, 255, 255, 255));

	}
}
bool __fastcall HookManager::WriteDeltaBuff::hook(void* ecx, void*, int slot, bf_write* buf, int from, int to, bool isnewcommand)
{
	if (g::m_nTickbaseShift <= 0)
		return oWriteDeltafunct(ecx, slot, buf, from, to, isnewcommand);
	if (from != -1)
		return true;

	auto CL_SendMove = []() {
		using CL_SendMove_t = void(__fastcall*)(void);
		static CL_SendMove_t CL_SendMoveF = (CL_SendMove_t)Utils::FindSignature("engine.dll", "55 8B EC 81 EC 64 01 00 00 53 56 8A F9"); //55 8B EC A1 ? ? ? ? 81 EC ? ? ? ? B9 ? ? ? ? 53 8B 98 Fix: 20210922

		CL_SendMoveF();
	};

	/*auto WriteUsercmd = [](bf_write* buf, CUserCmd* in, CUserCmd* out) {
		using WriteUsercmd_t = void(__fastcall*)(bf_write*, CUserCmd*, CUserCmd*);
		static WriteUsercmd_t WriteUsercmdF = (WriteUsercmd_t)Utils::FindSignature("client.dll", "55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D");
		WriteUsercmdF(buf, in, out);

	};*/
	// number of backup and new commands
	int* pNumBackupCommands = (int*)((uintptr_t)buf - 0x30);
	int* pNumNewCommands = (int*)((uintptr_t)buf - 0x2C);
	int32_t new_commands = *pNumNewCommands;
	// Manipulate CLC_Move
	auto nextcmdnumber = g_pClientState->m_last_outgoing_command + g_pClientState->m_choked_commands + 1;
	auto totalnewcommands = min(g::m_nTickbaseShift, 60);
	g::m_nTickbaseShift -= totalnewcommands;

	from = -1;
	*pNumNewCommands = totalnewcommands;
	*pNumBackupCommands = 0;
	//real cmds
	for (to = nextcmdnumber - new_commands + 1; to <= nextcmdnumber; to++)
	{
		if (!oWriteDeltafunct(ecx, slot, buf, from, to, isnewcommand))
			return false;

		from = to;
	}
	auto lastrealcmd = g_GameInput->GetUserCmd(slot, from);
	CUserCmd fromcmd;

	if (lastrealcmd)
		fromcmd = *lastrealcmd;

	CUserCmd tocmd = fromcmd;
	tocmd.command_number = nextcmdnumber++;
	tocmd.tick_count += 100;

	//fake cmds
	for (int i = new_commands; i <= totalnewcommands; i++)
	{
		//WriteUsercmd(buf, &tocmd, &fromcmd);
		fromcmd = tocmd;
		tocmd.command_number++;
		tocmd.tick_count++;
	}
	if (!g_pPrediction->bEnginePaused && g_pGlobalVars->frametime > 0.f)
		tocmd.tick_count++;

	return true;
}
void __fastcall HookManager::PaintTraverse::hook(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	static unsigned int panelID, panelHudID;
	std::string panel_name = g_pPanel->GetName(vguiPanel);
	Installfonts();
	if (panelID == vguiPanel)
	{
		g_Menu.Render();
		FEATURES::MISC::in_game_logger.Do();
		if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame())
			return oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);

		if (!strcmp(SteamStrings("MatSystemTopPanel"), g_pPanel->GetName(vguiPanel)))
		{
			g_Misc.ConsoleModulate();
			if (g_Menu.Config.BombEsp)
			{
				bool BomBugFix = false;
				for (int i = 0; i < g_pEntityList->GetHighestEntityIndex(); i++)
				{
					C_BaseEntity* pEnt = g_pEntityList->GetClientEntity(i);
					if (pEnt && pEnt != g::pLocalEntity)
					{
						Vector2D PlantedPos;
						Vector2D DroppedPos;

						if (pEnt->GetClientClass()->ClassID == EClassIds::CPlantedC4)
						{
							if (pEnt->GetOrigin() != Vector(0, 0, 0) && Utils::WorldToScreen(pEnt->GetOrigin(), PlantedPos))
							{
								int bombTimer = std::ceil(pEnt->m_flC4Blow() - g_pGlobalVars->curtime);
								if (bombTimer > 0 && !pEnt->m_bBombDefused())
								{
									std::string timer = std::to_string(bombTimer);
									g_pSurface->DrawT(PlantedPos.x, PlantedPos.y, Color(255, 0, 0), g::TahomaSmall, true, SteamStrings("Planted Bomb"));
									g_pSurface->DrawT(PlantedPos.x, PlantedPos.y + 10, Color(230, 160, 0), g::TahomaSmall, true, &timer[0]);
									if (g::pLocalEntity->GetTeam() == 3 && g::pLocalEntity->IsAlive())
									{
										if (g::pLocalEntity->hasdefuser() && bombTimer >= 5 || !g::pLocalEntity->hasdefuser() && bombTimer >= 10)
										{
											g_pSurface->DrawT(PlantedPos.x, PlantedPos.y + 20, Color(0, 255, 0), g::TahomaSmall, true, "You have enough time");
										}
										else if ((g::pLocalEntity->hasdefuser() && bombTimer <= 5) || (!g::pLocalEntity->hasdefuser() && bombTimer <= 10))
										{
											g_pSurface->DrawT(PlantedPos.x, PlantedPos.y + 20, Color(255, 40, 0), g::TahomaSmall, true, "You don't have enough time");
										}
										BomBugFix = false;
									}
								}
								else if (pEnt->m_bBombDefused())
								{
									g_pSurface->DrawT(PlantedPos.x, PlantedPos.y + 10, Color(0, 130, 255), g::TahomaSmall, true, SteamStrings("Bomb has been defused"));
									BomBugFix = false;
								}
								else if (bombTimer == 0 && !pEnt->m_bBombDefused())
								{
									g_pSurface->DrawT(PlantedPos.x, PlantedPos.y + 10, Color(255, 40, 0), g::TahomaSmall, true, SteamStrings("BOOM"));
									BomBugFix = false;
								}
							}
						}
						else if (pEnt->GetClientClass()->ClassID == EClassIds::CC4)
						{
							if (pEnt->GetOrigin() != Vector(0, 0, 0) && Utils::WorldToScreen(pEnt->GetOrigin(), DroppedPos))
							{
								g_pSurface->DrawT(DroppedPos.x, DroppedPos.y, Color(255, 40, 0), g::TahomaSmall, true, SteamStrings("Dropped Bomb"));
							}
						}
					}
				}
			}
		}
		g_Menu.WaterMark();
		g_Misc.OnPaintTraverse();
		g_ESP.DrawZeusRange();
		g_ESP.Render();
		g_RemasteredEsp.Render();
		g_ESP.QuakeKillCounter();
		if (g_Menu.Config.VisHitmarker)
			hit();
		g_SpecList.Draw();
		if (g_Menu.Config.GrenadePrediction)
			GrenadePrediction::get().Paint();
	}
	if (!panelHudID)
	{
		if (!strcmp(SteamStrings("HudZoom"), g_pPanel->GetName(vguiPanel)))
		{
			panelHudID = vguiPanel;
		}
	}


	if (!panelID)
	{
		if (!strcmp(SteamStrings("MatSystemTopPanel"), g_pPanel->GetName(vguiPanel)))
		{
			panelID = vguiPanel;
			bInitializedDrawManager = true;
		}
	}

	// Disable Panorama Blur
	ConVar* PanoramaDisableBlur = g_pCvar->FindVar(SteamStrings("@panorama_disable_blur"));
	PanoramaDisableBlur->SetValue(g_Menu.Config.DisablePanoramaBlur);

	if (!g::pLocalEntity || !g::pLocalEntity->IsAlive())
		return oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);

	static const auto real_color = Color(255, 0, 0, 200);
	static const auto fake_color = Color(50, 50, 255, 200);
	static const auto lby_color = Color(0, 255, 0, 200);
	static const auto advanced_shit = Color(255, 255, 255, 80);

	if (!g::pLocalEntity->GetActiveWeapon())
		return;

	if (g_Menu.Config.OverrideViewmodel) {
		ConVar* viewmodel_offset_x = g_pCvar->FindVar(SteamStrings("viewmodel_offset_x"));
		ConVar* viewmodel_offset_y = g_pCvar->FindVar(SteamStrings("viewmodel_offset_y"));
		ConVar* viewmodel_offset_z = g_pCvar->FindVar(SteamStrings("viewmodel_offset_z"));

		*(int*)((DWORD)&viewmodel_offset_x->fnChangeCallback + 0xC) = 0;
		*(int*)((DWORD)&viewmodel_offset_y->fnChangeCallback + 0xC) = 0;
		*(int*)((DWORD)&viewmodel_offset_z->fnChangeCallback + 0xC) = 0;

		static float old_value_x = viewmodel_offset_x->GetFloat();
		static float old_value_y = viewmodel_offset_y->GetFloat();
		static float old_value_z = viewmodel_offset_z->GetFloat();

		viewmodel_offset_x->SetValue(old_value_x + (g_Menu.Config.ViewmodelOffsetX - 20));
		viewmodel_offset_y->SetValue(old_value_y + (g_Menu.Config.ViewmodelOffsetY - 20));
		viewmodel_offset_z->SetValue(old_value_z + (g_Menu.Config.ViewmodelOffsetZ - 20));
	}


	if (g_Menu.Config.aspectratio)
		g_Misc.acpectratio(g_Menu.Config.aspectvalue);
	else
		g_Misc.acpectratio(0);

	if (g_Menu.Config.ShowServerHitboxes && g::pLocalEntity && g::pLocalEntity->IsAlive())
	{
		static uintptr_t pCall = (uintptr_t)Utils::FindSignature(SteamStrings("server.dll"), SteamStrings("55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 57 8B CE"));

		static float fDuration = 0.1f;

		PVOID pEntity = nullptr;
		pEntity = UTIL_PlayerByIndex(g::pLocalEntity->EntIndex());

		if (pEntity)
		{
			__asm
			{
				pushad
				movss xmm1, fDuration
				push 0 //bool monoColor
				mov ecx, pEntity
				call pCall
				popad
			}
		}
	}
	if (g_Menu.Config.NoScope && !strcmp("HudZoom", g_pPanel->GetName(vguiPanel)) && g::pLocalEntity->IsScoped() && g::pLocalEntity->IsAlive())
		return;
	if (g_Menu.Config.AntiAimIndicator)
	{
		if (!g_Menu.Config.Antiaim && !g_Menu.Config.LegitAA)
			return oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);

		if (!g_Menu.Config.thirdperson || !g_GameInput->m_fCameraInThirdPerson)
			return oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);

		float distance = 40.f;

		Vector local_origin = g::pLocalEntity->GetAbsOrigin();

		Vector2D screen1, screen2;

		Utils::WorldToScreen(local_origin, screen1);

		Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::FakeAngle.y, distance), screen2);
		g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, fake_color);
		g_pSurface->DrawT(screen2.x, screen2.y, fake_color, g::EspSmallText, true, SteamStrings("FAKE"));

		Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::RealAngle.y, distance), screen2);
		g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, real_color);
		g_pSurface->DrawT(screen2.x, screen2.y, real_color, g::EspSmallText, true, SteamStrings("REAL"));

		Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::pLocalEntity->GetLowerBodyYaw(), distance), screen2);
		g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, lby_color);
		g_pSurface->DrawT(screen2.x, screen2.y, lby_color, g::EspSmallText, true, SteamStrings("LBY"));

		if (!g_Menu.Config.AdvancedAAIndicator)
			return oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);

		Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::pLocalEntity->AnimState()->m_flGoalFeetYaw, 30.f), screen2);
		g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, advanced_shit);
		g_pSurface->DrawT(screen2.x, screen2.y, advanced_shit, g::EspSmallText, true, SteamStrings("GOAL_FEET"));

		Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::pLocalEntity->AnimState()->m_flEyeYaw, 30.f), screen2);
		g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, advanced_shit);
		g_pSurface->DrawT(screen2.x, screen2.y, advanced_shit, g::EspSmallText, true, SteamStrings("EYES"));
	}
	oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);
}

float dot_product(const Vector& a, const Vector& b) {
	return (a.x * b.x
		+ a.y * b.y
		+ a.z * b.z);
}
static void CapsuleOverlay(C_BaseEntity* pPlayer, Color col, float duration)
{
	if (!pPlayer)
		return;

	studiohdr_t* pStudioModel = g_pModelInfo->GetStudiomodel((model_t*)pPlayer->GetModel());
	if (!pStudioModel)
		return;

	static matrix3x4_t pBoneToWorldOut[128];
	if (!pPlayer->SetupBones(pBoneToWorldOut, 128, 256, 0))
		return;

	mstudiohitboxset_t* pHitboxSet = pStudioModel->GetHitboxSet(0);
	if (!pHitboxSet)
		return;

	auto VectorTransform2 = [](const Vector in1, matrix3x4_t in2, Vector& out)
	{

		out[0] = dot_product(in1, Vector(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
		out[1] = dot_product(in1, Vector(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
		out[2] = dot_product(in1, Vector(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
	};

	for (int i = 0; i < pHitboxSet->numhitboxes; i++)
	{
		mstudiobbox_t* pHitbox = pHitboxSet->GetHitbox(i);
		if (!pHitbox)
			continue;

		Vector vMin, vMax;
		VectorTransform2(pHitbox->min, pBoneToWorldOut[pHitbox->bone], vMin); //nullptr???
		VectorTransform2(pHitbox->max, pBoneToWorldOut[pHitbox->bone], vMax);

		if (pHitbox->radius > -1)
		{
			g_pDebugOverlay->AddCapsuleOverlay(vMin, vMax, pHitbox->radius, col.red, col.green, col.blue, col.alpha, duration);
		}
	}
}
template<class T2>
static T2* FindHudElement2(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(Utils::FindSignature(SteamStrings("client.dll"), SteamStrings("B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08")) + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(Utils::FindSignature(SteamStrings("client.dll"), SteamStrings("55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28")));
	return (T2*)find_hud_element(pThis, name);
}
PlayerInfo_t GetInfo(int Index) {
	PlayerInfo_t Info;
	g_pEngine->GetPlayerInfo(Index, &Info);
	return Info;
}
static bool buybotfix;
void Event::FireGameEvent(IGameEvent* event)
{
	if (!event)
		return;

	if (!g::pLocalEntity)
		return;

	if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame())
		return;
	if (buybotfix)
	{
		if (g_Menu.Config.BuyBot.enable)
		{
			if (g::pLocalEntity->IsAlive())
			{
				//main
				if (g_Menu.Config.BuyBot.main == 0)
					g_pEngine->ExecuteClientCmd("");
				if (g_Menu.Config.BuyBot.main == 1)
					g_pEngine->ExecuteClientCmd(SteamStrings("buy g3sg1; buy scar20"));
				if (g_Menu.Config.BuyBot.main == 2)
					g_pEngine->ExecuteClientCmd(SteamStrings("buy awp"));
				if (g_Menu.Config.BuyBot.main == 3)
					g_pEngine->ExecuteClientCmd(SteamStrings("buy ssg08"));
				//second
				if (g_Menu.Config.BuyBot.second == 0)
					g_pEngine->ExecuteClientCmd("");
				if (g_Menu.Config.BuyBot.second == 1)
					g_pEngine->ExecuteClientCmd(SteamStrings("buy deagle; buy revolver"));
				if (g_Menu.Config.BuyBot.second == 2)
					g_pEngine->ExecuteClientCmd(SteamStrings("buy elite"));
				//armor
				if (g_Menu.Config.BuyBot.armor == 0)
					g_pEngine->ExecuteClientCmd("");
				if (g_Menu.Config.BuyBot.armor == 1)
					g_pEngine->ExecuteClientCmd(SteamStrings("buy vesthelm; buy vest"));
				//Misc
				if (g_Menu.Config.BuyBot.misc == 0)
					g_pEngine->ExecuteClientCmd("");
				if (g_Menu.Config.BuyBot.misc == 1)
					g_pEngine->ExecuteClientCmd(SteamStrings("buy defuser"));
				if (g_Menu.Config.BuyBot.misc == 2)
					g_pEngine->ExecuteClientCmd(SteamStrings("buy taser"));
				if (g_Menu.Config.BuyBot.misc == 3)
					g_pEngine->ExecuteClientCmd(SteamStrings("buy defuser;buy taser"));
				//grenade
				if (g_Menu.Config.BuyBot.grenade == 0)
					g_pEngine->ExecuteClientCmd("");
				if (g_Menu.Config.BuyBot.grenade == 1)
					g_pEngine->ExecuteClientCmd(SteamStrings("buy hegrenade; buy molotov; buy smokegrenade; buy incgrenade;"));
			}
		}
		buybotfix = false;
	}

	if (strstr(event->GetName(), SteamStrings("round_start")) || strstr(event->GetName(), SteamStrings("round_start")) || strstr(event->GetName(), SteamStrings("round_prestart")) || strstr(event->GetName(), SteamStrings("client_disconnect")) || strstr(event->GetName(), SteamStrings("cs_game_disconnected"))) // trying everything
	{
		buybotfix = true;
		return;
	}
	auto filter = CHudChat::ChatFilters::CHAT_FILTER_NONE;
	auto name = event->GetName();
	if (strstr(event->GetName(), SteamStrings("player_hurt"))) {

		auto attacker = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt(SteamStrings("attacker"))));
		if (!attacker)
			return;

		if (attacker != g::pLocalEntity)
			return;

		int index = g_pEngine->GetPlayerForUserID(event->GetInt(SteamStrings("userid")));
		int hitgroup = event->GetInt(SteamStrings("hitgroup"));

		if (g_pEngine->GetPlayerForUserID(event->GetInt(SteamStrings("attacker"))) == g_pEngine->GetLocalPlayer())
		{
			HitMarker_t temp = { g_pGlobalVars->curtime + g_Menu.Config.ExpireTime, event->GetInt(SteamStrings("dmg_health")) };

			if (temp.damage > 0 && temp.damage < 100)
				HitMarkers.push_back(temp);
		}

		PlayerInfo_t pInfo;
		g_pEngine->GetPlayerInfo(index, &pInfo);

		g::Hit[index] = true;
		g::hits++;
		switch (g_Menu.Config.Hitmarker)
		{
		case 1:
			g_pSurface->PlaySoundA(SteamStrings("buttons\\arena_switch_press_02.wav")); // metalic
			break;
		case 2:
			g_pSurface->PlaySoundA(SteamStrings("physics\\metal\\paintcan_impact_hard3.wav")); // metalic2
			break;
		case 3:
			g_pSurface->PlaySoundA(SteamStrings("buttons\\combine_button7.wav"));  // beep1
			break;
		case 4:
			g_pSurface->PlaySoundA(SteamStrings("buttons\\button14.wav")); //beep 2
			break;
		case 5:
			g_pSurface->PlaySoundA(SteamStrings("buttons\\bell1.wav")); //bell
			break;
		case 6:
			g_pSurface->PlaySoundA(SteamStrings("player\\headshot1.wav")); // hs 1
			break;
		case 7:
			g_pSurface->PlaySoundA(SteamStrings("player\\headshot2.wav")); // hs 2
			break;
		case 8:
			PlaySoundA(bubble, NULL, SND_ASYNC | SND_MEMORY); break;
			break;
		case 9:
			PlaySoundA(fatality, NULL, SND_ASYNC | SND_MEMORY); break;
			break;
		case 10:
			PlaySoundA(cod, NULL, SND_ASYNC | SND_MEMORY); break;
			break;
		case 11:
			PlaySoundA(laser, NULL, SND_ASYNC | SND_MEMORY); break;
			break;
		default:
			break;
		}

		if (g_Menu.Config.VizualizeHits)
		{
			C_BaseEntity* hittedplayer = (C_BaseEntity*)g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt(SteamStrings("userid"))));

			if (hittedplayer && hittedplayer->EntIndex() > 0 && hittedplayer->EntIndex() < 64)
			{
				if (g::pLocalEntity && hittedplayer != g::pLocalEntity)
					CapsuleOverlay(hittedplayer, Color(g_Menu.Config.CapsuleColor.red, g_Menu.Config.CapsuleColor.green, g_Menu.Config.CapsuleColor.blue, 255), 3);
			}
		}

		static auto get_hitgroup_name = [](int hitgroup) -> std::string {
			switch (hitgroup) {
			case HITGROUP_HEAD:
				return SteamStrings("head");
			case HITGROUP_LEFTLEG:
				return SteamStrings("left leg");
			case HITGROUP_RIGHTLEG:
				return SteamStrings("right leg");
			case HITGROUP_STOMACH:
				return SteamStrings("stomach");
			case HITGROUP_LEFTARM:
				return SteamStrings("left arm");
			case HITGROUP_RIGHTARM:
				return SteamStrings("right arm");
			default:
				return SteamStrings("body");
			}
		};

		int attackerid = event->GetInt(SteamStrings("attacker"));
		int entityid = g_pEngine->GetPlayerForUserID(attackerid);

		int userid = event->GetInt(SteamStrings("userid"));
		int userentityid = g_pEngine->GetPlayerForUserID(userid);

		int health = event->GetInt(SteamStrings("health"));



		if (!health && health != 0)
			return;

		if (g_Menu.Config.LogDamage)
		{
			if (entityid == g_pEngine->GetLocalPlayer() && userentityid != g_pEngine->GetLocalPlayer())
			{
				int nUserID = event->GetInt(SteamStrings("attacker"));
				int nDead = event->GetInt(SteamStrings("userid"));
				int nPurchase = event->GetInt(SteamStrings("item_purchase"));
				if (nUserID || nDead)
				{
					PlayerInfo_t killer_info = GetInfo(g_pEngine->GetPlayerForUserID(nUserID));
					PlayerInfo_t killed_info = GetInfo(g_pEngine->GetPlayerForUserID(nDead));

					std::string prefix = SteamStrings("\x0D[Alpheus] \x01");
					std::string player = killed_info.szName;
					std::string gothit = SteamStrings(" got hit");
					std::string inthe = SteamStrings(" in the ");
					std::string hitbox = get_hitgroup_name(event->GetInt("hitgroup"));
					std::string forr = SteamStrings(" for ");
					std::string damage = event->GetString("dmg_health");
					std::string damage2 = SteamStrings(" damage");
					std::string fourb = SteamStrings(" (");
					std::string fiveb = event->GetString("health");
					std::string six = SteamStrings(" health remaining)");

					int wchars_num = MultiByteToWideChar(CP_UTF8, 0, player.c_str(), -1, NULL, 0);
					wchar_t* wstr = new wchar_t[wchars_num];
					MultiByteToWideChar(CP_UTF8, 0, player.c_str(), -1, wstr, wchars_num);
					_bstr_t b(wstr);
					const char* char_fixedname = b;

					if (g_Menu.Config.log_chat)
					{
						g_ChatElement->ChatPrintf(0, filter, (prefix + char_fixedname + gothit + inthe + hitbox + forr + damage + damage2 + fourb + fiveb + six).c_str());
					}
					g_pEngine->ExecuteClientCmd(("echo " + prefix + char_fixedname + gothit + inthe + hitbox + forr + damage + damage2 + fourb + fiveb + six).c_str());
					if (g_Menu.Config.log_logger)
					{
						FEATURES::MISC::InGameLogger::Log log;
						log.color_line.PushBack("[Alpheus] ", Color(190, 0, 255, 255));
						log.color_line.PushBack(char_fixedname, Color(255, 255, 255, 255));
						log.color_line.PushBack(" got hit", Color(255, 255, 255, 255));
						log.color_line.PushBack(" int the ", Color(255, 255, 255, 255));
						log.color_line.PushBack(hitbox, Color(255, 47, 0, 255));
						log.color_line.PushBack(" for ", Color(255, 255, 255, 255));
						log.color_line.PushBack(damage, Color(255, 47, 0, 255));
						log.color_line.PushBack(damage2, Color(255, 255, 255, 255));
						log.color_line.PushBack(fourb, Color(255, 255, 255, 255));
						log.color_line.PushBack(fiveb, Color(255, 47, 0, 255));
						log.color_line.PushBack(six, Color(255, 255, 255, 255));
						FEATURES::MISC::in_game_logger.AddLog(log);
					}
				}
			}
		}
		if (strstr(event->GetName(), SteamStrings("item_purchase")))
		{
			if (g_Menu.Config.LogPurchases)
			{
				auto* entity = (C_BaseEntity*)g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("userid")));
				if (!entity)
					return;

				if (entity->GetTeam() == g::pLocalEntity->GetTeam())
					return;

				PlayerInfo_s player_info;
				if (!g_pEngine->GetPlayerInfo(entity->EntIndex(), &player_info))
					return;
				std::string xerxes = "[Alpheus] ";
				std::string player = player_info.szName;
				std::string weapon = event->GetString(SteamStrings("weapon"));
				if (g_Menu.Config.log_chat)
				{
					g_ChatElement->ChatPrintf(0, filter, (xerxes + player + " bought a " + weapon).c_str());
				}
				if (g_Menu.Config.log_logger)
				{
					FEATURES::MISC::InGameLogger::Log log;
					log.color_line.PushBack(xerxes, Color(190, 0, 255, 255));
					log.color_line.PushBack(player, Color(255, 255, 255, 255));
					log.color_line.PushBack(" bought a ", Color(255, 255, 255, 255));
					log.color_line.PushBack(weapon, Color(255, 47, 0, 255));
					FEATURES::MISC::in_game_logger.AddLog(log);
				}
			}
		}
		if (g_Menu.Config.Killsay)
		{
			if (health == 0)
			{
				switch (g_Menu.Config.killsaytype)
				{
				case 0:
				{

					int nDead = event->GetInt(SteamStrings("userid"));
					PlayerInfo_t killed_info = GetInfo(g_pEngine->GetPlayerForUserID(nDead));

					std::string player = killed_info.szName;

					const char* trashtalks[47] =
					{
					"say WOW! imagine if your parents weren't siblings.",
					"say I PLAY WITH A RACING WHEEL",
					"say CS:GO is too hard for you m8 maybe consider a game that requires less skill, like idk.... solitaire",
					"say Bro you couldn't hit an elephant in the ass with a shotgun with aim like that",
					"say You're the human equivalent of a participation award.",
					"say server cvar 'sv_rekt' changed to 1.",
					"say Was that your spray on the wall or are you just happy to see me?",
					"say Oops, I must have chosen easy bots by accident",
					"say Is this casual?? I have 16k..",
					"say I thought I already finished chemistry.. So much NaCl around here..",
					"say Did you know that csgo is free to uninstall?",
					"say Even Noah can't carry these animals",
					"say Options -> How To Play",
					"say How much did you tag that wall for??",
					"say I thought I put bots on hard, why are they on easy?",
					"say Who are you sponsored by? Parkinson's?",
					"say How did you change your difficulty settings? My CS:GO is stuck on easy",
					"say Nice $4750 decoy",
					"say If I were to commit suicide, I would jump from your ego to your elo.",
					"say Is your monitor on",
					"say idk if u know but it's mouse1 to shoot",
					"say you guys buy accounts on ebay??",
					"say You have the reaction time of a dead puppy.",
					"say The only thing you carry is an extra chromosome.",
					"say How do you change your difficulty settings? My CSGO is stuck on easy",
					"say If I wanted to suicide, i'd jump from your ego to your elo",
					"say Options -> How To Play",
					"say Safest place for us to stand is in front of your gun",
					"say is that a decoy, or are you trying to shoot somebody?",
					"say I could swallow bullets and shit out a better spray than that",
					"say Not even Noah can carry these animals",
					"say deranking?",
					"say a bad awper = $5k decoy",
					"say Shut up, I fucked your dad.",
					"say How much did you tag that wall for?",
					"say You REALLY gotta win those",
					"say Buy sound next time",
					"say mad cuz bad",
					"say You can't even carry groceries in from the car",
					"say I kissed your mom last night. Her breath was globally offensive",
					"say Dude you're so fat you run out of breath rushing B",
					"say Did you learn your spray downs in a bukkake video?",
					"say You're almost as salty as the semen dripping from your mum's mouth",
					"say Rest in spaghetti never forgetti",
					"say Don't be upsetti, have some spaghetti",
					"say Stop buying an awp you $4750 Decoy",
					"say This guy is more toxic than the beaches at Fukushima"
					};

					g_pEngine->ExecuteClientCmd(trashtalks[rand() % 48]);
					break;

				}
				case 1:
				{
					int nDead = event->GetInt(SteamStrings("userid"));
					PlayerInfo_t killed_info = GetInfo(g_pEngine->GetPlayerForUserID(nDead));
					std::string player = killed_info.szName;
					std::string say = SteamStrings("say Hey ") + (std::string)killed_info.szName + SteamStrings(", I think you got rekted by Alpheus :)");
					g_pEngine->ExecuteClientCmd(say.c_str());
					break;
				}
				}
			}
		}
	}
	else if (strstr(event->GetName(), SteamStrings("player_death"))) {

		if (!g_pEngine->IsConnected() || !g_pEngine->IsConnected())
			return;

		if (!g::pLocalEntity)
			return;

		auto attacker = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt(SteamStrings("attacker"))));
		if (!attacker)
			return;

		if (attacker != g::pLocalEntity)
			return;

		int index = g_pEngine->GetPlayerForUserID(event->GetInt(SteamStrings("userid")));

		if (!index)
			return;

		static DWORD* deathNotice = FindHudElement2<DWORD>(SteamStrings("CCSGO_HudDeathNotice"));

		static DWORD* _death_notice = FindHudElement2<DWORD>(SteamStrings("CCSGO_HudDeathNotice"));
		static void(__thiscall * _clear_notices)(DWORD) = (void(__thiscall*)(DWORD))Utils::FindSignature(SteamStrings("client.dll"), SteamStrings("55 8B EC 83 EC 0C 53 56 8B 71 58"));


		if (_death_notice)
			*(float*)((DWORD)_death_notice + 0x50) = (g_Menu.Config.PreserveKillFeed ? 500 : 1.5);

		if (g_Menu.Config.KillCounter)
		{
			g::killtime = g_pGlobalVars->realtime;
			g::kills++;
		}

		g::globalAlpha = g_pGlobalVars->curtime;
	}
	else if (strstr(event->GetName(), SteamStrings("round_start")))
	{
		if (!g_pEngine->IsConnected() || !g_pEngine->IsConnected())
			return;

		if (!g::pLocalEntity)
			return;

		g::kills = 0;

		static DWORD* deathNotice = FindHudElement2<DWORD>(SteamStrings("CCSGO_HudDeathNotice"));

		static DWORD* _death_notice = FindHudElement2<DWORD>(SteamStrings("CCSGO_HudDeathNotice"));
		static void(__thiscall * _clear_notices)(DWORD) = (void(__thiscall*)(DWORD))Utils::FindSignature(SteamStrings("client.dll"), SteamStrings("55 8B EC 83 EC 0C 53 56 8B 71 58"));

		_death_notice = FindHudElement2<DWORD>(SteamStrings("CCSGO_HudDeathNotice"));
		_clear_notices(((DWORD)_death_notice - 20));
	}
	else if (strstr(event->GetName(), SteamStrings("bullet_impact")))
	{
		if (g_Menu.Config.BulletTracers)
		{
			auto* index = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt(SteamStrings("userid"))));


			Vector position(event->GetFloat(SteamStrings("x")), event->GetFloat(SteamStrings("y")), event->GetFloat(SteamStrings("z")));

			if (g::pLocalEntity)
			{
				if (index)
					trace_logs.push_back(trace_info(index->GetEyePosition(), position, g_pGlobalVars->curtime, event->GetInt(SteamStrings("userid"))));
			}
		}
	}
	else if (strstr(event->GetName(), SteamStrings("weapon_fire")) && !g::pLocalEntity->IsKnifeorNade())
	{
		if (g_pEngine->GetPlayerForUserID(event->GetInt(SteamStrings("userid"))) == g_pEngine->GetLocalPlayer())
			g::shots++;
	}
};



void __fastcall HookManager::OverrideView::hook(void* ecx, void* edx, CViewSetup* pSetup)
{
	if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
	{
		if (!g::pLocalEntity)
			return;

		if (g_Menu.Config.NoRecoil)
		{
			auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
			if (!local_player)
				return;

			auto weapon = local_player->GetActiveWeapon();
			if (weapon)
				weapon->GetAccuracyPenalty();

			g::pCmd->viewangles -= local_player->GetAimPunchAngle() * 2;
		}

		if (!g_GameInput->m_fCameraInThirdPerson)
			g_Menu.Config.CurrentFov = g_Menu.Config.Fov;

		if (!g::pLocalEntity->IsScoped())
			pSetup->fov = g_Menu.Config.CurrentFov;

		if (g::pLocalEntity->GetActiveWeapon()->IsNade())
			pSetup->fov = 90;

		if (g_Menu.Config.GrenadePrediction) {
			GrenadePrediction::get().View(pSetup);
		}

		if (g_Menu.Config.OverrideViewmodel)
			g_Misc.roll_fov(pSetup);

		if (g_Menu.Config.thirdperson)
			g_Misc.ThirdPerson();

		if (g_Menu.Config.NoZoom && g::pLocalEntity->IsAlive())
		{
			if (g::pLocalEntity->IsScoped())
				pSetup->fov = g_Menu.Config.CurrentFov;
		}

		if (g_Menu.Config.FakeDuck && GetAsyncKeyState(g_Menu.Config.FakeDuckKey)) {
			pSetup->origin.z = g::pLocalEntity->GetAbsOrigin().z + 64.f;
		}
	}

	oOverrideView(ecx, edx, pSetup);
}

void __fastcall HookManager::LockCursor::hook(ISurface* thisptr, void* edx)
{
	if (!g_Menu.menuOpened)
		return oLockCursor(thisptr, edx);

	g_pSurface->UnLockCursor();
}

float __stdcall HookManager::GetViewmodelFov::hook()
{
	if (g_pEngine->IsConnected() && g_pEngine->IsInGame() && g::pLocalEntity)
		return g_Menu.Config.ViewmodelFov;
}



LRESULT Hooks::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const auto getButtonHeld = [uMsg, wParam](bool& bButton, int vKey)
	{
		if (wParam != vKey) return;

		if (uMsg == WM_KEYDOWN)
			bButton = true;
		else if (uMsg == WM_KEYUP)
			bButton = false;
	};

	const auto getButtonToggle = [uMsg, wParam](bool& bButton, int vKey)
	{
		if (wParam != vKey) return;

		if (uMsg == WM_KEYUP)
			bButton = !bButton;
	};

	if (bInitializedDrawManager)
	{
		if (g_Menu.menuOpened)
		{
			return true;
		}
	}


	return CallWindowProcA(g_Hooks.pOriginalWNDProc, hWnd, uMsg, wParam, lParam);
}
