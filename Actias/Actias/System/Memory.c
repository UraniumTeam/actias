#include <Actias/System/Memory.h>
#include <stdlib.h>

void* ActiasAlloc(size_t byteSize)
{
    return malloc(byteSize);
}

void* ActiasRealloc(void* pointer, size_t newByteSize)
{
    return realloc(pointer, newByteSize);
}

void* ActiasAlignedAlloc(size_t byteAlignment, size_t byteSize)
{
#if ACTIAS_COMPILER_MSVC
    return _aligned_malloc(byteAlignment, byteSize);
#else
    return aligned_alloc(byteAlignment, byteSize);
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
