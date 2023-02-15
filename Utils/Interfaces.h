#pragma once
#include <Windows.h>
#include <string>
#include "xor.h"

template< class T >
T* FindClass(std::string szModuleName, std::string szInterfaceName, bool bSkip = false)
{
	if (szModuleName.empty() || szInterfaceName.empty())
		return nullptr;
	typedef PVOID(*CreateInterfaceFn)(const char* pszName, int* piReturnCode);
	CreateInterfaceFn hInterface = nullptr;
	while (!hInterface)
	{
		hInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(szModuleName.c_str()), SteamStrings("CreateInterface"));
		Sleep(5);
	}

	char pszBuffer[256];
	for (int i = 0; i < 100; i++)
	{
		sprintf_s(pszBuffer, "%s%0.3d", szInterfaceName.c_str(), i);
		PVOID pInterface = hInterface(pszBuffer, nullptr);

		if (pInterface && pInterface != NULL)
		{
			if (bSkip)
				sprintf_s(pszBuffer, "%s%0.3d", szInterfaceName.c_str(), i + 1);

			Sleep(5);
			break;
		}
	}

	return (T*)hInterface(pszBuffer, nullptr);
}
template<typename T>
static T* get_interface(const char* mod_name, const char* interface_name, bool exact = false) {
	T* iface = nullptr;
	InterfaceReg* register_list;
	int part_match_len = strlen(interface_name);

	DWORD interface_fn = reinterpret_cast<DWORD>(GetProcAddress(GetModuleHandleA(mod_name), crypt_str("CreateInterface")));

	if (!interface_fn) {
		return nullptr;
	}

	unsigned int jump_start = (unsigned int)(interface_fn)+4;
	unsigned int jump_target = jump_start + *(unsigned int*)(jump_start + 1) + 5;

	register_list = **reinterpret_cast<InterfaceReg***>(jump_target + 6);

	for (InterfaceReg* cur = register_list; cur; cur = cur->m_pNext) {
		if (exact == true) {
			if (strcmp(cur->m_pName, interface_name) == 0)
				iface = reinterpret_cast<T*>(cur->m_CreateFn());
		}
		else {
			if (!strncmp(cur->m_pName, interface_name, part_match_len) && std::atoi(cur->m_pName + part_match_len) > 0)
				iface = reinterpret_cast<T*>(cur->m_CreateFn());
		}
	}
	return iface;
}
template<typename T>
T* CaptureInterface(const char* szModuleName, const char* szInterfaceVersion)
{
	HMODULE moduleHandle = GetModuleHandleA(szModuleName);
	if (moduleHandle)
	{

		CreateInterfaceFn pfnFactory = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(moduleHandle, SteamStrings("CreateInterface")));
		return reinterpret_cast<T*>(pfnFactory(szInterfaceVersion, nullptr));
	}
	Utils::Log(SteamStrings("Error getting interface %"), szInterfaceVersion);
	return nullptr;
}
namespace interfaces
{
	void Init();
};

