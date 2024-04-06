#pragma once
#include <Actias/Strings/FixedString.hpp>
#include <Actias/Strings/StringSlice.hpp>
#include <Actias/System/Mutex.h>
#include <ActiasRuntime/Base/Base.hpp>

namespace Actias::Runtime
{
    //! \brief Loaded runtime module information.
    struct ModuleInfo
    {
        FixStr512 Name;              //!< Module name.
        ActiasHandle Handle     = 0; //!< Module handle.
        UInt64 ImageSize        = 0; //!< Size of the loaded image, used for deallocation.
        UInt32 ReferenceCounter = 0; //!< Number of references to this module, incremented by ActiasLoadModule.
    };

    namespace Kernel
    {
        //! \brief Runtime Kernel-specific scoped lock.
        class LockScope final
        {
            friend class Impl;

            ActiasMutex* m_pMutex;
            bool* m_pOwnerFlag;

            LockScope(const LockScope&)            = delete;
            LockScope& operator=(const LockScope&) = delete;

            inline LockScope(ActiasMutex* pMutex, bool* pOwnerFlag)
                : m_pMutex(pMutex)
                , m_pOwnerFlag(pOwnerFlag)
            {
                ActiasLockMutex(pMutex);
                *m_pOwnerFlag = true;
            }

        public:
            LockScope(LockScope&& other) noexcept
                : m_pMutex(other.m_pMutex)
                , m_pOwnerFlag(other.m_pOwnerFlag)
            {
                other.m_pMutex     = nullptr;
                other.m_pOwnerFlag = nullptr;
            }

            ~LockScope()
            {
                if (m_pMutex)
                {
                    ActiasUnlockMutex(m_pMutex);
                    if (m_pOwnerFlag)
                    {
                        *m_pOwnerFlag = false;
                    }
                }
            }
        };

        LockScope AquireLock();

        //! \brief Returns true if the module was found.
        bool TryGetModule(const StringSlice& name, ModuleInfo& moduleInfo);

        //! \brief Add a new module.
        void AddModule(ModuleInfo* pModuleInfo);

        //! \brief Returns true if the module must be unloaded.
        bool RemoveModuleReference(const ModuleInfo& moduleInfo);
    } // namespace Kernel
} // namespace Actias::Runtime
