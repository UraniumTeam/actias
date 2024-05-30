#pragma once
#include <Actias/Base/Base.hpp>
#include <Actias/Containers/ArraySlice.hpp>
#include <Actias/Strings/StringSlice.hpp>

namespace Actias
{
    //! \brief String class that uses HeapAllocator and UTF-8 encoding.
    class String final
    {
        struct LongMode
        {
            size_t Cap;
            size_t Size;
            TChar* Data;
        };

        inline static constexpr size_t MinCapacity = (sizeof(LongMode) - 1) / sizeof(TChar);

        struct ShortMode
        {
            union
            {
                UInt8 Size;
                [[maybe_unused]] TChar Lx;
            };
            TChar Data[MinCapacity];
        };

        union Ulx
        {
            [[maybe_unused]] LongMode Lm;
            [[maybe_unused]] ShortMode Sm;
        };

        inline static constexpr size_t WordCount = sizeof(Ulx) / sizeof(size_t);

        struct Rep
        {
            union
            {
                LongMode L;
                ShortMode S;
                size_t R[WordCount];
            };
        } m_Data;

        [[nodiscard]] inline bool IsLong() const noexcept
        {
            return m_Data.S.Size & 1;
        }

        [[nodiscard]] inline size_t GetLCap() const noexcept
        {
            return m_Data.L.Cap & static_cast<size_t>(~1);
        }

        inline void SetLCap(size_t cap) noexcept
        {
            m_Data.L.Cap = cap | 1;
        }

        inline void SetSSize(size_t size) noexcept
        {
            m_Data.S.Size = static_cast<UInt8>(size << 1);
        }

        inline void SetLSize(size_t size) noexcept
        {
            m_Data.L.Size = size;
        }

        inline void SetSize(size_t size) noexcept
        {
            if (IsLong())
                SetLSize(size);
            else
                SetSSize(size);
        }

        inline void Zero() noexcept
        {
            for (auto& i : m_Data.R)
            {
                i = 0;
            }
        }

        inline static constexpr size_t Alignment = 16;

        inline static size_t Recommend(size_t s) noexcept
        {
            if (s < MinCapacity)
            {
                return MinCapacity - 1;
            }

            size_t guess = Actias::AlignUp<Alignment / sizeof(TChar)>(s + 1) - 1;

            if (guess == MinCapacity)
            {
                ++guess;
            }

            return guess;
        }

        inline static TChar* Allocate(size_t s) noexcept
        {
            return static_cast<TChar*>(SystemAllocator::Get()->Allocate(s, Alignment));
        }

        inline static void Deallocate(TChar* c) noexcept
        {
            SystemAllocator::Get()->Deallocate(c);
        }

        inline static void CopyData(TChar* dest, const TChar* src, size_t size) noexcept
        {
            ActiasInlineCopyMemory(dest, src, size);
        }

        inline static void SetData(TChar* dest, TChar value, size_t size) noexcept
        {
            ActiasInlineSetMemory(dest, static_cast<UInt8>(value), size);
        }

        inline TChar* InitImpl(size_t size) noexcept
        {
            TChar* newPtr;
            if (size < MinCapacity)
            {
                SetSSize(size);
                newPtr = m_Data.S.Data;
            }
            else
            {
                size_t newCap = Recommend(size);
                newPtr        = Allocate(newCap + 1);
                m_Data.L.Data = newPtr;
                SetLCap(newCap + 1);
                SetLSize(size);
            }

            return newPtr;
        }

        inline void Init(const TChar* str, size_t size) noexcept
        {
            TChar* ptr = InitImpl(size);
            CopyData(ptr, str, size);
            ptr[size] = '\0';
        }

        inline void Init(size_t count, TChar value) noexcept
        {
            TChar* ptr = InitImpl(count);
            SetData(ptr, value, count);
            ptr[count] = '\0';
        }

