set(SRC
    TestLibrary/AddNumbers.h
    TestLibrary/AddNumbers.cpp
)

if (WIN32)
    foreach(flag_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        STRING (REGEX REPLACE "/RTC[^ ]*" "" ${flag_var} "${${flag_var}}")
    endforeach(flag_var)
endif ()

add_library(TestLibrary SHARED ${SRC})

target_link_libraries(TestLibrary Actias)

actias_configure_target(TestLibrary)

target_include_directories(TestLibrary PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(TestLibrary PROPERTIES FOLDER "Samples")

get_property("TARGET_SOURCE_FILES" TARGET TestLibrary PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})
