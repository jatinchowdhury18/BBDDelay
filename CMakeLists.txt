cmake_minimum_required(VERSION 3.15)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment target")
project(BBDDelay VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(modules)

# juce_set_vst2_sdk_path(C:/SDKs/VST_SDK/VST2_SDK/)

juce_add_plugin(BBDDelay
    COMPANY_NAME chowdsp
    PLUGIN_MANUFACTURER_CODE Chow
    PLUGIN_CODE bpz4
    FORMATS AU VST3 Standalone #VST
    ProductName "BBDDelay")

juce_generate_juce_header(BBDDelay)
add_subdirectory(src)
include_directories(src)
add_subdirectory(res)

target_compile_definitions(BBDDelay PUBLIC
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(BBDDelay PRIVATE juce_plugin_modules)
