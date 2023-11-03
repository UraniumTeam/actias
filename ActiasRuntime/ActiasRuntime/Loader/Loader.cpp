#include <Actias/IO/FileStream.hpp>
#include <ActiasRuntime/Base/Base.hpp>
#include <ActiasRuntime/Builder/ModuleBuilder.hpp>

using namespace Actias;
using namespace Actias::Runtime;

inline ACBXExportTableHeader* LocateExportTable(ActiasHandle moduleHandle)
{
    auto* ptr = ac_byte_cast(moduleHandle);
    ptr       = AlignUpPtr(ptr + sizeof(ACBXFileInformationHeader), ACBX_FILE_ALIGNMENT);
    return reinterpret_cast<ACBXExportTableHeader*>(ptr);
}

extern "C" ACTIAS_RUNTIME_API ActiasResult ACTIAS_ABI ActiasRtLoadModule(ACBXLoaderRunInfo* pRunInfo, ActiasHandle* pModuleHandle)
{
    Ptr pFile   = AllocateObject<IO::FileHandle>();
    Ptr pStream = AllocateObject<IO::FileStream>(pFile.Get());

    auto fileResult = pStream->Open(pRunInfo->pModulePath, IO::OpenMode::ReadOnly).UnwrapErrOrDefault();
    if (fileResult != IO::ResultCode::Success)
    {
        return static_cast<ActiasResult>(fileResult);
    }

    ModuleBuilder builder;
    auto loadResult = builder.LoadFromStream(pStream.Get()).UnwrapErrOrDefault();
    if (loadResult != IO::ResultCode::Success)
    {
        return static_cast<ActiasResult>(loadResult);
    }

    auto headerResult = builder.ParseHeaders().UnwrapErrOrDefault();
    if (headerResult != ResultCode::Success)
    {
        return static_cast<ActiasResult>(headerResult);
    }

    auto buildResult = builder.Build();
    if (buildResult)
    {
        *pModuleHandle = buildResult.Unwrap();
        return ACTIAS_SUCCESS;
    }

    return static_cast<ActiasResult>(buildResult.UnwrapErr());
}

extern "C" ACTIAS_RUNTIME_API ActiasResult ACTIAS_ABI ActiasRtUnloadModule(ActiasHandle moduleHandle)
{
    auto moduleSize = *reinterpret_cast<UInt64*>(moduleHandle);
    return ActiasVirtualFree(moduleHandle, static_cast<USize>(moduleSize));
}

extern "C" ACTIAS_RUNTIME_API ActiasResult ACTIAS_ABI ActiasRtFindSymbolAddress(ActiasHandle moduleHandle,
                                                                                ACTIAS_CONST char* pSymbolName,
                                                                                ActiasProc* pAddress)
{
    const auto* pExportHeader = LocateExportTable(moduleHandle);
    const auto* pExportTable  = reinterpret_cast<ACBXExportTableEntry*>(ac_byte_cast(moduleHandle) + pExportHeader->Address);
    for (UInt64 i = 0; i < pExportHeader->EntryCount; ++i)
    {
        const auto& entry = pExportTable[i];
        const char* pName = reinterpret_cast<const char*>(ac_byte_cast(moduleHandle) + entry.NameAddress);
        if (strcmp(pName, pSymbolName) == 0)
        {
            *pAddress = reinterpret_cast<ActiasProc>(ac_byte_cast(moduleHandle) + entry.SymbolAddress);
            return ACTIAS_SUCCESS;
        }
    }

    return ACTIAS_FAIL_SYMBOL_NOT_FOUND;
}
