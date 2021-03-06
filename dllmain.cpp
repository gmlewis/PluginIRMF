#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files

#ifdef _WIN32
	#include <windows.h>
#endif

#include <imgui/imgui.h>
#include "irmf.h"

#ifdef WIN32
# define FEXPORT __declspec(dllexport)
#else
# define FEXPORT // empty
#endif

extern "C" {
	FEXPORT irmf::IRMF* CreatePlugin() {
		return new irmf::IRMF();
	}
	FEXPORT void DestroyPlugin(irmf::IRMF* ptr) {
		delete ptr;
	}
	FEXPORT int GetPluginAPIVersion() {
		return 3;
	}
	FEXPORT int GetPluginVersion() {
		return 1;
	}
	FEXPORT const char* GetPluginName() {
		return "IRMF";
	}
}

#ifdef _WIN32
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
#endif