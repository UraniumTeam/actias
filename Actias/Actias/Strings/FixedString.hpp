#pragma once
#include <Actias/Base/Base.hpp>
#include <Actias/Containers/ArraySlice.hpp>
#include <Actias/Strings/StringSlice.hpp>

namespace Actias
{
    //! \brief String class that never allocates and uses UTF-8 encoding.
    template<USize TCapacity>
    class FixedString final
    {
        static_assert(TCapacity <= (1 << 24));

        TChar m_Data[TCapacity];
        UInt32 m_Zero : 8;
        UInt32 m_Size : 24;

    public:
        ACTIAS_RTTI_Struct(FixedString, "A8018F6C-0B8B-451C-98F5-5CCC9AA148A1");

        using Iterator = Internal::StrIterator;

        inline FixedString() noexcept
            : m_Zero(0)
            , m_Size(0)
        {
            *m_Data = 0;
        }

        inline FixedString(USize length, TChar value) noexcept
            : m_Zero(0)
        {
            ACTIAS_AssertDebug(length <= TCapacity);
            ActiasInlineSetMemory(m_Data, value, length);
            m_Size         = static_cast<UInt32>(length);
            m_Data[m_Size] = 0;
        }

        inline FixedString(const TChar* str, USize byteSize) noexcept
            : m_Zero(0)
        {
            ACTIAS_AssertDebug(byteSize <= TCapacity);
            ActiasInlineCopyMemory(m_Data, str, byteSize);
            m_Size         = static_cast<UInt32>(byteSize);
            m_Data[m_Size] = 0;
        }

        inline FixedString(StringSlice slice) noexcept
            : FixedString(slice.Data(), slice.Size())
        {
        }

        inline FixedString(const TChar* str) noexcept
            : FixedString(str, Str::ByteLength(str))
        {
        }

        inline FixedString(Iterator begin, Iterator end) noexcept
            : FixedString(begin.m_Iter, end.m_Iter - begin.m_Iter)
        {
        }

        [[nodiscard]] inline const TChar* Data() const noexcept
        {
            return m_Data;
        }

        [[nodiscard]] inline TChar* Data() noexcept
        {
            return m_Data;
        }

        // O(1)
        [[nodiscard]] inline TChar ByteAt(USize index) const
        {
            ACTIAS_Assert(index < Size(), "Invalid index");
            return Data()[index];
        }

        // O(N)
        [[nodiscard]] inline TCodepoint CodePointAt(USize index) const
        {
            auto begin    = Data();
            auto end      = begin + Size() + 1;
            USize cpIndex = 0;
            for (auto iter = begin; iter != end; UTF8::Decode(iter), ++cpIndex)
            {
                if (cpIndex == index)
                {
                    return UTF8::PeekDecode(iter);
                }
            }

            ACTIAS_Unreachable("Invalid index");
            return 0;
        }

        // O(1)
        [[nodiscard]] inline USize Size() const noexcept
        {
            return m_Size;
        }

        [[nodiscard]] inline bool Empty() const noexcept
        {
            return Size() == 0;
        }

        // O(1)
        [[nodiscard]] inline USize Capacity() const noexcept
        {
            return TCapacity;
        }

        // O(N)
        [[nodiscard]] inline USize Length() const noexcept
        {
            auto ptr = Data();
            return UTF8::Length(ptr, Size());
        }

        inline operator StringSlice() const noexcept
        {
            return { Data(), Size() };
        }

        inline StringSlice operator()(USize beginIndex, USize endIndex) const
        {
            auto begin = Data();
            auto end   = Data();
            UTF8::Advance(begin, beginIndex);
            UTF8::Advance(end, endIndex);
            return StringSlice(begin, end - begin);
        }

        [[nodiscard]] inline StringSlice ASCIISubstring(USize beginIndex, USize endIndex) const
        {
            auto begin = Data() + beginIndex;
            auto end   = Data() + endIndex;
            return StringSlice(begin, end - begin);
        }

        inline void Clear() noexcept
        {
            m_Size  = 0;
            *m_Data = 0;
        }

        inline FixedString& Append(const TChar* str, USize count)
        {
            ACTIAS_AssertDebug(count == 0 || str != nullptr, "Couldn't append more than 0 chars from a null string");
            ACTIAS_AssertDebug(count + Size() <= Capacity());
            if (count == 0)
            {
                return *this;
            }

            ActiasInlineCopyMemory(Data() + Size(), str, count);
            m_Size += static_cast<UInt32>(count);
            m_Data[m_Size] = 0;
            return *this;
        }

        inline FixedString& Append(StringSlice str)
        {
            return Append(str.Data(), str.Size());
        }

        inline FixedString& Append(TChar cp)
        {
            ACTIAS_AssertDebug(Size() < Capacity());
        }

        inline FixedString& Append(const TChar* str)
        {
            return Append(str, Str::ByteLength(str));
        }

        inline FixedString& operator+=(StringSlice str)
        {
            return Append(str);
        }

        inline FixedString& operator/=(const StringSlice& str)
        {
            Append(ACTIAS_PATH_SEPARATOR);
            return Append(str);
        }

        inline friend FixedString operator+(const FixedString& lhs, StringSlice rhs)
        {
            FixedString t{ lhs };
            t += rhs;
            return t;
        }

