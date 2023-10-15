#include <Actias/System/Core.h>
#include <Actias/System/Platform/Windows/WinHeaders.h>

Int32 ActiasGetNativeErrorCode(void)
{
    return GetLastError();
}
