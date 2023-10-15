﻿## Actias Binary Executable Format Specification

*Actias Binary Executable* is a portable OS-agnostic format for executable files and shared libraries
used with Actias Framework. ACBX and ACBL files can be built with Actias SDK and loaded with Actias runtime
loader.

Both libraries and executables have the same binary format and can have any extension in file names,
but it is recommended for executable files to have `.acbx` extension and for library files to
have `.acbl` extension.

If an Actias binary executable file has a `main()` function it can be executed as a program:
```sh
actias ./program.acbx
```

Otherwise it can be used as a shared (dynamic-link) library:
```cpp
// TODO: write actual example when the loader is functional
ActiasLibHandle handle = ActiasLoadLibrary("./library.acbl");
void* pFunction = ActiasGetEntryPointAddress(handle, "AddNumbers");
```

> :warning: Warning: the format specification and the APIs are yet unstable and are subject to change.

## Table of Contents

* [ACBX File Sections](#acbx-file-sections)
  * [File Signature](#file-signature)
  * [Information Headers](#information-headers)
    * [Architecture Types](#architecture-types)
    * [Attribute Flags](#attribute-flags)
  * [Section Headers](#section-headers)
    * [Section Flags](#section-flags)
  * [Export Table](#export-table)
    * [Export Table Headers](#export-table-headers)
    * [Export Table Entry](#export-table-entry)
  * [Relocations Table](#relocations-table)

## ACBX File Sections

### File Signature

The first 6 bytes of every ACBX file are: `25 41 43 42 58 2D` (hexadecimal). The same
converted to ASCII: `%ACBX-`.

The next 2 bytes are zeros.

### Information Headers

| Offset | Type | Field | Description|
|-------:|-----:|:------|:-----------|
| 0 | UInt64 | EntryPointAddress | Address of the entry point. |
| 8 | UInt16 | TargetArch | The target arch type, see [Architecture Types](#architecture-types). |
| 10 | UInt16 | SectionCount | The number of sections in the file. |
| 12 | UInt32 | AttributeFlags | File attribute flags, see [Attribute Flags](#attribute-flags). |
| 16 | UInt32 | SDKVersion | The version of the Actias SDK that built this file. |

#### Architecture Types

| Enum value | Hexadecimal value | Description |
|:-----------|------------------:|:------------|
| `ACBX_ARCH_TYPE_NONE` | `00 00` | Unknown or unsupported |
| `ACBX_ARCH_TYPE_x86` | `00 01` | x86 |
| `ACBX_ARCH_TYPE_x86_64` | `00 02` | x64 |

#### Attribute Flags

| Enum value | Hexadecimal value | Description |
|:-----------|------------------:|:------------|
| `ACBX_ATTRIBUTE_EXECUTABLE_BIT` | `00 00 00 01` | The file is executable and can be directly run. |
| `ACBX_ATTRIBUTE_LIBRARY_BIT` | `00 00 00 02` | The file is a library and *cannot* be directly run. |

### Section Headers

The section headers describe every code and data section. There must be exactly SectionCount (see
[Headers](#headers)) section headers (36 bytes each).

| Offset | Type | Field | Description|
|-------:|-----:|:------|:-----------|
| 0 | UInt64 | Address | Address of the section. |
| 8 | UInt64 | RawSize | Size of raw section data. |
| 16 | UInt64 | Size | Size of the section when loaded. |
| 24 | UInt64 | RelocationsAddress | Address of the relocations block, see [Relocations Table](#relocations-table). |
| 32 | UInt32 | SectionFlags | The section flags, see [Section Flags](#section-flags). |

#### Section Flags

| Enum value | Hexadecimal value | Description |
|:-----------|------------------:|:------------|
| `ACBX_SECTION_READ_BIT` | `00 00 00 01` | The section can be read. |
| `ACBX_SECTION_WRITE_BIT` | `00 00 00 02` | The section can be written to. |
| `ACBX_SECTION_EXECUTE_BIT` | `00 00 00 04` | The section can be executed as code. |

### Export Table

#### Export Table Headers

| Offset | Type | Field | Description|
|-------:|-----:|:------|:-----------|
| 0 | UInt64 | EntryCount | The number of export table entries. |
| 8 | UInt64 | Address | The address of export table. |

#### Export Table Entry

| Offset | Type | Field | Description|
|-------:|-----:|:------|:-----------|
| 0 | UInt64 | SymbolAddress | The exported symbol address. |
| 8 | UInt64 | NameAddress | The exported symbol name address. |

### Relocations Table

Each relocation table block starts with a header:

| Offset | Type | Field | Description|
|-------:|-----:|:------|:-----------|
| 0 | UInt64 | BaseAddress | Base address for relocations in the block. |

A relocation entry is stored in a UInt16, offset is specified in bits.

| Offset (bits) | Length | Field | Description|
|--------------:|-------:|:------|:-----------|
| 0 | 4 | Flags | The relocation flags (reserved, must be 0). |
| 4 | 12 | Offset | The offset of the relocation, an address relative to the base address. |