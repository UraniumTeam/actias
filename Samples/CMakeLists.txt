add_subdirectory(TestLibrary)
add_subdirectory(HelloWorld)

if (ACTIAS_BUILD_GUI)
    add_subdirectory(HelloWindow)
endif ()

add_subdirectory(SDKUsage)

add_subdirectory(CalculatorPluginSystem)
add_subdirectory(Calculator)
add_subdirectory(MultiplicationPlugin)
