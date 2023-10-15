set(SRC
    ActiasSDK/Parser/Result.h

    ActiasSDK/Platform/INativeExecutable.h
    
    ActiasSDK/Platform/Windows/Parser/Exports.h
    ActiasSDK/Platform/Windows/Parser/PECommon.h
    ActiasSDK/Platform/Windows/Parser/NTHeader.h
    ActiasSDK/Platform/Windows/Parser/NTHeader.cpp
    ActiasSDK/Platform/Windows/PortableExecutable.h
    ActiasSDK/Platform/Windows/PortableExecutable.cpp
)

add_library(ActiasSDK STATIC ${SRC})

actias_configure_target(ActiasSDK)
actias_enable_simd_for_target(ActiasSDK)

target_include_directories(ActiasSDK PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(ActiasSDK PROPERTIES FOLDER "SDK")
target_link_libraries(ActiasSDK Actias)

get_property("TARGET_SOURCE_FILES" TARGET ActiasSDK PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})

if (ACTIAS_BUILD_TESTS)
    add_subdirectory(Tests)
endif ()
