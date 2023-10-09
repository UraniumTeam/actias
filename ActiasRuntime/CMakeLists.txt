set(SRC
    ActiasRuntime/Base/Base.h
    ActiasRuntime/Base/CompilerTraits.h
    ActiasRuntime/Base/Platform.h
    ActiasRuntime/Base/PlatformTraits.h
    ActiasRuntime/Base/Byte.h
    ActiasRuntime/Base/Flags.h

    ActiasRuntime/Buffers/Internal/PoolBucket.h
    ActiasRuntime/Buffers/ArrayPool.h

    ActiasRuntime/Containers/HeapArray.h
    ActiasRuntime/Containers/List.h
    ActiasRuntime/Containers/ArraySlice.h

    ActiasRuntime/IO/BaseIO.h
    ActiasRuntime/IO/BaseIO.cpp
    ActiasRuntime/IO/FileHandle.h
    ActiasRuntime/IO/FileHandle.cpp
    ActiasRuntime/IO/IStream.h
    ActiasRuntime/IO/FileStream.h
    ActiasRuntime/IO/FileStream.cpp
    ActiasRuntime/IO/StdoutStream.h
    ActiasRuntime/IO/StdoutStream.cpp
    ActiasRuntime/IO/StreamBase.h

    ActiasRuntime/Memory/IAllocator.h
    ActiasRuntime/Memory/Memory.h
    ActiasRuntime/Memory/Object.h
    ActiasRuntime/Memory/Ptr.h
    ActiasRuntime/Memory/ReferenceCounter.h
    ActiasRuntime/Memory/SystemAllocator.h

    ActiasRuntime/RTTI/RTTI.h

    ActiasRuntime/Strings/String.h
    ActiasRuntime/Strings/StringSlice.h
    ActiasRuntime/Strings/StringSlice.cpp
    ActiasRuntime/Strings/Unicode.h

    ActiasRuntime/Time/DateTime.h
    ActiasRuntime/Time/DateTime.cpp
    ActiasRuntime/Time/TimeSpan.h

    ActiasRuntime/Utils/UUID.h
    ActiasRuntime/Utils/Result.h
    ActiasRuntime/Utils/BitUtils.h
    ActiasRuntime/Utils/UUID.cpp
)

add_library(ActiasRuntime STATIC ${SRC})

actias_configure_target(ActiasRuntime)
actias_enable_simd_for_target(ActiasRuntime)

target_include_directories(ActiasRuntime PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(ActiasRuntime PROPERTIES FOLDER "Runtime")

get_property("TARGET_SOURCE_FILES" TARGET ActiasRuntime PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})

if (ACTIAS_BUILD_TESTS)
    add_subdirectory(Tests)
endif ()
