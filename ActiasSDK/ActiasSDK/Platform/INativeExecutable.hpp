#pragma once
#include <Actias/ACBX/Headers.h>
#include <Actias/Base/Byte.hpp>
#include <Actias/Memory/Object.hpp>

namespace Actias::SDK
{
    class INativeExecutable : public IObject
    {
    public:
        ACTIAS_RTTI_Class(INativeExecutable, "35159633-1C8F-46FE-BB96-C776E2278722");

        //! \brief Retrieve native executable data and write it to an ACBX file information header.
        //!
        //! \param pHeader - A pointer to the variable that receives the header data, file signature is already filled.
        virtual void ACTIAS_ABI CreateInformationHeader(ACBXFileInformationHeader* pHeader) = 0;

        //! \brief Retrieve native executable section header and write it to an ACBX section header.
        //!
        //! \param sectionID - The ID of the section header to create.
        //! \param pHeader - A pointer to the variable that receives the header data, address is already filled.
        virtual void ACTIAS_ABI CreateSectionHeader(UInt32 sectionID, ACBXSectionHeader* pHeader) = 0;

        //! \brief Copy raw section data from a native executable.
        //!
        //! \param sectionID - The ID of the section to copy.
        //! \param pDestination - A pointer to the destination buffer.
        virtual void ACTIAS_ABI CopySection(UInt32 sectionID, Byte* pDestination) = 0;
    };
} // namespace Actias::SDK
