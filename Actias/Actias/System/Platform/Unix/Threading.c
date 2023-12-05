#define _GNU_SOURCE

#include <Actias/System/Threading.h>
#include <pthread.h>

ActiasResult ACTIAS_ABI ActiasSetThreadName(ActiasHandle threadHandle, const char* pName, USize nameLength)
{
    if (nameLength <= 0)
    {
        return ACTIAS_FAIL_INVALID_ENCODING;
    }

    pthread_setname_np((pthread_t)threadHandle, pName);
    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasCreateThread(const ActiasThreadCreateInfo* pCreateInfo, ActiasThreadInfo* pResult)
{
    pthread_t pThread;

    int result = pthread_create(&pThread, NULL, (void*)pCreateInfo->StartRoutine, pCreateInfo->StartParameter);
    if (result != 0)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    pResult->ID     = pThread;
    pResult->Handle = (ActiasHandle)pThread;

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasReleaseThread(ActiasHandle threadHandle)
{
    pthread_exit((void*)threadHandle);
    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasWaitForThread(ActiasHandle threadHandle, UInt64 millisecondTimeout)
{
    if (millisecondTimeout < 0)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    int result = pthread_join((pthread_t)threadHandle, NULL);

    if (result != 0)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    return ACTIAS_SUCCESS;
}
