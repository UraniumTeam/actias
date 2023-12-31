#pragma once
#include <Actias/Containers/ArraySlice.hpp>
#include <Actias/Containers/IBlob.hpp>
#include <Actias/Memory/IAllocator.hpp>
#include <Actias/Memory/SystemAllocator.hpp>

namespace Actias
{
    //! \brief Fixed-size heap-allocated array.
    template<class T>
    class HeapArray final
    {
        inline static constexpr USize Alignment = std::max(static_cast<USize>(16), alignof(T));

        static_assert(!std::is_const_v<T>);

        ArraySlice<T> m_Storage{};
        IAllocator* m_pAllocator{};

        inline void AllocateStorage(USize count)
        {
            void* pData = m_pAllocator->Allocate(count * sizeof(T), Alignment);
            m_Storage   = ArraySlice<T>(static_cast<T*>(pData), count);
        }

        inline void AllocateStorage(USize count, const T& value)
        {
            AllocateStorage(count);

            auto* storage = ACTIAS_AssumeAligned(Alignment, m_Storage.Data());
            for (USize i = 0; i < count; ++i)
            {
                storage[i] = value;
            }
        }

        inline void DeallocateStorage(const ArraySlice<T>& storage)
        {
            if (storage.Empty())
            {
                return;
            }

            m_pAllocator->Deallocate(storage.Data());
        }

        inline void DeallocateStorage()
        {
            DeallocateStorage(m_Storage);
            m_Storage = {};
        }

    public:
        //! \brief Create an empty array
        inline HeapArray() noexcept
            : m_pAllocator(SystemAllocator::Get())
        {
        }

        //! \brief Copy constructor.
        inline HeapArray(const HeapArray& other)
            : m_pAllocator(other.m_pAllocator)
        {
            AllocateStorage(other.Length());
            other.CopyDataTo(m_Storage);
        }

        //! \brief Move constructor.
        inline HeapArray(HeapArray&& other) noexcept
            : m_Storage(other.m_Storage)
            , m_pAllocator(other.m_pAllocator)
        {
            other.m_Storage = {};
        }

        //! \brief Create an array.
        //!
        //! \param data - An array slice to copy the data from.
        inline explicit HeapArray(const ArraySlice<const T>& data)
            : m_pAllocator(SystemAllocator::Get())
        {
            AllocateStorage(data.Length());
            data.CopyDataTo(m_Storage);
        }

        //! \brief Copy assignment.
        inline HeapArray& operator=(const HeapArray& other)
        {
            if (this == &other)
            {
                return *this;
            }

            DeallocateStorage();
            AllocateStorage(other.Length());
            other.CopyDataTo(m_Storage);

            return *this;
        }

        //! \brief Move assignment.
        inline HeapArray& operator=(HeapArray&& other) noexcept
        {
            DeallocateStorage();
            m_Storage       = other.m_Storage;
            m_pAllocator    = other.m_pAllocator;
            other.m_Storage = {};
            return *this;
        }

        inline ~HeapArray()
        {
            DeallocateStorage();
        }

        //! \brief Create an array.
        //!
        //! \param pAllocator - The allocator to use for heap allocations.
        inline explicit HeapArray(IAllocator* pAllocator)
            : m_pAllocator(pAllocator)
        {
        }

        //! \brief Create an array.
        //!
        //! \param size  - The number of array elements.
        //! \param value - The value to set to each element.
        inline explicit HeapArray(USize size, const T& value = {})
            : m_pAllocator(SystemAllocator::Get())
        {
            AllocateStorage(size, value);
        }

        //! \brief Create an array.
        //!
        //! \param pAllocator - The allocator to use for heap allocations.
        //! \param size       - The number of array elements.
        //! \param value      - The value to set to each element.
        inline HeapArray(IAllocator* pAllocator, USize size, const T& value = {})
            : m_pAllocator(pAllocator)
        {
            AllocateStorage(size, value);
        }

        //! \brief Set size of the array.
        //!
        //! \param size  - The number of array elements.
        //! \param value - The value to set to each new element.
        inline void Resize(USize size, const T& value = {})
        {
            ArraySlice<T> temp = m_Storage;
            AllocateStorage(size);
            temp.CopyDataTo(m_Storage);
            for (USize i = temp.Length(); i < Length(); ++i)
            {
                m_Storage[i] = value;
            }
            DeallocateStorage(temp);
        }

