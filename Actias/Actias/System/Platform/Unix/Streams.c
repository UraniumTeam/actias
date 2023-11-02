#include <Actias/System/Streams.h>
#include <stdio.h>

ActiasResult ActiasWrite(ACTIAS_CONST ActiasHandle* handle, USize byteSize, USize count, FILE* stream)
{
    auto result = fwrite(handle, byteSize, count, stream);
    if (result == 0)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }
    return ACTIAS_SUCCESS;
}

ActiasResult ActiasRead(ActiasHandle handle, USize byteSize, USize count, FILE* stream)
{
    auto result = fread(handle, byteSize, count, stream);
    if (result == 0)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }
    return ACTIAS_SUCCESS;
}