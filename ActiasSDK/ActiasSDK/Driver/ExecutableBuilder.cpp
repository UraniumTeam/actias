#include <Actias/ACBX/Headers.h>
#include <Actias/Containers/HeapArray.hpp>
#include <Actias/Containers/List.hpp>
#include <ActiasSDK/Driver/ExecutableBuilder.hpp>
#include <ActiasSDK/Platform/INativeExecutable.hpp>
#include <iostream>

namespace Actias::SDK
{
    class BlobBuilder final
    {
        List<HeapArray<Byte>> m_Data;
        List<USize> m_AddressStack;

    public:
        template<class T>
        [[nodiscard]] inline T* Allocate(USize count = 1)
        {
            const auto byteSize = count * sizeof(T);
            const auto address  = NextAddress();
            m_AddressStack.Push(address);

            auto data = HeapArray<Byte>::CreateUninitialized(byteSize);
            memset(data.Data(), 0, byteSize);
            return reinterpret_cast<T*>(m_Data.Push(std::move(data)).Data());
        }

        [[nodiscard]] inline USize NextAddress() const noexcept
        {
            return AlignUp(TotalSize(), ACBX_FILE_ALIGNMENT);
        }

        [[nodiscard]] inline USize TotalSize() const noexcept
        {
            if (m_AddressStack.Empty())
            {
                return 0;
            }

            return m_AddressStack.Back() + m_Data.Back().Length();
        }

        [[nodiscard]] inline IBlob* Build()
        {
            const auto totalSize = TotalSize();

            auto result = HeapArray<Byte>::CreateUninitialized(totalSize);
            auto* pData = result.Data();
            memset(pData, 0, totalSize);

            for (USize i = 0; i < m_Data.Size(); ++i)
            {
                const auto address = m_AddressStack[i];
                ActiasCopyMemory(pData + address, m_Data[i].Data(), m_Data[i].Length());
            }

            return AllocateObject<HeapArrayBlob>(std::move(result));
        }
    };

    class SymbolNameAllocator : public ISymbolNameAllocator
    {
        // TODO: optimize this by allocating larger blocks of memory from the blob builder
        //   to group multiple name allocations
        BlobBuilder* m_pBlobBuilder;

    public:
        inline SymbolNameAllocator(BlobBuilder* pBlobBuilder)
            : m_pBlobBuilder(pBlobBuilder)
        {
        }

        inline void* Allocate(USize byteSize, UInt64& address) override
        {
            address = m_pBlobBuilder->NextAddress();
            return m_pBlobBuilder->Allocate<Byte>(byteSize);
        }
    };
} // namespace Actias::SDK

using namespace Actias;
using namespace Actias::SDK;

extern "C" void ACTIAS_ABI ActiasBuildExecutable(IBlob** ppExecutableData, const ActiasExecutableBuildInfo* pBuildInfo)
{
    BlobBuilder builder;
    auto* pNative = pBuildInfo->pNativeExecutable;

    auto* pFileInfo = builder.Allocate<ACBXFileInformationHeader>();
    ActiasCopyMemory(&pFileInfo->Signature, ACBXSignature, ACBX_SIGNATURE_SIZE);
    pNative->CreateInformationHeader(pFileInfo);

    auto* pExportHeader = builder.Allocate<ACBXExportTableHeader>();
    pNative->CreateExportTableHeader(pExportHeader);

    List<ACBXSectionHeader*> sectionHeaders;
    sectionHeaders.Reserve(pFileInfo->SectionCount);
    for (UInt16 i = 0; i < pFileInfo->SectionCount; ++i)
    {
        auto* pSectionHeader = sectionHeaders.Push(builder.Allocate<ACBXSectionHeader>());
        pNative->CreateSectionHeader(i, pSectionHeader);
    }

    pExportHeader->Address = builder.NextAddress();
    auto* pExportTable     = builder.Allocate<ACBXExportTableEntry>(pExportHeader->EntryCount);

    SymbolNameAllocator nameAllocator(&builder);
    for (UInt64 i = 0; i < pExportHeader->EntryCount; ++i)
    {
        pNative->CreateExportTableEntry(i, pExportTable + i, &nameAllocator);
    }

    for (UInt16 i = 0; i < pFileInfo->SectionCount; ++i)
    {
        auto* pSectionHeader       = sectionHeaders[i];
        pSectionHeader->RawAddress = builder.NextAddress();
        auto* pSectionData         = builder.Allocate<Byte>(pSectionHeader->RawSize);

        pNative->CopySection(i, pSectionData);
    }

    *ppExecutableData = builder.Build();
}
