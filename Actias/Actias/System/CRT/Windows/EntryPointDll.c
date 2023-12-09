#include <Actias/System/CRT/EntryPoint.h>
#include <Actias/System/Platform/Windows/WinHeaders.h>
#include <Actias/System/Runtime.h>

ActiasLibraryMainProc ActiasLibraryMain;

BOOL WINAPI _DllMainCRTStartup(HINSTANCE hInst, DWORD dwReason, LPVOID lpvReserved)
{
    ACTIAS_Unused(hInst);
    ACTIAS_Unused(dwReason);
    ACTIAS_Unused(lpvReserved);

    if (ActiasLibraryMain() != 0)
    {
        return FALSE;
    }

    return TRUE;
}
