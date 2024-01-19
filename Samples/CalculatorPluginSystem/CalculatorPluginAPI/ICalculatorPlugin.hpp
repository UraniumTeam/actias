#pragma once
#include <Actias/System/Memory.h>
#include <CalculatorPluginAPI/IOperation.hpp>

ACTIAS_BEGIN_C

typedef struct OperationCollection
{
    CalculatorOperation* pOperations;
    USize OperationCount;
} OperationCollection;

inline static OperationCollection CreateOperationCollection(USize operationCount)
{
    OperationCollection result;
    result.OperationCount = operationCount;
    result.pOperations    = (CalculatorOperation*)ActiasAlloc(sizeof(CalculatorOperation) * operationCount);
    return result;
}

ACTIAS_END_C

#ifdef __cplusplus

class ICalculatorPlugin : public Actias::IObject
{
public:
    ACTIAS_RTTI_Class(ICalculatorPlugin, "70B6256D-6BDB-449A-BE5C-B258476760DD");

    virtual void GetOperations(IOperation** ppOperations, USize* pOperationCount) = 0;
};

class ExternalCalculatorPlugin : public Actias::Object<ICalculatorPlugin>
{
    Actias::List<Actias::Ptr<IOperation>> m_Operations;

public:
    ACTIAS_RTTI_Class(ExternalCalculatorPlugin, "4A1B5060-D314-45E1-8FCB-B5F69FE86FF1");

    inline ExternalCalculatorPlugin(const OperationCollection& operations)
    {
        for (USize i = 0; i < operations.OperationCount; ++i)
        {
            m_Operations.Push(Actias::AllocateObject<ExternalOperation>(operations.pOperations[i]));
        }
    }

    inline void GetOperations(IOperation** ppOperations, USize* pOperationCount) override
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

#endif
