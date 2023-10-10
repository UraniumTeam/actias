set(SRC
    Common/Common.h
    main.cpp
)

add_executable(ActiasSDKTests ${SRC})

set_target_properties(ActiasSDKTests PROPERTIES FOLDER "SDK")
target_link_libraries(ActiasSDKTests gtest gmock Actias)

get_property("TARGET_SOURCE_FILES" TARGET ActiasSDKTests PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})

include(GoogleTest)
gtest_discover_tests(ActiasSDKTests)
