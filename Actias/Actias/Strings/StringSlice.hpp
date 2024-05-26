#pragma once
#include <Actias/Containers/List.hpp>
#include <Actias/Strings/StringBase.hpp>
#include <Actias/Strings/Unicode.hpp>
#include <Actias/Utils/Result.hpp>
#include <cassert>
#include <codecvt>
#include <locale>
#include <ostream>
#include <string>
#include <type_traits>

namespace Actias
{
    enum class ParseErrorCode : UInt32
    {
        None,
        Overflow,
        InvalidSyntax,
        UnexpectedEnd
    };

    class ParseError final
    {
        // In case of 32-bit data
        // | 4-bit error code | 28-bit position |
        UInt32 m_Data;

        inline static constexpr UInt32 PositionByteCount = sizeof(UInt32) - 4;

    public:
        inline ParseError(ParseErrorCode code) noexcept // NOLINT(google-explicit-constructor)
            : ParseError(code, 0)
        {
        }

        inline ParseError(ParseErrorCode code, USize position) noexcept
            : m_Data(static_cast<UInt32>(position) | (static_cast<UInt32>(code) << PositionByteCount))
        {
        }

        [[nodiscard]] inline ParseErrorCode GetCode() const noexcept
        {
            return static_cast<ParseErrorCode>(m_Data >> PositionByteCount);
        }

        [[nodiscard]] inline UInt32 GetPosition() const noexcept
        {
            return m_Data & MakeMask(PositionByteCount, 0u);
        }

        inline friend bool operator==(ParseError parseError, ParseErrorCode code) noexcept
        {
            return parseError.GetCode() == code;
        }

        inline friend bool operator!=(ParseError parseError, ParseErrorCode code) noexcept
        {
            return parseError.GetCode() != code;
        }

        inline friend bool operator==(ParseError lhs, ParseError rhs) noexcept
        {
            return lhs.GetCode() == rhs.GetCode() && lhs.GetPosition() == rhs.GetPosition();
        }

        inline friend bool operator!=(ParseError lhs, ParseError rhs) noexcept
        {
            return lhs.GetCode() != rhs.GetCode() && lhs.GetPosition() != rhs.GetPosition();
        }
    };

    template<class T>
    struct ValueParser : std::false_type
    {
    };

    //! \brief A slice of String.
    class StringSlice final
    {
        const TChar* m_Data;
        size_t m_Size;

        ParseError TryToIntImpl(Int64& result) const;
        ParseError TryToUIntImpl(UInt64& result) const;

        ParseError TryToFloatImpl(Float64& result) const;

        template<class T>
        inline static constexpr bool is_signed_integer_v =
            std::is_signed_v<T> && std::is_integral_v<T> && !std::is_same_v<T, bool>;
        template<class T>
        inline static constexpr bool is_unsigned_integer_v =
            std::is_unsigned_v<T> && std::is_integral_v<T> && !std::is_same_v<T, bool>;

        template<class TInt>
        [[nodiscard]] inline std::enable_if_t<is_signed_integer_v<TInt>, ParseError> ParseImpl(TInt& result) const noexcept
        {
            Int64 temp;
            auto ret = TryToIntImpl(temp);
            if (ret != ParseErrorCode::None)
            {
                return ret;
            }

            result = static_cast<TInt>(temp);
            if (result != temp)
            {
                return ParseErrorCode::Overflow;
            }

            return ParseErrorCode::None;
        }

        template<class TInt>
        [[nodiscard]] inline std::enable_if_t<is_unsigned_integer_v<TInt>, ParseError> ParseImpl(TInt& result) const noexcept
        {
            UInt64 temp;
            auto ret = TryToUIntImpl(temp);
            if (ret != ParseErrorCode::None)
            {
                return ret;
            }

            result = static_cast<TInt>(temp);
            if (result != temp)
            {
                return ParseErrorCode::Overflow;
            }

            return ParseErrorCode::None;
        }

        template<class TFloat>
        [[nodiscard]] inline std::enable_if_t<std::is_floating_point_v<TFloat>, ParseError> ParseImpl(
            TFloat& result) const noexcept
        {
            Float64 temp;
            auto ret = TryToFloatImpl(temp);
            result   = static_cast<TFloat>(temp);
            return ret;
        }

        template<class TBool>
        [[nodiscard]] inline std::enable_if_t<std::is_same_v<TBool, bool>, ParseError> ParseImpl(TBool& result) const noexcept;

        template<class T>
        [[nodiscard]] inline std::enable_if_t<ValueParser<T>::value, ParseError> ParseImpl(T& result) const noexcept
        {
            return ValueParser<T>::TryConvert(*this, result);
        }

    public:
        ACTIAS_RTTI_Struct(StringSlice, "DCBAE48D-8751-4F0C-96F9-99866394482B");

        using Iterator = Internal::StrIterator;

        inline constexpr StringSlice() noexcept
            : m_Data(nullptr)
            , m_Size(0)
        {
        }

        inline constexpr StringSlice(const TChar* data, USize size) noexcept
            : m_Data(data)
            , m_Size(size)
        {
        }

        inline constexpr StringSlice(const std::string_view& stringView) noexcept
            : m_Data(stringView.data())
            , m_Size(stringView.size())
        {
        }

        inline constexpr StringSlice(const TChar* data) noexcept
            : m_Data(data)
            , m_Size(data == nullptr ? 0 : std::char_traits<TChar>::length(data))
        {
        }

        template<USize S>
        inline constexpr StringSlice(const TChar (&data)[S]) noexcept
            : m_Data(data)
            , m_Size(S)
        {
        }

        inline StringSlice(Iterator begin, Iterator end) noexcept
            : m_Data(begin.m_Iter)
            , m_Size(end.m_Iter - begin.m_Iter)
        {
        }

