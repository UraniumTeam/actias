#include <Actias/Strings/String.hpp>
#include <ActiasRuntime/Kernel/RuntimeKernel.hpp>
#include <unordered_map>

namespace Actias::Runtime::Kernel
{
    namespace
    {
        thread_local bool g_IsMutexOwned = false;
        ActiasMutex g_KernelMutex;

        struct KernelInitializer
        {
            KernelInitializer()
            {
                ActiasInitializeMutex(&g_KernelMutex);
            }

            ~KernelInitializer()
            {
                ActiasReleaseMutex(&g_KernelMutex);
            }
        } g_KernelInitObject;
    } // namespace

    class Impl
    {
        std::unordered_map<FixStr512, ModuleInfo*> m_ModuleMap;

    public:
        inline LockScope AquireLock()
        {
            ACTIAS_AssertCritical(!g_IsMutexOwned && "Kernel mutex is already owned by this thread");
            return LockScope(&g_KernelMutex, &g_IsMutexOwned);
        }

        inline bool TryGetModule(const StringSlice& name, ModuleInfo& moduleInfo) const
        {
            ACTIAS_AssertCritical(g_IsMutexOwned && "Kernel mutex is not owned by the caller thread");

            const auto it = m_ModuleMap.find(name);
            if (it != m_ModuleMap.end())
            {
                moduleInfo = *it->second;
                return true;
            }

            return false;
        }

        inline void AddModule(ModuleInfo* pModuleInfo)
        {
            ACTIAS_AssertCritical(g_IsMutexOwned && "Kernel mutex is not owned by the caller thread");
            ACTIAS_AssertDebug(m_ModuleMap.find(pModuleInfo->Name) == m_ModuleMap.end() && "Cannot add a module twice");

            m_ModuleMap.insert(std::make_pair(pModuleInfo->Name, pModuleInfo));
        }

        inline bool RemoveModuleReference(const ModuleInfo& moduleInfo)
        {
            ACTIAS_AssertCritical(g_IsMutexOwned && "Kernel mutex is not owned by the caller thread");

            const auto it = m_ModuleMap.find(moduleInfo.Name);
            ACTIAS_AssertCritical(it != m_ModuleMap.end() && "The module doesn't exist");
            ACTIAS_AssertCritical(it->second == &moduleInfo);
            ACTIAS_AssertDebug(it->second->ReferenceCounter > 0);

            if (--it->second->ReferenceCounter == 0)
            {
                m_ModuleMap.erase(it);
                return true;
            }

            return false;
        }
    };

    static Impl g_Impl;

    LockScope AquireLock()
    {
        return g_Impl.AquireLock();
    }

    bool TryGetModule(const StringSlice& name, ModuleInfo& moduleInfo)
    {
        return g_Impl.TryGetModule(name, moduleInfo);
    }

    void AddModule(ModuleInfo* pModuleInfo)
    {
        return g_Impl.AddModule(pModuleInfo);
    }

    bool RemoveModuleReference(const ModuleInfo& moduleInfo)
    {
        return g_Impl.RemoveModuleReference(moduleInfo);
    }
} // namespace Actias::Runtime::Kernel
