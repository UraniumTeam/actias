#pragma once
#include <Actias/Base/Base.hpp>
#include <Actias/RTTI/RTTI.hpp>
#include <Actias/System/Unicode.h>
#include <cassert>
#include <cstdint>
#include <string>

namespace Actias::UTF8
{
    using TChar      = char;
    using TCodepoint = char32_t;

    inline TCodepoint Decode(const TChar*& it) noexcept
    {
        UInt32 c;
        int e;
        void* input = reinterpret_cast<void*>(const_cast<TChar*>(it));
        it          = static_cast<const TChar*>(utf8_decode(input, &c, &e));
        ACTIAS_Assert(e == 0, "Invalid unicode");
        return c;
    }

    inline TCodepoint DecodePrior(const TChar*& it) noexcept
    {
        UInt32 c          = 0;
        int e             = 1;
        const TChar* iter = it;
        int maxlen        = 3;
        while (e && maxlen--)
        {
            void* input                 = reinterpret_cast<void*>(const_cast<TChar*>(--iter));
            [[maybe_unused]] void* temp = utf8_decode(input, &c, &e);
        }

        ACTIAS_Assert(e == 0, "Invalid prior unicode");
        it = iter;
        return c;
    }

    inline TCodepoint PeekDecode(const TChar* it) noexcept
    {
        return Decode(it);
    }

    inline int Compare(const TChar* lhs, const TChar* rhs, size_t length1, size_t length2) noexcept
    {
        size_t length = std::min(length1, length2);
        for (; 0 < length; --length, Decode(lhs), Decode(rhs))
        {
            if (*lhs != *rhs)
            {
                return PeekDecode(lhs) < PeekDecode(rhs) ? -1 : +1;
            }
        }

        if (length1 == length2)
        {
            return 0;
        }

        return length1 < length2 ? -1 : 1;
    }

    inline bool AreEqual(const TChar* lhs, const TChar* rhs, size_t length1, size_t length2, bool caseSensitive = true) noexcept
    {
        if (length1 != length2)
        {
            return false;
        }

        auto normalizeChar = [caseSensitive](TCodepoint cp) -> TCodepoint {
            if (caseSensitive)
            {
                return cp;
            }

            return std::isalpha(static_cast<char>(cp)) ? std::tolower(static_cast<char>(cp)) : cp;
        };

        for (; 0 < length1; --length1, Decode(lhs), Decode(rhs))
        {
            if (normalizeChar(*lhs) != normalizeChar(*rhs))
            {
                return false;
            }
        }

        return true;
    }

    inline size_t Length(const TChar* str, size_t byteLen) noexcept
    {
        size_t result = 0;
        while (byteLen-- && Decode(str))
            ++result;
        return result;
    }

    inline void Advance(const TChar*& str, size_t n) noexcept
    {
        while (n-- && Decode(str))
        {
        }
    }

    inline bool Valid(const TChar* str) noexcept
    {
        UInt32 c;
        int e = 0;
        while (*str)
        {
            void* input = reinterpret_cast<void*>(const_cast<TChar*>(str));
            str         = static_cast<const TChar*>(utf8_decode(input, &c, &e));
        }
        return e == 0;
    }
} // namespace Actias::UTF8
