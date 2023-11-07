#include <Actias/Memory/Memory.hpp>
#include <TestLibrary/AddNumbers.h>

extern "C" __declspec(dllimport) Int32 ACTIAS_ABI ActiasAtomicAddI32(volatile Int32* pAddend, Int32 Value);

Int32 ACTIAS_ABI Add(Int32 a, Int32 b)
{
    ActiasAtomicAddI32(&a, b);
    return a;
}

Int32 ACTIAS_ABI AddNumbers(Int32 a, Int32 b)
{
    ActiasAtomicAddI32(&a, b);
    return a;
    // return a + b;
}

Int32 ACTIAS_ABI AddNumbersEx(Int32 a, Int32 b, Int32 c)
{
    return AddNumbers(a, b) + c;
}

const char* ACTIAS_ABI GetMessage()
{
    return "Hello, World!";
}
