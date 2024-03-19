include(ThirdParty/get_cpm)
include(ThirdParty/args)

if (ACTIAS_BUILD_GUI)
    include(ThirdParty/glfw)
endif ()

if (ACTIAS_BUILD_TESTS)
    include(ThirdParty/gtest)
endif ()
