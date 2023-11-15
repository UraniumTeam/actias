#include <Actias/System/Streams.h>
#include <TestLibrary/AddNumbers.h>

extern "C"
{
    Int32 ACTIAS_ABI AddNumbers(Int32 a, Int32 b)
    {
        return a + b;
    }

    Int32 ACTIAS_ABI AddNumbersEx(Int32 a, Int32 b, Int32 c)
    {
        return AddNumbers(a, b) + c;
    }

    const char* ACTIAS_ABI GetMessage()
    {
        return "Hello, World!\n";
    }

    void ACTIAS_ABI PrintMessage()
    {
        const char* pMessage = GetMessage();

        ActiasHandle hStdout;
        ActiasGetStdFileHandle(ACTIAS_STDOUT, &hStdout);
        ActiasWrite(hStdout, pMessage, 14, nullptr);
    }
}
