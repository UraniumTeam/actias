## Actias Binary Executable Format Specification

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

Otherwise it can be used as a shared (dynamic-linked) library:
```cpp
// TODO: write actual example when the loader is functional
ActiasLibHandle handle = ActiasLoadLibrary("./library.acbl");
void* pFunction = ActiasGetEntryPointAddress(handle, "AddNumbers");
```

> ⚠️ Warning: the format specification and the APIs are yet unstable and are subject to change.

## Table of Contents

* [ACBX File Sections]
  * [File Signature]
  * [Headers]
    * [Machine Types]
    * [Attribute Flags]
  * [Section Headers]
    * [Section Flags]
  * [Export Table]
  * [Relocations Table]

## ACBX File Sections

### File Signature

The first 6 bytes of every ACBX file are: `25 41 43 42 58 2D` (hexadecimal). The same
converted to ASCII: `%ACBX-`.

The next 2 bytes are zeros.

### Headers

| Offset | Type | Field | Description|
|-------:|-----:|:------|:-----------|
| 0 | UInt16 | Machine | Target machine type, see [Machine Types]. |
| 2 | UInt16 | SectionCount | The number of sections in the file. |
| 4 | UInt32 | AttributeFlags | File attribute flags, see [Attribute Flags]. |
| 8 | UInt32 | SDKVersion | The version of the Actias SDK that built this file. |
| 12 | UInt64 | EntryPointAddress | Address of the entry point. |
| 20 | ACBXSpan | ExportTable | [Export Table] address and size. |
| 20 | ACBXSpan | RelocTable | [Relocations Table] address and size. |

`ACBXSpan` definition:

```cpp
typedef struct {
	UInt64 Address;
	UInt64 Size;
} ACBXSpan;
```

#### Machine Types

| Enum value | Hexadecimal value | Description |
|:-----------|------------------:|:------------|
| `ACBX_MACHINE_TYPE_NONE` | `00 00` | Unknown or unsupported |
| `ACBX_MACHINE_TYPE_I386` | `00 01` | x86 |
| `ACBX_MACHINE_TYPE_AMD64` | `00 02` | x64 |

#### Attribute Flags

| Enum value | Hexadecimal value | Description |
|:-----------|------------------:|:------------|
| `ACBX_ATTRIBUTE_EXECUTABLE_BIT` | `00 00 00 01` | The file is executable and can be directly run. |
| `ACBX_ATTRIBUTE_LIBRARY_BIT` | `00 00 00 02` | The file and *cannot* be directly run. |

### Section Headers

The section headers describe every code and data section. There must be exactly SectionCount (see
[Headers]) section headers (40 bytes each).

| Offset | Type | Field | Description|
|-------:|-----:|:------|:-----------|
| 0 | UInt64 | Address | Address of the section. |
| 8 | UInt64 | RawSize | Size of raw section data. |
| 16 | UInt64 | Size | Size of the section when loaded. |
| 24 | UInt64 | RelocationsAddress | Address of the relocation table. |
| 32 | UInt32 | RelocationCount | The number of relocations. |
| 36 | UInt32 | Flags | The section flags, see [Section Flags]. |

#### Section Flags

| Enum value | Hexadecimal value | Description |
|:-----------|------------------:|:------------|
| `ACBX_SECTION_READ_BIT` | `00 00 00 01` | The section can be read. |
| `ACBX_SECTION_WRITE_BIT` | `00 00 00 02` | The section can be written to. |
| `ACBX_SECTION_EXECUTE_BIT` | `00 00 00 04` | The section can be executed as code. |

### Export Table

TODO

### Relocations Table

TODO
