set(SRC
    ActiasRuntime/Base/Base.hpp

    ActiasRuntime/Builder/ModuleBuilder.hpp
    ActiasRuntime/Builder/ModuleBuilder.cpp

    ActiasRuntime/Loader/Loader.cpp
)

add_library(ActiasRuntime SHARED ${SRC})

actias_configure_target(ActiasRuntime)
actias_enable_simd_for_target(ActiasRuntime)

target_include_directories(ActiasRuntime PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(ActiasRuntime Actias)

set_target_properties(ActiasRuntime PROPERTIES FOLDER "Runtime")

get_property("TARGET_SOURCE_FILES" TARGET ActiasRuntime PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})

if (ACTIAS_BUILD_TESTS)
    add_subdirectory(Tests)
endif ()
