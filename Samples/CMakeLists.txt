add_subdirectory(HelloWorld)
add_subdirectory(HelloActias)

if (ACTIAS_BUILD_GUI)
    add_subdirectory(HelloWindow)
endif ()

add_subdirectory(SDKUsage)

add_subdirectory(CalculatorPluginSystem)
add_subdirectory(Calculator)
add_subdirectory(MultiplicationPlugin)
