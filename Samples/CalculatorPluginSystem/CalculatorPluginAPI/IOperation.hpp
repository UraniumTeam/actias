#pragma once
#include <Actias/System/Base.h>

ACTIAS_BEGIN_C

typedef Float64(ACTIAS_ABI* CalculatorOperationCallback)(Float64, Float64);

typedef struct CalculatorOperation
{
    CalculatorOperationCallback Callback;
    const char* pName;
} CalculatorOperation;

ACTIAS_END_C

#ifdef __cplusplus

#    include <Actias/Memory/Object.hpp>
#    include <Actias/Strings/String.hpp>

class IOperation : public Actias::IObject
{
public:
    ACTIAS_RTTI_Class(IOperation, "1B4E8C83-AB31-4A19-8C95-C37FFB13A014");

    virtual Actias::StringSlice GetName() const         = 0;
    virtual Float64 Calculate(Float64 lhs, Float64 rhs) = 0;
};

class ExternalOperation : public Actias::Object<IOperation>
{
    CalculatorOperationCallback m_Operation;
    Actias::String m_Name;

public:
    inline ExternalOperation(CalculatorOperation operation)
    {
        m_Operation = operation.Callback;
        m_Name      = operation.pName;
    }

    inline Actias::StringSlice GetName() const override
    {
        return m_Name;
    }

    inline Float64 Calculate(Float64 lhs, Float64 rhs) override
    {
        return m_Operation(lhs, rhs);
    }
};

#endif
