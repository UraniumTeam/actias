#pragma once
#include <Actias/Memory/Ptr.hpp>
#include <Actias/Memory/SystemAllocator.hpp>
#include <Actias/RTTI/RTTI.hpp>
#include <unordered_map>

namespace Actias
{
    namespace Memory
    {
        //! \brief Create a new object of type T using the provided allocator.
        //!
        //! \param pAllocator - The allocator to use.
        //! \param args       - The arguments to call constructor of T with.
        //!
        //! \tparam T          - The type of the object to allocate.
        //! \tparam TAllocator - The type of the provided allocator.
        //! \tparam Args       - The types of the arguments to call the constructor of T with.
        //!
        //! \return The allocated object.
        template<class T, class TAllocator, class... Args>
        inline T* New(TAllocator* pAllocator, Args&&... args)
        {
            return new (pAllocator->Allocate(sizeof(T), alignof(T))) T(std::forward<Args>(args)...);
        }

        //! \brief Delete an object previously created via Memory::New().
        //!
        //! \param pAllocator - The allocator to use.
        //! \param pointer    - The pointer to the object to delete previously returned by Memory::New().
        //!
        //! \tparam T          - The type of the provided object.
        //! \tparam TAllocator - The type of the provided allocator.
        template<class T, class TAllocator>
        inline void Delete(TAllocator* pAllocator, T* pointer)
        {
            pointer->~T();
            pAllocator->Deallocate(pointer);
        }
    } // namespace Memory

    //! \brief Create a reference counted object.
    //!
    //! This function allocates storage for a ReferenceCounter and an object of type T.
    //! It attaches reference counter to the allocated object.
    //!
    //! \param pAllocator - The allocator to use.
    //! \param args       - The arguments to call the constructor of T with.
    //!
    //! \tparam T          - The type of the object to allocate.
    //! \tparam TAllocator - The type of the provided allocator.
    //! \tparam Args       - The types of the arguments to call the constructor of T with.
    //!
    //! \return The allocated object.
    template<class T, class TAllocator, class... Args>
    inline T* AllocateObjectEx(TAllocator* pAllocator, Args&&... args)
    {
        USize counterSize = AlignUp(sizeof(ReferenceCounter), alignof(T));
        USize wholeSize   = sizeof(T) + counterSize;

        auto* ptr     = static_cast<UInt8*>(pAllocator->Allocate(wholeSize, alignof(T)));
        auto* counter = new (ptr) ReferenceCounter(pAllocator);

        T* object = new (ptr + counterSize) T(std::forward<Args>(args)...);
        object->AttachRefCounter(counter);
        return object;
    }

    //! \brief Create a reference counted object.
    //!
    //! This function allocates storage for a ReferenceCounter and an object of type T.
    //! It attaches reference counter to the allocated object.
    //!
    //! \param args - The arguments to call the constructor of T with.
    //!
    //! \tparam T    - The type of the object to allocate.
    //! \tparam Args - The types of the arguments to call the constructor of T with.
    //!
    //! \return The created object.
    template<class T, class... Args>
    inline T* AllocateObject(Args&&... args)
    {
        return AllocateObjectEx<T, SystemAllocator>(SystemAllocator::Get(), std::forward<Args>(args)...);
    }

    //! \brief A wrapper for IAllocator compatible with `std::allocator`.
    template<class T, class TAlloc>
    class StdAllocator
    {
        mutable IAllocator* m_Instance;

    public:
        ACTIAS_RTTI_Struct(StdAllocator, "18778CFF-60EF-49FA-A560-46CE59C2BEFF");

        using value_type = T;

        inline StdAllocator(const StdAllocator& other) noexcept
            : m_Instance(other.m_Instance)
        {
        }

        template<class TOther>
        inline explicit StdAllocator(const StdAllocator<TOther, TAlloc>& other) noexcept
            : m_Instance(other.GetImpl())
        {
        }

        inline StdAllocator() noexcept
        {
            m_Instance = SystemAllocator::Get();
        }

        inline explicit StdAllocator(TAlloc* instance) noexcept
        {
            m_Instance = instance;
        }

        inline IAllocator* GetImpl() const noexcept
        {
            return m_Instance;
        }

        inline value_type* allocate(size_t n) noexcept
        {
            return static_cast<value_type*>(m_Instance->Allocate(n * sizeof(T), alignof(T)));
        }

        inline void deallocate(value_type* ptr, size_t) noexcept
        {
            m_Instance->Deallocate(ptr);
        }
    };

    template<class T1, class T2, class TAlloc>
    inline bool operator==(const StdAllocator<T1, TAlloc>&, const StdAllocator<T2, TAlloc>&) noexcept
    {
        return true;
    }

    template<class T1, class T2, class TAlloc>
    inline bool operator!=(const StdAllocator<T1, TAlloc>& x, const StdAllocator<T2, TAlloc>& y) noexcept
    {
        return !(x == y);
    }

    //! \brief An alias for StdAllocator that uses HeapAllocator.
    template<class T>
    using StdHeapAllocator = StdAllocator<T, SystemAllocator>;

    //! \brief An alias for `std::wstring` that uses HeapAllocator through StdHeapAllocator.
    using WString = std::basic_string<wchar_t, std::char_traits<wchar_t>, StdHeapAllocator<wchar_t>>;

    //! \brief An alias for `std::unordered_map` that uses HeapAllocator through StdHeapAllocator.
    template<class TKey, class TValue, class THasher = std::hash<TKey>, class TEq = std::equal_to<TKey>>
    using UnorderedMap = std::unordered_map<TKey, TValue, THasher, TEq, StdHeapAllocator<std::pair<const TKey, TValue>>>;
} // namespace Actias
