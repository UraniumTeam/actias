#pragma once
#include <Actias/Base/Byte.hpp>
#include <Actias/IO/BaseIO.hpp>
#include <Actias/IO/FileSystem.hpp>
#include <Actias/Strings/FixedString.hpp>
#include <Actias/Strings/StringSlice.hpp>
#include <Actias/System/Path.h>
#include <Actias/Time/DateTime.hpp>

namespace Actias
{
    class IBlob;
}

namespace Actias::IO
{
    using Path = FixedString<ACTIAS_MAX_PATH>;

    //! \brief Provides common functions to work with files.
    namespace FileSystem
    {
        //! \brief Get current working directory of the running process.
        [[nodiscard]] inline static Path GetCurrentDirectory()
        {
            TChar result[ACTIAS_MAX_PATH + 1];
            ActiasGetCurrentDirectory(result, sizeof(result));
            return Path{ result };
        }

        //! \brief Get parent of the specified directory without memory allocations.
        //!
        //! \param fileName - The slice of the provided string with only the parent directory.
        [[nodiscard]] inline static StringSlice GetParent(StringSlice fileName)
        {
            auto endIter = fileName.FindLastOf('/');
            if (endIter == fileName.end())
            {
                endIter = fileName.FindLastOf('/');
            }

            return { fileName.begin(), endIter };
        }

        //! \brief Check if a file exists.
        //!
        //! \param fileName - The name of the file to check.
        [[nodiscard]] bool Exists(StringSlice fileName);

        //! \brief Read an entire text file to a string.
        //!
        //! \param fileName - The name of the file to read.
        //!
        //! \return Either a string with the file contents or an error code.
        //!
        //! \see ResultCode
        [[nodiscard]] Result<String, ResultCode> ReadAllText(StringSlice fileName);

        //! \brief Read an entire binary file to a list of bytes.
        //!
        //! \param fileName - The name of the file to read.
        //!
        //! \return Either a list with the file contents or an error code.
        //!
        //! \see ResultCode
        [[nodiscard]] Result<List<Byte>, ResultCode> ReadAllBytes(StringSlice fileName);

        //! \brief Write an entire blob to a file.
        //!
        //! \param fileName - The name of the file to write to.
        //! \param pBlob - The blob containing the data to write.
        //! \param openMode - The OpenMode to use to open the file.
        //!
        //! \return Either a list with the file contents or an error code.
        //!
        //! \see ResultCode
        [[nodiscard]] Result<USize, ResultCode> WriteBlob(StringSlice fileName, IBlob* pBlob,
                                                          OpenMode openMode = OpenMode::CreateNew);

        //! \brief Delete a file.
        //!
        //! \param fileName - The name of the file to delete.
        //!
        //! \return An error code if the operation was not successful.
        //!
        //! \see ResultCode
        [[nodiscard]] VoidResult<ResultCode> Delete(StringSlice fileName);
    }; // namespace FileSystem

    //! \brief A low-level file, without buffering.
    //!
    //! \note Prefer using streams in most cases.
    class PlatformFile final : public Object<IObject>
    {
        Path m_FileName;
        ActiasHandle m_Handle      = nullptr;
        OpenMode m_OpenMode        = OpenMode::None;
        ActiasResult m_StatsResult = ACTIAS_FAIL_UNKNOWN;
        ActiasFileStats m_Stats    = { 0 };

    public:
        ACTIAS_RTTI_Class(PlatformFile, "58D19D75-CE53-4B11-B151-F82583B3EAD8");

        PlatformFile();
        ~PlatformFile() override;

        //! \brief Open a file.
        //!
        //! \param fileName - The path to the file to open.
        //! \param openMode - The OpenMode to use.
        //!
        //! \return An error code if the operation was not successful.
        [[nodiscard]] VoidResult<ResultCode> Open(StringSlice fileName, OpenMode openMode);

        //! \brief Close the handle.
        void Close();

        //! \brief Add offset to the current file position.
        //!
        //! \param offset - Offset to add to the current position.
        //! \param seekMode - The SeekMode to use.
        //!
        //! \return An error code if the operation was not successful.
        [[nodiscard]] VoidResult<ResultCode> Seek(SSize offset, SeekMode seekMode);

        //! \brief Tell current file offset.
        //!
        //! \return Either the current file offset or an error code.
        [[nodiscard]] Result<USize, ResultCode> Tell() const;

        //! \brief Get creation time of the file.
        //!
        //! \return Either the creation time or an error code.
        [[nodiscard]] Result<UTCDateTime, ResultCode> GetCreationTime() const;

        //! \brief Get last modification time of the file.
        //!
        //! \return Either the last modification time or an error code.
        [[nodiscard]] Result<UTCDateTime, ResultCode> GetLastModificationTime() const;

        //! \brief Get last access time of the file.
        //!
        //! \return Either the last access time or an error code.
        [[nodiscard]] Result<UTCDateTime, ResultCode> GetLastAccessTime() const;

        //! \brief Get current file open mode.
        [[nodiscard]] OpenMode GetOpenMode() const;

        //! \brief Read from the file to a buffer.
        //!
        //! \param buffer - The buffer to read the file data to.
        //! \param size - The size of the provided buffer.
        //!
        //! \return Either the number of bytes actually read or an error code.
        [[nodiscard]] Result<USize, ResultCode> Read(void* buffer, USize size);

        //! \brief Write from buffer to the file.
        //!
        //! \param buffer - The buffer to write the file data from.
        //! \param size   - The size of the provided buffer.
        //!
        //! \return Either the number of bytes actually written or an error code.
        [[nodiscard]] Result<USize, ResultCode> Write(const void* buffer, USize size);

        //! \brief Get length of the file in bytes.
        //!
        //! \return Either the length or an error code.
        [[nodiscard]] Result<USize, ResultCode> Length() const;

        //! \brief Get file name.
        [[nodiscard]] StringSlice GetName() const;

        //! \brief Check if the file is open.
        [[nodiscard]] bool IsOpen() const;
    };
} // namespace Actias::IO
