#pragma once
#include <ActiasRuntime/IO/StreamBase.h>

namespace Actias::IO
{
    //! \brief A stream that writes to STDIO.
    class StdoutStream : public WStreamBase
    {
    public:
        ACTIAS_RTTI_Class(StdoutStream, "2D5441F8-10B1-4358-B486-5C6BF02DDB24");

        [[nodiscard]] bool IsOpen() const override;
        [[nodiscard]] Result<USize, ResultCode> WriteFromBuffer(const void* buffer, USize size) override;
        [[nodiscard]] StringSlice GetName() const override;
        void Close() override;
    };
} // namespace Actias::IO
