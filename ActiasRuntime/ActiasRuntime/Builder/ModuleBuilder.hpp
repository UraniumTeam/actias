#pragma once
#include <Actias/ACBX/Headers.h>
#include <Actias/Base/Byte.hpp>
#include <Actias/Containers/HeapArray.hpp>
#include <Actias/IO/IStream.hpp>
#include <ActiasRuntime/Base/Base.hpp>

namespace Actias::Runtime
{
    class ModuleBuilder
    {
        HeapArray<Byte> m_RawData;
        ACBXFileInformationHeader* m_pFileInfoHeader = nullptr;
        List<ACBXSectionHeader*> m_Sections;
        ACBXExportTableHeader* m_pExportHeader = nullptr;
        ACBXImportTableHeader* m_pImportHeader = nullptr;
        Byte* m_pMapped                        = nullptr;

        USize m_TotalHeaderSize = 0;

    public:
        VoidResult<IO::ResultCode> LoadFromStream(IO::IStream* pStream);
        VoidResult<ResultCode> ParseHeaders();
        Result<ActiasHandle, ResultCode> Build();
        VoidResult<ResultCode> ImportAll();
    };
} // namespace Actias::Runtime
