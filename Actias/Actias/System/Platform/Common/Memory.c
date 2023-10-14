#include <Actias/System/Memory.h>
#include <stdlib.h>

void* ActiasAlloc(USize byteSize)
{
    return malloc(byteSize);
}

void* ActiasRealloc(void* pointer, USize newByteSize)
{
    return realloc(pointer, newByteSize);
}

void ActiasFree(void* pointer)
{
    free(pointer);
}
