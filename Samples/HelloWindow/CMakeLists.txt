add_executable(HelloWindow main.cpp)

actias_configure_target(HelloWindow)

set_target_properties(HelloWindow PROPERTIES FOLDER "Samples")
target_link_libraries(HelloWindow Actias)
