#pragma once
#include "CGlobalVarsBase.h"
#include "ClientClass.h"
#include "../Utils/Utils.h"

class IBaseClientDLL
{
public:
	// Connect appsystem components, get global interfaces, don't run any other init code
	virtual int              Connect(CreateInterfaceFn appSystemFactory, CGlobalVarsBase* pg) = 0;
	virtual int              Disconnect(void) = 0;
	virtual int              Init(CreateInterfaceFn appSystemFactory, CGlobalVarsBase* pg) = 0;
	virtual void             PostInit() = 0;
	virtual void             Shutdown(void) = 0;
	virtual void             LevelInitPreEntity(char const* pMapName) = 0;
	virtual void             LevelInitPostEntity() = 0;
	virtual void             LevelShutdown(void) = 0;
	virtual ClientClass* GetAllClasses(void) = 0;

	bool DispatchUserMessage(int messageType, int arg, int arg1, void* data)
	{
		using DispatchUserMessage_t = bool* (__thiscall*)(void*, int, int, int, void*);
		return  Utils::GetVFunc<DispatchUserMessage_t>(this, 38)(this, messageType, arg, arg1, data);
	}
};
extern IBaseClientDLL* g_pClientDll;