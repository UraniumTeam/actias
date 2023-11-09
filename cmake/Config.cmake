if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(ACTIAS_COMPILER_CLANG ON)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(ACTIAS_COMPILER_GCC ON)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(ACTIAS_COMPILER_MSVC ON)
endif ()

if (WIN32)
    set(ACTIAS_WINDOWS ON)
endif ()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ACTIAS_64_BIT ON)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ACTIAS_32_BIT ON)
endif ()

set(CMAKE_DEBUG_POSTFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

if (ACTIAS_WINDOWS)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_definitions(_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
    add_compile_definitions(_ENABLE_EXTENDED_ALIGNED_STORAGE)
endif ()

option(ACTIAS_USE_SSE3 ON)
option(ACTIAS_USE_SSE41 ON)
option(ACTIAS_USE_AVX2 ON)

if (ACTIAS_USE_SSE41)
    if (NOT ACTIAS_USE_SSE3)
        message(WARNING "SSE4.1 instructions are enabled, but SSE3 are not")
        set(ACTIAS_USE_SSE3 ON)
    endif ()
endif ()

if (ACTIAS_COMPILER_MSVC)
    string(APPEND CMAKE_CXX_FLAGS " /W4 /WX")
    set(CMAKE_USE_WIN32_THREADS_INIT ON)
else ()
    string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Werror")
    if (NOT ACTIAS_WINDOWS)
        add_link_options("-fuse-ld=lld")
        string(APPEND CMAKE_CXX_FLAGS " -fPIC")
    endif ()
endif ()

set(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS})

if (ACTIAS_WINDOWS)
    set(CMAKE_ASM_NASM_FLAGS "-DACTIAS_WINDOWS=1")
else ()
    set(CMAKE_ASM_NASM_FLAGS "-DACTIAS_LINUX=1")
endif ()

if (ACTIAS_USE_SSE41 AND NOT ACTIAS_COMPILER_MSVC)
    string(APPEND CMAKE_CXX_FLAGS " -msse4.1")
    string(APPEND CMAKE_C_FLAGS " -msse4.1")
endif ()
if (ACTIAS_USE_AVX2 AND NOT ACTIAS_COMPILER_MSVC)
    string(APPEND CMAKE_CXX_FLAGS " -mavx -mavx2")
    string(APPEND CMAKE_C_FLAGS " -mavx -mavx2")
endif ()

function(actias_enable_simd_for_target SIMD_TARGET)
    if (ACTIAS_USE_SSE3)
        target_compile_definitions(${SIMD_TARGET} PUBLIC ACTIAS_SSE3_SUPPORTED=1)
    endif ()
    if (ACTIAS_USE_SSE41)
        target_compile_definitions(${SIMD_TARGET} PUBLIC ACTIAS_SSE41_SUPPORTED=1)
    endif ()
    if (ACTIAS_USE_AVX2)
        target_compile_definitions(${SIMD_TARGET} PUBLIC ACTIAS_AVX2_SUPPORTED=1)
    endif ()
endfunction()

function(actias_configure_target TARGET)
    if (ACTIAS_64_BIT)
        target_compile_definitions(${TARGET} PUBLIC ACTIAS_ARCH_64_BIT=1)
    elseif (ACTIAS_32_BIT)
        target_compile_definitions(${TARGET} PUBLIC ACTIAS_ARCH_32_BIT=1)
    endif ()
endfunction()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
