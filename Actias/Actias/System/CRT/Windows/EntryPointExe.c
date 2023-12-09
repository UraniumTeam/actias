#include <Actias/System/CRT/EntryPoint.h>
#include <Actias/System/Platform/Windows/WinHeaders.h>

ActiasMainProc ActiasMain;

int __cdecl main(int argc, char** argv);

VOID wmainCRTStartup()
{
    int exitCode = main(0, NULL);
    // int exitCode = ActiasMain(ACTIAS_MAIN_CALL_REASON_EXECUTABLE_START, GetCommandLineA());
    ExitProcess(exitCode);
}
