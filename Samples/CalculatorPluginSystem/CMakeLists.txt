set(SRC
	CalculatorPluginAPI/ICalculatorPlugin.hpp
	CalculatorPluginAPI/IOperation.hpp
)

add_library(CalculatorPluginSystem INTERFACE ${SRC})

target_link_libraries(CalculatorPluginSystem INTERFACE Actias)

target_include_directories(CalculatorPluginSystem INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
set_target_properties(CalculatorPluginSystem PROPERTIES FOLDER "Samples")

get_property("TARGET_SOURCE_FILES" TARGET CalculatorPluginSystem PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})
