#include <Actias/System/Memory.h>
#include <Actias/System/Platform/Windows/WinHeaders.h>
#include <stdlib.h>

inline DWORD ActiasConvertMemoryProtection(ActiasFlags protection)
{
    DWORD result = PAGE_READONLY;
    if (protection & ACTIAS_MEMORY_PROTECTION_WRITE_BIT)
    {
        result = PAGE_READWRITE;
    }
    if (protection & ACTIAS_MEMORY_PROTECTION_EXECUTE_BIT)
    {
        result = PAGE_EXECUTE_READWRITE;
    }

    return result;
}

void* ActiasAlignedAlloc(USize byteAlignment, USize byteSize)
{
    return _aligned_malloc(byteSize, byteAlignment);
}

void ActiasAlignedFree(void* pointer)
{
    _aligned_free(pointer);
}

void* ActiasVirtualAlloc(void* pAddress, USize byteSize, ActiasFlags protection)
{
    return VirtualAlloc(pAddress, byteSize, MEM_COMMIT | MEM_RESERVE, ActiasConvertMemoryProtection(protection));
}

ActiasResult ActiasVirtualFree(void* pointer, USize byteSize)
{
    ACTIAS_UNUSED(byteSize);

    BOOL result = VirtualFree(pointer, 0, MEM_RELEASE);
    if (result == TRUE)
    {
        return ACTIAS_SUCCESS;
    }

    return ACTIAS_FAIL_UNKNOWN;
}
