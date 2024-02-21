#define _OPEN_THREADS

#include <Actias/System/Mutex.h>
#include <pthread.h>

void ACTIAS_ABI ActiasInitializeMutex(ActiasMutex* pMutex)
{
    pthread_mutex_init((pthread_mutex_t*)pMutex, NULL);
}

void ACTIAS_ABI ActiasReleaseMutex(ActiasMutex* pMutex)
{
    pthread_mutex_destroy((pthread_mutex_t*)pMutex);
}

void ACTIAS_ABI ActiasLockMutex(ActiasMutex* pMutex)
{
    pthread_mutex_lock((pthread_mutex_t*)pMutex);
}

ActiasBool ACTIAS_ABI ActiasTryLockMutex(ActiasMutex* pMutex)
{
    return pthread_mutex_trylock((pthread_mutex_t*)pMutex);
}

void ACTIAS_ABI ActiasUnlockMutex(ActiasMutex* pMutex)
{
    pthread_mutex_unlock((pthread_mutex_t*)pMutex);
}
