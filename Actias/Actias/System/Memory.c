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

void* ActiasAlignedAlloc(USize byteAlignment, USize byteSize)
{
#if ACTIAS_COMPILER_MSVC
    return _aligned_malloc(byteSize, byteAlignment);
#else
    return ::memalign(byteAlignment, byteSize);
#endif
}

void ActiasFree(void* pointer)
{
    free(pointer);
}

void ActiasAlignedFree(void* pointer)
{
#if ACTIAS_COMPILER_MSVC
    _aligned_free(pointer);
#else
    free(pointer);
#endif
}
