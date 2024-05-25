#include <Actias/IO/FileSystem.hpp>
#include <Actias/System/Memory.h>
#include <Actias/System/Runtime.h>
#include <Actias/Utils/LibraryLoader.hpp>
#include <ActiasSDK/Driver/ExecutableBuilder.hpp>
#include <ActiasSDK/Parser/Result.hpp>
#include <ActiasSDK/Platform/INativeExecutable.hpp>
#include <ActiasSDK/Platform/NativeExecutableFactory.hpp>
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

typedef ExecutableParseError ACTIAS_ABI ActiasLoadNativeExecutableProc(INativeExecutable** ppExecutable,
                                                                       const ActiasNativeExecutableLoadInfo* pLoadInfo);

typedef void ACTIAS_ABI ActiasBuildExecutableProc(IBlob** ppExecutableData, const ActiasExecutableBuildInfo* pBuildInfo);
typedef ActiasResult ACTIAS_ABI ActiasStartMainProc(ActiasHandle);

static int Build(StringSlice executable, bool library)
{
    const LibraryLoader loader{ "ActiasSDK", true };
    const auto executableRead = FileSystem::ReadAllBytes(executable);

    if (!executableRead)
    {
        return EXIT_FAILURE;
    }

    auto bytes = executableRead.Unwrap();

    ActiasNativeExecutableLoadInfo loadInfo{};
    loadInfo.pRawData        = bytes.Data();
    loadInfo.RawDataByteSize = bytes.Size();

    Ptr<INativeExecutable> nativeExecutable;

    auto* actiasLoadNativeExecutable = loader.FindFunction<ActiasLoadNativeExecutableProc>("ActiasLoadNativeExecutable");
    const ExecutableParseError loadNativeResult = actiasLoadNativeExecutable(&nativeExecutable, &loadInfo);

    if (loadNativeResult != ExecutableParseError::None)
    {
        std::cerr << "Error loading a PE: " << ExecutableParseErrorTypeToString(loadNativeResult) << std::endl;
        return EXIT_FAILURE;
    }

    const ActiasExecutableBuildInfo buildInfo{ nativeExecutable.Get() };

    Ptr<IBlob> pExecutableData;

    auto* actiasBuildExecutable = loader.FindFunction<ActiasBuildExecutableProc>("ActiasBuildExecutable");
    actiasBuildExecutable(&pExecutableData, &buildInfo);

    const StringSlice outputPath{ executable.begin(), executable.FindFirstOf('.') };
    const char* extension  = library ? ".acbl" : ".acbx";
    const auto writeResult = FileSystem::WriteBlob(String{ outputPath } + extension, pExecutableData.Get(), OpenMode::Create);

    if (writeResult.IsErr())
    {
        std::cerr << "Error writing ACBX file: " << IO::GetResultDesc(writeResult.UnwrapErr()) << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static int Run(StringSlice executableName)
{
    const LibraryLoader loader{ executableName, false };
    ACTIAS_Assert(loader);
    ACTIAS_Assert(!loader.IsNative() && "We should only run an ACBX here");

    const auto mainFunc           = loader.FindFunction<ActiasStartMainProc>("ActiasStartMain");
    const ActiasResult mainResult = mainFunc(loader.GetHandle());
    if (mainResult == ACTIAS_NO_USER_ENTRY_POINT_FOUND)
    {
        std::cerr << "No user defined entry point found. Did you forget to define an ActiasMain?" << std::endl;
    }

    return mainResult == ACTIAS_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;
}

int main(int argc, char** argv)
{
    int result = 0;

    const std::vector<std::string> args{ argv + 1, argv + argc };

    if (ActiasInit(nullptr) != ACTIAS_SUCCESS)
    {
        std::cerr << "Failed to initialize the Actias Runtime" << std::endl;
        return EXIT_FAILURE;
    }

    args::ArgumentParser parser{ DescriptionMessage };
    parser.Prog(argv[0]);

    args::Group arguments{ "Arguments" };
    const args::HelpFlag help{ arguments, "help", "Display this help message", { 'h', "help" } };
    const args::Flag versionFlag{
        arguments, "version", "Show the version of Actias runtime and SDK", { 'v', "version" }, args::Options::KickOut
    };

    args::Group commands{ parser, "SDK Commands" };
    const args::Command build(commands, "build", "Build an Actias project", [&result](args::Subparser& p) {
        args::Positional<std::string> executable{
            p, "executable", "The native OS executable to build (dll, exe, so, etc.)", args::Options::Required
        };
        const args::Flag lib{
            p, "lib", "Build a dynamic library, otherwise builds an executable", { "lib" }, args::Options::Single
        };
        p.Parse();

        result = Build(StringSlice{ executable.Get() }, lib.Get());
    });

    const char* runCommandDesc = "Run an executable file, builds first if a native executable is provided";
    const args::Command run(commands, "run", runCommandDesc, [&result](args::Subparser& p) {
        args::Positional<std::string> executable{
            p, "executable", "The executable (native or ACBX) file to run", args::Options::Required
        };
        p.Parse();

        const StringSlice executableName{ executable.Get() };
        const StringSlice executableExtension{ executableName.FindLastOf('.'), executableName.end() };
        const StringSlice nameWithoutExt{ executableName.begin(), executableName.FindLastOf('.') };

        if (executableExtension == ".acbx")
        {
            result = Run(executableName);
        }
        else if (executableExtension == ACTIAS_NATIVE_DL_EXTENSION)
        {
            result = Build(executableName, false);
            if (result)
            {
                return;
            }

            result = Run(String{ nameWithoutExt } + ".acbx");
        }
    });

    const args::GlobalOptions globals{ parser, arguments };

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

    return result;
}
