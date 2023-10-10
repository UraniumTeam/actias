set(SRC
	ActiasCore/Base/Base.h
	ActiasCore/Base/Byte.h
	ActiasCore/Base/CompilerTraits.h
	ActiasCore/Base/Flags.h
	ActiasCore/Base/Platform.h
	ActiasCore/Base/PlatformTraits.h

    ActiasCore/Temp.cpp
)

add_library(ActiasCore STATIC ${SRC})

actias_configure_target(ActiasCore)
actias_enable_simd_for_target(ActiasCore)

target_include_directories(ActiasCore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(ActiasCore PROPERTIES FOLDER "Core")

get_property("TARGET_SOURCE_FILES" TARGET ActiasCore PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})

if (ACTIAS_BUILD_TESTS)
    add_subdirectory(Tests)
endif ()
