#pragma once
#include <Actias/Strings/StringSlice.hpp>

namespace Actias
{
    class LibraryLoader
    {
        ActiasHandle m_Handle;
        bool m_IsNative;

    public:
        LibraryLoader(StringSlice libraryName, bool forceNative = false);

        ~LibraryLoader();

        void* FindSymbol(StringSlice symbolName);

        template <class T>
        inline T* FindFunction(StringSlice name)
        {
            return reinterpret_cast<T*>(FindSymbol(name));
        }
    };
} // namespace Actias
