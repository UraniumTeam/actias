cmake_minimum_required(VERSION 3.19)
project(Actias VERSION 0.1.0
    LANGUAGES CXX C ASM_NASM
)

enable_language(ASM_NASM)
if (APPLE)
    string(APPEND CMAKE_ASM_NASM_FLAGS "--prefix _")
endif ()

option(ACTIAS_BUILD_SAMPLES OFF)
option(ACTIAS_BUILD_TESTS OFF)
option(ACTIAS_BUILD_GUI ON)

enable_testing()
set(ACTIAS_PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
list(PREPEND CMAKE_MODULE_PATH "${ACTIAS_PROJECT_ROOT}/cmake")

include(Config)
include(deps)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(Actias)
add_subdirectory(ActiasRuntime)
add_subdirectory(ActiasSDK)
add_subdirectory(ActiasRuntimeTools)

if (ACTIAS_BUILD_SAMPLES)
    add_subdirectory(Samples)
endif ()
