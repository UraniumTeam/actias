/*
 * Actias SDK System
 *
 * Copyright (c) 2024 UraniumTeam
 */

#pragma once
#include <Actias/System/Platform.h>

ACTIAS_BEGIN_C

//! \brief Inline function to find length of a C-style string with null termination.
//!
//! \param pString - The string to find the length of.
ACTIAS_NO_BUILTIN(strlen)
inline static USize ACTIAS_ABI ActiasInlineStringLength(const char* pString)
{
    const UInt64* wordIter;
    const char* charIter;
    for (charIter = pString; ((UInt64)(charIter) & (sizeof(UInt64) - 1)) != 0; ++charIter)
    {
        if (*charIter == '\0')
        {
            return charIter - pString;
        }
    }

    wordIter = (UInt64*)charIter;

    const UInt64 himagic = UINT64_C(0x8080808080808080);
    const UInt64 lomagic = UINT64_C(0x0101010101010101);

    while (1)
    {
        const UInt64 wordValue = *wordIter++;

        if (((wordValue - lomagic) & ~wordValue & himagic) != 0)
        {
            const char* cp = (const char*)(wordIter - 1);

            if (cp[0] == 0)
                return cp - pString;
            if (cp[1] == 0)
                return cp - pString + 1;
            if (cp[2] == 0)
                return cp - pString + 2;
            if (cp[3] == 0)
                return cp - pString + 3;
            if (cp[4] == 0)
                return cp - pString + 4;
            if (cp[5] == 0)
                return cp - pString + 5;
            if (cp[6] == 0)
                return cp - pString + 6;
            if (cp[7] == 0)
                return cp - pString + 7;
        }
    }
}

ACTIAS_END_C
