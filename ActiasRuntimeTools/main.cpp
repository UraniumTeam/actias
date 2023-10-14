#include <Actias/System/Memory.h>
#include <args.hxx>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

constexpr const char* DescriptionMessage = "Actias CLI (runtime and SDK)";
constexpr const char* VersionMessage     = R"(Actias SDK version 0.1
Actias runtime version 0.1)";

int main(int argc, char** argv)
{
    const std::vector<std::string> args(argv + 1, argv + argc);

    args::ArgumentParser parser(DescriptionMessage);
    parser.Prog(argv[0]);

    args::Group arguments("Arguments");
    args::HelpFlag help(arguments, "help", "Display this help message", { 'h', "help" });
    args::Flag versionFlag(
        arguments, "version", "Show the version of Actias runtime and SDK", { 'v', "version" }, args::Options::KickOut);

    args::Group commands(parser, "SDK Commands", args::Group::Validators::Xor);
    args::Command build(commands, "build", "Build an Actias project", [](args::Subparser& p) {
        args::Positional<std::string> executable(
            p, "executable", "The native OS executable to build (dll, exe, so, etc.)", args::Options::Required);
        p.Parse();

        std::cout << "Building: " << executable.Get() << std::endl;
    });

    args::Command run(commands, "run", "Run an ACBX executable file", [](args::Subparser& p) {
        args::Positional<std::string> executable(p, "executable", "The executable (ACBX) file to run", args::Options::Required);
        p.Parse();

        std::cout << "Running: " << executable.Get() << std::endl;
    });

    args::GlobalOptions globals(parser, arguments);

    try
    {
        parser.ParseCLI(args);
    }
    catch (args::Help)
    {
        std::cout << parser;
        return 0;
    }
    catch (args::ParseError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    catch (args::ValidationError e)
    {
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
