#pragma once
#include <Actias/System/Core.h>

ACTIAS_BEGIN_C

enum ActiasMainCallReasonValues
{
    ACTIAS_MAIN_CALL_REASON_NONE = 0x00,

    ACTIAS_MAIN_CALL_REASON_LIBRARY_LOAD   = 0x11,
    ACTIAS_MAIN_CALL_REASON_LIBRARY_UNLOAD = 0x12,

    ACTIAS_MAIN_CALL_REASON_EXECUTABLE_START = 0x21,
    ACTIAS_MAIN_CALL_REASON_EXECUTABLE_EXIT  = 0x22
};

typedef Int32 ActiasMainCallReason;

#define ActiasMain_ProcName "ActiasMain"
#define ActiasLibraryMain_ProcName "ActiasLibraryMain"

typedef Int32 ACTIAS_ABI ActiasMainProc(ActiasMainCallReason, const char*);
typedef ActiasResult ACTIAS_ABI ActiasLibraryMainProc(ActiasMainCallReason, ActiasHandle);

ACTIAS_END_C
