#include <Actias/IO/Console.hpp>

namespace Actias::IO::Console
{
    static void WriteImpl(StringSlice str, Int32 descriptor)
    {
        ActiasHandle stdoutHandle;
        const ActiasResult stdoutResult = ActiasGetStdFileHandle(descriptor, &stdoutHandle);
        ACTIAS_Assert(stdoutResult == ACTIAS_SUCCESS);

        const ActiasResult writeResult = ActiasWriteFile(stdoutHandle, str.Data(), str.Size(), nullptr);
        ACTIAS_Assert(writeResult == ACTIAS_SUCCESS);
    }

    static void WriteLineImpl(StringSlice str, Int32 descriptor)
    {
        ActiasHandle stdoutHandle;
        const ActiasResult stdoutResult = ActiasGetStdFileHandle(descriptor, &stdoutHandle);
        ACTIAS_Assert(stdoutResult == ACTIAS_SUCCESS);

        ActiasResult writeResult = ActiasWriteFile(stdoutHandle, str.Data(), str.Size(), nullptr);
        ACTIAS_Assert(writeResult == ACTIAS_SUCCESS);
        writeResult = ActiasWriteFile(stdoutHandle, "\n", 1, nullptr);
        ACTIAS_Assert(writeResult == ACTIAS_SUCCESS);
    }

    void Write(StringSlice str)
    {
        WriteImpl(str, ACTIAS_STDOUT);
    }

    void WriteLine(StringSlice str)
    {
        WriteLineImpl(str, ACTIAS_STDOUT);
    }

    void WriteErr(StringSlice str)
    {
        WriteImpl(str, ACTIAS_STDERR);
    }

    void WriteLineErr(StringSlice str)
    {
        WriteLineImpl(str, ACTIAS_STDERR);
    }
} // namespace Actias::IO::Console
