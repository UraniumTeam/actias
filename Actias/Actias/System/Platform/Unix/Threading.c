#define _GNU_SOURCE

#include <Actias/System/Threading.h>
#include <errno.h>
#include <pthread.h>

ActiasResult ACTIAS_ABI ActiasSetThreadName(ActiasHandle threadHandle, const char* pName, USize nameLength)
{
    if (nameLength == 0)
    {
        return ACTIAS_FAIL_INVALID_ARGUMENT;
    }

    int result = pthread_setname_np((pthread_t)threadHandle, pName);

    if (result == ERANGE)
    {
        return ACTIAS_FAIL_ARGUMENT_OUT_OF_RANGE;
    }

    if (result != 0)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasCreateThread(const ActiasThreadCreateInfo* pCreateInfo, ActiasThreadInfo* pResult)
{
    pthread_t thread;

    int result = pthread_create(&thread, NULL, (void*)pCreateInfo->StartRoutine, pCreateInfo->StartParameter);

    switch (result)
    {
    case EAGAIN:
    case EINVAL:
        return ACTIAS_FAIL_UNKNOWN;
    case EPERM:
        return ACTIAS_FAIL_PERMISSION_DENIED;
    }

    pResult->ID     = thread;
    pResult->Handle = (ActiasHandle)thread;

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
        return ACTIAS_FAIL_INVALID_ARGUMENT;
    case ETIMEDOUT:
        return ACTIAS_FAIL_THREAD_WAIT_ERROR;

    default:
        return ACTIAS_FAIL_UNKNOWN;
    }
}
