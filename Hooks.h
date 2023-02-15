#pragma once

#include "Utils\Interfaces.h"
#include "SDK\IBaseClientDll.h"
#include "SDK\IClientMode.h"
#include "SDK\ISurface.h"
#include "SDK\IGameEvent.h"
#include "SDK\CInput.h"
#include "SDK\CModelRender.h"
#include "SDK\IVModelInfo.h"
#include "SDK\IMaterial.h"
#include "SDK\IVRenderView.h"
#include <assert.h>
#include "SDK\CInput.h"
#include <Windows.h>
#include <unordered_map>
#include <cstdint>
#include <stdexcept>
#include <assert.h>
#include "SDK\MinHook\MinHook.h"
#pragma comment (lib,"MinHook.x86.lib")
class Protobuffs;
extern Protobuffs write;
enum EGCResultxd
{
	k_EGCResultOKxd = 0,
	k_EGCResultNoMessagexd = 1,           // There is no message in the queue
	k_EGCResultBufferTooSmallxd = 2,      // The buffer is too small for the requested message
	k_EGCResultNotLoggedOnxd = 3,         // The client is not logged onto Steam
	k_EGCResultInvalidMessagexd = 4,      // Something was wrong with the message being sent with SendMessage
};

namespace vtable_indexes
{
	constexpr auto view = 18;
	constexpr auto paint = 41;
	constexpr auto createMove = 24;
	constexpr auto createMoveFix = 22;
	constexpr auto lockCursor = 67;
	constexpr auto frameStage = 37;
	constexpr auto dme = 21;
	constexpr auto extraBonePro = 197 + 1; //-1 Fix 20220525
	constexpr auto sceneEnd = 9;
	constexpr auto viewmodelFov = 35;
	constexpr auto drawSetColor = 15;
	constexpr auto emitSound = 5;
	constexpr auto retrieve_message = 2;
	constexpr auto send_message = 0;
	constexpr auto SendDatagram = 46;

}

class Event : public IGameEventListener
{
public:
	void FireGameEvent(IGameEvent* event);
	int  GetEventDebugID = 42;
	void Init()
	{
		g_pEventManager->AddListener(this, "player_hurt", false);
		g_pEventManager->AddListener(this, "player_death", false);
		g_pEventManager->AddListener(this, "round_start", false);
		g_pEventManager->AddListener(this, "round_end", false);
		g_pEventManager->AddListener(this, "round_prestart", false);
		g_pEventManager->AddListener(this, "cs_game_disconnected", false);
		g_pEventManager->AddListener(this, "client_disconnect", false);
		g_pEventManager->AddListener(this, "bullet_impact", false);
		g_pEventManager->AddListener(this, "item_purchase", false);
		//g_pEventManager->AddListener(this, "weapon_purchase", false);
		g_pEventManager->AddListener(this, "weapon_fire", false);
		g_pEventManager->AddListener(this, "round_freeze_end", false);
		//g_pEventManager->AddListener(this, "bomb_planted", false);
	}
};

class c_nadepoint {
public:
	c_nadepoint() {
		m_valid = false;
	}

	c_nadepoint(Vector start, Vector end, bool plane, bool valid, Vector normal, bool detonate) {
		m_start = start;
		m_end = end;
		m_plane = plane;
		m_valid = valid;
		m_normal = normal;
		m_detonate = detonate;
	}

	Vector m_start, m_end, m_normal;
	bool m_valid, m_plane, m_detonate;
};


class c_nade_prediction {
	std::array< c_nadepoint, 500 >	_points{ };
	bool		 _predicted = false;

	void predict(CUserCmd* ucmd);

public:
	bool detonated();
	void trace(CUserCmd* ucmd);
	void draw();
};

extern c_nade_prediction g_nadepred;

struct StartSoundParams_t // apparently outdated thank sen66 for pointing it out.
{
	bool			staticsound;
	int			userdata;
	int			soundsource;
	int			entchannel;
	//CSfxTable* pSfx;
	Vector		origin;
	Vector		direction;
	bool			bUpdatePositions;
	float			fvol;
	//soundlevel_t	soundlevel;
	int			flags;
	int			pitch;
	bool			fromserver;
	float			delay;
	int			speakerentity;
	bool			suppressrecording;
	int			initialStreamPosition;
};

