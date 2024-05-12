if (WIN32)
    foreach(flag_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        STRING (REGEX REPLACE "/RTC[^ ]*" "" ${flag_var} "${${flag_var}}")
    endforeach(flag_var)
endif ()

add_library(HelloVulkan SHARED main.cpp)
add_executable(HelloVulkan_Native main.cpp)

actias_configure_target(HelloVulkan)
actias_configure_target(HelloVulkan_Native)

set_target_properties(HelloVulkan PROPERTIES FOLDER "Samples")
set_target_properties(HelloVulkan_Native PROPERTIES FOLDER "Samples")

target_link_libraries(HelloVulkan Actias)
target_link_libraries(HelloVulkan_Native Actias)