        //! \brief Create a subslice from this array.
        //!
        //! \param beginIndex - The index of the first element.
        //! \param endIndex   - The index of the first element after the end.
        //!
        //! \return The created subslice.
        inline ArraySlice<T> operator()(USize beginIndex, USize endIndex) const noexcept
        {
            return m_Storage(beginIndex, endIndex);
        }

        //! \brief Get an element by index.
        //!
        //! \param index - The index of the element to get.
        inline T& operator[](USize index) const noexcept
        {
            return m_Storage[index];
        }

        //! \brief Get index of the first element equal to the passed value.
        //!
        //! \param value - The value to look for.
        //!
        //! \return The index of the value or -1.
        [[nodiscard]] inline SSize FindFirstOf(const T& value) const
        {
            return m_Storage.FindFirstOf(value);
        }

        //! \brief Get index of the last element equal to the passed value.
        //!
        //! \param value - The value to look for.
        //!
        //! \return The index of the value or -1.
        [[nodiscard]] inline SSize FindLastOf(const T& value) const
        {
            return m_Storage.FindLastOf(value);
        }

        //! \brief Check if the element is present in the array.
        //!
        //! \param value - The value to look for.
        [[nodiscard]] inline bool Contains(const T& value)
        {
            return IndexOf(value) != -1;
        }

        //! \bried Length of the array.
        [[nodiscard]] inline USize Length() const
        {
            return m_Storage.Length();
        }

        //! \brief Check if the array is empty.
        [[nodiscard]] inline bool Empty() const
        {
            return Length() == 0;
        }

        //! \brief Check if the array has any elements.
        [[nodiscard]] inline bool Any() const
        {
            return Length() > 0;
        }

        //! \brief Reset the array to empty state.
        inline void Reset()
        {
            DeallocateStorage();
        }

        //! \brief Get pointer to the beginning of the array.
        [[nodiscard]] inline T* Data()
        {
            return m_Storage.Data();
        }

        //! \brief Get pointer to the beginning of the array.
        [[nodiscard]] inline const T* Data() const
        {
            return m_Storage.Data();
        }

        //! \brief Copy data from this array to a slice.
        //!
        //! \param destination - The destination slice.
        //!
        //! \return The number of actually copied elements.
        inline USize CopyDataTo(ArraySlice<T> destination) const
        {
            return m_Storage.CopyDataTo(destination);
        }

        //! \brief Create a heap array by copying data from another container.
        //!
        //! \param arraySlice - An ArraySlice with the data to be copied.
        [[nodiscard]] inline static HeapArray<T> CopyFrom(const ArraySlice<const T>& arraySlice)
        {
            return HeapArray<T>(arraySlice);
        }

        //! \brief Create an uninitialized heap array.
        //!
        //! \param pAllocator - The allocator to use for heap allocations.
        //! \param size       - The number of array elements.
        [[nodiscard]] inline static HeapArray<T> CreateUninitialized(IAllocator* pAllocator, USize size)
        {
            HeapArray<T> result(pAllocator);
            result.AllocateStorage(size);
            return result;
        }

        //! \brief Create an uninitialized heap array.
        //!
        //! \param size - The number of array elements.
        [[nodiscard]] inline static HeapArray<T> CreateUninitialized(USize size)
        {
            HeapArray<T> result(SystemAllocator::Get());
            result.AllocateStorage(size);
            return result;
        }

        [[nodiscard]] inline const T* begin() const
        {
            return ACTIAS_AssumeAligned(Alignment, m_Storage.begin());
        }

        [[nodiscard]] inline const T* end() const
        {
            return ACTIAS_AssumeAligned(Alignment, m_Storage.end());
        }

        inline operator ArraySlice<const T>() const // NOLINT(google-explicit-constructor)
        {
            return m_Storage;
        }

        inline operator ArraySlice<T>() // NOLINT(google-explicit-constructor)
        {
            return m_Storage;
        }
    };

    class HeapArrayBlob : public Object<IBlob>
    {
        HeapArray<Byte> m_Data;

    public:
        inline HeapArrayBlob(HeapArray<Byte>&& data)
            : m_Data(std::move(data))
        {
        }

        [[nodiscard]] inline virtual Byte* ACTIAS_ABI Data()
        {
            return m_Data.Data();
        }

        [[nodiscard]] inline virtual USize ACTIAS_ABI ByteSize() const
        {
            return m_Data.Length();
        }
    };
} // namespace Actias
