#pragma once
#include <Actias/Base/Byte.hpp>
#include <Actias/Containers/IBlob.hpp>
#include <Actias/IO/StreamBase.hpp>

namespace Actias::IO
{
    template<USize TCapacity>
    class FixedBlob final : public Object<IBlob>
    {
        Byte m_Buffer[TCapacity];

    public:
        ACTIAS_RTTI_Class(FixedBlob, "AB56ED4C-8286-41FD-BF09-F50875A3A37E");

        [[nodiscard]] inline Byte* ACTIAS_ABI Data() override
        {
            return m_Buffer;
        }

        [[nodiscard]] inline USize ACTIAS_ABI ByteSize() const override
        {
            return TCapacity;
        }
    };

    template<USize TCapacity>
    class BufferedStream final : public WStreamBase
    {
        FixedBlob<TCapacity> m_Storage;
        USize m_Current = 0;
        Ptr<IStream> m_pStream;

    public:
        inline BufferedStream(IStream* pStream)
            : m_pStream(pStream)
        {
        }

        [[nodiscard]] inline bool IsOpen() const override
        {
            return m_pStream->IsOpen();
        }

        [[nodiscard]] inline Result<USize, ResultCode> WriteFromBuffer(const void* buffer, USize size) override
        {
            if (m_Current + size <= m_Storage.ByteSize())
            {
                ActiasCopyMemory(m_Storage.Data() + m_Current, buffer, size);
                m_Current += size;
                return size;
            }

            const auto result = m_pStream->WriteFromBuffer(m_Storage.Data(), m_Current);
            if (result.IsErr())
            {
                return result;
            }

            m_Current = 0;
            if (size <= m_Storage.ByteSize())
            {
                ActiasCopyMemory(m_Storage.Data(), buffer, size);
                m_Current += size;
                return size;
            }

            return m_pStream->WriteFromBuffer(buffer, size);
        }

        [[nodiscard]] inline StringSlice GetName() const override
        {
            return m_pStream->GetName();
        }

        inline void Close() override
        {
            m_pStream->Close();
        }
    };
} // namespace Actias::IO
