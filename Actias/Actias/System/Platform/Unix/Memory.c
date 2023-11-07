#include <Actias/System/Memory.h>
#include <stdlib.h>
#include <sys/mman.h>

void* ACTIAS_ABI ActiasAlignedAlloc(USize byteAlignment, USize byteSize)
{
    return memalign(byteAlignment, byteSize);
}

void ACTIAS_ABI ActiasAlignedFree(void* pointer)
{
    free(pointer);
}

inline USize ACTIAS_ABI ActiasConvertMemoryProtection(ActiasFlags protection)
{
    switch (protection)
    {
    case ACTIAS_MEMORY_PROTECTION_READ_BIT:
        return PROT_READ;
    case ACTIAS_MEMORY_PROTECTION_WRITE_BIT:
        return PROT_WRITE;
    case ACTIAS_MEMORY_PROTECTION_EXECUTE_BIT:
        return PROT_EXEC;
    default:
        return PROT_NONE;
    }
}

void* ACTIAS_ABI ActiasVirtualAlloc(void* pAddress, USize byteSize, ActiasFlags protection)
{
    return mmap(pAddress, byteSize, ActiasConvertMemoryProtection(protection), MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
}

ActiasResult ACTIAS_ABI ActiasVirtualFree(void* pAddress, USize byteSize)
{
    munmap(pAddress, byteSize);
    if (errno != 0)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }
    return ACTIAS_SUCCESS;
}
