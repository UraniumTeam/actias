#pragma once
#include <CalculatorPluginAPI/ICalculatorPlugin.hpp>

class BasicPlugin final : public Actias::Object<ICalculatorPlugin>
{
    Actias::List<Actias::Ptr<IOperation>> m_Operations;

public:
    ACTIAS_RTTI_Class(BasicPlugin, "BFEC7C3C-91B8-4304-8340-B0CBD05DC064");

    BasicPlugin();

    void GetOperations(IOperation** ppOperations, USize* pOperationCount) override;
};
