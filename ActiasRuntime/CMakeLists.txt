set(SRC
    ActiasRuntime/Temp.cpp
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
