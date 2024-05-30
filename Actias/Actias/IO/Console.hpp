#pragma once
#include <Actias/Strings/StringSlice.hpp>
#include <Actias/System/Streams.h>

namespace Actias::IO::Console
{
    void Write(StringSlice str);
    void WriteLine(StringSlice str);

    void WriteErr(StringSlice str);
    void WriteLineErr(StringSlice str);
} // namespace Actias::IO::Console
