#include <Actias/System/Core.h>
#include <string.h>

void ActiasCopyMemory(void* pDestination, ACTIAS_CONST void* pSource, USize byteSize)
{
    memcpy(pDestination, pSource, byteSize);
}
