cmake_minimum_required(VERSION 3.22)
project(Actias VERSION 0.1.0
    LANGUAGES CXX C
)

option(ACTIAS_BUILD_SAMPLES OFF)
option(ACTIAS_BUILD_TESTS OFF)

enable_testing()
set(ACTIAS_PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
list(PREPEND CMAKE_MODULE_PATH "${ACTIAS_PROJECT_ROOT}/cmake")

include(Config)
include(ThirdParty/get_cpm)

if (ACTIAS_BUILD_TESTS)
    include(ThirdParty/gtest)
endif ()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

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
