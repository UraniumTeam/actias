#include <Actias/System/Core.h>
#include <Actias/System/Platform/Common/InlineCopyMemory.h>
#include <immintrin.h>

#define ACTIAS_DEFAULT_L3_CACHE_SIZE 4 * 1024 * 1024

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

        // The aligned copy functions require the input size to be a multiple of 256
        USize alignedCopyBytes = byteSize & ~((USize)0xFF);

        if (byteSize >= ACTIAS_DEFAULT_L3_CACHE_SIZE)
        {
            // If the size of the copied memory buffer is greater than assumed size
            // of the L3 CPU cache, we use non-temporal SIMD instructions
            ActiasStreamMemory(dst, src, alignedCopyBytes);
        }
        else
        {
            ActiasCopyAlignedMemory(dst, src, alignedCopyBytes);
        }

        src += alignedCopyBytes;
        dst += alignedCopyBytes;
        byteSize -= alignedCopyBytes;

        // Jump to tail to copy the rest
        goto tail;
    }
}
#else
{
    ActiasInlineCopyMemory(pDestination, pSource, byteSize);
}
#endif