        inline void GrowAndReplace(size_t oldCap, size_t deltaCap, size_t oldSize, size_t copyCount, size_t delCount,
                                   size_t addCount, const TChar* newChars)
        {
            TChar* oldData = Data();
            size_t cap     = Recommend(std::max(oldCap + deltaCap, 2 * oldCap));
            TChar* newData = Allocate(cap + 1);
            if (copyCount)
                CopyData(newData, oldData, copyCount);
            if (addCount)
                CopyData(newData + copyCount, newChars, addCount);
            size_t copySize = oldSize - delCount - copyCount;
            if (copySize)
                CopyData(newData + copyCount + addCount, oldData + delCount, copySize);
            if (oldCap + 1 != MinCapacity)
                Deallocate(oldData);
            m_Data.L.Data = (newData);
            SetLCap(cap + 1);
            oldSize = copyCount + addCount + copySize;
            SetLSize(oldSize);
            newData[oldSize] = '\0';
        }

    public:
        ACTIAS_RTTI_Struct(String, "340DE9B8-EC27-4248-9E0D-7D40330E8EBA");

        using Iterator = Internal::StrIterator;

        inline String() noexcept
        {
            Zero();
        }

        inline String(const String& other) noexcept
        {
            if (!other.IsLong())
                m_Data = other.m_Data;
            else
                Init(other.m_Data.L.Data, other.m_Data.L.Size);
        }

        inline String& operator=(const String& other) noexcept
        {
            Clear();
            Shrink();
            if (!other.IsLong())
                m_Data = other.m_Data;
            else
                Init(other.m_Data.L.Data, other.m_Data.L.Size);
            return *this;
        }

        inline String(String&& other) noexcept
            : m_Data(other.m_Data)
        {
            other.Zero();
        }

        inline String& operator=(String&& other) noexcept
        {
            Clear();
            Shrink();
            m_Data = other.m_Data;
            other.Zero();
            return *this;
        }

        inline String(size_t length, TChar value) noexcept
        {
            Init(length, value);
        }

        inline String(const TChar* str, size_t byteSize) noexcept
        {
            Init(str, byteSize);
        }

        inline String(StringSlice slice) noexcept
        {
            Init(slice.Data(), slice.Size());
        }

        inline String(const TChar* str) noexcept
            : String(str, Str::ByteLength(str))
        {
        }

        inline String(Iterator begin, Iterator end) noexcept
            : String(begin.m_Iter, end.m_Iter - begin.m_Iter)
        {
        }

        inline ~String() noexcept
        {
            if (IsLong())
            {
                Deallocate(m_Data.L.Data);
            }
        }

        [[nodiscard]] inline const TChar* Data() const noexcept
        {
            return IsLong() ? m_Data.L.Data : m_Data.S.Data;
        }

