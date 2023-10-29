#include <Actias/IO/FileHandle.hpp>
#include <ActiasSDK/Driver/ExecutableBuilder.hpp>
#include <ActiasSDK/Platform/INativeExecutable.hpp>
#include <ActiasSDK/Platform/NativeExecutableFactory.hpp>
#include <iostream>

using namespace Actias;
using namespace Actias::IO;
using namespace Actias::SDK;

int main()
{
    auto dllRead = File::ReadAllBytes("TestLibrary.dll");
    if (dllRead)
    {
        auto bytes = dllRead.Unwrap();

        ActiasNativeExecutableLoadInfo loadInfo{};
        loadInfo.pRawData        = bytes.Data();
        loadInfo.RawDataByteSize = bytes.Size();

        Ptr<INativeExecutable> executable;
        auto result = ActiasLoadNativeExecutable(&executable, &loadInfo);

        if (result != ExecutableParseError::None)
        {
            std::cout << "Error loading a PE: " << ExecutableParseErrorTypeToString(result) << std::endl;
        }

        ActiasExecutableBuildInfo buildInfo{};
        buildInfo.pNativeExecutable = executable.Get();

        Ptr<IBlob> pExecutableData;
        ActiasBuildExecutable(&pExecutableData, &buildInfo);

        auto writeResult = File::WriteBlob("TestLibrary.acbx", pExecutableData.Get(), OpenMode::Create);
        if (writeResult.IsErr())
        {
            std::cout << "Error writing ACBX file: " << IO::GetResultDesc(writeResult.UnwrapErr()) << std::endl;
        }
    }
    else
    {
        std::cout << "DLL not found" << std::endl;
    }
}
