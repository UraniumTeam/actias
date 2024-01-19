#pragma once
#include <Actias/Strings/StringSlice.hpp>

namespace Actias
{
    class LibraryLoader
    {
        ActiasHandle m_Handle;
        bool m_IsNative;

        LibraryLoader(const LibraryLoader&)            = delete;
        LibraryLoader& operator=(const LibraryLoader&) = delete;

        void Release();

    public:
        LibraryLoader(StringSlice libraryName, bool forceNative = false);

        LibraryLoader(LibraryLoader&& other) noexcept
        {
            m_Handle       = other.m_Handle;
            m_IsNative     = other.m_IsNative;
            other.m_Handle = nullptr;
        }

        LibraryLoader& operator=(LibraryLoader&& other) noexcept
        {
            Release();

            m_Handle       = other.m_Handle;
            m_IsNative     = other.m_IsNative;
            other.m_Handle = nullptr;

            return *this;
        }

        ~LibraryLoader();

        void* FindSymbol(StringSlice symbolName);

        template<class T>
        inline T* FindFunction(StringSlice name)
        {
            return reinterpret_cast<T*>(FindSymbol(name));
        }
    };
} // namespace Actias
