#include "BasicPlugin.hpp"

using namespace Actias;

class AddOperation final : public Object<IOperation>
{
public:
    ACTIAS_RTTI_Class(AddOperation, "8BC4C96E-75EF-4C01-99F5-88CE78FC58BA");

    Actias::StringSlice GetName() const override
    {
        return ac_nameof(*this);
    }

    Float64 Calculate(Float64 lhs, Float64 rhs) override
    {
        return lhs + rhs;
    }
};

class SubtractOperation final : public Object<IOperation>
{
public:
    ACTIAS_RTTI_Class(SubtractOperation, "26902DC5-7637-46BE-94B4-E6B5A22D3871");

    Actias::StringSlice GetName() const override
    {
        return ac_nameof(*this);
    }

    Float64 Calculate(Float64 lhs, Float64 rhs) override
    {
        return lhs - rhs;
    }
};

BasicPlugin::BasicPlugin()
{
    m_Operations.Push(AllocateObject<AddOperation>());
    m_Operations.Push(AllocateObject<SubtractOperation>());
}

void BasicPlugin::GetOperations(IOperation** ppOperations, USize* pOperationCount)
{
    if (pOperationCount)
    {
        *pOperationCount = m_Operations.Size();
    }

    if (ppOperations)
    {
        for (USize i = 0; i < m_Operations.Size(); ++i)
        {
            ppOperations[i] = m_Operations[i].Get();
            ppOperations[i]->AddRef();
        }
    }
}
