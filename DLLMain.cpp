#include <thread>
#include "Hooks.h"
#include "Utils\Utils.h"
#include "Utils\GlobalVars.h"
#include "Menu/Menu.h"
#include "AntiLeak.h"
#include "Features/Knife/Knife.h"
#include "Utils/xor.h"
#include <fstream>
#pragma comment(lib, "urlmon.lib")
using namespace std;
HINSTANCE HThisModule;
UCHAR szFileSys[255], szVolNameBuff[255];
DWORD dwMFL, dwSysFlags, sNumber;

LPCTSTR szHD = SteamStrings("C:\\");
unsigned long WINAPI initialize(void* instance) {
	/*while (!GetModuleHandleA("serverbrowser.dll"))
		Sleep(200);*/

	try {
		Hooks::Init();
		knifeload();
	}

	catch (const std::runtime_error& error) {
		MessageBoxA(nullptr, error.what(), "csgo-cheat error!", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
	}

	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	Hooks::Restore();
	//wait for paint_traverse::hook to be called and restore input.
	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
}

unsigned long WINAPI release() {
	Hooks::Restore();
	return TRUE;
}
DWORD GetProcessId(LPCTSTR name)
{

	PROCESSENTRY32 pe32;
	HANDLE snapshot = NULL;
	DWORD pid = 0;

	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot != INVALID_HANDLE_VALUE) {
		pe32.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(snapshot, &pe32)) {
			do {
				std::string sName = pe32.szExeFile;
				std::transform(sName.begin(), sName.end(), sName.begin(), ::tolower);

				if (!lstrcmp(sName.c_str(), name)) {
					pid = pe32.th32ProcessID;
					break;
				}
			} while (Process32Next(snapshot, &pe32));
		}

		CloseHandle(snapshot);
	}

	return pid;
}

void CheckDebugger()
{
	std::vector<std::string> m_processList;
	m_processList.push_back(SteamStrings("ollydbg.exe"));
	m_processList.push_back(SteamStrings("wireshark.exe"));
	m_processList.push_back(SteamStrings("lordpe.exe"));
	m_processList.push_back(SteamStrings("hookshark.exe"));
	m_processList.push_back(SteamStrings("idag.exe"));
	m_processList.push_back(SteamStrings("MPGH Virus Scan Tool v6.exe"));
	m_processList.push_back(SteamStrings("dnSpy-x86.exe"));
	m_processList.push_back(SteamStrings("ida64.exe"));
	m_processList.push_back(SteamStrings("ida86.exe"));
	m_processList.push_back(SteamStrings("loadll.exe"));
	m_processList.push_back(SteamStrings("HxD.exe"));
	m_processList.push_back(SteamStrings("Windowrenamer.exe"));
	m_processList.push_back(SteamStrings("windowsrenamer.exe"));

	for (unsigned int ax = 0; ax < m_processList.size(); ax++)
	{
		std::string sProcess = m_processList.at(ax);
		if (GetProcessId(sProcess.c_str()) != 0)
		{
			exit(0);
			MessageBox(0, SteamStrings("A Debugging Program is ON... Exiting."), SteamStrings("Lean Anti-Debug"), MB_OK | MB_ICONERROR);
		}
	}
}
#define Me 0000000000
std::int32_t WINAPI DllMain(const HMODULE instance [[maybe_unused]], const unsigned long reason, const void* reserved [[maybe_unused]] ) {
	switch (reason) {
	case DLL_PROCESS_ATTACH: {
		DisableThreadLibraryCalls(instance);
		GetVolumeInformation("C:\\", (LPTSTR)szVolNameBuff, 255, &sNumber, &dwMFL, &dwSysFlags, (LPTSTR)szFileSys, 255);
		{
			if (!sNumber)
				return DllMain(instance, reason, reserved);
			//if (sNumber == Me) //There was a little bit of protection...or something. :)
			//	initialize(instance);
			if (sNumber != Me)
				initialize(instance);
			else
				FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
		}
		break;
	}
	case DLL_PROCESS_DETACH: {
		release();
		break;
	}
	}
	return true;
}