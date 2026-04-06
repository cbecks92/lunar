cmake_minimum_required(VERSION 3.16)

project(CarouselReverb VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set SMTG SDK path
set(SMTG_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vst3sdk" CACHE PATH "Path to VST 3 SDK")

if(NOT EXISTS "${SMTG_SDK_PATH}/CMakeLists.txt")
    message(FATAL_ERROR "VST 3 SDK not found at ${SMTG_SDK_PATH}")
endif()

# Add VST3 SDK
add_subdirectory("${SMTG_SDK_PATH}" vst3sdk)

# Plugin source files
set(PLUGIN_SOURCES
    src/processor.cpp
    src/controller.cpp
    src/entry.cpp
    src/lfo.cpp
)

# Create the VST3 plugin using SDK helper function
smtg_add_vst3plugin(
    carouselreverb
    PACKAGE_NAME "Carousel Reverb"
    SOURCES_LIST ${PLUGIN_SOURCES}
)

message(STATUS "=== CarouselReverb VST3 Plugin Configured ===")
message(STATUS "Target: carouselreverb")
message(STATUS "Package Name: Carousel Reverb")
if(TARGET carouselreverb)
    message(STATUS "Plugin target successfully created")
    get_target_property(PLUGIN_PACKAGE_PATH carouselreverb SMTG_PLUGIN_PACKAGE_PATH)
    if(PLUGIN_PACKAGE_PATH)
        message(STATUS "Plugin output: ${PLUGIN_PACKAGE_PATH}")
    endif()
else()
    message(FATAL_ERROR "Failed to create plugin target")
endif()
