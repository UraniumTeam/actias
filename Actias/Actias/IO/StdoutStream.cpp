#include <Actias/IO/StdoutStream.hpp>

namespace Actias::IO
{
    StdoutStream::StdoutStream()
    {
        const ActiasResult result = ActiasGetStdFileHandle(ACTIAS_STDOUT, &m_StreamHandle);
        ACTIAS_Assert(result == ACTIAS_SUCCESS, "Unable to open STDOUT stream");
    }

    bool StdoutStream::IsOpen() const
    {
        return true;
    }

    Result<USize, ResultCode> StdoutStream::WriteFromBuffer(const void* buffer, USize size)
    {
        ACTIAS_Assert(buffer, "Buffer was nullptr");

        USize bytesWritten;
        const ActiasResult result = ActiasWrite(m_StreamHandle, buffer, size, &bytesWritten);
        if (result == ACTIAS_SUCCESS)
            return bytesWritten;

        return static_cast<ResultCode>(result);
    }

    StringSlice StdoutStream::GetName() const
    {
        return "STDOUT";
    }

    void StdoutStream::Close() {}
} // namespace Actias::IO
