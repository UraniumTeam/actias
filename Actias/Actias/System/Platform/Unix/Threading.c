#define _GNU_SOURCE

#include <Actias/System/Memory.h>
#include <Actias/System/Threading.h>
#include <errno.h>

#if __has_include(<pthread.h>)
#    include <pthread.h>
#endif

#if __has_include(<pthread_np.h>)
#    include <pthread_np.h>
#endif

#if defined(__linux__)
#    define pthread_setname_np(thread, name) (pthread_setname_np(thread, name));
#endif

#if defined(__NetBSD__)
#    define pthread_setname_np(thread, name) (pthread_setname_np(thread, name, NULL));
#endif

#if defined(__FreeBSD__) || defined(__OpenBSD__)
#    define pthread_setname_np(thread, name) (pthread_setname_np(thread, name));
#endif

static void* UnixThreadFunction(void* startParameter)
{
    ActiasThreadCreateInfo* info = (ActiasThreadCreateInfo*)startParameter;
    info->StartRoutine(info->StartParameter);
    ActiasFree(info);

    return NULL;
}

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

    ActiasThreadCreateInfo* pHeapCreateInfo = (ActiasThreadCreateInfo*)ActiasAlloc(sizeof(ActiasThreadCreateInfo));
    ActiasInlineCopyMemory(pHeapCreateInfo, pCreateInfo, sizeof(ActiasThreadCreateInfo));

    int result = pthread_create(&thread, NULL, UnixThreadFunction, pHeapCreateInfo);

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

static void ConvertMsToTimespec(struct timespec* ts, UInt64 milliseconds)
{
    ts->tv_sec  = milliseconds / 1000;
    ts->tv_nsec = (milliseconds % 1000) * 1000000;
}

ActiasResult ACTIAS_ABI ActiasWaitForThread(ActiasHandle threadHandle, UInt64 millisecondTimeout)
{
    struct timespec ts;
    ConvertMsToTimespec(&ts, millisecondTimeout);

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
        return ACTIAS_THREAD_WAIT_TIMEOUT;

    default:
        return ACTIAS_FAIL_UNKNOWN;
    }
}
