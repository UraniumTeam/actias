#include <Actias/System/Memory.h>
#include <Actias/System/Platform/Windows/WinHeaders.h>
#include <stdlib.h>

inline DWORD ACTIAS_ABI ActiasConvertMemoryProtection(ActiasFlags protection)
{
    DWORD result = PAGE_READONLY;
    if (protection & ACTIAS_MEMORY_PROTECTION_WRITE_BIT)
    {
        result = PAGE_READWRITE;
        if (protection & ACTIAS_MEMORY_PROTECTION_EXECUTE_BIT)
        {
            result = PAGE_EXECUTE_READWRITE;
        }
    }
    else if (protection & ACTIAS_MEMORY_PROTECTION_EXECUTE_BIT)
    {
        result = PAGE_EXECUTE_READ;
    }

    return result;
}

void* ACTIAS_ABI ActiasAlignedAlloc(USize byteAlignment, USize byteSize)
{
    return _aligned_malloc(byteSize, byteAlignment);
}

void ACTIAS_ABI ActiasAlignedFree(void* pointer)
{
    _aligned_free(pointer);
}

void* ACTIAS_ABI ActiasVirtualAlloc(void* pAddress, USize byteSize, ActiasFlags protection)
{
    return VirtualAlloc(pAddress, byteSize, MEM_COMMIT | MEM_RESERVE, ActiasConvertMemoryProtection(protection));
}

ActiasResult ACTIAS_ABI ActiasVirtualFree(void* pointer, USize byteSize)
{
    ACTIAS_Unused(byteSize);

    BOOL result = VirtualFree(pointer, 0, MEM_RELEASE);
    if (result)
    {
        return ACTIAS_SUCCESS;
    }

    return ACTIAS_FAIL_UNKNOWN;
}

ActiasResult ACTIAS_ABI ActiasVirtualProtect(void* pointer, USize byteSize, ActiasFlags protection)
{
    DWORD prevProtect = 0;

    BOOL result = VirtualProtect(pointer, byteSize, ActiasConvertMemoryProtection(protection), &prevProtect);
    if (result)
    {
        return ACTIAS_SUCCESS;
    }

    return ACTIAS_FAIL_UNKNOWN;
}
