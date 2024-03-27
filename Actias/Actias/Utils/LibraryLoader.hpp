#pragma once
#include <Actias/Strings/StringSlice.hpp>

namespace Actias
{
    class LibraryLoader final
    {
        ActiasHandle m_Handle;
        bool m_IsNative;

        LibraryLoader(const LibraryLoader&)            = delete;
        LibraryLoader& operator=(const LibraryLoader&) = delete;

        void Release();

    public:
        LibraryLoader(StringSlice libraryName, bool forceNative = false);

        inline LibraryLoader(LibraryLoader&& other) noexcept
        {
            m_Handle       = other.m_Handle;
            m_IsNative     = other.m_IsNative;
            other.m_Handle = nullptr;
        }

        inline LibraryLoader& operator=(LibraryLoader&& other) noexcept
        {
            Release();

            m_Handle       = other.m_Handle;
            m_IsNative     = other.m_IsNative;
            other.m_Handle = nullptr;

            return *this;
        }

        ~LibraryLoader();

        void* FindSymbol(StringSlice symbolName) const;

        template<class T>
        inline T* FindFunction(StringSlice name) const
        {
            return reinterpret_cast<T*>(FindSymbol(name));
        }

        inline explicit operator bool() const noexcept
        {
            return m_Handle;
        }
    };
} // namespace Actias