        inline friend FixedString operator/(const FixedString& lhs, StringSlice rhs)
        {
            FixedString t{ lhs };
            t /= rhs;
            return t;
        }

        [[nodiscard]] inline Iterator FindFirstOf(Iterator start, TCodepoint search) const noexcept
        {
            const USize size  = Size();
            const TChar* data = Data();
            ACTIAS_AssertDebug(start.m_Iter >= data && start.m_Iter <= data + size);

            const USize searchSize = data + size - start.m_Iter;
            return Str::FindFirstOf(start.m_Iter, searchSize, search);
        }

        [[nodiscard]] inline Iterator FindFirstOf(TCodepoint search) const noexcept
        {
            return FindFirstOf(begin(), search);
        }

        [[nodiscard]] inline Iterator FindLastOf(TCodepoint search) const noexcept
        {
            return Str::FindLastOf(Data(), Size(), search);
        }

        [[nodiscard]] inline List<StringSlice> Split(TCodepoint c = ' ', IAllocator* allocator = nullptr) const
        {
            List<StringSlice> result{ allocator };
            Split(result, c);
            return result;
        }

        inline void Split(List<StringSlice>& result, TCodepoint c = ' ') const
        {
            StringSlice(Data(), Size()).Split(result, c);
        }

        [[nodiscard]] inline List<StringSlice> SplitLines(IAllocator* allocator = nullptr) const
        {
            List<StringSlice> result{ allocator };
            SplitLines(result);
            return result;
        }

        inline void SplitLines(List<StringSlice>& result) const
        {
            StringSlice(Data(), Size()).SplitLines(result);
        }

        [[nodiscard]] inline StringSlice StripLeft(StringSlice chars = "\n\r\t ") const noexcept
        {
            return { Iterator{ Str::StripLeft(Data(), Size(), chars.Data(), chars.Size()) }, end() };
        }

        [[nodiscard]] inline StringSlice StripRight(StringSlice chars = "\n\r\t ") const noexcept
        {
            return { begin(), Iterator{ Str::StripRight(Data(), Size(), chars.Data(), chars.Size()) } };
        }

        [[nodiscard]] inline StringSlice Strip(StringSlice chars = "\n\r\t ") const noexcept
        {
            return StripLeft(chars).StripRight(chars);
        }

        [[nodiscard]] inline Int32 Compare(const StringSlice& other) const noexcept
        {
            return UTF8::Compare(Data(), other.Data(), Size(), other.Size());
        }

        [[nodiscard]] inline bool IsEqualTo(const StringSlice& other, bool caseSensitive = true) const noexcept
        {
            return UTF8::AreEqual(Data(), other.Data(), Size(), other.Size(), caseSensitive);
        }

        [[nodiscard]] inline bool StartsWith(StringSlice prefix, bool caseSensitive = true) const noexcept
        {
            if (prefix.Size() > Size())
            {
                return false;
            }

            return UTF8::AreEqual(Data(), prefix.Data(), prefix.Size(), prefix.Size(), caseSensitive);
        }

        [[nodiscard]] inline bool EndsWith(StringSlice suffix, bool caseSensitive = true) const noexcept
        {
            if (suffix.Size() > Size())
            {
                return false;
            }

            return UTF8::AreEqual(Data() + Size() - suffix.Size(), suffix.Data(), suffix.Size(), suffix.Size(), caseSensitive);
        }

        template<class T>
        [[nodiscard]] inline Result<T, ParseError> Parse()
        {
            return StringSlice(Data(), Size()).Parse<T>();
        }

        [[nodiscard]] inline WString ToWideString() const
        {
            WString result;
            result.reserve(Length());
            for (TCodepoint cp : *this)
            {
                result += static_cast<wchar_t>(cp);
            }

            return result;
        }

        [[nodiscard]] inline static FixedString Join(const StringSlice& separator, const ArraySlice<StringSlice>& strings)
        {
            FixedString result;
            for (USize i = 0; i < strings.Length(); ++i)
            {
                result.Append(strings[i]);
                if (i != strings.Length() - 1)
                {
                    result.Append(separator);
                }
            }

            return result;
        }

        [[nodiscard]] inline explicit operator UUID() const noexcept
        {
            return UUID(Data());
        }

        [[nodiscard]] inline Iterator begin() const noexcept
        {
            auto ptr = Data();
            return Iterator(ptr);
        }

        [[nodiscard]] inline Iterator end() const noexcept
        {
            auto ptr  = Data();
            auto size = Size();
            return Iterator(ptr + size);
        }
    };

    template<USize TCapacity>
    struct ValueParser<FixedString<TCapacity>> : std::true_type
    {
        inline static ParseError TryConvert(const StringSlice& str, FixedString<TCapacity>& result)
        {
            result = str;
            return ParseErrorCode::None;
        }
    };

    using FixStr32  = FixedString<32>;
    using FixStr64  = FixedString<64>;
    using FixStr128 = FixedString<128>;
    using FixStr256 = FixedString<256>;
    using FixStr512 = FixedString<512>;

    using FixStr = FixStr256;

    template<USize TCapacity>
    struct Hash<Actias::FixedString<TCapacity>>
    {
        inline size_t operator()(const Actias::FixedString<TCapacity>& str) const noexcept
        {
            return Actias::Str::Hash(str.Data(), str.Size());
        }
    };
} // namespace Actias
