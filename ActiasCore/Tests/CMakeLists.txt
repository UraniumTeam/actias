set(SRC
    Common/Common.h
    main.cpp
)

add_executable(ActiasCoreTests ${SRC})

set_target_properties(ActiasCoreTests PROPERTIES FOLDER "Core")
target_link_libraries(ActiasCoreTests gtest gmock Actias)

get_property("TARGET_SOURCE_FILES" TARGET ActiasCoreTests PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})

include(GoogleTest)
gtest_discover_tests(ActiasCoreTests)
