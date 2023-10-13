set(SRC
    Actias/Base/Base.h
    Actias/Base/Byte.h
    Actias/Base/CompilerTraits.h
    Actias/Base/Flags.h
    Actias/Base/Platform.h
    Actias/Base/PlatformTraits.h

    Actias/Containers/HeapArray.h
    Actias/Containers/List.h
    Actias/Containers/ArraySlice.h

    Actias/IO/BaseIO.h
    Actias/IO/BaseIO.cpp
    Actias/IO/FileHandle.h
    Actias/IO/FileHandle.cpp
    Actias/IO/IStream.h
    Actias/IO/FileStream.h
    Actias/IO/FileStream.cpp
    Actias/IO/StdoutStream.h
    Actias/IO/StdoutStream.cpp
    Actias/IO/StreamBase.h

    Actias/Memory/IAllocator.h
    Actias/Memory/Memory.h
    Actias/Memory/Object.h
    Actias/Memory/Ptr.h
    Actias/Memory/ReferenceCounter.h
    Actias/Memory/SystemAllocator.h

    Actias/RTTI/RTTI.h

    Actias/Strings/String.h
    Actias/Strings/StringSlice.h
    Actias/Strings/StringSlice.cpp
    Actias/Strings/Unicode.h

    Actias/System/Memory.h
    Actias/System/Memory.c

    Actias/Time/DateTime.h
    Actias/Time/DateTime.cpp
    Actias/Time/TimeSpan.h

    Actias/Utils/UUID.h
    Actias/Utils/Result.h
    Actias/Utils/BitUtils.h
    Actias/Utils/UUID.cpp
)

add_library(Actias STATIC ${SRC})

actias_configure_target(Actias)
actias_enable_simd_for_target(Actias)

target_include_directories(Actias PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(Actias PROPERTIES FOLDER "Runtime")

get_property("TARGET_SOURCE_FILES" TARGET Actias PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})

if (ACTIAS_BUILD_TESTS)
    add_subdirectory(Tests)
endif ()
