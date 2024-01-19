#include <CalculatorPluginAPI/ICalculatorPlugin.hpp>

using namespace Actias;

static Float64 ACTIAS_ABI CalculateMultiply(Float64 lhs, Float64 rhs)
{
    return lhs * rhs;
}

static Float64 ACTIAS_ABI CalculateDivide(Float64 lhs, Float64 rhs)
{
    return lhs / rhs;
}

extern "C" ACTIAS_EXPORT OperationCollection ACTIAS_ABI GetCalculatorOperations()
{
    auto result = CreateOperationCollection(2);

    result.pOperations[0].Callback = &CalculateMultiply;
    result.pOperations[0].pName    = "Multiply";

    result.pOperations[1].Callback = &CalculateDivide;
    result.pOperations[1].pName    = "Divide";

    return result;
}

// Currently the version with classes doesn't work
// TODO: fix virtual tables

#if 0

class MultiplyOperation final : public Object<IOperation>
{
public:
    ACTIAS_RTTI_Class(MultiplyOperation, "1BF1202A-5445-4FFC-8972-7785784A1F9F");

    Actias::StringSlice GetName() const override
    {
        return ac_nameof(*this);
    }

    Float64 Calculate(Float64 lhs, Float64 rhs) override
    {
        return CalculateMultiply(lhs, rhs);
    }
};

class DivideOperation final : public Object<IOperation>
{
public:
    ACTIAS_RTTI_Class(DivideOperation, "FBFF3AC1-92E4-45C9-A71C-A9CCA836D6FD");

    Actias::StringSlice GetName() const override
    {
        return ac_nameof(*this);
    }

    Float64 Calculate(Float64 lhs, Float64 rhs) override
    {
        return CalculateDivide(lhs, rhs);
    }
};

class MultiplicationPlugin final : public Object<ICalculatorPlugin>
{
    Actias::List<Actias::Ptr<IOperation>> m_Operations;

public:
    MultiplicationPlugin()
    {
        m_Operations.Push(AllocateObject<MultiplyOperation>());
        m_Operations.Push(AllocateObject<DivideOperation>());
    }

    void GetOperations(IOperation** ppOperations, USize* pOperationCount) override
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
};

extern "C" ACTIAS_EXPORT ICalculatorPlugin* ACTIAS_ABI CreateCalculatorPlugin()
{
    SystemAllocator allocator;
    return AllocateObjectEx<MultiplicationPlugin>(&allocator);
}

#endif