        [[nodiscard]] inline TChar* Data() noexcept
        {
            return IsLong() ? m_Data.L.Data : m_Data.S.Data;
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
            auto begin     = Data();
            auto end       = begin + Size() + 1;
            size_t cpIndex = 0;
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
        [[nodiscard]] inline size_t Size() const noexcept
        {
            return IsLong() ? m_Data.L.Size : m_Data.S.Size >> 1;
        }

        [[nodiscard]] inline bool Empty() const noexcept
        {
            return Size() == 0;
        }

        // O(1)
        [[nodiscard]] inline size_t Capacity() const noexcept
        {
            return (IsLong() ? GetLCap() : MinCapacity) - 1;
        }

        // O(N)
        [[nodiscard]] inline size_t Length() const noexcept
        {
            auto ptr = Data();
            return UTF8::Length(ptr, Size());
        }

        inline operator StringSlice() const noexcept
        {
            return { Data(), Size() };
        }

        inline StringSlice operator()(size_t beginIndex, size_t endIndex) const
        {
            auto begin = Data();
            auto end   = Data();
            UTF8::Advance(begin, beginIndex);
            UTF8::Advance(end, endIndex);
            return StringSlice(begin, end - begin);
        }

        [[nodiscard]] inline StringSlice ASCIISubstring(size_t beginIndex, size_t endIndex) const
        {
            auto begin = Data() + beginIndex;
            auto end   = Data() + endIndex;
            return StringSlice(begin, end - begin);
        }

        inline void Reserve(size_t reserve) noexcept
        {
            size_t cap = Capacity();
            if (cap >= reserve)
                return;

            reserve        = Recommend(reserve);
            TChar* newData = Allocate(reserve + 1);
            TChar* oldData = Data();
            auto oldSize   = Size();
            CopyData(newData, oldData, oldSize + 1);
            if (IsLong())
                Deallocate(oldData);

            SetLCap(reserve + 1);
            SetLSize(oldSize);
            m_Data.L.Data = newData;
        }

        inline void Shrink() noexcept
        {
            size_t cap     = Capacity();
            size_t size    = Size();
            size_t reserve = Recommend(size);
            if (reserve == cap)
                return;

            if (reserve == MinCapacity - 1)
            {
                TChar* newData = m_Data.S.Data;
                TChar* oldData = m_Data.L.Data;

                CopyData(newData, oldData, size + 1);
                Deallocate(oldData);
                SetSSize(size);
            }
            else
            {
                TChar* newData = Allocate(reserve + 1);
                TChar* oldData = m_Data.L.Data;

                CopyData(newData, oldData, size + 1);
                Deallocate(oldData);

                SetLCap(reserve + 1);
                SetLSize(size);
                m_Data.L.Data = newData;
            }
        }

        inline void Clear() noexcept
        {
            *Data() = '\0';
            SetSize(0);
        }

        inline String& Append(const TChar* str, size_t count)
        {
            ACTIAS_Assert(count == 0 || str != nullptr, "Couldn't append more than 0 chars from a null string");
            if (count == 0)
            {
                return *this;
            }

            size_t size = Size();
            size_t cap  = Capacity();
            if (cap - size >= count)
            {
                TChar* data = Data();
                CopyData(data + size, str, count);
                size += count;
                SetSize(size);
                data[size] = '\0';
            }
            else
            {
                GrowAndReplace(cap, size + count - cap, size, size, 0, count, str);
            }

            return *this;
        }

        inline String& Append(StringSlice str)
        {
            return Append(str.Data(), str.Size());
        }

        inline String& Append(TChar cp)
        {
            return Append(&cp, 1); // TODO: optimize this for single chars
        }

        inline String& Append(const TChar* str)
        {
            return Append(str, Str::ByteLength(str));
        }

        inline String& operator+=(StringSlice str)
        {
            return Append(str);
        }

        inline String& operator/=(const StringSlice& str)
        {
            Append(ACTIAS_PATH_SEPARATOR);
            return Append(str);
        }

        inline friend String operator+(const String& lhs, StringSlice rhs)
        {
            String t;
            t.Reserve(lhs.Size() + rhs.Size() + 1);
            t += lhs;
            t += rhs;
            return t;
        }

        inline friend String operator/(const String& lhs, StringSlice rhs)
        {
            String t;
            t.Reserve(lhs.Size() + rhs.Size() + 2);
            t += lhs;
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

        [[nodiscard]] inline static String Join(const StringSlice& separator, const ArraySlice<StringSlice>& strings)
        {
            String result;
            USize capacity = 0;
            for (auto& string : strings)
            {
                capacity += string.Size() + separator.Size();
            }

            result.Reserve(capacity);
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

    template<>
    struct ValueParser<String> : std::true_type
    {
        inline static ParseError TryConvert(const StringSlice& str, String& result)
        {
            result = str;
            return ParseErrorCode::None;
        }
    };

    template<>
    struct Hash<Actias::String>
    {
        inline size_t operator()(const Actias::String& str) const noexcept
        {
            return Actias::Str::Hash(str.Data(), str.Size());
        }
    };
} // namespace Actias
