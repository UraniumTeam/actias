#include <Actias/Containers/IBlob.hpp>
#include <Actias/IO/FileHandle.hpp>
#include <Actias/System/Streams.h>

#if ACTIAS_WINDOWS
#    include <direct.h>
#else
#    include <sys/stat.h>
#    include <unistd.h>
#endif

namespace Actias::IO
{
    FileHandle::FileHandle()
        : m_Handle(nullptr)
    {
    }

    FileHandle::~FileHandle()
    {
        Close();
    }

    VoidResult<ResultCode> FileHandle::Open(StringSlice fileName, OpenMode openMode)
    {
        switch (openMode)
        {
        case OpenMode::ReadOnly:
        case OpenMode::WriteOnly:
        case OpenMode::Append:
        case OpenMode::ReadWrite:
            ACTIAS_Guard(File::Exists(fileName), ResultCode::NoFileOrDirectory);
            break;
        case OpenMode::CreateNew:
            ACTIAS_Guard(!File::Exists(fileName), ResultCode::FileExists);
            break;
        default:
            break;
        }

        Close();
        ActiasOpenFile(fileName.Data(), static_cast<ActiasFlags>(openMode), &m_Handle);
        if (m_Handle)
        {
            m_FileName = fileName;
            m_OpenMode = openMode;
            return OK();
        }

        return Err(Internal::GetResultCode(errno));
    }

    void FileHandle::Close()
    {
        if (IsOpen())
        {
            ActiasCloseFile(m_Handle);
            m_Handle   = nullptr;
            m_OpenMode = OpenMode::None;
            m_FileName.Clear();
        }
    }

    VoidResult<ResultCode> FileHandle::Seek(SSize offset, SeekMode seekMode)
    {
        ACTIAS_Guard(IsOpen(), ResultCode::NotOpen);

        ActiasResult result = ActiasSeekFile(m_Handle, offset, static_cast<ActiasFileSeekModeBits>(seekMode));
        if (result == ACTIAS_FAIL_UNKNOWN)
        {
            return Err(ResultCode::InvalidSeek);
        }

        return OK();
    }

    Result<USize, ResultCode> FileHandle::Tell() const
    {
        ACTIAS_Guard(IsOpen(), ResultCode::NotOpen);
        USize position;
        ActiasTellFile(m_Handle, &position);

        return position;
    }

    Result<USize, ResultCode> FileHandle::Read(void* buffer, USize size)
    {
        ACTIAS_Guard(IsOpen(), ResultCode::NotOpen);
        USize bytesRead;
        auto result = ActiasReadFile((ActiasHandle)m_Handle, buffer, size, &bytesRead);
        if (result == ACTIAS_SUCCESS)
            return bytesRead;

        return result;
    }

    Result<USize, ResultCode> FileHandle::Write(const void* buffer, USize size)
    {
        ACTIAS_Guard(IsOpen(), ResultCode::NotOpen);
        ACTIAS_Guard(IsWriteAllowed(GetOpenMode()), ResultCode::WriteNotAllowed);
        ActiasResult result = ActiasWriteFile(m_Handle, buffer, size, NULL);
        if (result == ACTIAS_FAIL_UNKNOWN)
        {
            return Err(Internal::GetResultCode(errno));
        }

        return result;
    }

    VoidResult<ResultCode> FileHandle::Flush()
    {
        ACTIAS_Guard(IsOpen(), ResultCode::NotOpen);
        ActiasFlushFile(m_Handle);

        return OK();
    }

    Result<USize, ResultCode> FileHandle::Length() const
    {
        ACTIAS_Guard(IsOpen(), ResultCode::NotOpen);
        struct stat st; // NOLINT
        if (stat(m_FileName.Data(), &st) == 0)
        {
            return st.st_size;
        }

        return Err(Internal::GetResultCode(errno));
    }

    Result<DateTime, ResultCode> FileHandle::GetLastModificationTime() const
    {
        ACTIAS_Guard(IsOpen(), ResultCode::NotOpen);
        struct stat st; // NOLINT
        if (stat(m_FileName.Data(), &st) == 0)
        {
            return DateTime::CreateLocal(st.st_mtime);
        }

        return Err(Internal::GetResultCode(errno));
    }

    StringSlice FileHandle::GetName() const
    {
        return m_FileName;
    }

    bool FileHandle::IsOpen() const
    {
        return m_Handle;
    }

    OpenMode FileHandle::GetOpenMode() const
    {
        return m_OpenMode;
    }

    String Directory::GetCurrentDirectory()
    {
        static char buffer[256];
#if ACTIAS_WINDOWS
        _getcwd(buffer, sizeof(buffer));
#else
        getcwd(buffer, sizeof(buffer));
#endif
        return buffer;
    }

    StringSlice Directory::GetParent(StringSlice fileName)
    {
        auto endIter = fileName.FindLastOf(ACTIAS_PATH_SEPARATOR);
        if (endIter == fileName.end())
        {
            endIter = fileName.FindLastOf('/');
        }

        return { fileName.begin(), endIter };
    }

    bool File::Exists(StringSlice fileName)
    {
        struct stat buffer; // NOLINT
        return stat(fileName.Data(), &buffer) == 0;
    }

    Result<USize, ResultCode> File::WriteBlob(StringSlice fileName, IBlob* pBlob, OpenMode openMode)
    {
        FileHandle file;
        auto open = file.Open(fileName, openMode);
        ACTIAS_GuardResult(open);

        return file.Write(pBlob->Data(), pBlob->ByteSize());
    }

    VoidResult<ResultCode> File::Delete(StringSlice fileName)
    {
        if (remove(fileName.Data()) == 0)
        {
            return OK();
        }

        return Err(Internal::GetResultCode(errno));
    }

    Result<String, ResultCode> File::ReadAllText(StringSlice fileName)
    {
        FileHandle file;
        auto open = file.Open(fileName, OpenMode::ReadOnly);
        ACTIAS_GuardResult(open);

        auto lengthRes = file.Length();
        ACTIAS_Guard(lengthRes, lengthRes.UnwrapErr());

        auto length = lengthRes.Unwrap();
        String buffer(length, ' ');
        auto read = file.Read(buffer.Data(), length);
        ACTIAS_GuardResult(read);
        return buffer;
    }

    Result<List<Byte>, ResultCode> File::ReadAllBytes(StringSlice fileName)
    {
        FileHandle file;
        auto open = file.Open(fileName, OpenMode::ReadOnly);
        ACTIAS_GuardResult(open);

        auto lengthRes = file.Length();
        ACTIAS_Guard(lengthRes, lengthRes.UnwrapErr());

        auto length = lengthRes.Unwrap();
        List<Byte> buffer;
        buffer.Resize(length);
        auto read = file.Read(buffer.Data(), length);
        ACTIAS_GuardResult(read);
        return buffer;
    }
} // namespace Actias::IO
