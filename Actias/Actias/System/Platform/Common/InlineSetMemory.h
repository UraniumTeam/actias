/*
 * Actias SDK System
 *
 * Copyright (c) 2023 UraniumTeam
 */

#pragma once
#include <Actias/Base/Platform.h>
#include <emmintrin.h>

ACTIAS_BEGIN_C

#define ACTIAS_MEMSET_MASK_8 ((UInt8)1)
#define ACTIAS_MEMSET_MASK_16 (((UInt16)ACTIAS_MEMSET_MASK_8 << 8) | (UInt16)ACTIAS_MEMSET_MASK_8)
#define ACTIAS_MEMSET_MASK_32 (((UInt32)ACTIAS_MEMSET_MASK_16 << 16) | (UInt32)ACTIAS_MEMSET_MASK_16)
#define ACTIAS_MEMSET_MASK_64 (((UInt64)ACTIAS_MEMSET_MASK_32 << 32) | (UInt64)ACTIAS_MEMSET_MASK_32)

//! \internal
ACTIAS_FORCE_INLINE void ActiasSetSmallBuffer(Int8* pDestination, UInt8 value, USize byteSize)
{
    // byteSize must be <= 16

    if (byteSize >= 8)
    {
        UInt64* tmp1 = ((UInt64*)(pDestination + byteSize - 8));
        UInt64* tmp2 = (UInt64*)pDestination;
        *tmp1 = *tmp2 = ACTIAS_MEMSET_MASK_64 * value;
    }
    else if (byteSize >= 4)
    {
        UInt32* tmp1 = ((UInt32*)(pDestination + byteSize - 4));
        UInt32* tmp2 = (UInt32*)pDestination;
        *tmp1 = *tmp2 = ACTIAS_MEMSET_MASK_32 * value;
    }
    else if (byteSize >= 2)
    {
        UInt16* tmp1 = ((UInt16*)(pDestination + byteSize - 2));
        UInt16* tmp2 = (UInt16*)pDestination;
        *tmp1 = *tmp2 = ACTIAS_MEMSET_MASK_16 * value;
    }
    else if (byteSize >= 1)
    {
        *((UInt8*)pDestination) = value;
    }
}

#undef ACTIAS_MEMSET_MASK_8

//! \brief Inline function to copy a value to each of the first bytes of the provided memory buffer, use to avoid referencing shared libraries.
//!
//! \param pDestination - A pointer to the memory to fill.
//! \param value - The fill byte.
//! \param byteCount - The number of bytes to fill.
ACTIAS_NO_BUILTIN(memset) inline void ACTIAS_ABI ActiasInlineSetMemory(void* pDestination, UInt8 value, USize byteCount)
{
    Int8* dst = (Int8*)(pDestination);

tail:
    if (byteCount <= 16)
    {
        ActiasSetSmallBuffer(dst, value, byteCount);
    }
    else
    {
        __m128i vec = _mm_set1_epi8((char)value);
        if (byteCount <= 128)
        {
            _mm_storeu_si128((__m128i*)(dst + byteCount - 16), vec);

            while (byteCount > 16)
            {
                _mm_storeu_si128((__m128i*)dst, vec);
                dst += 16;
                byteCount -= 16;
            }
        }
        else
        {
            USize padding = (16 - (((USize)dst) & 15)) & 15;

            if (padding > 0)
            {
                _mm_storeu_si128((__m128i*)dst, vec);
                dst += padding;
                byteCount -= padding;
            }

            while (byteCount >= 128)
            {
                _mm_store_si128(((__m128i*)dst) + 0, vec);
                _mm_store_si128(((__m128i*)dst) + 1, vec);
                _mm_store_si128(((__m128i*)dst) + 2, vec);
                _mm_store_si128(((__m128i*)dst) + 3, vec);
                _mm_store_si128(((__m128i*)dst) + 4, vec);
                _mm_store_si128(((__m128i*)dst) + 5, vec);
                _mm_store_si128(((__m128i*)dst) + 6, vec);
                _mm_store_si128(((__m128i*)dst) + 7, vec);
                dst += 128;

                byteCount -= 128;
            }

            goto tail;
        }
    }
}

ACTIAS_END_C
