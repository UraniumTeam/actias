#include <Actias/System/Platform/Windows/WinHeaders.h>

int __cdecl main(int argc, LPCSTR argv[]);

VOID wmainCRTStartup()
{
    int exitCode = main(0, NULL);
    ExitProcess(exitCode);
}
