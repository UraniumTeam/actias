if (WIN32)
    foreach(flag_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        STRING (REGEX REPLACE "/RTC[^ ]*" "" ${flag_var} "${${flag_var}}")
    endforeach(flag_var)
endif ()

add_library(HelloWindow SHARED main.cpp)

actias_configure_target(HelloWindow)

set_target_properties(HelloWindow PROPERTIES FOLDER "Samples")
target_link_libraries(HelloWindow Actias)
