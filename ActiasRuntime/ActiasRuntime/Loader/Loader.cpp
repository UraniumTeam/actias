#include <Actias/IO/FileStream.hpp>
#include <ActiasRuntime/Base/Base.hpp>
#include <ActiasRuntime/Builder/ModuleBuilder.hpp>

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

    auto buildResult     = builder.Build();
    auto buildResultCode = buildResult.UnwrapErrOrDefault();
    if (buildResultCode != ResultCode::Success)
    {
        return static_cast<ActiasResult>(buildResultCode);
    }

    *pModuleHandle = buildResult.Unwrap();

    return static_cast<ActiasResult>(builder.ImportAll().UnwrapErrOrDefault());
}

extern "C" ACTIAS_RUNTIME_API ActiasResult ACTIAS_ABI ActiasRtUnloadModule(ActiasHandle moduleHandle)
{
    auto moduleSize = *reinterpret_cast<UInt64*>(moduleHandle);
    return ActiasVirtualFree(moduleHandle, static_cast<USize>(moduleSize));
}

extern "C" ACTIAS_RUNTIME_API ActiasResult ACTIAS_ABI ActiasRtFindSymbolAddress(ActiasHandle moduleHandle,
                                                                                const char* pSymbolName, ActiasProc* pAddress)
{
    auto* pModule = reinterpret_cast<Byte*>(moduleHandle);

    const auto* pExportHeader = LocateExportTable(moduleHandle);
    const auto* pExportTable  = reinterpret_cast<ACBXExportTableEntry*>(pModule + pExportHeader->Address);
    for (UInt64 i = 0; i < pExportHeader->EntryCount; ++i)
    {
        const auto& entry = pExportTable[i];
        const char* pName = reinterpret_cast<const char*>(pModule + entry.NameAddress);
        if (strcmp(pName, pSymbolName) == 0)
        {
            *pAddress = reinterpret_cast<ActiasProc>(pModule + entry.SymbolAddress);
            return ACTIAS_SUCCESS;
        }
    }

    return ACTIAS_FAIL_SYMBOL_NOT_FOUND;
}
