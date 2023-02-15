#include <fstream>
#include <thread>
#include "Interfaces.h"
#include "Utils.h"
#include "..\SDK\IClientMode.h"
#include "..\SDK\IBaseClientDll.h"
#include "..\SDK\IClientEntityList.h"
#include "..\SDK\IVEngineClient.h"
#include "..\SDK\CPrediction.h"
#include "..\SDK\IGameEvent.h"
#include "..\SDK\ISurface.h"
#include "..\SDK\IEngineTrace.h"
#include "..\SDK\ISurfaceData.h"
#include "..\SDK\ICvar.h"
#include "..\SDK\IVModelInfo.h"
#include "..\SDK\CModelRender.h"
#include "..\SDK\IMaterial.h"
#include "..\SDK\IVRenderView.h"
#include "..\SDK\CGlowObjectManager.h"
#include "..\SDK\IClientState.h"
#include "..\SDK\CHudChat.h"
#include "..\SDK\IGameConsole.h"
#include "../SDK/SteamAPI.h"
#include "../Utils/xor.h"
#include "../Features/ProfileChanger/ProfileChanger/MemAlloc.h"
#include "../Features/ProfileChanger/protobuf/SteamClient.h"

#define enc_str(s) std::string(s)
#define enc_char(s) enc_str(s).c_str()
#define enc_wstr(s) std::wstring(enc_str(s).begin(), enc_str(s).end())
#define enc_wchar(s) enc_wstr(s).c_str()
#define WaitOnFind(EXP) \
	while ((EXP) == 0) { std::this_thread::sleep_for(std::chrono::seconds(1)); }

ISteamGameCoordinator* g_SteamGameCoordinator = nullptr;
#ifdef RELEASE
MemAlloc* g_pMemAlloc = nullptr;
ISteamUser* g_SteamUser = nullptr;
ISteamClient* g_SteamClient = nullptr;
#endif
IBaseClientDLL* g_pClientDll = nullptr;
IClientMode* g_pClientMode = nullptr;
IClientEntityList* g_pEntityList = nullptr;
IVEngineClient* g_pEngine = nullptr;
CPrediction* g_pPrediction = nullptr;
IGameMovement* g_pMovement = nullptr;
IMoveHelper* g_pMoveHelper = nullptr;
CGlobalVarsBase* g_pGlobalVars = nullptr;
IGameEventManager* g_pEventManager = nullptr;
ISurface* g_pSurface = nullptr;
IEngineTrace* g_pTrace = nullptr;
IPhysicsSurfaceProps* g_pSurfaceData = nullptr;
ICVar* g_pCvar = nullptr;
IPanel* g_pPanel = nullptr;
IVModelInfo* g_pModelInfo = nullptr;
CModelRender* g_pModelRender = nullptr;
IMaterialSystem* g_pMaterialSys = nullptr;
IVRenderView* g_pRenderView = nullptr;
IVDebugOverlay* g_pDebugOverlay = nullptr;
IClientState* g_pClientState = nullptr;
CGlowObjectManager* g_GlowManager = nullptr;
CHudChat* g_ChatElement = nullptr;
CInput* g_GameInput = nullptr;
IEngineSound* g_EngineSound = nullptr;
CCSGameRules* g_GameRules = nullptr;
IGameConsole* g_pGameConsole = nullptr;

namespace interfaces
{

	void Init()
	{
		const auto client = SteamStrings("client.dll"); //Valve....using client.dll again? But why?
		const auto enginedll = SteamStrings("engine.dll");
		const auto vguimatsurfacedll = SteamStrings("vguimatsurface.dll");
		const auto materialsystemdll = SteamStrings("materialsystem.dll");
		const auto vstdlibdll = SteamStrings("vstdlib.dll");
		const auto vgui2dll = SteamStrings("vgui2.dll");

		/*-----------------------------------------------------------Game Coordinator for rank changer-----------------------------------------------------------*/
		HMODULE SteamDLLHandle = nullptr;
		WaitOnFind(SteamDLLHandle = GetModuleHandle(SteamStrings("steam_api.dll")));
#ifdef RELEASE
		g_SteamClient = (ISteamClient*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(SteamStrings("steamclient.dll")), SteamStrings("CreateInterface")))	(SteamStrings("SteamClient017"), nullptr));
		auto GetHSteamPipe = reinterpret_cast<HSteamPipe(*)()>(GetProcAddress(SteamDLLHandle, SteamStrings("GetHSteamPipe")));
		auto GetHSteamUser = reinterpret_cast<HSteamUser(*)()>(GetProcAddress(SteamDLLHandle, SteamStrings("GetHSteamUser")));
		g_SteamUser = g_SteamClient->GetISteamUser(GetHSteamUser(), GetHSteamPipe(), SteamStrings("SteamUser019"));
		g_SteamGameCoordinator = (ISteamGameCoordinator*)g_SteamClient->GetISteamGenericInterface(GetHSteamUser(), GetHSteamPipe(), SteamStrings("SteamGameCoordinator001"));
		g_pMemAlloc = *reinterpret_cast<MemAlloc**>(GetProcAddress(GetModuleHandleA(SteamStrings("tier0.dll")), SteamStrings("g_pMemAlloc")));
