#pragma once
#include "Definitions.h"
#include "IClientUnknown.h"
#include "IClientEntityList.h"
#include "CInput.h"
#include "..\Utils\Utils.h"
#include "..\Utils\NetvarManager.h"
#include "CGlobalVarsBase.h"

// class predefinition
class C_BaseCombatWeapon;
#define NETVAR(type, name, table, netvar)                           \
    type& name##() const {                                          \
        static int _##name = g_pNetvars->GetOffset(table, netvar);     \
        return *(type*)((uintptr_t)this + _##name);                 \
	}
#define PNETVAR(type, name, table, netvar)                           \
    type* name##() const {                                          \
        static int _##name = g_pNetvars->GetOffset(table, netvar);     \
        return (type*)((uintptr_t)this + _##name);                 \
	}
class C_AnimState
{
public:
	char pad[3];
	char bUnknown; //0x4
	char pad2[91];
	void* pBaseEntity; //0x60
	void* pActiveWeapon; //0x64
	void* pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flEyePitch; //0x74
	float m_flEyeYaw; //0x78
	float m_flPitch; //0x7C
	float m_flGoalFeetYaw; //0x80
	float m_flCurrentFeetYaw; //0x84
	float m_flCurrentTorsoYaw; //0x88
	float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
	float m_flLeanAmount; //0x90
	char pad4[4]; //NaN
	float m_flFeetCycle; //0x98 0 to 1
	float m_flFeetYawRate; //0x9C 0 to 1
	float m_fUnknown2;
	float m_fDuckAmount; //0xA4
	float m_fLandingDuckAdditiveSomething; //0xA8
	float m_fUnknown3; //0xAC
	Vector m_vOrigin; //0xB0, 0xB4, 0xB8
	Vector m_vLastOrigin; //0xBC, 0xC0, 0xC4
	float m_vVelocityX; //0xC8
	float m_vVelocityY; //0xCC
	char pad5[4];
	float m_flUnknownFloat1; //0xD4 Affected by movement and direction
	char pad6[8];
	float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
	float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
	float m_unknown; //0xE8
	float speed_2d; //0xEC
	float flUpVelocity; //0xF0
	float m_flSpeedNormalized; //0xF4 //from 0 to 1
	float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
	float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	unsigned char m_bOnGround; //0x108
	unsigned char m_bInHitGroundAnimation; //0x109
	char pad7[10];
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
	float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	char _0x0120[1];
	BYTE N01EE3F18; //0x0121 
	char _0x0122[2];
	float m_flUnknownFraction; //0x0124 
	char pad10[517];
	__int8 N01EE3F9B; //0x032D 
	char _0x032E[2];
	float min_yaw; //0x0330 
	float max_yaw; //0x0334 
	char _0x0338[385];
};
class AnimationLayer {
public:
	bool m_bClientBlend;		 //0x0000
	float m_flBlendIn;			 //0x0004
	void* m_pStudioHdr;			 //0x0008
	int m_nDispatchSequence;     //0x000C
	int m_nDispatchSequence_2;   //0x0010
	uint32_t m_nOrder;           //0x0014
	uint32_t m_nSequence;        //0x0018
	float_t m_flPrevCycle;       //0x001C
	float_t m_flWeight;          //0x0020
	float_t m_flWeightDeltaRate; //0x0024
	float_t m_flPlaybackRate;    //0x0028
	float_t m_flCycle;           //0x002C
	void* m_pOwner;              //0x0030
	char pad_0038[4];            //0x0034
}; //Size: 0x0038
struct datamap_t;
class typedescription_t;
enum
{
	TD_OFFSET_NORMAL = 0,
	TD_OFFSET_PACKED = 1,

	// Must be last
	TD_OFFSET_COUNT,
};
class typedescription_t
{
public:
	int32_t fieldType; //0x0000
	char* fieldName; //0x0004
	int fieldOffset[TD_OFFSET_COUNT]; //0x0008
	int16_t fieldSize_UNKNWN; //0x0010
	int16_t flags_UNKWN; //0x0012
	char pad_0014[12]; //0x0014
	datamap_t* td; //0x0020
	char pad_0024[24]; //0x0024
}; //Size: 0x003C
struct datamap_t
{
	typedescription_t* dataDesc;
	int                    dataNumFields;
	char const* dataClassName;
	datamap_t* baseMap;

