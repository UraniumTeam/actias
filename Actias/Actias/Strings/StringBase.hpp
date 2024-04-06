#pragma once
#include <Actias/Base/Base.hpp>
#include <Actias/Strings/Unicode.hpp>

namespace Actias
{
    using UTF8::TChar;
    using UTF8::TCharTraits;
    using UTF8::TCodepoint;
    using UTF8::TCodepointTraits;

    class Str;
    class String;
    class StringSlice;

    template<USize TCapacity>
    class FixedString;

    namespace Internal
    {
        class StrIterator final
        {
            friend class Actias::Str;
            friend class Actias::String;
            friend class Actias::StringSlice;

            template<USize TCapacity>
            friend class Actias::FixedString;

            const TChar* m_Iter;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = TCodepoint;
            using pointer           = const TCodepoint*;
            using reference         = const TCodepoint&;

            inline StrIterator(const TChar* iter)
                : m_Iter(iter)
            {
            }

            inline value_type operator*() const
            {
                return UTF8::PeekDecode(m_Iter);
            }

            inline StrIterator& operator++()
            {
                UTF8::Decode(m_Iter);
                return *this;
            }

            inline StrIterator operator++(int)
            {
                StrIterator t = *this;
                ++(*this);
                return t;
            }

            inline StrIterator& operator--()
            {
                UTF8::DecodePrior(m_Iter);
                return *this;
            }

            inline StrIterator operator--(int)
            {
                StrIterator t = *this;
                UTF8::DecodePrior(m_Iter);
                return t;
            }

            inline friend StrIterator operator+(StrIterator lhs, Int32 rhs)
            {
                if (rhs > 0)
                {
                    while (rhs--)
                        ++lhs;
                }
                else
                {
                    while (rhs--)
                        --lhs;
                }

                return lhs;
            }

            inline friend StrIterator operator-(const StrIterator& lhs, Int32 rhs)
            {
                return lhs + (-rhs);
            }

            inline friend bool operator==(const StrIterator& a, const StrIterator& b)
            {
                return a.m_Iter == b.m_Iter;
            };

            inline friend bool operator!=(const StrIterator& a, const StrIterator& b)
            {
                return a.m_Iter != b.m_Iter;
            };
        };
    } // namespace Internal

    class Str final
    {
        using Iter = Internal::StrIterator;

    public:
        [[nodiscard]] inline static USize ByteLength(const TChar* str) noexcept
        {
            return strlen(str);
        }

        [[nodiscard]] inline static USize Length(const TChar* str, USize byteSize) noexcept
        {
            return UTF8::Length(str, byteSize);
        }

        [[nodiscard]] inline static USize Length(const TChar* str) noexcept
        {
            return UTF8::Length(str, ByteLength(str));
        }

        [[nodiscard]] inline static Int32 ByteCompare(const TChar* lhs, const TChar* rhs) noexcept
        {
            return strcmp(lhs, rhs);
        }

        [[nodiscard]] inline static Int32 Compare(const TChar* lhs, const TChar* rhs, size_t length1, size_t length2) noexcept
        {
            return UTF8::Compare(lhs, rhs, length1, length2);
        }

        [[nodiscard]] inline static Int32 Compare(const TChar* lhs, const TChar* rhs) noexcept
        {
            return Compare(lhs, rhs, ByteLength(lhs), ByteLength(rhs));
        }

        [[nodiscard]] inline static TCodepoint CodepointAt(const TChar* str, USize size, USize index)
        {
            const TChar* begin = str;
            const TChar* end   = begin + size + 1;
            size_t cpIndex     = 0;
            for (auto iter = begin; iter != end; UTF8::Decode(iter), ++cpIndex)
            {
                if (cpIndex == index)
                    return UTF8::PeekDecode(iter);
            }

            ACTIAS_Unreachable("");
            return 0;
        }

        [[nodiscard]] inline static const TChar* FindFirstOf(const TChar* str, USize size, TCodepoint search) noexcept
        {
            const Iter e{ str + size };
            for (Iter iter = str; iter != e; ++iter)
            {
                if (*iter == search)
                {
                    return iter.m_Iter;
                }
            }

            return e.m_Iter;
        }

        [[nodiscard]] inline static const TChar* FindLastOf(const TChar* str, USize size, TCodepoint search) noexcept
        {
            const Iter e{ str + size };
            Iter result = e;
            for (Iter iter = str; iter != e; ++iter)
            {
                if (*iter == search)
                {
                    result = iter;
                }
            }

            return result.m_Iter;
        }

        [[nodiscard]] inline static bool Contains(const TChar* str, USize size, TCodepoint search) noexcept
        {
            return FindFirstOf(str, size, search) != str + size;
        }

        [[nodiscard]] inline static const TChar* StripLeft(const TChar* str, USize size, const TChar* chars,
                                                           USize charsSize) noexcept
        {
            const Iter endIter{ str + size };
            Iter result{ str };
            for (Iter iter = str; iter != endIter; ++iter)
            {
                if (!Contains(chars, charsSize, *iter))
                {
                    break;
                }

                result = iter;
                ++result;
            }

            return result.m_Iter;
        }

        [[nodiscard]] inline static const TChar* StripRight(const TChar* str, USize size, const TChar* chars,
                                                            USize charsSize) noexcept
        {
            const Iter beginIter{ str };
            Iter result{ str + size };
            Iter iter = result;
            --iter;
            for (; iter != beginIter; --iter)
            {
                if (!Contains(chars, charsSize, *iter))
                {
                    break;
                }

                result = iter;
            }

            return result.m_Iter;
        }
    };

} // namespace Actias
