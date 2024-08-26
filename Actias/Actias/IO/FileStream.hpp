#pragma once
#include <Actias/IO/BufferedStream.hpp>
#include <Actias/IO/FileSystem.hpp>
#include <Actias/IO/StreamBase.hpp>

namespace Actias::IO
{
    class PlatformFileStream : public StreamBase
    {
        Ptr<PlatformFile> m_File;

    public:
        explicit PlatformFileStream(PlatformFile* file);

        ~PlatformFileStream() override = default;

        [[nodiscard]] VoidResult<ResultCode> Open(StringSlice fileName, OpenMode openMode);

        [[nodiscard]] bool WriteAllowed() const noexcept override;
        [[nodiscard]] bool ReadAllowed() const noexcept override;
        [[nodiscard]] bool SeekAllowed() const noexcept override;
        [[nodiscard]] bool IsOpen() const override;
        [[nodiscard]] VoidResult<ResultCode> Seek(SSize offset, SeekMode seekMode) override;
        [[nodiscard]] Result<USize, ResultCode> Tell() const override;
        [[nodiscard]] Result<USize, ResultCode> Length() const override;
        [[nodiscard]] Result<USize, ResultCode> ReadToBuffer(void* buffer, USize size) override;
        [[nodiscard]] Result<USize, ResultCode> WriteFromBuffer(const void* buffer, USize size) override;
        [[nodiscard]] StringSlice GetName() const override;
        [[nodiscard]] OpenMode GetOpenMode() const override;
        void Close() override;
    };

    class FileStream : public StreamBase
    {
        Ptr<PlatformFileStream> m_File;
        Ptr<IStream> m_WriteStream;

    public:
        FileStream()           = default;
        ~FileStream() override = default;

        [[nodiscard]] VoidResult<ResultCode> Open(StringSlice fileName, OpenMode openMode);

        [[nodiscard]] bool WriteAllowed() const noexcept override;
        [[nodiscard]] bool ReadAllowed() const noexcept override;
        [[nodiscard]] bool SeekAllowed() const noexcept override;
        [[nodiscard]] bool IsOpen() const override;
        [[nodiscard]] VoidResult<ResultCode> Seek(SSize offset, SeekMode seekMode) override;
        [[nodiscard]] Result<USize, ResultCode> Tell() const override;
        [[nodiscard]] Result<USize, ResultCode> Length() const override;
        [[nodiscard]] Result<USize, ResultCode> ReadToBuffer(void* buffer, USize size) override;
        [[nodiscard]] Result<USize, ResultCode> WriteFromBuffer(const void* buffer, USize size) override;
        [[nodiscard]] StringSlice GetName() const override;
        [[nodiscard]] OpenMode GetOpenMode() const override;
        void Close() override;
    };
} // namespace Actias::IO
