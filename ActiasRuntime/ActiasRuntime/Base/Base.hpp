#pragma once
#include <Actias/System/Core.h>
#include <Actias/System/Platform/Common/RuntimeDef.h>

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
