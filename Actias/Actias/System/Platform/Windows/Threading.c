#include <Actias/System/Memory.h>
#include <Actias/System/Platform/Windows/WinHeaders.h>
#include <Actias/System/Threading.h>

static DWORD WINAPI WindowsThreadFunction(LPVOID lpParam)
{
    ActiasThreadCreateInfo* info = (ActiasThreadCreateInfo*)lpParam;

    ActiasResult result = info->StartRoutine(info->StartParameter);
    ActiasFree(info);

    if (result == ACTIAS_SUCCESS)
    {
        return 0;
    }

    return 1;
}

ActiasResult ACTIAS_ABI ActiasSetThreadName(ActiasHandle threadHandle, const char* pName, USize nameLength)
{
    int length       = nameLength == 0 ? -1 : (int)nameLength;
    LPWSTR pWideName = ActiasConvertUTF8ToWide(pName, length);
    if (pWideName == NULL)
    {
        return ACTIAS_FAIL_INVALID_ENCODING;
    }

    HRESULT descResult = SetThreadDescription((HANDLE)threadHandle, pWideName);
    ActiasFree(pWideName);

    if (FAILED(descResult))
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasCreateThread(const ActiasThreadCreateInfo* pCreateInfo, ActiasThreadInfo* pResult)
{
    ActiasThreadCreateInfo* pHeapCreateInfo = (ActiasThreadCreateInfo*)ActiasAlloc(sizeof(ActiasThreadCreateInfo));
    ActiasInlineCopyMemory(pHeapCreateInfo, pCreateInfo, sizeof(ActiasThreadCreateInfo));

    DWORD id;
    HANDLE hThread = CreateThread(NULL, pCreateInfo->StackSize, WindowsThreadFunction, pHeapCreateInfo, CREATE_SUSPENDED, &id);

    if (hThread == NULL)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    if (pCreateInfo->pName)
    {
        ActiasResult nameResult = ActiasSetThreadName((ActiasHandle)hThread, pCreateInfo->pName, pCreateInfo->NameLength);
        if (nameResult != ACTIAS_SUCCESS)
        {
            ActiasCloseHandle(hThread);
            return nameResult;
        }
    }

    if (ResumeThread(hThread) == (DWORD)-1)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    pResult->ID     = id;
    pResult->Handle = (ActiasHandle)hThread;

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasReleaseThread(ActiasHandle threadHandle)
{
    return ActiasCloseHandle(threadHandle);
}

ActiasResult ACTIAS_ABI ActiasWaitForThread(ActiasHandle threadHandle, UInt64 millisecondTimeout)
{
    DWORD result = WaitForSingleObject((HANDLE)threadHandle, (DWORD)millisecondTimeout);

    switch (result)
    {
    case WAIT_TIMEOUT:
        return ACTIAS_THREAD_WAIT_TIMEOUT;
    case WAIT_OBJECT_0:
        return ACTIAS_SUCCESS;
    case WAIT_FAILED:
    default:
        return ACTIAS_FAIL_UNKNOWN;
    }
}