	bool                chains_validated;
	// Have the "packed" offsets been computed
	bool                packed_offsets_computed;
	int                    packed_size;
};
class i_game_event {
public:
	virtual ~i_game_event() = 0;
	virtual const char* get_name() const = 0;

	virtual bool            is_reliable() const = 0;
	virtual bool            is_local() const = 0;
	virtual bool            is_empty(const char* keyName = nullptr) = 0;

	virtual bool            get_bool(const char* keyName = nullptr, bool defaultValue = false) = 0;
	virtual int             get_int(const char* keyName = nullptr, int defaultValue = 0) = 0;
	virtual uint64_t        get_uint_64(const char* keyName = nullptr, unsigned long defaultValue = 0) = 0;
	virtual float           get_float(const char* keyName = nullptr, float defaultValue = 0.0f) = 0;
	virtual const char* get_string(const char* keyName = nullptr, const char* defaultValue = "") = 0;
	virtual const wchar_t* get_wstring(const char* keyName, const wchar_t* defaultValue = L"") = 0;

	virtual void            set_bool(const char* keyName, bool value) = 0;
	virtual void            set_int(const char* keyName, int value) = 0;
	virtual void            set_uint_64(const char* keyName, unsigned long value) = 0;
	virtual void            set_float(const char* keyName, float value) = 0;
	virtual void            set_string(const char* keyName, const char* value) = 0;
	virtual void            set_wstring(const char* keyName, const wchar_t* value) = 0;
};
class CCSGameRules
{
public:
	bool IsValveDS()
	{
		if (!this)
			return false;
		return *(bool*)((uintptr_t)this + 0x7C); //0x75 Fix: 20210922
	}

	bool IsBombDropped()
	{
		if (!this)
			return false;
		return *(bool*)((uintptr_t)this + 0x9A4); //0x99C Fix: 20210922
	}

	bool IsBombPlanted()
	{
		if (!this)
			return false;
		return *(bool*)((uintptr_t)this + 0x9A5); //0x99D Fix: 20210922
	}

	bool IsFreezeTime()
	{
		if (!this)
			return false;
		return *(bool*)((uintptr_t)this + 0x20);
	}
};
class viewmodel_t;
class C_BaseEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable
{
private:
	template<class T>
	T GetPointer(const int offset)
	{
		return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}

	template <typename T>
	T& SetValue(uintptr_t offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset);
	}
	template< typename T >
	T getvfunc(void* vTable, int iIndex)
	{
		return (*(T**)vTable)[iIndex];
	}
	unsigned int FindInDataMap(datamap_t* pMap, const char* name)
	{
		while (pMap)
		{
			for (int i = 0; i < pMap->dataNumFields; i++)
			{
				if (pMap->dataDesc[i].fieldName == NULL)
					continue;

				if (strcmp(name, pMap->dataDesc[i].fieldName) == 0)
					return pMap->dataDesc[i].fieldOffset[TD_OFFSET_NORMAL];

				if (pMap->dataDesc[i].fieldType == 10)
				{
					if (pMap->dataDesc[i].td)
					{
						unsigned int offset;

						if ((offset = FindInDataMap(pMap->dataDesc[i].td, name)) != 0)
							return offset;
					}
				}
			}
			pMap = pMap->baseMap;
		}

		return 0;
	}
