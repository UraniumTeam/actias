#include <Actias/System/Core.h>
#include <string.h>

void ACTIAS_ABI ActiasSetMemory(void* pDestination, Int32 value, USize byteCount)
{
    memset(pDestination, value, byteCount);
}
