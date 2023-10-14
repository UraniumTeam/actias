set(SRC
	main.cpp
)

add_executable(ActiasRuntimeTools ${SRC})
set_target_properties(ActiasRuntimeTools PROPERTIES OUTPUT_NAME "actias")

actias_configure_target(ActiasRuntimeTools)
actias_enable_simd_for_target(ActiasRuntimeTools)

target_include_directories(ActiasRuntimeTools PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(ActiasRuntimeTools PROPERTIES FOLDER "Runtime")
target_link_libraries(ActiasRuntimeTools Actias args)

get_property("TARGET_SOURCE_FILES" TARGET ActiasRuntimeTools PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})
