#include <Actias/IO/FileSystem.hpp>
#include <Actias/System/Runtime.h>
#include <ActiasSDK/Driver/ExecutableBuilder.hpp>
#include <ActiasSDK/Platform/INativeExecutable.hpp>
#include <ActiasSDK/Platform/NativeExecutableFactory.hpp>
#include <TestLibrary/AddNumbers.h>
#include <iostream>

using namespace Actias;
using namespace Actias::IO;
using namespace Actias::SDK;

int main()
{
    if (ActiasInit(nullptr) < 0)
    {
        std::cerr << "Failed to initialize the Runtime!" << std::endl;
        return EXIT_FAILURE;
    }

#if ACTIAS_WINDOWS
    auto dllRead = File::ReadAllBytes("TestLibrary.dll");

    if (!dllRead)
    {
        std::cout << "DLL not found" << std::endl;
        return EXIT_FAILURE;
    }

    auto bytes = dllRead.Unwrap();

    ActiasNativeExecutableLoadInfo loadInfo{};
    loadInfo.pRawData        = bytes.Data();
    loadInfo.RawDataByteSize = bytes.Size();

    Ptr<INativeExecutable> executable;
    auto result = ActiasLoadNativeExecutable(&executable, &loadInfo);

    if (result != ExecutableParseError::None)
    {
        std::cout << "Error loading a PE: " << ExecutableParseErrorTypeToString(result) << std::endl;
        return EXIT_FAILURE;
    }

    ActiasExecutableBuildInfo buildInfo{};
    buildInfo.pNativeExecutable = executable.Get();

    Ptr<IBlob> pExecutableData;
    ActiasBuildExecutable(&pExecutableData, &buildInfo);

    auto writeResult = File::WriteBlob("TestLibrary.acbl", pExecutableData.Get(), OpenMode::Create);
    if (writeResult.IsErr())
    {
        std::cout << "Error writing ACBX file: " << IO::GetResultDesc(writeResult.UnwrapErr()) << std::endl;
        return EXIT_FAILURE;
    }
#endif

    ActiasHandle moduleHandle;
    auto loadResult = ActiasLoadModule("TestLibrary.acbl", &moduleHandle);
    if (loadResult != ACTIAS_SUCCESS)
    {
        std::cout << "Error loading ACBX file: " << loadResult << std::endl;
        return EXIT_FAILURE;
    }

    typedef decltype(AddNumbers) AddNumbersProc;
    typedef decltype(AddNumbersEx) AddNumbersExProc;
    typedef decltype(GetMessage) GetMessageProc;
    typedef decltype(PrintMessage) PrintMessageProc;

    ActiasProc address;
    auto symResult = ActiasFindSymbolAddress(moduleHandle, "AddNumbers", &address);
    if (symResult != ACTIAS_SUCCESS)
    {
        std::cout << "Error finding a symbol in ACBX module: " << symResult << std::endl;
        return EXIT_FAILURE;
    }

    auto* add = reinterpret_cast<AddNumbersProc*>(address);
    std::cout << add(2, 3) << std::endl;

    symResult = ActiasFindSymbolAddress(moduleHandle, "AddNumbersEx", &address);
    if (symResult != ACTIAS_SUCCESS)
    {
        std::cout << "Error finding a symbol in ACBX module: " << symResult << std::endl;
        return EXIT_FAILURE;
    }

    auto* add2 = reinterpret_cast<AddNumbersExProc*>(address);
    std::cout << add2(2, 3, 4) << std::endl;

    symResult = ActiasFindSymbolAddress(moduleHandle, "GetMessage", &address);
    if (symResult != ACTIAS_SUCCESS)
    {
        std::cout << "Error finding a symbol in ACBX module: " << symResult << std::endl;
        return EXIT_FAILURE;
    }

    auto* getMsg = reinterpret_cast<GetMessageProc*>(address);
    std::cout << getMsg() << std::endl;

    symResult = ActiasFindSymbolAddress(moduleHandle, "PrintMessage", &address);
    if (symResult != ACTIAS_SUCCESS)
    {
        std::cout << "Error finding a symbol in ACBX module: " << symResult << std::endl;
        return EXIT_FAILURE;
    }

    auto* printMsg = reinterpret_cast<PrintMessageProc*>(address);
    printMsg();

    auto unloadResult = ActiasUnloadModule(moduleHandle);
    if (unloadResult != ACTIAS_SUCCESS)
    {
        std::cout << "Error unloading ACBX module: " << unloadResult << std::endl;
        return EXIT_FAILURE;
    }
}
