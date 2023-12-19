#include <Actias/System/CRT/EntryPoint.h>
#include <Actias/System/Platform/Windows/WinHeaders.h>
#include <Actias/System/Runtime.h>

ActiasLibraryMainProc ActiasLibraryMain;

inline static ActiasMainCallReason ConvertReason(DWORD dwReason)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        return ACTIAS_MAIN_CALL_REASON_LIBRARY_LOAD;
    case DLL_PROCESS_DETACH:
        return ACTIAS_MAIN_CALL_REASON_LIBRARY_UNLOAD;
    default:
        return ACTIAS_MAIN_CALL_REASON_NONE;
    }
}

BOOL WINAPI _DllMainCRTStartup(HINSTANCE hInst, DWORD dwReason, LPVOID lpvReserved)
{
    ACTIAS_Unused(lpvReserved);

    ActiasMainCallReason reason = ConvertReason(dwReason);
    if (reason == ACTIAS_MAIN_CALL_REASON_NONE)
    {
        return TRUE;
    }

    if (ActiasLibraryMain(reason, (ActiasHandle)hInst) != 0)
    {
        return FALSE;
    }

    return TRUE;
}
