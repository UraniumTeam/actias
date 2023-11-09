/*
 * Actias SDK System
 * 
 * Binary executable format support library
 * 
 * Copyright (c) 2023 UraniumTeam
 */

#pragma once
#include <Actias/Base/CompilerTraits.h>

ACTIAS_BEGIN_C

//! \brief ACBX file signature size in bytes.
//!
//! \see ACBXSignature
#define ACBX_SIGNATURE_SIZE 8

//! \brief ACBX file alignment in bytes.
//!
//! Specifies alignment of file parts: ACBXFileInformationHeader, ACBXSectionHeader, ACBXExportTableHeader.
#define ACBX_FILE_ALIGNMENT 16

//! \brief ACBX file signature bytes.
static const char ACBXSignature[ACBX_SIGNATURE_SIZE] = { 0x25, 0x41, 0x43, 0x42, 0x58, 0x2D, 0x00, 0x00 };

//! \brief ACBX machine architecture.
typedef enum ACBXArchType
{
    ACBX_ARCH_TYPE_NONE   = 0x00, //!< Unknown or unsupported.
    ACBX_ARCH_TYPE_x86    = 0x01, //!< x86.
    ACBX_ARCH_TYPE_x86_64 = 0x02  //!< x64.
} ACBXArchType;

//! \brief ACBX file attributes.
typedef enum ACBXAttributeFlagBits
{
    ACBX_ATTRIBUTE_EXECUTABLE_BIT = 0x0001, //!< The file is executable and can be directly run.
    ACBX_ATTRIBUTE_LIBRARY_BIT    = 0x0002  //!< The file is a library and cannot be directly run.
} ACBXAttributeFlagBits;

//! \brief ACBX file information header.
//!
//! Contains general information about an executable file.
//!
//! \see ACBXAttributeFlagBits, ACBXArchType
typedef struct ACBXFileInformationHeader
{
    char Signature[ACBX_SIGNATURE_SIZE]; //!< File signature.
    UInt64 EntryPointAddress;            //!< Address of the entry point.
    UInt16 TargetArch;                   //!< The target arch type.
    UInt16 SectionCount;                 //!< The number of sections in the file.
    UInt32 AttributeFlags;               //!< File attribute flags.
    UInt32 SDKVersion;                   //!< The version of the Actias SDK that built this file.
} ACBXFileInformationHeader;

//! \brief ACBX file section flags.
//!
//! Specifies section privileges (read, write, execute).
typedef enum ACBXSectionFlagBits
{
    ACBX_SECTION_READ_BIT    = 0x0001, //!< The section can be read.
    ACBX_SECTION_WRITE_BIT   = 0x0002, //!< The section can be written to.
    ACBX_SECTION_EXECUTE_BIT = 0x0004  //!< The section can be executed as code.
} ACBXSectionFlagBits;

//! \brief ACBX file section header.
//!
//! The section headers describe every code and data section.
//! There must be exactly ACBXFileInformationHeader::SectionCount section headers (36 bytes each).
//!
//! \see ACBXSectionFlagBits
typedef struct ACBXSectionHeader
{
    UInt64 RawAddress;         //!< Address of the section in the file.
    UInt64 Address;            //!< Address of the section when loaded.
    UInt64 RawSize;            //!< Size of raw section data.
    UInt64 Size;               //!< Size of the section when loaded.
    UInt64 RelocationsAddress; //!< Address of the relocations block, see Relocations Table.
    UInt32 SectionFlags;       //!< The section flags, see ACBXSectionFlagBits.
} ACBXSectionHeader;

//! \brief ACBX file export table header.
//!
//! \see ACBXExportTableEntry
typedef struct ACBXExportTableHeader
{
    UInt64 EntryCount; //!< The number of export table entries.
    UInt64 Address;    //!< The address of export table.
} ACBXExportTableHeader;

//! \brief ACBX file export table entry.
//!
//! \see ACBXExportTableHeader
typedef struct ACBXExportTableEntry
{
    UInt64 SymbolAddress; //!< The exported symbol address.
    UInt64 NameAddress;   //!< The exported symbol name address.
} ACBXExportTableEntry;

//! \brief ACBX file relocation block header.
//!
//! Each relocation table block starts with such header.
//!
//! \see ACBXRelocationEntry
typedef struct ACBXRelocationBlockHeader
{
    UInt64 BaseAddress; //!< Base address for relocations in the block.
} ACBXRelocationBlockHeader;

//! \brief Create a value for EntryData field of ACBXRelocationEntry.
//!
//! \param flags - The relocation flags (reserved, must be 0).
//! \param offset - The offset of the relocation, an address relative
//!     to the base address, specified in ACBXRelocationBlockHeader::BaseAddress.
//!
//! \see ACBXRelocationEntry
#define ACBX_CreateRelocationEntry(flags, offset) ((flags)&0xf) | (((offset)&0xfff) << 4)

//! \brief ACBX file relocation entry.
//!
//! Data layout:
//!   Flags (least significant 4 bits);
//!   Offset (remaining 12 bits);
//!
//! \see ACBX_CreateRelocationEntry, ACBXRelocationBlockHeader
typedef struct ACBXRelocationEntry
{
    UInt16 EntryData; //!< Relocation entry data, created with ACBX_CreateRelocationEntry.
} ACBXRelocationEntry;

ACTIAS_END_C
