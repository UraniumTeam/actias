#include <Actias/System/Core.h>

void ACTIAS_ABI ActiasSetMemory(void* pDestination, UInt8 value, USize byteCount)
{
    ActiasInlineSetMemory(pDestination, value, byteCount);
}
