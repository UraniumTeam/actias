#include <Actias/IO/FileStream.hpp>

namespace Actias::IO
{
    PlatformFileStream::PlatformFileStream(PlatformFile* file)
        : m_File(file)
    {
    }

    bool PlatformFileStream::WriteAllowed() const noexcept
    {
        return IsWriteAllowed(GetOpenMode());
    }

    bool PlatformFileStream::ReadAllowed() const noexcept
    {
        return IsReadAllowed(GetOpenMode());
    }

    bool PlatformFileStream::SeekAllowed() const noexcept
    {
        return true;
    }

    bool PlatformFileStream::IsOpen() const
    {
        return m_File->IsOpen();
    }

    VoidResult<ResultCode> PlatformFileStream::Seek(SSize offset, SeekMode seekMode)
    {
        return m_File->Seek(offset, seekMode);
    }

    Result<USize, ResultCode> PlatformFileStream::Tell() const
    {
        return m_File->Tell();
    }

    Result<USize, ResultCode> PlatformFileStream::Length() const
    {
        return m_File->Length();
    }

    Result<USize, ResultCode> PlatformFileStream::ReadToBuffer(void* buffer, USize size)
    {
        return m_File->Read(buffer, size);
    }

    Result<USize, ResultCode> PlatformFileStream::WriteFromBuffer(const void* buffer, USize size)
    {
        return m_File->Write(buffer, size);
    }

    StringSlice PlatformFileStream::GetName() const
    {
        return m_File->GetName();
    }

    OpenMode PlatformFileStream::GetOpenMode() const
    {
        return m_File->GetOpenMode();
    }

    void PlatformFileStream::Close()
    {
        m_File.Reset();
    }

    VoidResult<ResultCode> PlatformFileStream::Open(StringSlice fileName, OpenMode openMode)
    {
        return m_File->Open(fileName, openMode);
    }

    bool FileStream::WriteAllowed() const noexcept
    {
        return IsWriteAllowed(GetOpenMode());
    }

    bool FileStream::ReadAllowed() const noexcept
    {
        return IsReadAllowed(GetOpenMode());
    }

    bool FileStream::SeekAllowed() const noexcept
    {
        return true;
    }

    bool FileStream::IsOpen() const
    {
        return m_File->IsOpen();
    }

    VoidResult<ResultCode> FileStream::Seek(SSize offset, SeekMode seekMode)
    {
        return m_File->Seek(offset, seekMode);
    }

    Result<USize, ResultCode> FileStream::Tell() const
    {
        return m_File->Tell();
    }

    Result<USize, ResultCode> FileStream::Length() const
    {
        return m_File->Length();
    }

    Result<USize, ResultCode> FileStream::ReadToBuffer(void* buffer, USize size)
    {
        return m_File->ReadToBuffer(buffer, size);
    }

    Result<USize, ResultCode> FileStream::WriteFromBuffer(const void* buffer, USize size)
    {
        return m_WriteStream->WriteFromBuffer(buffer, size);
    }

    StringSlice FileStream::GetName() const
    {
        return m_File->GetName();
    }

    OpenMode FileStream::GetOpenMode() const
    {
        return m_File->GetOpenMode();
    }

    void FileStream::Close()
    {
        m_File.Reset();
        m_WriteStream.Reset();
    }

    VoidResult<ResultCode> FileStream::Open(StringSlice fileName, OpenMode openMode)
    {
        m_File = AllocateObject<PlatformFileStream>(AllocateObject<PlatformFile>());

        const auto openResult = m_File->Open(fileName, openMode);
        ACTIAS_GuardResult(openResult);
        m_WriteStream = AllocateObject<BufferedStream<256>>(m_File.Get());
        return openResult;
    }
} // namespace Actias::IO
