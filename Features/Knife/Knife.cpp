#include "Knife.h"
#include "../..//SDK/SDKinc.h"
#include "../../SDK/Recv.h"
#include "../../Utils/xor.h"
#include "../../SDK/IBaseClientDll.h"



typedef void(*RecvVarProxyFn)(const CRecvProxyData* pData, void* pStruct, void* pOut);

#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1
using namespace std;
// Helper function to generate a random sequence number.
inline int RandomSequence(int low, int high) {
	return (rand() % (high - low + 1) + low);
}

#define	LIFE_ALIVE 0

#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);



RecvVarProxyFn fnSequenceProxyFn = nullptr;

RecvVarProxyFn oRecvnModelIndex;
// xd
int iBayonet;
int iButterfly;
int iFlip;
int iGut;
int iKarambit;
int iM9Bayonet;
int iHuntsman;
int iFalchion;
int iDagger;
int iBowie;
int iGunGame;


void Hooked_RecvProxy_Viewmodel(CRecvProxyData* pData, void* pStruct, void* pOut)
{
	// Get the knife view model id's
	int default_t = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_default_t.mdl"));
	int default_ct = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_default_ct.mdl"));
	iBayonet = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_bayonet.mdl"));
	iButterfly = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_butterfly.mdl"));
	iFlip = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_flip.mdl"));
	iGut = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_gut.mdl"));
	iKarambit = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_karam.mdl"));
	iM9Bayonet = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_m9_bay.mdl"));
	iHuntsman = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_tactical.mdl"));
	iFalchion = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_falchion_advanced.mdl"));
	iDagger = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_push.mdl"));
	iBowie = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_survival_bowie.mdl"));
	iGunGame = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_gg.mdl"));

	int Navaja = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_gypsy_jackknife.mdl"));
	int Stiletto = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_stiletto.mdl"));
	int Ursus = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_ursus.mdl"));
	int Talon = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_widowmaker.mdl"));
	int Classic = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_css.mdl"));
	int Cord = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_cord.mdl"));
	int Canis = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_canis.mdl"));
	int Skeleton = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_skeleton.mdl"));
	int Nomad = g_pModelInfo->GetModelIndex(("models/weapons/v_knife_outdoor.mdl"));



	if (g_Menu.Config.knivchanger && g::pLocalEntity)
	{

		if (g::pLocalEntity->IsAlive() && (
			pData->m_Value.m_Int == default_t ||
			pData->m_Value.m_Int == default_ct ||
			pData->m_Value.m_Int == iBayonet ||
			pData->m_Value.m_Int == iFlip ||
			pData->m_Value.m_Int == iGunGame ||
			pData->m_Value.m_Int == iGut ||
			pData->m_Value.m_Int == iKarambit ||
			pData->m_Value.m_Int == iM9Bayonet ||
			pData->m_Value.m_Int == iHuntsman ||
			pData->m_Value.m_Int == iBowie ||
			pData->m_Value.m_Int == iButterfly ||
			pData->m_Value.m_Int == iFalchion ||
			pData->m_Value.m_Int == iDagger ||
			pData->m_Value.m_Int == Navaja ||
			pData->m_Value.m_Int == Stiletto ||
			pData->m_Value.m_Int == Ursus ||
			pData->m_Value.m_Int == Classic ||
			pData->m_Value.m_Int == Cord ||
			pData->m_Value.m_Int == Canis ||
			pData->m_Value.m_Int == Skeleton ||
			pData->m_Value.m_Int == Nomad ||
			pData->m_Value.m_Int == Talon))
		{
			// Set whatever knife we want
			if (g_Menu.Config.Knife == 0)
				pData->m_Value.m_Int = iBayonet;
			else if (g_Menu.Config.Knife == 9)
				pData->m_Value.m_Int = iBowie;
			else if (g_Menu.Config.Knife == 6)
				pData->m_Value.m_Int = iButterfly;
			else if (g_Menu.Config.Knife == 7)
				pData->m_Value.m_Int = iFalchion;
			else if (g_Menu.Config.Knife == 1)
				pData->m_Value.m_Int = iFlip;
			else if (g_Menu.Config.Knife == 2)
				pData->m_Value.m_Int = iGut;
			else if (g_Menu.Config.Knife == 5)
				pData->m_Value.m_Int = iHuntsman;
			else if (g_Menu.Config.Knife == 3)
				pData->m_Value.m_Int = iKarambit;
			else if (g_Menu.Config.Knife == 4)
				pData->m_Value.m_Int = iM9Bayonet;
			else if (g_Menu.Config.Knife == 8)
				pData->m_Value.m_Int = iDagger;
			else if (g_Menu.Config.Knife == 10)
				pData->m_Value.m_Int = Navaja;
			else if (g_Menu.Config.Knife == 11)
				pData->m_Value.m_Int = Stiletto;
			else if (g_Menu.Config.Knife == 12)
				pData->m_Value.m_Int = Ursus;
			else if (g_Menu.Config.Knife == 13)
				pData->m_Value.m_Int = Talon;
			else if (g_Menu.Config.Knife == 14)
				pData->m_Value.m_Int = Classic;
			else if (g_Menu.Config.Knife == 15)
				pData->m_Value.m_Int = Cord;
			else if (g_Menu.Config.Knife == 16)
				pData->m_Value.m_Int = Canis;
			else if (g_Menu.Config.Knife == 17)
				pData->m_Value.m_Int = Skeleton;
			else if (g_Menu.Config.Knife == 18)
				pData->m_Value.m_Int = Nomad;
		}
	}

	// Carry on the to original proxy
	oRecvnModelIndex(pData, pStruct, pOut);
}

