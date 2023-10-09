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

add_executable(ActiasRuntimeTests ${SRC})

set_target_properties(ActiasRuntimeTests PROPERTIES FOLDER "Runtime")
target_link_libraries(ActiasRuntimeTests gtest gmock ActiasRuntime)

get_property("TARGET_SOURCE_FILES" TARGET ActiasRuntimeTests PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})

include(GoogleTest)
gtest_discover_tests(ActiasRuntimeTests)
