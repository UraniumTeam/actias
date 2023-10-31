#include <Actias/IO/FileStream.hpp>
#include <ActiasRuntime/Builder/ModuleBuilder.hpp>
#include <ActiasRuntime/Loader/Loader.hpp>

using namespace Actias;
using namespace Actias::Runtime;

extern "C" ACTIAS_RUNTIME_API ActiasResult ACTIAS_ABI ActiasRtRunLoader(ActiasHandle* pModuleHandle, ACBXLoaderRunInfo* pRunInfo)
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

    return ACTIAS_SUCCESS;
}
