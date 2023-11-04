/*
 * Actias SDK System
 *
 * Copyright (c) 2023 UraniumTeam
 */

#pragma once
#include <Actias/Base/Platform.h>
#include <emmintrin.h>

ACTIAS_BEGIN_C

//! \internal
#define ACTIAS_MAKE_COPY_PROC(bitCount)                                                                                          \
    ACTIAS_FORCE_INLINE void ActiasCopy##bitCount(void* pDestination, const void* pSource)                                       \
    {                                                                                                                            \
        UInt##bitCount* dst = (UInt##bitCount*)pDestination;                                                                     \
        UInt##bitCount* src = (UInt##bitCount*)pSource;                                                                          \
        *dst                = *src;                                                                                              \
    }

//! \internal
ACTIAS_MAKE_COPY_PROC(8);

//! \internal
ACTIAS_MAKE_COPY_PROC(16);

//! \internal
ACTIAS_MAKE_COPY_PROC(32);

//! \internal
ACTIAS_MAKE_COPY_PROC(64);

#undef ACTIAS_MAKE_COPY_PROC

//! \internal
ACTIAS_FORCE_INLINE void ActiasCopySmallBuffer(Int8* ACTIAS_RESTRICT pDestination, const Int8* ACTIAS_RESTRICT pSource,
                                               USize byteSize)
{
    // byteSize must be <= 16

    if (byteSize >= 8)
    {
        ActiasCopy64(pDestination + byteSize - 8, pSource + byteSize - 8);
        ActiasCopy64(pDestination, pSource);
    }
    else if (byteSize >= 4)
    {
        ActiasCopy32(pDestination + byteSize - 4, pSource + byteSize - 4);
        ActiasCopy32(pDestination, pSource);
    }
    else if (byteSize >= 2)
    {
        ActiasCopy16(pDestination + byteSize - 2, pSource + byteSize - 2);
        ActiasCopy16(pDestination, pSource);
    }
    else if (byteSize >= 1)
    {
        ActiasCopy8(pDestination, pSource);
    }
}

//! \brief Inline function to copy one memory buffer to another, use to avoid referencing shared libraries.
//!
//! This implementation can be beneficial for small buffers, while main ActiasCopyMemory from shared ActiasSystem library
//! can perform better for large buffers on CPUs with AVX support.
//!
//! \param pDestination - A pointer to the memory to copy to.
//! \param pSource - A pointer to the memory to copy from.
//! \param byteSize - The number of bytes to copy.
inline void ACTIAS_ABI ActiasInlineCopyMemory(void* ACTIAS_RESTRICT pDestination, const void* ACTIAS_RESTRICT pSource,
                                              USize byteSize)
{
    Int8* ACTIAS_RESTRICT dst       = (Int8 * ACTIAS_RESTRICT)(pDestination);
    const Int8* ACTIAS_RESTRICT src = (const Int8* ACTIAS_RESTRICT)(pSource);

tail:
    if (byteSize <= 16)
    {
        ActiasCopySmallBuffer(dst, src, byteSize);
    }
    else
    {
        if (byteSize <= 128)
        {
            _mm_storeu_si128((__m128i*)(dst + byteSize - 16), _mm_loadu_si128((const __m128i*)(src + byteSize - 16)));

            while (byteSize > 16)
            {
                _mm_storeu_si128((__m128i*)dst, _mm_loadu_si128((const __m128i*)src));
                dst += 16;
                src += 16;
                byteSize -= 16;
            }
        }
        else
        {
            USize padding = (16 - (((USize)dst) & 15)) & 15;

            if (padding > 0)
            {
                __m128i head = _mm_loadu_si128((const __m128i*)src);
                _mm_storeu_si128((__m128i*)dst, head);
                dst += padding;
                src += padding;
                byteSize -= padding;
            }

            __m128i c0, c1, c2, c3, c4, c5, c6, c7;

            while (byteSize >= 128)
            {
                c0 = _mm_loadu_si128(((const __m128i*)src) + 0);
                c1 = _mm_loadu_si128(((const __m128i*)src) + 1);
                c2 = _mm_loadu_si128(((const __m128i*)src) + 2);
                c3 = _mm_loadu_si128(((const __m128i*)src) + 3);
                c4 = _mm_loadu_si128(((const __m128i*)src) + 4);
                c5 = _mm_loadu_si128(((const __m128i*)src) + 5);
                c6 = _mm_loadu_si128(((const __m128i*)src) + 6);
                c7 = _mm_loadu_si128(((const __m128i*)src) + 7);
                src += 128;
                _mm_store_si128(((__m128i*)dst) + 0, c0);
                _mm_store_si128(((__m128i*)dst) + 1, c1);
                _mm_store_si128(((__m128i*)dst) + 2, c2);
                _mm_store_si128(((__m128i*)dst) + 3, c3);
                _mm_store_si128(((__m128i*)dst) + 4, c4);
                _mm_store_si128(((__m128i*)dst) + 5, c5);
                _mm_store_si128(((__m128i*)dst) + 6, c6);
                _mm_store_si128(((__m128i*)dst) + 7, c7);
                dst += 128;

                byteSize -= 128;
            }

            goto tail;
        }
    }
}

ACTIAS_END_C
