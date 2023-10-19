#pragma once
#include <Actias/Utils/Result.hpp>

namespace Actias::SDK
{
    enum class ExecutableParseError : Int32
    {
        None,
        InsufficientSize        = -1,
        UnknownExecutableFormat = -2,

        InvalidDOSHeader = -3,
        InvalidNTHeader  = -4,

        SectionOutOfBounds = -5,
    };

    inline const char* ExecutableParseErrorTypeToString(ExecutableParseError type)
    {
#define ACTIAS_ENUM_STRING_ENTRY(name)                                                                                           \
    case ExecutableParseError::name:                                                                                             \
        return #name
        switch (type)
        {
            ACTIAS_ENUM_STRING_ENTRY(None);
            ACTIAS_ENUM_STRING_ENTRY(InsufficientSize);
            ACTIAS_ENUM_STRING_ENTRY(UnknownExecutableFormat);
            ACTIAS_ENUM_STRING_ENTRY(InvalidDOSHeader);
            ACTIAS_ENUM_STRING_ENTRY(InvalidNTHeader);
            ACTIAS_ENUM_STRING_ENTRY(SectionOutOfBounds);
        default:
            return "<Unknown>";
        }
#undef ACTIAS_ENUM_STRING_ENTRY
    }

    template<class T>
    using ExecutableParseResult = Result<T, ExecutableParseError>;
} // namespace Actias::SDK
