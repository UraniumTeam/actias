add_executable(SDKUsage main.cpp)

actias_configure_target(SDKUsage)

set_target_properties(SDKUsage PROPERTIES FOLDER "Samples")
target_link_libraries(SDKUsage Actias ActiasSDK ActiasRuntime TestLibrary)
