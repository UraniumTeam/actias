set(SRC
    "ActiasSDK/Base/Base.hpp"

    ActiasSDK/Parser/Result.hpp

    ActiasSDK/Platform/INativeExecutable.hpp
	ActiasSDK/Platform/NativeExecutableFactory.hpp
	ActiasSDK/Platform/NativeExecutableFactory.cpp

    ActiasSDK/Platform/Windows/Parser/Exports.hpp
    ActiasSDK/Platform/Windows/Parser/NTHeader.hpp
    ActiasSDK/Platform/Windows/Parser/NTHeader.cpp
    ActiasSDK/Platform/Windows/Parser/PECommon.hpp
    ActiasSDK/Platform/Windows/Parser/PEModuleBuffer.hpp
    ActiasSDK/Platform/Windows/Parser/PEModuleBuffer.cpp
    ActiasSDK/Platform/Windows/PortableExecutable.hpp
    ActiasSDK/Platform/Windows/PortableExecutable.cpp
)

add_library(ActiasSDK SHARED ${SRC})

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
