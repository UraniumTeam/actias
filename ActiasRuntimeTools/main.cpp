#include <Actias/IO/FileHandle.hpp>
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

static int Build(const String& executable)
{
    const LibraryLoader loader{ "ActiasSDK", true };
    const auto executableRead = File::ReadAllBytes(executable);

    if (!executableRead)
    {
        return EXIT_FAILURE;
    }

    auto bytes = executableRead.Unwrap();

    ActiasNativeExecutableLoadInfo loadInfo{};
    loadInfo.pRawData        = bytes.Data();
    loadInfo.RawDataByteSize = bytes.Size();

    Ptr<INativeExecutable> nativeExecutable;

    auto* actiasLoadNativeExecutable      = loader.FindFunction<ActiasLoadNativeExecutableProc>("ActiasLoadNativeExecutable");
    ExecutableParseError loadNativeResult = actiasLoadNativeExecutable(&nativeExecutable, &loadInfo);
    ActiasExecutableBuildInfo buildInfo{};

    if (loadNativeResult != ExecutableParseError::None)
    {
        std::cerr << "Error loading a PE: " << ExecutableParseErrorTypeToString(loadNativeResult) << std::endl;
        return EXIT_FAILURE;
    }

    buildInfo.pNativeExecutable = nativeExecutable.Get();

    Ptr<IBlob> pExecutableData;

    auto* actiasBuildExecutable = loader.FindFunction<ActiasBuildExecutableProc>("ActiasBuildExecutable");
    actiasBuildExecutable(&pExecutableData, &buildInfo);

    String outputPath(executable.begin(), executable.FindFirstOf('.'));
    auto writeResult = File::WriteBlob(outputPath + ".acbl", pExecutableData.Get(), OpenMode::Create);

    if (writeResult.IsErr())
    {
        std::cerr << "Error writing ACBX file: " << IO::GetResultDesc(writeResult.UnwrapErr()) << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
    int result = 0;

    const std::vector<std::string> args(argv + 1, argv + argc);

    args::ArgumentParser parser(DescriptionMessage);
    parser.Prog(argv[0]);

    args::Group arguments("Arguments");
    const args::HelpFlag help(arguments, "help", "Display this help message", { 'h', "help" });
    const args::Flag versionFlag(
        arguments, "version", "Show the version of Actias runtime and SDK", { 'v', "version" }, args::Options::KickOut);

    args::Group commands(parser, "SDK Commands");
    const args::Command build(commands, "build", "Build an Actias project", [&result](args::Subparser& p) {
        args::Positional<std::string> executable(
            p, "executable", "The native OS executable to build (dll, exe, so, etc.)", args::Options::Required);
        p.Parse();

        auto buildResult = Build(executable.Get().data());
        if (buildResult)
        {
            result = EXIT_FAILURE;
            return;
        }
    });

    const args::Command run(commands, "run", "Run an ACBX executable file", [&result](args::Subparser& p) {
        args::Positional<std::string> executable(p, "executable", "The executable (ACBX) file to run", args::Options::Required);
        p.Parse();

        const String executableName = executable.Get().data();
        const StringSlice executableExtension(((StringSlice)executableName).FindLastOf('.'), ((StringSlice)executableName).end());

        if (executableExtension == ".acbx")
        {
            const LibraryLoader loader{ executableName, false };
            const ActiasProc* mainFunc = loader.FindFunction<ActiasProc>("ActiasMain");

            std::cout << "Running: " << executableName << std::endl;
            const ActiasResult runResult = reinterpret_cast<ActiasResult (*)()>(mainFunc)();

            if (runResult != ACTIAS_SUCCESS)
            {
                result = EXIT_FAILURE;
                return;
            }
        }
        else if (executableExtension == ACTIAS_NATIVE_DL_EXTENSION)
        {
            auto buildResult = Build(executable.Get().data());
            if (buildResult)
            {
                result = EXIT_FAILURE;
                return;
            }

            const LibraryLoader loader{ executableName, true };
            const ActiasProc* mainFunc = loader.FindFunction<ActiasProc>("ActiasMain");

            std::cout << "Running: " << executableName << std::endl;
            const ActiasResult runResult = reinterpret_cast<ActiasResult (*)()>(mainFunc)();

            if (runResult != ACTIAS_SUCCESS)
            {
                result = EXIT_FAILURE;
                return;
            }
        }
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

    return result;
}
