#include "BasicPlugin.hpp"
#include <Actias/Containers/ArraySlice.hpp>
#include <Actias/System/Runtime.h>
#include <Actias/Utils/LibraryLoader.hpp>
#include <filesystem>
#include <iostream>

using namespace Actias;

List<LibraryLoader> LoadPluginLibraries()
{
    List<LibraryLoader> result;

    for (auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path()))
    {
        if (!entry.is_regular_file())
        {
            continue;
        }

        if (entry.path().extension() == ".acbl")
        {
            auto path = entry.path().u8string();
            LibraryLoader lib{ StringSlice(path) };
            if (lib.FindSymbol("CreateCalculatorPlugin") || lib.FindSymbol("GetCalculatorOperations"))
            {
                result.Push(std::move(lib));
            }
        }
    }

    return result;
}

List<Ptr<IOperation>> LoadOperationsFromPlugins(const ArraySlice<Ptr<ICalculatorPlugin>>& plugins)
{
    List<Ptr<IOperation>> result;

    for (auto& plugin : plugins)
    {
        USize count;
        plugin->GetOperations(nullptr, &count);

        result.Resize(result.Size() + count);
        plugin->GetOperations((result.end() - count)->GetAddressOf(), &count);
    }

    return result;
}

typedef ICalculatorPlugin* ACTIAS_ABI CreateCalculatorPluginProc();

typedef OperationCollection ACTIAS_ABI GetCalculatorOperationsProc();

int main()
{
    if (ActiasInit() < 0)
    {
        std::cerr << "Failed to initialize the Runtime!" << std::endl;
        return EXIT_FAILURE;
    }

    auto libraries = LoadPluginLibraries();

    List<Ptr<ICalculatorPlugin>> plugins;
    plugins.Push(AllocateObject<BasicPlugin>());

    for (auto& library : libraries)
    {
        auto* createPlugin = library.FindFunction<CreateCalculatorPluginProc>("CreateCalculatorPlugin");

        if (createPlugin)
        {
            plugins.Push(createPlugin());
            continue;
        }

        auto* getOperations = library.FindFunction<GetCalculatorOperationsProc>("GetCalculatorOperations");

        plugins.Push(AllocateObject<ExternalCalculatorPlugin>(getOperations()));
    }

    auto operations = LoadOperationsFromPlugins(plugins);

    Float64 leftOperand;
    Float64 rightOperand;

    std::cout << "Enter the left operand: ";
    std::cin >> leftOperand;

    std::cout << std::endl << "Enter the right operand: ";
    std::cin >> rightOperand;

    std::cout << std::endl << "\nList of available operations:\n";

    for (USize i = 0; i < operations.Size(); ++i)
    {
        std::cout << i << ":\t" << operations[i]->GetName() << "\n";
    }

    USize operation;

    std::cout << std::endl << "Choose the operation: ";
    std::cin >> operation;

    if (operation >= operations.Size())
    {
        std::cerr << std::endl << "Error: there is no operation at " << operation << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << std::endl << "The result is: " << operations[operation]->Calculate(leftOperand, rightOperand) << std::endl;
}
