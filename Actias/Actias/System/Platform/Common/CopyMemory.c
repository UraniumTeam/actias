#include <Actias/System/Core.h>
#include <Actias/System/Platform/Common/InlineCopyMemory.h>
#include <immintrin.h>

void ACTIAS_ABI ActiasCopyMemory(void* ACTIAS_RESTRICT pDestination, const void* ACTIAS_RESTRICT pSource, USize byteSize)
#if ACTIAS_AVX2_SUPPORTED
{
    Int8* ACTIAS_RESTRICT dst       = (Int8 * ACTIAS_RESTRICT)(pDestination);
    const Int8* ACTIAS_RESTRICT src = (const Int8* ACTIAS_RESTRICT)(pSource);

    if (byteSize <= 256)
    {
    tail:
        ActiasInlineCopyMemory(dst, src, byteSize);
    }
    else
    {
        USize padding = (32 - (((USize)dst) & 31)) & 31;

        if (padding > 0)
        {
            __m256i head = _mm256_loadu_si256((const __m256i*)src);
            _mm256_storeu_si256((__m256i*)dst, head);
            dst += padding;
            src += padding;
            byteSize -= padding;
        }

        ActiasCopyAlignedMemory(dst, src, byteSize);

        USize alignedCopyBytes = byteSize & ~((USize)0xFF);
        src += alignedCopyBytes;
        dst += alignedCopyBytes;
        byteSize -= alignedCopyBytes;

        goto tail;
    }
}
#else
{
    ActiasInlineCopyMemory(pDestination, pSource, byteSize);
}
#endif
