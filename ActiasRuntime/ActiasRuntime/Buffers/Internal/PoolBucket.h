#pragma once
#include <ActiasRuntime/Base/Base.h>
#include <ActiasRuntime/Containers/ArraySlice.h>
#include <ActiasRuntime/Containers/HeapArray.h>
#include <mutex>

namespace Actias::Internal
{
    template<class T, class TLock = std::mutex>
    class PoolBucket final
    {
        inline static constexpr USize Alignment = std::max(static_cast<USize>(16), alignof(T));

        HeapArray<ArraySlice<T>> m_Buffers;
        IAllocator* m_pAllocator;
        USize m_BufferLength;
        USize m_Index;
        TLock m_Mutex;

    public:
        inline PoolBucket(IAllocator* pAllocator, USize bufferLength, USize bufferCount)
            : m_Buffers(pAllocator, bufferCount)
            , m_pAllocator(pAllocator)
            , m_BufferLength(bufferLength)
            , m_Index(0)
        {
        }

        inline ~PoolBucket()
        {
            for (auto& buffer : m_Buffers)
            {
                DeallocateStorage(buffer);
            }

            m_Buffers.Reset();
        }

        ACTIAS_FORCE_INLINE ArraySlice<T> AllocateStorage()
        {
            void* pData = m_pAllocator->Allocate(m_BufferLength * sizeof(T), alignof(T));
            return ArraySlice<T>(static_cast<T*>(pData), m_BufferLength);
        }

        ACTIAS_FORCE_INLINE void DeallocateStorage(const ArraySlice<T>& storage)
        {
            if (storage.Empty())
            {
                return;
            }

            m_pAllocator->Deallocate(storage.Data());
        }

        inline ArraySlice<T> Rent()
        {
            ArraySlice<T> buffer;
            auto allocateBuffer = false;

            {
                std::unique_lock lk(m_Mutex);

                if (m_Index < m_Buffers.Length())
                {
                    buffer               = m_Buffers[m_Index];
                    m_Buffers[m_Index++] = {};
                    allocateBuffer       = buffer.Empty();
                }
            }

            if (allocateBuffer)
            {
                buffer = AllocateStorage();
            }

            return buffer;
        }

        inline void Return(const ArraySlice<T>& buffer)
        {
            ACTIAS_Assert(buffer.Length() == m_BufferLength, "Incorrect bucket");

            auto returned = false;

            {
                std::unique_lock lk(m_Mutex);

                returned = m_Index != 0;
                if (returned)
                {
                    m_Buffers[--m_Index] = buffer;
                }
            }

            if (!returned)
            {
                DeallocateStorage(buffer);
            }
        }
    };
} // namespace Actias::Internal
