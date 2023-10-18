#pragma once
#include <Actias/Utils/Result.h>

namespace Actias::SDK
{
    enum class ExecutableParseErrorType
    {
        None,
        InsufficientSize,
        UnknownExecutableFormat,

        InvalidDOSHeader,
        InvalidNTHeader,

        SectionOutOfBounds,
    };

    struct ExecutableParseError
    {
        ExecutableParseErrorType Type;
        UInt64 Offset;

        inline ExecutableParseError(ExecutableParseErrorType type, UInt64 offset)
            : Type(type)
            , Offset(offset)
        {
        }

        inline static ExecutableParseError InsufficientSize(UInt64 offset = 0) noexcept
        {
            return ExecutableParseError(ExecutableParseErrorType::InsufficientSize, offset);
        }

        inline static ExecutableParseError UnknownExecutableFormat(UInt64 offset = 0) noexcept
        {
            return ExecutableParseError(ExecutableParseErrorType::UnknownExecutableFormat, offset);
        }

        inline static ExecutableParseError InvalidDOSHeader(UInt64 offset) noexcept
        {
            return ExecutableParseError(ExecutableParseErrorType::InvalidDOSHeader, offset);
        }

        inline static ExecutableParseError InvalidNTHeader(UInt64 offset) noexcept
        {
            return ExecutableParseError(ExecutableParseErrorType::InvalidNTHeader, offset);
        }

        inline static ExecutableParseError SectionOutOfBounds(UInt64 offset) noexcept
        {
            return ExecutableParseError(ExecutableParseErrorType::SectionOutOfBounds, offset);
        }
    };

    template<class T>
    using ExecutableParseResult = Result<T, ExecutableParseError>;
} // namespace Actias::SDK
