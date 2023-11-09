#include <Actias/Memory/Memory.hpp>
#include <TestLibrary/AddNumbers.h>

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
    return "Hello, World!";
}