void SetViewModelSequence2(const CRecvProxyData* pDataConst, void* pStruct, void* pOut)
{

	// Make the incoming data editable.
	CRecvProxyData* pData = const_cast<CRecvProxyData*>(pDataConst);

	// Confirm that we are replacing our view model and not someone elses.
	C_BaseViewModel* pViewModel = static_cast<C_BaseViewModel*>(pStruct);

	if (pViewModel)
	{
		C_BaseEntity* pOwner = static_cast<C_BaseEntity*>(g_pEntityList->GetClientEntity(pViewModel->GetOwner() & 0xFFF));

		// Compare the owner entity of this view model to the local player entity.
		if (pOwner && pOwner->EntIndex() == g_pEngine->GetLocalPlayer())
		{
			// Get the filename of the current view model.
			std::string szModel = g_pModelInfo->GetModelNamevoid(g_pModelInfo->GetModel(pViewModel->GetModelIndex()));

			// Store the current sequence.
			int m_nSequence = pData->m_Value.m_Int;
			if (szModel == ("models/weapons/v_knife_butterfly.mdl"))
			{
				// Fix animations for the Butterfly Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
				}
			}
			else if (szModel == ("models/weapons/v_knife_falchion_advanced.mdl"))
			{
				// Fix animations for the Falchion Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
					break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence--;
				}
			}
			else if (szModel == ("models/weapons/v_knife_push.mdl"))
			{
				// Fix animations for the Shadow Daggers.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
					break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
					break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence += 3; break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence += 2;
				}
			}
			else if (szModel == ("models/weapons/v_knife_survival_bowie.mdl"))
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_BOWIE_IDLE1;
					break;
				default:
					m_nSequence--;
				}
			}
			else if (szModel == ("models/weapons/v_knife_ursus.mdl"))
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
					break;
				}
			}
			else if (szModel == ("models/weapons/v_knife_cord.mdl"))
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
					break;
				}
			}
			else if (szModel == ("models/weapons/v_knife_canis.mdl"))
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
					break;
				}
			}
			else if (szModel == ("models/weapons/v_knife_skeleton.mdl"))
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
					break;
				}
			}
			else if (szModel == ("models/weapons/v_knife_outdoor.mdl"))
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
					break;
				}
			}
			else if (szModel == ("models/weapons/v_knife_stiletto.mdl"))
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(12, 13);
					break;
				}
			}
			else if (szModel == ("models/weapons/v_knife_widowmaker.mdl"))
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(14, 15);
					break;
				}
			}
			pData->m_Value.m_Int = m_nSequence;
		}
	}
	fnSequenceProxyFn(pData, pStruct, pOut);
}
void AnimationFixHook()
{
	for (ClientClass* pClass = g_pClientDll->GetAllClasses(); pClass; pClass = pClass->pNext) {
		if (!strcmp(pClass->pNetworkName, ("CBaseViewModel"))) {
			// Search for the 'm_nModelIndex' property.
			RecvTable* pClassTable = pClass->pRecvTable;

			for (int nIndex = 0; nIndex < pClassTable->nProps; nIndex++) {
				RecvProp* pProp = &pClassTable->pProps[nIndex];

				if (!pProp || strcmp(pProp->pVarName, ("m_nSequence")))
					continue;

				// Store the original proxy function.
				fnSequenceProxyFn = static_cast<RecvVarProxyFn>(pProp->ProxyFn);

				// Replace the proxy function with our sequence changer.
				pProp->ProxyFn = static_cast<RecvVarProxyFn>(SetViewModelSequence2);

				break;
			}

			break;
		}
	}
}



