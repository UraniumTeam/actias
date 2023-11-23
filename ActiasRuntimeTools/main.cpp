#include "../ActiasSDK/ActiasSDK/Driver/ExecutableBuilder.hpp"
#include "../ActiasSDK/ActiasSDK/Platform/NativeExecutableFactory.cpp"
#include "../ActiasSDK/ActiasSDK/Platform/NativeExecutableFactory.hpp"
#include <../ActiasSDK/ActiasSDK/Driver/ExecutableBuilder.cpp>
#include <../ActiasSDK/ActiasSDK/Parser/Result.hpp>
#include <Actias/IO/FileHandle.hpp>
#include <Actias/System/Memory.h>
#include <args.hxx>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace Actias;
using namespace Actias::IO;
using namespace Actias::SDK;

constexpr const char* DescriptionMessage = "Actias CLI (runtime and SDK)";
constexpr const char* VersionMessage     = R"(Actias SDK version 0.1
Actias runtime version 0.1)";

int main(int argc, char** argv)
{
    const std::vector<std::string> args(argv + 1, argv + argc);

    args::ArgumentParser parser(DescriptionMessage);
    parser.Prog(argv[0]);

    args::Group arguments("Arguments");
    const args::HelpFlag help(arguments, "help", "Display this help message", { 'h', "help" });
    const args::Flag versionFlag(
        arguments, "version", "Show the version of Actias runtime and SDK", { 'v', "version" }, args::Options::KickOut);

    args::Group commands(parser, "SDK Commands");
    const args::Command build(commands, "build", "Build an Actias project", [](args::Subparser& p) {
        args::Positional<std::string> executable(
            p, "executable", "The native OS executable to build (dll, exe, so, etc.)", args::Options::Required);
        p.Parse();

        auto ExecutableRead = File::ReadAllBytes((StringSlice)executable.Get());

        if (!ExecutableRead)
        {
            std::cout << "Executable file not found" << std::endl;
            return EXIT_FAILURE;
        }

        auto bytes = ExecutableRead.Unwrap();

        ActiasNativeExecutableLoadInfo loadInfo{};
        loadInfo.pRawData        = bytes.Data();
        loadInfo.RawDataByteSize    = bytes.Size();
        
        Ptr<INativeExecutable> nativeExecutable;

        ExecutableParseError result = ActiasLoadNativeExecutable(&nativeExecutable, &loadInfo);
        ActiasExecutableBuildInfo buildInfo{};

        if (result != ExecutableParseError::None)
        {
            std::cout << "Error loading a PE: " << ExecutableParseErrorTypeToString(result) << std::endl;
            return EXIT_FAILURE;
        }
        
        buildInfo.pNativeExecutable = nativeExecutable.Get();

        Ptr<IBlob> pExecutableData;
        ActiasBuildExecutable(&pExecutableData, &buildInfo);

        auto writeResult = File::WriteBlob((StringSlice)(executable.Get() + ".acbl"), pExecutableData.Get(), OpenMode::Create);
        if (writeResult.IsErr())
        {
            std::cout << "Error writing ACBX file: " << IO::GetResultDesc(writeResult.UnwrapErr()) << std::endl;
            return EXIT_FAILURE;
        }

        std::cout << "Building: " << IO::GetResultDesc(writeResult.UnwrapErr()) << std::endl;
    });

    const args::Command run(commands, "run", "Run an ACBX executable file", [](args::Subparser& p) {
        args::Positional<std::string> executable(p, "executable", "The executable (ACBX) file to run", args::Options::Required);
        p.Parse();

        std::cout << "Running: " << executable.Get() << std::endl;
    });

    const args::GlobalOptions globals(parser, arguments);

    try
    {
        parser.ParseCLI(args);
    }
    catch (const args::Help&)
    {
        std::cout << parser;
        return 0;
    }
    catch (const args::ParseError& e)
    {
        std::cerr << "Command line parse error:\n";
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    catch (const args::ValidationError& e)
    {
        std::cerr << "Command line validation error:\n";
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    if (versionFlag)
    {
        std::cout << VersionMessage << std::endl;
        return 0;
    }
}
