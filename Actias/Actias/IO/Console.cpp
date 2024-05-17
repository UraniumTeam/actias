#include <Actias/IO/Console.hpp>

namespace Actias::IO::Console
{
    void Write(StringSlice str)
    {
        ActiasHandle stdoutHandle;
        const ActiasResult stdoutResult = ActiasGetStdFileHandle(ACTIAS_STDOUT, &stdoutHandle);
        ACTIAS_Assert(stdoutResult == ACTIAS_SUCCESS);

        const ActiasResult writeResult = ActiasWrite(stdoutHandle, str.Data(), str.Size(), nullptr);
        ACTIAS_Assert(writeResult == ACTIAS_SUCCESS);
    }

    void WriteLine(StringSlice str)
    {
        ActiasHandle stdoutHandle;
        const ActiasResult stdoutResult = ActiasGetStdFileHandle(ACTIAS_STDOUT, &stdoutHandle);
        ACTIAS_Assert(stdoutResult == ACTIAS_SUCCESS);

        ActiasResult writeResult = ActiasWrite(stdoutHandle, str.Data(), str.Size(), nullptr);
        ACTIAS_Assert(writeResult == ACTIAS_SUCCESS);
        writeResult = ActiasWrite(stdoutHandle, "\n", 1, nullptr);
        ACTIAS_Assert(writeResult == ACTIAS_SUCCESS);
    }
} // namespace Actias::IO::Console