void knifeload()
{
	AnimationFixHook();
	ClientClass* pClass = g_pClientDll->GetAllClasses();
	while (pClass)
	{
		const char* pszName = pClass->pRecvTable->pNetTableName;
		if (!strcmp(pszName, ("DT_SmokeGrenadeProjectile")))
		{
			for (int i = 0; i < pClass->pRecvTable->nProps; i++)
			{
				RecvProp* pProp = &(pClass->pRecvTable->pProps[i]);
				const char* name = pProp->pVarName;
				if (!strcmp(name, ("m_bDidSmokeEffect")))
				{
					//fnNoSmoke = (RecvVarProxyFn)pProp->m_ProxyFn;
					//pProp->m_ProxyFn = NoSmoke;
				}

			}
		}
		else if (!strcmp(pszName, ("DT_BaseViewModel")))
		{
			for (int i = 0; i < pClass->pRecvTable->nProps; i++)
			{
				RecvProp* pProp = &(pClass->pRecvTable->pProps[i]);
				const char* name = pProp->pVarName;
				if (!strcmp(name, ("m_nModelIndex")))
				{
					oRecvnModelIndex = (RecvVarProxyFn)pProp->ProxyFn;
					pProp->ProxyFn = (RecvVarProxyFn)Hooked_RecvProxy_Viewmodel;
				}
			}
		}
		pClass = pClass->pNext;
	}
}

std::uint8_t* pattern_scan(void* module, const char* signature) {
	static auto pattern_to_byte = [](const char* pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dos_headers = (PIMAGE_DOS_HEADER)module;
	auto nt_headers = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dos_headers->e_lfanew);

	auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	auto pattern_bytes = pattern_to_byte(signature);
	auto scan_bytes = reinterpret_cast<std::uint8_t*>(module);

	auto s = pattern_bytes.size();
	auto d = pattern_bytes.data();

	for (auto i = 0ul; i < size_of_image - s; ++i) {
		bool found = true;
		for (auto j = 0ul; j < s; ++j) {
			if (scan_bytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}
		if (found) {
			return &scan_bytes[i];
		}
	}
	return nullptr;
}

struct hud_weapons_t {
	std::int32_t* get_weapon_count() {
		return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
	}
};
template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(Utils::FindSignature(("client.dll"), ("B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08")) + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(Utils::FindSignature(("client.dll"), ("55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39")));
	return (T*)find_hud_element(pThis, name);
}

typedef void(*CL_FullUpdate_t) (void);
CL_FullUpdate_t CL_FullUpdate = nullptr;

void KnifeApplyCallbk()
{
	static auto clear_hud_weapon_icon_fn = reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(Utils::FindSignature((("client.dll")), (("55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C"))));

	auto element = FindHudElement<std::uintptr_t*>((("CCSGO_HudWeaponSelection")));

	auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xA0);
	if (hud_weapons == nullptr)
		return;

	if (!*hud_weapons->get_weapon_count())
		return;

	for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
		i = clear_hud_weapon_icon_fn(hud_weapons, i);

	g_pClientState->force_full_update();
}