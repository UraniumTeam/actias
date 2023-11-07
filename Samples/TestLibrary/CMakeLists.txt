set(SRC
	TestLibrary/AddNumbers.h
	TestLibrary/AddNumbers.cpp
)

add_library(TestLibrary SHARED ${SRC})

target_link_libraries(TestLibrary Actias)

actias_configure_target(TestLibrary)

target_include_directories(TestLibrary PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(TestLibrary PROPERTIES FOLDER "Samples")

get_property("TARGET_SOURCE_FILES" TARGET TestLibrary PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})
