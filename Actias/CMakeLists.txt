set(SRC
    Actias/ACBX/Headers.h
    Actias/ACBX/Loader.h

    Actias/Base/Base.hpp
    Actias/Base/Byte.hpp
    Actias/Base/Flags.hpp
    Actias/Base/Hash.hpp
    Actias/Base/StartMain.cpp
    Actias/Base/wyhash.h

    Actias/Containers/ArraySlice.hpp
    Actias/Containers/HashSet.hpp
    Actias/Containers/HeapArray.hpp
    Actias/Containers/IBlob.hpp
    Actias/Containers/List.hpp

    Actias/IO/BaseIO.hpp
    Actias/IO/BaseIO.cpp
    Actias/IO/BufferedStream.hpp
    Actias/IO/Console.hpp
    Actias/IO/Console.cpp
    Actias/IO/FileSystem.hpp
    Actias/IO/FileSystem.cpp
    Actias/IO/IStream.hpp
    Actias/IO/FileStream.hpp
    Actias/IO/FileStream.cpp
    Actias/IO/StdoutStream.hpp
    Actias/IO/StdoutStream.cpp
    Actias/IO/StreamBase.hpp

    Actias/Memory/IAllocator.hpp
    Actias/Memory/Memory.hpp
    Actias/Memory/MemoryPool.hpp
    Actias/Memory/Object.hpp
    Actias/Memory/Ptr.hpp
    Actias/Memory/ReferenceCounter.hpp
    Actias/Memory/SystemAllocator.hpp

    Actias/RTTI/RTTI.hpp

    Actias/Strings/FixedString.hpp
    Actias/Strings/String.hpp
    Actias/Strings/StringBase.hpp
    Actias/Strings/StringSlice.hpp
    Actias/Strings/StringSlice.cpp
    Actias/Strings/Unicode.hpp

    Actias/Time/DateTime.hpp
    Actias/Time/DateTime.cpp
    Actias/Time/TimeSpan.hpp

    Actias/Utils/BitUtils.hpp
    Actias/Utils/LibraryLoader.hpp
    Actias/Utils/LibraryLoader.cpp
    Actias/Utils/Result.hpp
    Actias/Utils/UUID.cpp
    Actias/Utils/UUID.hpp
)

set(SRC_SYSTEM
    Actias/System/Assert.h
    Actias/System/Atomic.h
    Actias/System/Base.h
    Actias/System/CompilerTraits.h
    Actias/System/Core.h
    Actias/System/Memory.h
    Actias/System/MemoryPool.h
    Actias/System/Mutex.h
    Actias/System/Path.h
    Actias/System/Platform.h
    Actias/System/PlatformTraits.h
    Actias/System/Result.h
    Actias/System/Runtime.h
    Actias/System/Streams.h
    Actias/System/Threading.h
    Actias/System/Time.h
    Actias/System/Unicode.h

    Actias/System/Platform/Common/AtomicImpl.h
    Actias/System/Platform/Common/AtomicImpl8.h
    Actias/System/Platform/Common/AtomicImpl16.h
    Actias/System/Platform/Common/AtomicImpl32.h
    Actias/System/Platform/Common/AtomicImpl64.h
    Actias/System/Platform/Common/CopyMemory.c
    Actias/System/Platform/Common/CopyMemoryAVX.asm
    Actias/System/Platform/Common/Core.c
    Actias/System/Platform/Common/InlineCopyMemory.h
    Actias/System/Platform/Common/InlineSetMemory.h
    Actias/System/Platform/Common/InlineStringLength.h
    Actias/System/Platform/Common/Memory.c
    Actias/System/Platform/Common/Path.c
    Actias/System/Platform/Common/Runtime.c
    Actias/System/Platform/Common/RuntimeDef.h
    Actias/System/Platform/Common/SetMemory.c
    Actias/System/Platform/Common/Time.c
)

if (ACTIAS_BUILD_GUI)
    find_package(Vulkan QUIET)

    if (Vulkan_INCLUDE_DIRS)
        set(VULKAN_INCLUDES "${Vulkan_INCLUDE_DIRS}")
    elseif(DEFINED ENV{VULKAN_SDK})
        set(VULKAN_INCLUDES "$ENV{VULKAN_SDK}/include")
    endif()

    set(SRC_SYSTEM
        ${SRC_SYSTEM}
        Actias/System/Window.h
        Actias/System/Platform/Common/volk.h
        Actias/System/Platform/Common/volk.c
        Actias/System/ActiasVulkan.h
        Actias/System/ActiasVulkan.c
        Actias/System/Platform/Common/Window.c
    )
endif ()

if (ACTIAS_COMPILER_MSVC)
    set(SRC_SYSTEM
        ${SRC_SYSTEM}
        Actias/System/Compiler/AtomicImplMSVC.h
    )
elseif (ACTIAS_COMPILER_CLANG)
    set(SRC_SYSTEM
        ${SRC_SYSTEM}
        Actias/System/Compiler/AtomicImplClang.h
    )
else ()
endif ()

if (ACTIAS_WINDOWS)
    set(SRC_SYSTEM
        ${SRC_SYSTEM}
        Actias/System/Platform/Windows/Core.c
        Actias/System/Platform/Windows/Memory.c
        Actias/System/Platform/Windows/Mutex.c
        Actias/System/Platform/Windows/Path.c
        Actias/System/Platform/Windows/Runtime.c
        Actias/System/Platform/Windows/Streams.c
        Actias/System/Platform/Windows/Threading.c
        Actias/System/Platform/Windows/Time.c
        Actias/System/Platform/Windows/WinHeaders.h
    )
else ()
    set(SRC_SYSTEM
        ${SRC_SYSTEM}
        Actias/System/Platform/Unix/Core.c
        Actias/System/Platform/Unix/Memory.c
        Actias/System/Platform/Unix/Mutex.c
        Actias/System/Platform/Unix/Runtime.c
        Actias/System/Platform/Unix/Streams.c
        Actias/System/Platform/Unix/Threading.c
        Actias/System/Platform/Unix/Time.c
    )
endif ()

add_library(ActiasSystem SHARED ${SRC_SYSTEM})

if(VULKAN_INCLUDES)
    target_include_directories(ActiasSystem PUBLIC "${VULKAN_INCLUDES}")
endif()

actias_configure_target(ActiasSystem)
actias_enable_simd_for_target(ActiasSystem)

target_include_directories(ActiasSystem PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(ActiasSystem PROPERTIES FOLDER "Runtime")

if (ACTIAS_BUILD_GUI)
    target_link_libraries(ActiasSystem glfw)
endif ()

get_property("TARGET_SOURCE_FILES" TARGET ActiasSystem PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})

add_library(Actias STATIC ${SRC})

actias_configure_target(Actias)
actias_enable_simd_for_target(Actias)

target_include_directories(Actias PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(Actias PROPERTIES FOLDER "Runtime")
target_link_libraries(Actias ActiasSystem)

get_property("TARGET_SOURCE_FILES" TARGET Actias PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})

if (ACTIAS_BUILD_TESTS)
    add_subdirectory(Tests)
endif ()