        [[nodiscard]] inline constexpr const TChar* Data() const noexcept
        {
            return m_Data;
        }

        [[nodiscard]] inline constexpr size_t Size() const noexcept
        {
            return m_Size;
        }

        // O(N)
        [[nodiscard]] inline size_t Length() const noexcept
        {
            return UTF8::Length(Data(), Size());
        }

        inline StringSlice operator()(size_t beginIndex, size_t endIndex) const
        {
            auto begin = Data();
            auto end   = Data();
            UTF8::Advance(begin, beginIndex);
            UTF8::Advance(end, endIndex);
            return StringSlice(begin, end - begin);
        }

        // O(1)
        [[nodiscard]] inline TChar ByteAt(size_t index) const
        {
            ACTIAS_Assert(index < Size(), "Invalid index");
            return Data()[index];
        }

        // O(N)
        [[nodiscard]] inline TCodepoint CodePointAt(size_t index) const
        {
            return Str::CodepointAt(Data(), Size(), index);
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

        [[nodiscard]] inline bool Contains(TCodepoint search) const noexcept
        {
            return FindFirstOf(search) != end();
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

        [[nodiscard]] inline List<StringSlice> Split(TCodepoint c = ' ', IAllocator* allocator = nullptr) const
        {
            List<StringSlice> result{ allocator };
            Split(result, c);
            return result;
        }

        inline void Split(List<StringSlice>& result, TCodepoint c = ' ') const
        {
            auto current = begin();
            while (current != end())
            {
                auto cPos = FindFirstOf(current, c);
                result.Emplace(current.m_Iter, cPos.m_Iter - current.m_Iter);
                current = cPos;
                if (current != end())
                {
                    ++current;
                }
            }
        }

        [[nodiscard]] inline List<StringSlice> SplitLines(IAllocator* allocator = nullptr) const
        {
            List<StringSlice> result{ allocator };
            SplitLines(result);
            return result;
        }

        inline void SplitLines(List<StringSlice>& result) const
        {
            auto current = begin();
            while (current != end())
            {
                auto cPos = FindFirstOf(current, '\n');
                auto line = StringSlice{ current.m_Iter, static_cast<USize>(cPos.m_Iter - current.m_Iter) }.StripRight("\r");
                result.Emplace(line);
                current = cPos;
                if (current != end())
                {
                    ++current;
                }
            }
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

        [[nodiscard]] inline int Compare(const StringSlice& other) const noexcept
        {
            return UTF8::Compare(Data(), other.Data(), Size(), other.Size());
        }

        [[nodiscard]] inline bool IsEqualTo(const StringSlice& other, bool caseSensitive = true) const noexcept
        {
            return UTF8::AreEqual(Data(), other.Data(), Size(), other.Size(), caseSensitive);
        }

        template<class T>
        [[nodiscard]] inline Result<T, ParseError> Parse() const
        {
            T result;
            auto err = ParseImpl(result);
            if (err == ParseErrorCode::None)
            {
                return result;
            }

            return Err(err);
        }

        [[nodiscard]] inline explicit operator UUID() const noexcept
        {
            return UUID(Data());
        }

        [[nodiscard]] inline Iterator begin() const noexcept
        {
            return Iterator(Data());
        }

        [[nodiscard]] inline Iterator end() const noexcept
        {
            return Iterator(Data() + Size());
        }
    };

    inline bool operator==(const StringSlice& lhs, const StringSlice& rhs) noexcept
    {
        return lhs.Size() == rhs.Size() && lhs.Compare(rhs) == 0;
    }

    inline bool operator!=(const StringSlice& lhs, const StringSlice& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    inline bool operator<(const StringSlice& lhs, const StringSlice& rhs) noexcept
    {
        return lhs.Compare(rhs) < 0;
    }

    inline bool operator>(const StringSlice& lhs, const StringSlice& rhs) noexcept
    {
        return lhs.Compare(rhs) > 0;
    }

    inline bool operator<=(const StringSlice& lhs, const StringSlice& rhs) noexcept
    {
        return lhs.Size() == rhs.Size() && lhs.Compare(rhs) <= 0;
    }

    inline bool operator>=(const StringSlice& lhs, const StringSlice& rhs) noexcept
    {
        return lhs.Size() == rhs.Size() && lhs.Compare(rhs) >= 0;
    }

    template<class TBool>
    std::enable_if_t<std::is_same_v<TBool, bool>, ParseError> StringSlice::ParseImpl(TBool& result) const noexcept
    {
        if (*this == "true" || *this == "1")
        {
            result = true;
            return ParseErrorCode::None;
        }
        if (*this == "false" || *this == "0")
        {
            result = false;
            return ParseErrorCode::None;
        }

        return ParseErrorCode::InvalidSyntax;
    }

    template<>
    struct ValueParser<UUID> : std::true_type
    {
        inline static ParseError TryConvert(const StringSlice& str, UUID& result)
        {
            if (str.Length() != 36)
            {
                return { ParseErrorCode::UnexpectedEnd, str.Length() - 1 };
            }

            return UUID::TryParse(str.Data(), result, false) ? ParseErrorCode::None : ParseErrorCode::InvalidSyntax;
        }
    };
} // namespace Actias

namespace std
{
    inline ostream& operator<<(ostream& stream, Actias::StringSlice str)
    {
        return stream << std::string_view(str.Data(), str.Size());
    }

    template<>
    struct hash<Actias::StringSlice>
    {
        inline size_t operator()(const Actias::StringSlice& str) const noexcept
        {
            return Actias::Str::Hash(str.Data(), str.Size());
        }
    };
} // namespace std
