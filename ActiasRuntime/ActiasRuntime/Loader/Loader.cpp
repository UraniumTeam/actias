#include <Actias/IO/FileStream.hpp>
#include <ActiasRuntime/Base/Base.hpp>
#include <ActiasRuntime/Builder/ModuleBuilder.hpp>
#include <ActiasRuntime/Kernel/RuntimeKernel.hpp>
#include <algorithm>

using namespace Actias;
using namespace Actias::Runtime;

inline ACBXExportTableHeader* LocateExportTable(ActiasHandle moduleHandle)
{
    auto* ptr = reinterpret_cast<Byte*>(moduleHandle);
    ptr       = AlignUpPtr(ptr + sizeof(ACBXFileInformationHeader), ACBX_FILE_ALIGNMENT);
    return reinterpret_cast<ACBXExportTableHeader*>(ptr);
}

extern "C" ACTIAS_RUNTIME_API ActiasResult ACTIAS_ABI ActiasRtLoadModule(const ACBXLoaderRunInfo* pRunInfo,
                                                                         ActiasHandle* pModuleHandle)
{
    const Kernel::LockScope lk = Kernel::AquireLock();

    const StringSlice moduleName = pRunInfo->pModulePath;
    ModuleInfo moduleInfo{};
    if (Kernel::TryGetModule(moduleName, moduleInfo))
    {
        *pModuleHandle = moduleInfo.Handle;
        return ACTIAS_SUCCESS;
    }

    Ptr pFile   = AllocateObject<IO::FileHandle>();
    Ptr pStream = AllocateObject<IO::FileStream>(pFile.Get());

    const auto fileResult = pStream->Open(moduleName, IO::OpenMode::ReadOnly).UnwrapErrOrDefault();
    if (fileResult != IO::ResultCode::Success)
    {
        return static_cast<ActiasResult>(fileResult);
    }

    ModuleBuilder builder;
    const auto loadResult = builder.LoadFromStream(pStream.Get()).UnwrapErrOrDefault();
    if (loadResult != IO::ResultCode::Success)
    {
        return static_cast<ActiasResult>(loadResult);
    }

    const auto headerResult = builder.ParseHeaders().UnwrapErrOrDefault();
    if (headerResult != ResultCode::Success)
    {
        return static_cast<ActiasResult>(headerResult);
    }

    const auto buildResult     = builder.Build();
    const auto buildResultCode = buildResult.UnwrapErrOrDefault();
    if (buildResultCode != ResultCode::Success)
    {
        return static_cast<ActiasResult>(buildResultCode);
    }

    ModuleInfo* pInfo = new (ActiasAlloc(sizeof(ModuleInfo))) ModuleInfo;
    *pInfo            = buildResult.Unwrap();
    pInfo->Name       = moduleName;
    *pModuleHandle    = pInfo->Handle;
    Kernel::AddModule(pInfo);
    *reinterpret_cast<ModuleInfo**>(pInfo->Handle) = pInfo;

    const auto result = static_cast<ActiasResult>(builder.ImportAll().UnwrapErrOrDefault());
    if (result < 0)
    {
        ACTIAS_AssertDebug(Kernel::RemoveModuleReference(*pInfo));
        ActiasVirtualFree(pInfo->Handle, pInfo->ImageSize);
        pInfo->~ModuleInfo();
        ActiasFree(pInfo);
        return result;
    }

    return result;
}

extern "C" ACTIAS_RUNTIME_API ActiasResult ACTIAS_ABI ActiasRtUnloadModule(ActiasHandle moduleHandle)
{
    ModuleInfo* pInfo = *reinterpret_cast<ModuleInfo**>(moduleHandle);
    if (Kernel::RemoveModuleReference(*pInfo))
    {
        const auto imageSize = pInfo->ImageSize;
        pInfo->~ModuleInfo();
        ActiasFree(pInfo);
        return ActiasVirtualFree(moduleHandle, static_cast<USize>(imageSize));
    }

    return ACTIAS_SUCCESS;
}

extern "C" ACTIAS_RUNTIME_API ActiasResult ACTIAS_ABI ActiasRtFindSymbolAddress(ActiasHandle moduleHandle,
                                                                                const char* pSymbolName, ActiasProc* pAddress)
{
    const auto* pModule = reinterpret_cast<const Byte*>(moduleHandle);

    const auto* pExportHeader = LocateExportTable(moduleHandle);
    const auto* pExportTable  = reinterpret_cast<const ACBXExportTableEntry*>(pModule + pExportHeader->Address);

    const auto* pEntry = std::lower_bound(pExportTable,
                                          pExportTable + pExportHeader->EntryCount,
                                          pSymbolName,
                                          [pModule](const ACBXExportTableEntry& lhs, const char* rhs) {
                                              const char* pName = reinterpret_cast<const char*>(pModule + lhs.NameAddress);
                                              return strcmp(pName, rhs) < 0;
                                          });

    if (pEntry - pExportTable >= static_cast<SSize>(pExportHeader->EntryCount))
    {
        return ACTIAS_FAIL_SYMBOL_NOT_FOUND;
    }

    const char* pFoundName = reinterpret_cast<const char*>(pModule + pEntry->NameAddress);
    if (strcmp(pFoundName, pSymbolName) == 0)
    {
        *pAddress = reinterpret_cast<ActiasProc>(pModule + pEntry->SymbolAddress);
        return ACTIAS_SUCCESS;
    }

    return ACTIAS_FAIL_SYMBOL_NOT_FOUND;
}
