#define NOMINMAX
#include <windows.h>

#include "logging.h"
#include "param_patch.h"

namespace
{
DWORD WINAPI ModThread(LPVOID)
{
    logging::Debug("ModThread started");
    InitRuntimeParamPatch();
    logging::Debug("ModThread finished");
    return 0;
}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        logging::Debug("DLL loaded, built: " __DATE__ " " __TIME__);
        CreateThread(nullptr, 0, ModThread, nullptr, 0, nullptr);
    }

    return TRUE;
}
