#define _GNU_SOURCE

#include <Actias/System/Threading.h>
#include <errno.h>
#include <pthread.h>

ActiasResult ACTIAS_ABI ActiasSetThreadName(ActiasHandle threadHandle, const char* pName, USize nameLength)
{
    if (nameLength <= 0)
    {
        return ACTIAS_FAIL_INVALID_ENCODING;
    }

    int result = pthread_setname_np((pthread_t)threadHandle, pName);

    if (result != 0 || result == ERANGE)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasCreateThread(const ActiasThreadCreateInfo* pCreateInfo, ActiasThreadInfo* pResult)
{
    pthread_t pThread;

    int result = pthread_create(&pThread, NULL, (void*)pCreateInfo->StartRoutine, pCreateInfo->StartParameter);

    switch (result)
    {
    case EAGAIN:
        return ACTIAS_FAIL_UNKNOWN;
    case EINVAL:
        return ACTIAS_FAIL_UNKNOWN;
    case EPERM:
        return ACTIAS_FAIL_PERMISSION_DENIED;
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
    struct timespec ts;
    ts.tv_nsec = millisecondTimeout * 1000000;

    int result = pthread_timedjoin_np((pthread_t)threadHandle, NULL, &ts);

    switch (result)
    {
    case 0:
        return ACTIAS_SUCCESS;
    case EBUSY:
        return ACTIAS_FAIL_UNKNOWN;
    case EINVAL:
        return ACTIAS_FAIL_UNKNOWN;
    case ETIMEDOUT:
        return ACTIAS_THREAD_WAIT_TIMEOUT;

    default:
        return ACTIAS_FAIL_UNKNOWN;
    }
}
