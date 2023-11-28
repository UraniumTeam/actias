#include <Actias/System/Runtime.h>
#include <Actias/Utils/LibraryLoader.hpp>
#include <Actias/Strings/String.hpp>

namespace Actias
{
    LibraryLoader::LibraryLoader(StringSlice libraryName, bool forceNative)
    {
        String name = libraryName;
        name += ".acbl";

        if (!forceNative)
        {
            auto result = ActiasLoadModule(name.Data(), &m_Handle);
            if (result == ACTIAS_SUCCESS)
            {
                m_IsNative = false;
                return;
            }
        }

        String nativeName = libraryName;
        nativeName += ACTIAS_NATIVE_DL_EXTENSION;

        ActiasLoadNativeModule(nativeName.Data(), &m_Handle);
        m_IsNative = true;
    }

    LibraryLoader::~LibraryLoader()
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

    void* LibraryLoader::FindSymbol(StringSlice symbolName)
    {
        ActiasProc address;
        auto result = m_IsNative ? ActiasFindNativeSymbolAddress(m_Handle, symbolName.Data(), &address)
                                 : ActiasFindSymbolAddress(m_Handle, symbolName.Data(), &address);
        if (result == ACTIAS_SUCCESS)
        {
            return address;
        }

        return nullptr;
    }
} // namespace Actias
