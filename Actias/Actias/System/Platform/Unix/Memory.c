#include <Actias/System/Memory.h>
#include <malloc.h>
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

inline int ACTIAS_ABI ActiasConvertMemoryProtection(ActiasFlags protection)
{
    int result = PROT_NONE;
    if (protection & ACTIAS_MEMORY_PROTECTION_READ_BIT)
    {
        result |= PROT_READ;
    }
    if (protection & ACTIAS_MEMORY_PROTECTION_WRITE_BIT)
    {
        result |= PROT_WRITE;
    }
    if (protection & ACTIAS_MEMORY_PROTECTION_EXECUTE_BIT)
    {
        result |= PROT_EXEC;
    }

    return result;
}

void* ACTIAS_ABI ActiasVirtualAlloc(void* pAddress, USize byteSize, ActiasFlags protection)
{
    return mmap(pAddress, byteSize, ActiasConvertMemoryProtection(protection), MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
}

ActiasResult ACTIAS_ABI ActiasVirtualFree(void* pAddress, USize byteSize)
{
    int result = munmap(pAddress, byteSize);
    if (result == -1)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    return ACTIAS_SUCCESS;
}
