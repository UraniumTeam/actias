set(SRC
    Common/Common.h
    main.cpp

    Buffers/ArrayPool.cpp
    Utils/UUID.cpp
    Containers/List.cpp
    RTTI/RTTI.cpp
    Strings/Format.cpp
    Strings/String.cpp
)

add_executable(ActiasTests ${SRC})

set_target_properties(ActiasTests PROPERTIES FOLDER "Runtime")
target_link_libraries(ActiasTests gtest gmock Actias)

get_property("TARGET_SOURCE_FILES" TARGET ActiasTests PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})

include(GoogleTest)
gtest_discover_tests(ActiasTests)
