add_library(HelloActias SHARED main.cpp)

actias_configure_target(HelloActias)

set_target_properties(HelloActias PROPERTIES FOLDER "Samples")
target_link_libraries(HelloActias Actias)
