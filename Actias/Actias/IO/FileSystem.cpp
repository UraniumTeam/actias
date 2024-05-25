#include <Actias/Containers/IBlob.hpp>
#include <Actias/IO/FileSystem.hpp>
#include <Actias/System/Streams.h>

namespace Actias::IO
{
    PlatformFile::PlatformFile()
        : m_Handle(nullptr)
    {
    }

    PlatformFile::~PlatformFile()
    {
        Close();
    }

    VoidResult<ResultCode> PlatformFile::Open(StringSlice fileName, OpenMode openMode)
    {
        Close();
        const ActiasResult result = ActiasOpenFile(fileName.Data(), static_cast<ActiasFlags>(openMode), &m_Handle);
        if (result != ACTIAS_SUCCESS)
        {
            return Err(static_cast<ResultCode>(result));
        }

        m_StatsResult = ActiasGetFileStats(m_Handle, &m_Stats);
        m_FileName    = fileName;
        m_OpenMode    = openMode;
        return OK();
    }

    void PlatformFile::Close()
    {
        if (IsOpen())
        {
            ActiasCloseFile(m_Handle);
            m_Handle   = nullptr;
            m_OpenMode = OpenMode::None;
            m_FileName.Clear();
        }
    }

    VoidResult<ResultCode> PlatformFile::Seek(SSize offset, SeekMode seekMode)
    {
        ACTIAS_Guard(IsOpen(), ResultCode::NotOpen);

        const ActiasResult result = ActiasSeekFile(m_Handle, offset, static_cast<ActiasFileSeekMode>(seekMode));
        if (result != ACTIAS_SUCCESS)
        {
            return Err(static_cast<ResultCode>(result));
        }

        return OK();
    }

    Result<USize, ResultCode> PlatformFile::Tell() const
    {
        ACTIAS_Guard(IsOpen(), ResultCode::NotOpen);

        USize position;
        const ActiasResult result = ActiasTellFile(m_Handle, &position);
        if (result != ACTIAS_SUCCESS)
        {
            return Err(static_cast<ResultCode>(result));
        }

        return position;
    }

    Result<USize, ResultCode> PlatformFile::Read(void* buffer, USize size)
    {
        ACTIAS_Guard(IsOpen(), ResultCode::NotOpen);

        USize bytesRead;
        const ActiasResult result = ActiasReadFile((ActiasHandle)m_Handle, buffer, size, &bytesRead);
        if (result != ACTIAS_SUCCESS)
        {
            return Err(static_cast<ResultCode>(result));
        }

        return bytesRead;
    }

    Result<USize, ResultCode> PlatformFile::Write(const void* buffer, USize size)
    {
        ACTIAS_Guard(IsOpen(), ResultCode::NotOpen);
        ACTIAS_Guard(IsWriteAllowed(GetOpenMode()), ResultCode::WriteNotAllowed);

        const ActiasResult result = ActiasWriteFile(m_Handle, buffer, size, NULL);
        if (result != ACTIAS_SUCCESS)
        {
            return Err(static_cast<ResultCode>(result));
        }

        return result;
    }

    Result<USize, ResultCode> PlatformFile::Length() const
    {
        ACTIAS_Guard(IsOpen(), ResultCode::NotOpen);
        if (m_StatsResult != ACTIAS_SUCCESS)
        {
            return Err(static_cast<ResultCode>(m_StatsResult));
        }

        return m_Stats.ByteSize;
    }

    Result<UTCDateTime, ResultCode> PlatformFile::GetCreationTime() const
    {
        ACTIAS_Guard(IsOpen(), ResultCode::NotOpen);
        if (m_StatsResult != ACTIAS_SUCCESS)
        {
            return Err(static_cast<ResultCode>(m_StatsResult));
        }

        return UTCDateTime::FromUnixTime(m_Stats.CreationTimeUTC);
    }

    Result<UTCDateTime, ResultCode> PlatformFile::GetLastModificationTime() const
    {
        ACTIAS_Guard(IsOpen(), ResultCode::NotOpen);
        if (m_StatsResult != ACTIAS_SUCCESS)
        {
            return Err(static_cast<ResultCode>(m_StatsResult));
        }

        return UTCDateTime::FromUnixTime(m_Stats.LastModificationTimeUTC);
    }

    Result<UTCDateTime, ResultCode> PlatformFile::GetLastAccessTime() const
    {
        ACTIAS_Guard(IsOpen(), ResultCode::NotOpen);
        if (m_StatsResult != ACTIAS_SUCCESS)
        {
            return Err(static_cast<ResultCode>(m_StatsResult));
        }

        return UTCDateTime::FromUnixTime(m_Stats.LastAccessTimeUTC);
    }

    StringSlice PlatformFile::GetName() const
    {
        return m_FileName;
    }

    bool PlatformFile::IsOpen() const
    {
        return m_Handle;
    }

    OpenMode PlatformFile::GetOpenMode() const
    {
        return m_OpenMode;
    }

    namespace FileSystem
    {
        bool Exists(StringSlice fileName)
        {
            PlatformFile probeFile;
            return probeFile.Open(fileName, OpenMode::None).IsOk();
        }

        Result<USize, ResultCode> WriteBlob(StringSlice fileName, IBlob* pBlob, OpenMode openMode)
        {
            PlatformFile file;
            const auto open = file.Open(fileName, openMode);
            ACTIAS_GuardResult(open);

            return file.Write(pBlob->Data(), pBlob->ByteSize());
        }

        VoidResult<ResultCode> Delete(StringSlice fileName)
        {
            const ActiasResult result = ActiasRemoveFile(Path{ fileName }.Data());
            if (result != ACTIAS_SUCCESS)
            {
                return static_cast<ResultCode>(result);
            }

            return OK{};
        }

        Result<String, ResultCode> ReadAllText(StringSlice fileName)
        {
            PlatformFile file;
            const auto open = file.Open(fileName, OpenMode::ReadOnly);
            ACTIAS_GuardResult(open);

            const auto lengthRes = file.Length();
            ACTIAS_Guard(lengthRes, lengthRes.UnwrapErr());

            const auto length = lengthRes.Unwrap();
            String buffer(length, ' ');
            const auto read = file.Read(buffer.Data(), length);
            ACTIAS_GuardResult(read);
            return buffer;
        }

        Result<List<Byte>, ResultCode> ReadAllBytes(StringSlice fileName)
        {
            PlatformFile file;
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
    } // namespace FileSystem
} // namespace Actias::IO
