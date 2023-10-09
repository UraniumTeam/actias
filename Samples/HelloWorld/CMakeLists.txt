add_executable(HelloWorld main.cpp)

actias_configure_target(HelloWorld)

set_target_properties(HelloWorld PROPERTIES FOLDER "Samples")
target_link_libraries(HelloWorld ActiasRuntime)
