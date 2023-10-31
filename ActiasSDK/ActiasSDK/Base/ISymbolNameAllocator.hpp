#pragma once
#include <Actias/Strings/StringSlice.hpp>

namespace Actias::SDK
{
    class ISymbolNameAllocator
    {
    public:
        virtual void* Allocate(USize byteSize, UInt64& address) = 0;
    };
} // namespace Actias::SDK
