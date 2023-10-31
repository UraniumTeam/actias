#pragma once
#include <Actias/System/Core.h>

#if ActiasRuntime_EXPORTS
#    define ACTIAS_RUNTIME_API ACTIAS_EXPORT
#else
#    define ACTIAS_RUNTIME_API ACTIAS_IMPORT
#endif

namespace Actias::Runtime
{
    enum class ResultCode : ActiasResult
    {
        Success = ACTIAS_SUCCESS,

        UnknownError       = ACTIAS_FAIL_UNKNOWN,
        NotSupported       = ACTIAS_FAIL_NOT_SUPPORTED,
        InsufficientSize   = ACTIAS_FAIL_INSUFFICIENT_SIZE,
        InvalidSignature   = ACTIAS_FAIL_ACBX_INVALID_SIGNATURE,
        SectionOutOfBounds = ACTIAS_FAIL_ACBX_SECTION_OUT_OF_BOUNDS,
    };
} // namespace Actias::Runtime
