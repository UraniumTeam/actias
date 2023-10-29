#include <Actias/System/Memory.h>
#include <stdlib.h>

void* ACTIAS_ABI ActiasAlloc(USize byteSize)
{
    return malloc(byteSize);
}

void* ACTIAS_ABI ActiasRealloc(void* pointer, USize newByteSize)
{
    return realloc(pointer, newByteSize);
}

void ACTIAS_ABI ActiasFree(void* pointer)
{
    free(pointer);
}
