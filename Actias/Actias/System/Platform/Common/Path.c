#include <Actias/System/Path.h>
#include <Actias/System/Unicode.h>

void ACTIAS_ABI ActiasNormalizePath(char* pPath, USize pathByteSize)
{
    char* pIter = pPath;
    UInt32 c    = ActiasUTF8PeekDecode(pIter);
    while (c && (pIter - pPath) < (Int64)pathByteSize)
    {
        if (c == '\\')
        {
            *pIter = '/';
        }
    }
}