#endif
		/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
		g_pEngine = FindClass<IVEngineClient>(enginedll, "VEngineClient");
		g_pClientDll = FindClass<IBaseClientDLL>(client, "VClient");
		g_pClientState = **reinterpret_cast<IClientState***>((*reinterpret_cast<uintptr_t**>(g_pEngine))[12] + 0x10);
		g_pGlobalVars = **reinterpret_cast<CGlobalVarsBase***>((*reinterpret_cast<uintptr_t**>(g_pClientDll))[0] + 0x1F);
		g_pClientMode = **reinterpret_cast<IClientMode***>    ((*reinterpret_cast<uintptr_t**>(g_pClientDll))[10] + 0x5);
		g_pEntityList = FindClass<IClientEntityList>(client, "VClientEntityList");
		g_pPrediction = FindClass<CPrediction>(client, "VClientPrediction");
		g_pMovement = FindClass<IGameMovement>(client, "GameMovement");
		g_pMoveHelper = **reinterpret_cast<IMoveHelper***>((Utils::FindSignature(client, "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 0x2));
		g_pEventManager = CaptureInterface<IGameEventManager>(enginedll, "GAMEEVENTSMANAGER002");
		g_pSurface = FindClass<ISurface>(vguimatsurfacedll, "VGUI_Surface");
		g_pTrace = FindClass<IEngineTrace>(enginedll, "EngineTraceClient");
		g_pSurfaceData = FindClass<IPhysicsSurfaceProps>("vphysics.dll", "VPhysicsSurfaceProps");
		g_pCvar = FindClass<ICVar>(vstdlibdll, "VEngineCvar");
		g_pPanel = FindClass<IPanel>(vgui2dll, "VGUI_Panel");
		g_pModelInfo = FindClass<IVModelInfo>(enginedll, "VModelInfoClient");
		g_pModelRender = FindClass<CModelRender>(enginedll, "VEngineModel");
		g_pMaterialSys = FindClass<IMaterialSystem>(materialsystemdll, "VMaterialSystem");
		g_pRenderView = FindClass<IVRenderView>(enginedll, "VEngineRenderView");
		g_GlowManager = *reinterpret_cast<CGlowObjectManager**>(Utils::FindSignature(client, "0F 11 05 ? ? ? ? 83 C8 01") + 3); //0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00 Fix 20220525
		g_pDebugOverlay = CaptureInterface<IVDebugOverlay>(enginedll, "VDebugOverlay004");
		g_EngineSound = FindClass<IEngineSound>(enginedll, "IEngineSoundClient");
		g_GameInput = *(CInput**)(Utils::FindSignature(client, "B9 ? ? ? ? 8B 40 38 FF D0 84 C0 0F 85") + 1);
		g_GameRules = **reinterpret_cast<CCSGameRules***>(Utils::FindSignature(client, "A1 ? ? ? ? 85 C0 0F 84 ? ? ? ? 80 B8 ? ? ? ? ? 74 7A") + 0x1); //A1 ? ? ? ? 8B 0D ? ? ? ? 6A 00 68 ? ? ? ? C6 Fix 20220525
		g_pGameConsole = CaptureInterface<IGameConsole>(enginedll, SteamStrings("GameConsole004")); //It's not working, but im not using this. (crash)
		std::ofstream{ SteamStrings("csgo/materials/chamsAnimated.vmt") } <<
			"VertexLitGeneric { $envmap editor/cube_vertigo $envmapcontrast 1 $envmaptint \"[.7 .7 .7]\" $basetexture dev/zone_warning proxies { texturescroll { texturescrollvar $basetexturetransform texturescrollrate 0.6 texturescrollangle 90 } } }";
		std::ofstream(SteamStrings("csgo/materials/skeetchams.vmt")) << R"#("VertexLitGeneric"
{
      "$basetexture" "vgui/white_additive"
      "$ignorez" "0"
      "$additive" "0"
      "$envmap"  "models/effects/cube_white"
      "$normalmapalphaenvmapmask" "1"
      "$envmaptint" "[0 0.26 1]"
      "$envmapfresnel" "1"
      "$envmapfresnelminmaxexp" "[0 1 2]"

      "$envmapcontrast" "1"
      "$nofog" "1"
      "$model" "1"
      "$nocull" "0"
      "$selfillum" "1"
      "$halflambert" "1"
      "$znearer" "0"
      "$flat" "1" 
}
)#";


		std::ofstream(SteamStrings("csgo/materials/glowOverlay.vmt")) << R"#("VertexLitGeneric" {
     
    	"$additive" "1"
    	"$envmap" "models/effects/cube_white"
    	"$envmaptint" "[1 1 1]"
    	"$envmapfresnel" "1"
    	"$envmapfresnelminmaxexp" "[0 1 2]"
    	"$alpha" "0.8"
    })#";

		std::ofstream(SteamStrings("csgo/materials/testnigger.vmt")) << R"#("VertexLitGeneric" {
     	
		"$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       "env_cubemap"
  "$envmaptint"   "[0.6 0.6 0.6]"
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"



    })#";

		std::ofstream(SteamStrings("csgo\\materials\\custom_virt.vmt")) << R"("VertexLitGeneric" 
{
	"$basetexture"	"vgui/white"
	"$envmap"       "env_cubemap"
	"$model"		"1"
	"$flat"			"0"
	"$nocull"		"1"
	"$halflambert"	"1"
	"$nofog"		"1"
	"$ignorez"		"0"
	"$znearer"		"0"
	"$wireframe"	"0"
})";




	}
}