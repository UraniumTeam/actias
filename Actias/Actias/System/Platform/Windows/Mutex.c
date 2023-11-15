#include <Actias/System/Mutex.h>
#include <Actias/System/Platform/Windows/WinHeaders.h>

void ACTIAS_ABI ActiasInitializeMutex(ActiasMutex* pMutex)
{
    InitializeCriticalSection((LPCRITICAL_SECTION)pMutex);
}

void ACTIAS_ABI ActiasReleaseMutex(ActiasMutex* pMutex)
{
    DeleteCriticalSection((LPCRITICAL_SECTION)pMutex);
}

void ACTIAS_ABI ActiasEnterMutex(ActiasMutex* pMutex)
{
    EnterCriticalSection((LPCRITICAL_SECTION)pMutex);
}

ActiasBool ACTIAS_ABI ActiasTryEnterMutex(ActiasMutex* pMutex)
{
    return TryEnterCriticalSection((LPCRITICAL_SECTION)pMutex);
}

void ACTIAS_ABI ActiasLeaveMutex(ActiasMutex* pMutex)
{
    LeaveCriticalSection((LPCRITICAL_SECTION)pMutex);
}
