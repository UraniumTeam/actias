#include <ActiasRuntime/IO/StdoutStream.h>
#include <iostream>

namespace Actias::IO
{
    bool StdoutStream::IsOpen() const
    {
        return true;
    }

    Result<USize, ResultCode> StdoutStream::WriteFromBuffer(const void* buffer, USize size)
    {
        ACTIAS_Assert(buffer, "Buffer was nullptr");
        std::cout << StringSlice(static_cast<const char*>(buffer), size);
        return 0;
    }

    StringSlice StdoutStream::GetName() const
    {
        return "stdout";
    }

    void StdoutStream::Close() {}
} // namespace Actias::IO
