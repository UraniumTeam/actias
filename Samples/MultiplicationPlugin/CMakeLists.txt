set(SRC
    MultiplicationPlugin.cpp
)

if (WIN32)
    foreach(flag_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        STRING (REGEX REPLACE "/RTC[^ ]*" "" ${flag_var} "${${flag_var}}")
    endforeach(flag_var)
endif ()

add_library(MultiplicationPlugin SHARED ${SRC})

target_link_libraries(MultiplicationPlugin Actias CalculatorPluginSystem)

actias_configure_target(MultiplicationPlugin)

target_include_directories(MultiplicationPlugin PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(MultiplicationPlugin PROPERTIES FOLDER "Samples")

get_property("TARGET_SOURCE_FILES" TARGET MultiplicationPlugin PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})
