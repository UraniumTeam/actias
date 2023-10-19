#include <Actias/IO/FileHandle.hpp>
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

        std::cout << "Loaded a DLL" << std::endl;

        ACBXFileInformationHeader fileInfo{};
        ActiasCopyMemory(&fileInfo.Signature, ACBXSignature, ACBX_SIGNATURE_SIZE);

        executable->CreateInformationHeader(&fileInfo);

        ACBXSectionHeader textSectionHeader{};
        textSectionHeader.Address = 0x1000;
        executable->CreateSectionHeader(0, &textSectionHeader);

        std::cout << "Completed retrieving headers" << std::endl;
    }
    else
    {
        std::cout << "DLL not found" << std::endl;
    }
}
