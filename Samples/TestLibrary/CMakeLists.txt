set(SRC
	AddNumbers.h
	AddNumbers.cpp
)

add_library(TestLibrary SHARED ${SRC})

actias_configure_target(TestLibrary)

set_target_properties(TestLibrary PROPERTIES FOLDER "Samples")
target_link_libraries(TestLibrary Actias)