public:
	// To get value from the pointer itself
	template<class T>
	T GetValue(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	datamap_t* GetPredDescMap()
	{
		typedef datamap_t* (__thiscall* o_GetPredDescMap)(void*);
		return getvfunc<o_GetPredDescMap>(this, 17)(this);
	}
	NETVAR(HANDLE, m_hWeaponWorldModel_h, "DT_BaseCombatWeapon", "m_hWeaponWorldModel"); //knife
	NETVAR(bool, hasdefuser, "DT_CSPlayer", "m_bHasDefuser");
	NETVAR(bool, spotted, "DT_BaseEntity", "m_bSpotted");
	NETVAR(CBaseHandle, m_hWeaponWorldModel_c, "DT_BaseCombatWeapon", "m_hWeaponWorldModel"); //knife
	NETVAR(int, InBombZone, "DT_CSPlayer", "m_bInBombZone");
	NETVAR(float, m_flC4Blow, "DT_PlantedC4", "m_flC4Blow");
	NETVAR(bool, m_hBombDefuser, "DT_PlantedC4", "m_hBombDefuser");
	NETVAR(bool, m_bBombDefused, "DT_PlantedC4", "m_bBombDefused");
	NETVAR(CHandle<viewmodel_t>, m_hViewModel, "DT_BasePlayer", "m_hViewModel[0]");
	NETVAR(int, m_hMyWearables, "DT_CSPlayer", "m_hMyWearables");
	NETVAR(float, m_fThrowTime, "DT_BaseCSGrenade", "m_fThrowTime");
	PNETVAR(CBaseHandle, GetWearables, "DT_BaseCombatCharacter", "m_hMyWearables");
	NETVAR(float, m_fDuckSpeed, "DT_BasePlayer", "m_flDuckSpeed");
	NETVAR(int, GetObserverMode, "DT_BasePlayer", "m_iObserverMode");
	NETVAR(int, GetTeam, "DT_BaseEntity", "m_iTeamNum");
	NETVAR(int, m_fFlags, "DT_BasePlayer", "m_fFlags");
	NETVAR(float, GetLowerBodyYaw, "DT_CSPlayer", "m_flLowerBodyYawTarget");
	NETVAR(bool, GetHeavyArmor, "DT_CSPlayer", "m_bHasHeavyArmor");
	NETVAR(int, ArmorValue, "DT_CSPlayer", "m_ArmorValue");
	NETVAR(bool, HasHelmet, "DT_CSPlayer", "m_bHasHelmet");
	NETVAR(bool, GetLifeState, "DT_BasePlayer", "m_lifeState");
	NETVAR(bool, IsSpotted, "DT_BaseEntity", "m_bSpotted");
	NETVAR(bool, IsScoped, "DT_CSPlayer", "m_bIsScoped");
	NETVAR(int, GetHealth, "DT_BasePlayer", "m_iHealth");
	NETVAR(bool, IsImmune, "DT_CSPlayer", "m_bGunGameImmunity");
	NETVAR(int, GetMoney, "DT_CSPlayer", "m_iAccountID");
	NETVAR(CHandle<C_BaseEntity>, m_hObserverTarget, "DT_BasePlayer", "m_hObserverTarget"); //Broken... but why
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////Skinchanger/////////////////////////////////////////////////
	int GetActiveWeaponIndex()
	{
		static int m_hActiveWeapon = g_pNetvars->GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
		return *reinterpret_cast<int*>(uintptr_t(this) + m_hActiveWeapon) & 0xFFF; //m_hActiveWeapon //Fix: 20211128
	}
	void SetItemDefinitionIndex(ItemDefinitionIndex value) //knife
	{
		static int m_iItemDefinitionIndex = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
		*reinterpret_cast<ItemDefinitionIndex*>(uintptr_t(this) + m_iItemDefinitionIndex) = value;//Fix: 20211120
	}
	void GetAccountId(int value)
	{
		static int m_iAccountID = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iAccountID");
		*reinterpret_cast<int*>(uintptr_t(this) + m_iAccountID) = value;//Fix: 20211120
	}
	void ItemIDHigh(int value)
	{
		static int m_iItemIDHigh = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemIDHigh");
		*reinterpret_cast<int*>(uintptr_t(this) + m_iItemIDHigh) = value;//Fix: 20211120
	}
	void SetEntityQuality(int value) //knife
	{
		static int m_iEntityQuality = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iEntityQuality");
		*reinterpret_cast<int*>(uintptr_t(this) + m_iEntityQuality) = value;//Fix: 20211120
	}
	void FallbackSeed(int value)
	{
		static int m_nFallbackSeed = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_nFallbackSeed");
		*reinterpret_cast<int*>(uintptr_t(this) + m_nFallbackSeed) = value;//Fix: 20211120
	}
	void FallbackWear(int value)
	{
		static int m_flFallbackWear = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_flFallbackWear");
		*reinterpret_cast<int*>(uintptr_t(this) + m_flFallbackWear) = value;//Fix: 20211120
	}
	void FallbackStartTrak(int value)
	{
		static int m_nFallbackStatTrak = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_nFallbackStatTrak");
		*reinterpret_cast<int*>(uintptr_t(this) + m_nFallbackStatTrak) = value;//Fix: 20211120
	}
	void OwnerXuidLow(int value)
	{
		static int m_OriginalOwnerXuidLow = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
		*reinterpret_cast<int*>(uintptr_t(this) + m_OriginalOwnerXuidLow) = value;//Fix: 20211120
	}
	void OwnerXuidHigh(int value)
	{
		static int m_OriginalOwnerXuidHigh = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
		*reinterpret_cast<int*>(uintptr_t(this) + m_OriginalOwnerXuidHigh) = value;//Fix: 20211120
	}
	void SetWeaponModelIndex(int value) //knife
	{
		static int m_nModelIndex = g_pNetvars->GetOffset("DT_BaseEntity", "m_nModelIndex");
		*reinterpret_cast<int*>(uintptr_t(this) + m_nModelIndex) = value; //Fix: 20211120
	}
	void SetViewModelIndex(int value) //knife
	{
		static int m_iViewModelIndex = g_pNetvars->GetOffset("DT_BaseCombatWeapon", "m_iViewModelIndex");
		*reinterpret_cast<int*>(uintptr_t(this) + m_iViewModelIndex) = value; //Fix: 20211120
	}
	void SetFallbackPaintKit(int value)
	{
		static int m_nFallbackPaintKit = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_nFallbackPaintKit");
		*reinterpret_cast<int*>(uintptr_t(this) + m_nFallbackPaintKit) = value; //Fix: 20211120 - FIXME
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	EntityFlags GetFlags()
	{
		static int m_fFlags = g_pNetvars->GetOffset("DT_BasePlayer", "m_fFlags");
		return GetValue<EntityFlags>(m_fFlags);
	}
	CBaseHandle* m_hMyWeapons()
	{
		static int get_weapons = g_pNetvars->GetOffset("DT_BaseCombatCharacter", "m_hMyWeapons");
		return (CBaseHandle*)((uintptr_t)this + get_weapons); //0x2DF8 Fix: 20210922 Trying to fix: 20210925
	}
	int* IntGetItemDefinitionIndex()
	{
		static int m_iItemDefinitionIndex = g_pNetvars->GetOffset(SteamStrings("DT_BaseAttributableItem"), SteamStrings("m_AttributeManager"), SteamStrings("m_Item"), SteamStrings("m_iItemDefinitionIndex"));
		return GetValue<int*>(m_iItemDefinitionIndex);//Trying to fix: 20210925
	}

	Vector GetVelocity()
	{
		static int m_vecVelocity = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_vecVelocity[0]");
		return GetValue<Vector>(m_vecVelocity);
	}
	Vector GetViewPunch()
	{
		static int m_viewPunchAngle = g_pNetvars->GetOffset("DT_BaseEntity", "m_viewPunchAngle");
		return GetValue<Vector>(m_viewPunchAngle);
	}
	Vector GetViewOffset()
	{
		static int m_vecViewOffset = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_vecViewOffset[0]");
		return GetValue<Vector>(m_vecViewOffset);
	}
	Vector GetOrigin()
	{
		static int m_vecOrigin = g_pNetvars->GetOffset("DT_BaseEntity", "m_vecOrigin");
		return GetValue<Vector>(m_vecOrigin);
	}
	Vector& GetEyeAngles()
	{
		static int m_angEyeAngles = g_pNetvars->GetOffset("DT_CSPlayer", "m_angEyeAngles");
		return GetValue<Vector>(m_angEyeAngles);
	}
	int GetSequence()
	{
		static int m_nSequence = g_pNetvars->GetOffset("DT_BaseAnimating", "m_nSequence");
		return GetValue<int>(m_nSequence);
	}
	bool& GetClientSideAnimation() {
		static int m_bClientSideAnimation = g_pNetvars->GetOffset("DT_BaseAnimating", "m_bClientSideAnimation");;
		return *reinterpret_cast<bool*>((DWORD)this + m_bClientSideAnimation);
	}
	float GetSimulationTime()
	{
		static int m_flSimulationTime = g_pNetvars->GetOffset("DT_BaseEntity", "m_flSimulationTime");
		return GetValue<float>(m_flSimulationTime);
	}
	// Something is not working here, so i need to use the standard netvar manager.
	Vector& GetAbsVelocity()
	{
		static unsigned int offset = FindInDataMap(GetPredDescMap(), "m_vecAbsVelocity");
		return *(Vector*)((uintptr_t)this + offset);
	}
	void SetVelocity(Vector velocity)
	{
		static int m_vecVelocity = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_vecVelocity[0]");
		*reinterpret_cast<Vector*>(uintptr_t(this) + m_vecVelocity) = velocity;
	}

	Vector GetAimPunchAngle()
	{
		return *reinterpret_cast<Vector*>(uintptr_t(this) + uintptr_t(0x303C)); //0x302C Fix: 20210922
	}

	Vector GetViewPunchAngle()
	{
		return *reinterpret_cast<Vector*>(uintptr_t(this) + uintptr_t(0x3030)); //0x3020 Fix: 20210922
	}

	Vector GetVecViewOffset()
	{
		return *reinterpret_cast<Vector*>(uintptr_t(this) + uintptr_t(0x108));
	}

	Vector GetEyePosition()
	{
		Vector ret;
		typedef void(__thiscall* OrigFn)(void*, Vector&);
		Utils::GetVFunc<OrigFn>(this, 169)(this, ret); //284 Fix: 20210922
		return ret;
	}

	inline Vector ExtrapolateTick(Vector p0, Vector v0)
	{
		return p0 + (v0 * g_pGlobalVars->intervalPerTick);
	}

	Vector GetPredicted(Vector p0)
	{
		return ExtrapolateTick(p0, this->GetVelocity());
	}
	bool SetupBones2(matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		auto backupval = *reinterpret_cast<uint8_t*>((uintptr_t)this + 0x274);

		*reinterpret_cast<uint8_t*>((uintptr_t)this + 0x274) = 0;
		bool setuped_bones = this->SetupBones(pBoneToWorldOut, nMaxBones, boneMask, currentTime);
		*reinterpret_cast<uint8_t*>((uintptr_t)this + 0x274) = backupval;

		return setuped_bones;
	}
	Vector GetBonePosition(int iBone)
	{
		/*matrix3x4_t boneMatrixes[128];
		if (this->SetupBones2(boneMatrixes, 128, 0x100, 0))
		{
			matrix3x4_t boneMatrix = boneMatrixes[iBone];
			return Vector(boneMatrix.flMatVal[0][3], boneMatrix.flMatVal[1][3], boneMatrix.flMatVal[2][3]);
		}
		else
			return Vector(0, 0, 0);*/
		if (matrix3x4_t boneMatrices[128]; SetupBones2(boneMatrices, 128, 0x100, g_pGlobalVars->curtime))
		{
			matrix3x4_t boneMatrix = boneMatrices[iBone];
			return Vector(boneMatrix.flMatVal[0][3], boneMatrix.flMatVal[1][3], boneMatrix.flMatVal[2][3]);
		}
		else
			return Vector(0, 0, 0);
	}

	ICollideable* GetCollideable()
	{
		return reinterpret_cast<ICollideable*>(reinterpret_cast<uintptr_t>(this) + 0x318);
	}

	void SetCurrentCommand(CUserCmd* cmd)
	{
		/*static int m_hConstraintEntity = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_hConstraintEntity");
		*reinterpret_cast<CUserCmd**>(uintptr_t(this) + m_hConstraintEntity - 0xC) = cmd;*/
		*reinterpret_cast<CUserCmd**>(uintptr_t(this) + 0x3348) = cmd;
	}
	float FireRate();

	void FixSetupBones(matrix3x4_t* Matrix);
	Vector GetHitboxPosition(int Hitbox, matrix3x4_t* Matrix, float* Radius);
	Vector GetHitboxPosition(int Hitbox, matrix3x4_t* Matrix);
	Vector GetHitboxPositionxd(int Hitbox);
	Vector hitbox_position(int hitbox_id);
	bool IsVisible(int bone);


	uint32_t& m_fEffects();
	uint32_t& m_iMostRecentModelBoneCounter();
	float& m_flLastBoneSetupTime();
	void invalidate_bone_cache();
	bool setup_bones_fixed(matrix3x4_t* matrix, int mask);
	int animlayer_count();
	void SetEyeAngles(Vector Angle)
	{
		static int m_angEyeAngles = g_pNetvars->GetOffset("DT_CSPlayer", "m_angEyeAngles");
		*reinterpret_cast<Vector*>(uintptr_t(this) + m_angEyeAngles) = Angle;
	}

	void SetOrigin(Vector Origin)
	{
		static int m_vecOrigin = g_pNetvars->GetOffset("DT_BaseEntity", "m_vecOrigin");
		*reinterpret_cast<Vector*>(uintptr_t(this) + m_vecOrigin) = Origin;
	}
	void SetTickBase(int TickBase)
	{
		static int m_nTickBase = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_nTickBase");
		*reinterpret_cast<int*>(uintptr_t(this) + m_nTickBase) = TickBase;
	}
	int GetShotsFired()
	{
		static int m_iShotsFired = g_pNetvars->GetOffset("DT_CSPlayer", "cslocaldata", "m_iShotsFired");
		return GetValue<int>(m_iShotsFired);
	}
	int GetTickBase()
	{
		static int m_nTickBase = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_nTickBase");
		return GetValue<int>(m_nTickBase);
	}
	void ClientAnimations(bool value)
	{
		static int m_bClientSideAnimation = g_pNetvars->GetOffset("DT_BaseAnimating", "m_bClientSideAnimation");
		*reinterpret_cast<bool*>(uintptr_t(this) + m_bClientSideAnimation) = value;
	}
	void SetLowerBodyYaw(float value)
	{
		static int m_flLowerBodyYawTarget = g_pNetvars->GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
		*reinterpret_cast<float*>(uintptr_t(this) + m_flLowerBodyYawTarget) = value;
	}


	void SetFlashDuration(float fDuration)
	{
		static int pFlashDuration = g_pNetvars->GetOffset("DT_CSPlayer", "m_flFlashDuration");
		*reinterpret_cast<int*>(uintptr_t(this) + pFlashDuration) = fDuration;
	}
	bool is_valid(C_BaseEntity* local_player = nullptr, bool is_player = true, bool dormant = true, bool team_check = true) {
		if (!this)
			return false;

		if (dormant)
			if (this->IsDormant())
				return false;

		if (team_check && local_player)
			if (this->GetTeam() == local_player->GetTeam())
				return false;

		if (is_player)
			if (!this->IsPlayer())
				return false;

		if (this->GetHealth() <= 0)
			return false;

		return true;
	}

	bool IsPlayer() {
		using original_fn = bool(__thiscall*)(C_BaseEntity*);
		return (*(original_fn**)this)[158](this); //157 Fix: 20210922
	}

	bool IsKnifeorNade();
	bool IsNade();
	bool IsScout();
	bool IsNotDT();
	bool IsAwp();
	int GetSequenceActivity(int sequence);
	bool IsPistol();
	bool IsShotgun();
	bool IsRifle();
	bool IsGrenade();
	bool IsSniper();
	bool IsEnemy();
	bool IsKnife();
	bool IsAlive() { return this->GetHealth() > 0 && this->GetLifeState() == 0; }

	float GetOldSimulationTime()
	{
		static int m_flOldSimulationTime = g_pNetvars->GetOffset("DT_BaseEntity", "m_flSimulationTime") + 4;
		return GetValue<float>(m_flOldSimulationTime);
	}

	matrix3x4_t GetBoneMatrix(int BoneID)
	{
		matrix3x4_t matrix;

		auto offset = *reinterpret_cast<uintptr_t*>(uintptr_t(this) + 0x268C); //0x26A8 Fix: 20210922
		if (offset)
			matrix = *reinterpret_cast<matrix3x4_t*>(offset + 0x30 * BoneID);

		return matrix;
	}
	void SetFlags(int offset)
	{
		static int m_fFlags = g_pNetvars->GetOffset("DT_BasePlayer", "m_fFlags");
		*reinterpret_cast<int*>(uintptr_t(this) + m_fFlags) = offset;
	}

	MoveType_t GetMoveType()
	{
		static int m_Movetype = g_pNetvars->GetOffset("DT_BaseEntity", "m_nRenderMode") + 1;
		return GetValue<MoveType_t>(m_Movetype);
	}
	AnimationLayer* GetAnimOverlays4()
	{
		return *(AnimationLayer**)((DWORD)this + 0x2990);//0x2980 Fix: 20210922
	}
	AnimationLayer* GetAnimOverlay4(int i)
	{
		if (i < 16)
			return &GetAnimOverlays4()[i];
	}
	int& EFlags()
	{
		return SetValue<int>(0x104); //0xE4 Fix: 20210922
	}
	int SetObserverMode(int mode)
	{
		static int m_iObserverMode = g_pNetvars->GetOffset("DT_BasePlayer", "m_iObserverMode");
		return *reinterpret_cast<int*>(uintptr_t(this) + m_iObserverMode) = mode;
	}

	C_BaseCombatWeapon* GetActiveWeapon()
	{
		static int m_hActiveWeapon = g_pNetvars->GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
		const auto weaponData = GetValue<CBaseHandle>(m_hActiveWeapon);
		return reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntityFromHandle(weaponData));
	}
	void SetAbsVelocity(Vector velocity);
	C_BaseEntity* GetObserverTarget()
	{
		//return Utils::GetVFunc<C_BaseEntity* (__thiscall*)(void*)>(this, 291)(this);
		DWORD entityData = *(DWORD*)((DWORD)this + g_pNetvars->GetOffset("DT_BasePlayer", "m_hObserverTarget"));
		return(C_BaseEntity*)g_pEntityList->GetClientEntityFromHandle(entityData);
	}

	Vector GetAbsOrigin()
	{
		return Utils::GetVFunc<Vector& (__thiscall*)(void*)>(this, 10)(this);
	}

	Vector GetVecOrigin()
	{
		return *reinterpret_cast<Vector*>(uintptr_t(this) + 0x138); //0x138
	}

	void SetAbsAngles(Vector angles);
	void SetAbsOrigin(Vector origin);

	float GetNextAttack()
	{
		return *reinterpret_cast<float*>(uint32_t(this) + 0x2D80); //0x2D70 Fix: 20210922 //NEM RELEVÁNS
	}
	void SetModelIndex(int index)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		return Utils::GetVFunc<OriginalFn>(this, 75)(this, index);
	}

	C_AnimState* AnimState()
	{
		return *reinterpret_cast<C_AnimState**>(uintptr_t(this) + 0x9960); //0x3914 Fix: 20210922
	}

	AnimationLayer* AnimOverlays()
	{
		return *reinterpret_cast<AnimationLayer**>(uintptr_t(this) + 0x2990); //0x2980 Fix: 20210922
	}
	AnimationLayer* GetAnimOverlay(int i)
	{
		if (i < 15)
			return &AnimOverlays()[i];
		return nullptr;
	}

	std::array <float, 24>& m_flPoseParameter()
	{
		static auto _m_flPoseParameter = g_pNetvars->GetOffset(SteamStrings("DT_BaseAnimating"), SteamStrings("m_flPoseParameter"));
		return *(std::array <float, 24>*)((uintptr_t)this + _m_flPoseParameter);
	}
	float& m_flSpawnTime()
	{
		return *(float*)((uintptr_t)this + 0xAD4); //0xA360 Fix: 20210922
	}
	int GetNumAnimOverlays() {
		return *(int*)((DWORD)this + 0x299C); //0x298C Fix: 20210922 mby 0x298C?
	}

	void UpdateClientSideAnimation() {
		Utils::CallVFunc<224, void>(this); //219 Fix: 20210922
	}

	int NumOverlays()
	{
		return 15;
	}

	void UpdateClientAnimation()
	{
		Utils::GetVFunc<void(__thiscall*)(void*)>(this, 224)(this); //223 Fix: 20210922
	}
	void UpdateAnimationState(C_AnimState* state, Vector angle)
	{
		using fn = void(__vectorcall*)(void*, void*, float, float, float, void*);
		static auto ret = reinterpret_cast<fn>(Utils::FindSignature("client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3")); //55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24 Fix: 20211108

		if (!ret)
			return;

		ret(state, NULL, NULL, angle.y, angle.x, NULL);
	}
	float C_BaseEntity::get_max_desync_delta()
	{
		const auto speed_factor = std::max<float>(0.f, std::min<float>(1, this->AnimState()->m_flSpeedNormalized));
		const auto speed_fraction = std::max<float>(0.f, std::min<float>(this->AnimState()->m_flFeetSpeedForwardsOrSideWays, 1.f));

		const auto unk1 = (this->AnimState()->m_fLandingDuckAdditiveSomething * -0.30000001 - 0.19999999) * speed_fraction;
		float unk2 = unk1 + 1.f;

		if (this->AnimState()->m_fDuckAmount > 0)
			unk2 += this->AnimState()->m_fDuckAmount * speed_factor * (0.5f - unk2);

		return this->AnimState()->max_yaw * unk2;
	}
	float C_BaseEntity::get_max_desync_delta_aimware()
	{

		auto thisptr = uintptr_t(this->AnimState());


		auto v1 = ((float*)thisptr)[62];
		auto v2 = 1.0f;
		auto v3 = 0.0f;
		auto v4 = 0.f;
		auto v7 = 0.0f;
		auto v8 = 0.f;
		auto v10 = 0.0f;
		auto v11 = v1;

		if (v1 <= 1.0f) {
			v4 = v10;

			if (v1 >= 0.0)
				v4 = v11;

			v3 = v4;
		}
		else
			v3 = 1.0f;

		auto v5 = ((float*)thisptr)[41];
		auto v6 = (float)((float)((float)(((float*)thisptr)[71] * -0.3f) - 2.0f) * v3) + 1.0f;

		if (v5 > 0.0) {
			v7 = ((float*)thisptr)[63];
			v11 = 0.0;
			v10 = v7;

			if (v7 <= 1.0f) {
				v8 = v11;

				if (v7 >= 0.0)
					v8 = v10;

				//v2 = v8;
			}

			v6 = v6 + (float)((float)(v2 * v5) * (float)(0.5f - v6));
		}

		return ((float*)thisptr)[205] * v6;
	};
	static int find_in_datamap_s(datamap_t* map, const char* name)
	{
		while (map)
		{
			for (auto i = 0; i < map->dataNumFields; ++i)
			{
				if (!map->dataDesc[i].fieldName)
					continue;

				if (!strcmp(name, map->dataDesc[i].fieldName))
					return map->dataDesc[i].fieldOffset[TD_OFFSET_NORMAL];

				if (map->dataDesc[i].fieldType == 10)
				{
					if (map->dataDesc[i].td)
					{
						unsigned int offset;

						if (offset = find_in_datamap_s(map->dataDesc[i].td, name))
							return offset;
					}
				}
			}

			map = map->baseMap;
		}

		return 0;
	}
};



