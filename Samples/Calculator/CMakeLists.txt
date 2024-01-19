add_executable(Calculator main.cpp
	BasicPlugin.hpp
	BasicPlugin.cpp
)

actias_configure_target(Calculator)

set_target_properties(Calculator PROPERTIES FOLDER "Samples")
target_link_libraries(Calculator Actias CalculatorPluginSystem)