extern Event g_Event;
class IMatRenderContext;
class CBoneBitList;
class VMTHook;
class ShadowVTManager;
namespace HookManager
{
	inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }

	namespace EmitSound {
		using fn = void(__fastcall*)(void* ecx, void* edx, void* filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const vec3_t* pOrigin, const vec3_t* pDirection, vec3_t* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, StartSoundParams_t& params);
		void __fastcall hook(void* ecx, void* edx, void* filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const vec3_t* pOrigin, const vec3_t* pDirection, vec3_t* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, StartSoundParams_t& params);
	};
	namespace SceneEnd {
		using fn = void(__fastcall*)(void*, void* z);
		void     __fastcall  hook(void* ecx, void* edx);
	};
	namespace DrawModelExecute {
		using fn = void(__thiscall*)(void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
		void     __fastcall  hook(void* ecx, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& render_info, matrix3x4_t* matrix);
	};
	namespace PaintTraverse {
		using fn = void(__thiscall*)(PVOID, unsigned int, bool, bool);
		void		__fastcall  hook(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
	};
	namespace LockCursor {
		using fn = void(__fastcall*)(ISurface*, void*);
		void     __fastcall  hook(ISurface*, void*);
	};
	namespace OverrideView {
		using fn = void(__fastcall*)(void*, void*, CViewSetup*);
		void     __fastcall  hook(void* ecx, void* edx, CViewSetup* pSetup);
	};
	namespace GetViewmodelFov {
		using fn = float(__stdcall*)();
		float	__stdcall  hook();
	};
	namespace WriteDeltaBuff {
		using fn = bool(__thiscall*)(void*, int, void*, int, int, bool);
		bool __fastcall hook(void* ecx, void*, int slot, bf_write* buf, int from, int to, bool isnewcommand);
	};
	namespace FrameStageNotify {
		using fn = void(__thiscall*)(void*, ClientFrameStage_t);
		void     __fastcall  hook(void* ecx, void* edx, ClientFrameStage_t curStage);
	};
	namespace CreateMove {
		using fn = bool(__thiscall*)(IClientMode*, float, CUserCmd*);
		bool __stdcall hook(float input_sample_frametime, CUserCmd* cmd);
	};
	namespace NewHookedCreateMove {
		using fn = void(__thiscall*)(IBaseClientDLL*, int, float, bool);
		void __stdcall  hook(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket);
	};
	namespace SendDatagram {
		using fn = int(__fastcall*)(NetChannel*, void*, void*);
		int __fastcall  hook(NetChannel* network, void* edx, void* datagram);
	};
	namespace hkGCSendMessage {
		using GCRetrieveMessage = EGCResultxd(__thiscall*)(void*, uint32_t unMsgType, const void* pubData, uint32_t cubData);
		EGCResultxd __fastcall  hook(void* ecx, void*, uint32_t unMsgType, const void* pubData, uint32_t cubData);
	};
	namespace hkGCRetrieveMessage {
		using GCSendMessage = EGCResultxd(__thiscall*)(void*, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize);
		EGCResultxd __fastcall hook(void* ecx, void*, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize);
	};
}
class Hooks
{
public:
	// Initialization setup, called on injection
	static void Init();
	static void Restore();
	static void HookPlayers();
	static void SetClanTag(const char* tag, const char* name);

	/*---------------------------------------------*/
	/*-------------Hooked functions----------------*/
	/*---------------------------------------------*/

	void MessageSend();
	static void     __fastcall  DoExtraBonesProcessing(void* ECX, void* EDX, void* unkn1, void* unkn2, void* unkn3, void* unkn4, CBoneBitList& unkn5, void* unkn6);
	static LRESULT  __stdcall   WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND                           hCSGOWindow = nullptr; // CSGO window handle
		/*---------------------------------------------*/
	/*-------------Hook prototypes-----------------*/
	/*---------------------------------------------*/

	typedef void(__thiscall* ExtraBoneProcess_t) (void*, void*, void*, void*, void*, CBoneBitList&, void*);
	using DoPostScreenEffects_t = void(__thiscall*)(void* thisptr, CViewSetup* setup);
	std::unique_ptr<VMTHook> pClientHook;
private:
	/*---------------------------------------------*/
	/*-------------VMT Hook pointers---------------*/
	/*---------------------------------------------*/

	std::unique_ptr<ShadowVTManager> pPlayerHook[64];

private:
	WNDPROC                        pOriginalWNDProc = nullptr; // Original CSGO window proc

	template<class Type> // GLAD
	Type HookManual(uintptr_t* instance, int offset, Type hook)
	{
		DWORD Dummy;
		Type fnOld = (Type)instance[offset];
		VirtualProtect((void*)(instance + offset * 0x4), 0x4, PAGE_EXECUTE_READWRITE, &Dummy);
		instance[offset] = (uintptr_t)hook;
		VirtualProtect((void*)(instance + offset * 0x4), 0x4, Dummy, &Dummy);
		return fnOld;
	}

	//bool __fastcall hkdWriteUsercmdDeltaToBuffer(void* ecx, void*, int slot, bf_write* buf, int from, int to, bool isnewcommand);

};

using send_net_msg_fn = int(__thiscall*)(void*, INetMessage&, bool, bool);
extern send_net_msg_fn orig_send_net_msg;
static bool  __fastcall send_net_msg(INetChannelInfo* pNetChan, void* edx, INetMessage& msg, bool bForceReliable, bool bVoice);
extern Hooks g_Hooks;



class trace_info
{
public:

	trace_info(Vector starts, Vector positions, float times, int userids)
	{
		this->start = starts;
		this->position = positions;
		this->time = times;
		this->userid = userids;
		this->pPlayer = nullptr;
	}

	C_BaseEntity* pPlayer;
	Vector position;
	Vector start;
	float time;
	int userid;
};

extern std::vector<trace_info> trace_logs;

class VMTHook
{
public:
	VMTHook(void* ppClass)
	{
		this->ppBaseClass = static_cast<std::uintptr_t**>(ppClass);

		// loop through all valid class indexes. When it will hit invalid (not existing) it will end the loop
		while (static_cast<std::uintptr_t*>(*this->ppBaseClass)[this->indexCount])
			++this->indexCount;

		const std::size_t kSizeTable = this->indexCount * sizeof(std::uintptr_t);

		this->pOriginalVMT = *this->ppBaseClass;
		this->pNewVMT = std::make_unique<std::uintptr_t[]>(this->indexCount);

		// copy original vtable to our local copy of it
		std::memcpy(this->pNewVMT.get(), this->pOriginalVMT, kSizeTable);

		// replace original class with our local copy
		*this->ppBaseClass = this->pNewVMT.get();
	};
	~VMTHook() { *this->ppBaseClass = this->pOriginalVMT; };

	template<class Type>
	Type GetOriginal(const std::size_t index)
	{
		return reinterpret_cast<Type>(this->pOriginalVMT[index]);
	};
	HRESULT Hook(const std::size_t index, void* fnNew)
	{
		if (index > this->indexCount)   // check if given index is valid
			return E_INVALIDARG;

		this->pNewVMT[index] = reinterpret_cast<std::uintptr_t>(fnNew);
		return S_OK;
	};
	HRESULT Unhook(const std::size_t index)
	{
		if (index > this->indexCount)
			return E_INVALIDARG;

		this->pNewVMT[index] = this->pOriginalVMT[index];
		return S_OK;
	};

private:
	std::unique_ptr<std::uintptr_t[]> pNewVMT = nullptr;    // Actual used vtable
	std::uintptr_t** ppBaseClass = nullptr;             // Saved pointer to original class
	std::uintptr_t* pOriginalVMT = nullptr;             // Saved original pointer to the VMT
	std::size_t                       indexCount = 0;                     // Count of indexes inside out f-ction
};


class ProtectGuard
{
public:

	ProtectGuard(void* base, uint32_t len, uint32_t protect)
	{
		this->base = base;
		this->len = len;

		if (!VirtualProtect(base, len, protect, (PDWORD)&old_protect))
			throw std::runtime_error("Failed to protect region!");
	}

	~ProtectGuard()
	{
		VirtualProtect(base, len, old_protect, (PDWORD)&old_protect);
	}

private:

	void* base;
	uint32_t len;
	uint32_t old_protect;
};

class ShadowVTManager // GLAD
{

public:

	ShadowVTManager() : class_base(nullptr), method_count(0), shadow_vtable(nullptr), original_vtable(nullptr) {}
	ShadowVTManager(void* base) : class_base(base), method_count(0), shadow_vtable(nullptr), original_vtable(nullptr) {}
	~ShadowVTManager()
	{
		RestoreTable();

		delete[] shadow_vtable;
	}

	inline void Setup(void* base = nullptr)
	{
		if (base != nullptr)
			class_base = base;

		if (class_base == nullptr)
			return;

		original_vtable = *(uintptr_t**)class_base;
		method_count = GetMethodCount(original_vtable);

		if (method_count == 0)
			return;

		shadow_vtable = new uintptr_t[method_count + 1]();

		shadow_vtable[0] = original_vtable[-1];
		std::memcpy(&shadow_vtable[1], original_vtable, method_count * sizeof(uintptr_t));

		try
		{
			auto guard = ProtectGuard{ class_base, sizeof(uintptr_t), PAGE_READWRITE };
			*(uintptr_t**)class_base = &shadow_vtable[1];
		}
		catch (...)
		{
			delete[] shadow_vtable;
		}
	}

	template<typename T>
	inline void Hook(uint32_t index, T method)
	{
		assert(index < method_count);
		shadow_vtable[index + 1] = reinterpret_cast<uintptr_t>(method);
	}
	inline void Unhook(uint32_t index)
	{
		assert(index < method_count);
		shadow_vtable[index + 1] = original_vtable[index];
	}

	template<typename T>
	inline T GetOriginal(uint32_t index)
	{
		return (T)original_vtable[index];
	}

	inline void RestoreTable()
	{
		try
		{
			if (original_vtable != nullptr)
			{
				auto guard = ProtectGuard{ class_base, sizeof(uintptr_t), PAGE_READWRITE };
				*(uintptr_t**)class_base = original_vtable;
				original_vtable = nullptr;
			}
		}
		catch (...) {}
	}

private:

	inline uint32_t GetMethodCount(uintptr_t* vtable_start)
	{
		uint32_t len = -1;

		do ++len; while (vtable_start[len]);

		return len;
	}

	void* class_base;
	uint32_t method_count;
	uintptr_t* shadow_vtable;
	uintptr_t* original_vtable;
};

namespace detail
{
	class protect_guard
	{
	public:
		protect_guard(void* base, size_t len, std::uint32_t flags)
		{
			_base = base;
			_length = len;
			if (!VirtualProtect(base, len, flags, (PDWORD)&_old))
				throw std::runtime_error("Failed to protect region.");
		}
		~protect_guard()
		{
			VirtualProtect(_base, _length, _old, (PDWORD)&_old);
		}

	private:
		void* _base;
		size_t        _length;
		std::uint32_t _old;
	};
}

class vfunc_hook
{
public:
	uintptr_t* search_free_data_page(const char* module_name, const std::size_t vmt_size);
	vfunc_hook();
	vfunc_hook(void* base);
	~vfunc_hook();

	bool setup(void* base, const char* moduleName = nullptr);
	template<typename T>
	void hook_index(int index, T fun)
	{
		assert(index >= 0 && index <= (int)vftbl_len);
		new_vftb1[index + 1] = reinterpret_cast<std::uintptr_t>(fun);
	}
	void unhook_index(int index)
	{
		new_vftb1[index] = old_vftbl[index];
	}
	void unhook_all()
	{
		try {
			if (old_vftbl != nullptr) {
				auto guard = detail::protect_guard{ class_base, sizeof(std::uintptr_t), PAGE_READWRITE };
				*(std::uintptr_t**)class_base = old_vftbl;
				old_vftbl = nullptr;
			}
		}
		catch (...) {
		}
	}

	template<typename T>
	T get_original(int index)
	{
		return (T)old_vftbl[index];
	}

private:

	static inline std::size_t estimate_vftbl_length(std::uintptr_t* vftbl_start);

	void* class_base;
	std::size_t     vftbl_len;
	std::uintptr_t* new_vftb1;
	std::uintptr_t* old_vftbl;
	LPCVOID         search_base = nullptr;
	bool wasAllocated = false;
};

