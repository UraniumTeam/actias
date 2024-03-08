#include <Actias/Strings/String.hpp>
#include <Actias/System/Runtime.h>
#include <Actias/Utils/LibraryLoader.hpp>

namespace Actias
{
    void LibraryLoader::Release()
    {
        if (!m_Handle)
        {
            return;
        }

        if (m_IsNative)
        {
            ActiasUnloadNativeModule(m_Handle);
            return;
        }

        ActiasUnloadModule(m_Handle);
    }

    LibraryLoader::LibraryLoader(StringSlice libraryName, bool forceNative)
    {
        String name = libraryName;

        if (!libraryName.EndsWith(".acbl"))
        {
            name += ".acbl";
        }

        if (!forceNative)
        {
            const auto result = ActiasLoadModule(name.Data(), &m_Handle);
            if (result == ACTIAS_SUCCESS)
            {
                m_IsNative = false;
                return;
            }
        }

        String nativeName = libraryName;
        if (!libraryName.EndsWith(ACTIAS_NATIVE_DL_EXTENSION))
        {
            nativeName += ACTIAS_NATIVE_DL_EXTENSION;
        }

        ActiasLoadNativeModule(nativeName.Data(), &m_Handle);
        m_IsNative = true;
    }

    LibraryLoader::~LibraryLoader()
    {
        Release();
    }

    void* LibraryLoader::FindSymbol(StringSlice symbolName) const
    {
        ActiasProc address;
        const auto result = m_IsNative ? ActiasFindNativeSymbolAddress(m_Handle, symbolName.Data(), &address)
                                       : ActiasFindSymbolAddress(m_Handle, symbolName.Data(), &address);
        if (result == ACTIAS_SUCCESS)
        {
            return reinterpret_cast<void*>(address);
        }

        return nullptr;
    }
} // namespace Actias