class C_BaseCombatWeapon : public C_BaseEntity
{
private:
	template<class T>
	T GetPointer(const int offset)
	{
		return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	// To get value from the pointer itself
	template<class T>
	T GetValue(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}

public:
	NETVAR(float, GetAccuracyPenalty, "DT_WeaponCSBase", "m_fAccuracyPenalty");
	NETVAR(float, m_flPostponeFireReadyTime, "DT_WeaponCSBase", "m_flPostponeFireReadyTime");
	NETVAR(float, GetLastShotTime, "DT_WeaponCSBase", "m_fLastShotTime");
	NETVAR(int, GetAmmo, "DT_BaseCombatWeapon", "m_iClip1");
	NETVAR(float, GetNextPrimaryAttack, "DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack");
	//NETVAR(ItemDefinitionIndex, GetItemDefinitionIndex, "DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
	ItemDefinitionIndex GetItemDefinitionIndex()
	{
		static int m_iItemDefinitionIndex = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
		return GetValue<ItemDefinitionIndex>(m_iItemDefinitionIndex);
	}
	float GetInaccuracy()
	{
		typedef float(__thiscall* oInaccuracy)(PVOID);
		return Utils::GetVFunc< oInaccuracy >(this, 483)(this); //482 Fix: 20210922
	}

	float GetSpread()
	{
		typedef float(__thiscall* oWeaponSpread)(PVOID);
		return Utils::GetVFunc< oWeaponSpread >(this, 453)(this); //452 Fix: 20210922
	}

	WeaponInfo_t* GetCSWpnData()
	{
		return Utils::CallVFunc<461, WeaponInfo_t*>(this); //460 Fix: 20220525
	}

	std::string GetName()
	{
		return std::string(this->GetCSWpnData()->szWeaponName);
	}
};
extern CCSGameRules* g_GameRules;
class C_BaseViewModel
{
public:
	NETVAR(DWORD, GetOwner, "DT_BaseViewModel", "m_hOwner");
	NETVAR(int, GetModelIndex, "DT_BaseViewModel", "m_nModelIndex");
};

class viewmodel_t : public C_BaseEntity {
public:
	NETVAR(int, m_nModelIndex, "DT_BaseViewModel", "m_nModelIndex");
	NETVAR(int, m_nViewModelIndex, "DT_BaseViewModel", "m_nViewModelIndex");
	NETVAR(CHandle<C_BaseCombatWeapon>, m_hWeapon, "DT_BaseViewModel", "m_hWeapon");
	NETVAR(CHandle<C_BaseEntity>, m_hOwner, "DT_BaseViewModel", "m_hOwner");
	NETVAR(int, m_nAnimationParity, "CBaseViewModel", "m_nAnimationParity");
	//NETVAR(, , "", "");
	float& m_flCycle()
	{
		static auto m_flCycle = find_in_datamap_s(GetPredDescMap(), SteamStrings("m_flCycle"));
		return *(float*)(uintptr_t(this) + m_flCycle);
	}
	float& m_flAnimTime()
	{
		static auto m_flAnimTime = find_in_datamap_s(GetPredDescMap(), SteamStrings("m_flAnimTime"));
		return *(float*)(uintptr_t(this) + m_flAnimTime);
	}
};
#define _WIN32_DCOM