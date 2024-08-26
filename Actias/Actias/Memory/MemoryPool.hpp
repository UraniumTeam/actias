#pragma once
#include <Actias/Memory/IAllocator.hpp>
#include <Actias/System/MemoryPool.h>

namespace Actias
{
    class PoolAllocator final : public IAllocator
    {
        ActiasMemoryPool m_Pool  = { 0 };
        UInt32 m_AllocationCount = 0;

        PoolAllocator(const PoolAllocator&)            = delete;
        PoolAllocator& operator=(const PoolAllocator&) = delete;

    public:
        PoolAllocator() = default;
        ~PoolAllocator();

        PoolAllocator(PoolAllocator&& other) noexcept;
        PoolAllocator& operator=(PoolAllocator&& other) noexcept;

        template<class T>
        void Init(UInt32 elementsInPage);
        void Init(UInt64 elementByteSize, UInt32 elementsInPage);
        void Reset();

        void* Allocate(USize size, USize alignment) override;
        void Deallocate(void* pointer) override;
        [[nodiscard]] const char* GetName() const override;

        [[nodiscard]] UInt64 GetElementByteSize() const;
        [[nodiscard]] UInt64 GetPageByteSize() const;
        [[nodiscard]] UInt32 GetAllocationCount() const;
    };

    inline PoolAllocator::~PoolAllocator()
    {
        ACTIAS_Assert(m_AllocationCount == 0);
        Reset();
    }

    inline PoolAllocator::PoolAllocator(PoolAllocator&& other) noexcept
        : m_Pool{ other.m_Pool }
    {
        other.m_Pool = { 0 };
    }

    inline PoolAllocator& PoolAllocator::operator=(PoolAllocator&& other) noexcept
    {
        Reset();

        m_Pool       = other.m_Pool;
        other.m_Pool = { 0 };

        return *this;
    }

    template<class T>
    inline void PoolAllocator::Init(UInt32 elementsInPage)
    {
        Init(sizeof(T), elementsInPage);
    }

    inline void PoolAllocator::Init(UInt64 elementByteSize, UInt32 elementsInPage)
    {
        ACTIAS_AssertDebug(m_Pool.ElementByteSize == 0 && "Pool already initialized");

        ActiasMemoryPoolDesc desc;
        desc.ElementByteSize = elementByteSize;
        desc.ElementsInPage  = elementsInPage;

        ActiasCreateMemoryPool(&desc, &m_Pool);
    }

    inline void PoolAllocator::Reset()
    {
        ACTIAS_AssertDebug(m_Pool.ElementByteSize > 0 && "Pool not initialized");

        ActiasDestroyMemoryPool(&m_Pool);
        m_Pool = { 0 };
    }

    inline void* PoolAllocator::Allocate(USize size, USize alignment)
    {
        ACTIAS_AssertDebug(m_Pool.ElementByteSize > 0 && "Pool not initialized");
        ACTIAS_AssertDebug(size <= m_Pool.ElementByteSize);
        ACTIAS_AssertDebug(alignment <= ACTIAS_DEFAULT_ALIGNMENT);

        ++m_AllocationCount;
        return ActiasMemoryPoolAllocate(&m_Pool);
    }

    inline void PoolAllocator::Deallocate(void* pointer)
    {
        --m_AllocationCount;
        ActiasMemoryPoolFree(&m_Pool, pointer);
    }

    inline const char* PoolAllocator::GetName() const
    {
        return "Pool allocator";
    }

    inline UInt64 PoolAllocator::GetElementByteSize() const
    {
        return m_Pool.ElementByteSize;
    }

    inline UInt64 PoolAllocator::GetPageByteSize() const
    {
        return m_Pool.PageByteSize;
    }

    inline UInt32 PoolAllocator::GetAllocationCount() const
    {
        return m_AllocationCount;
    }

    template<class T>
    class Pool final
    {
        PoolAllocator m_Allocator;

    public:
        inline Pool(UInt32 elementsInPage)
        {
            m_Allocator.Init<T>(elementsInPage);
        }

        inline UInt32 GetAllocationCount() const
        {
            return m_Allocator.GetAllocationCount();
        }

        template<class... Args>
        [[nodiscard]] inline T* New(Args&&... args)
        {
            return Memory::New<T>(&m_Allocator, std::forward<Args>(args)...);
        }

        inline void Delete(T* pointer)
        {
            Memory::Delete<T>(&m_Allocator, pointer);
        }
    };
} // namespace Actias
