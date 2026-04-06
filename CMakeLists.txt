cmake_minimum_required(VERSION 3.16)

project(CarouselReverb VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set SMTG SDK path
set(SMTG_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vst3sdk" CACHE PATH "Path to VST 3 SDK")

if(NOT EXISTS "${SMTG_SDK_PATH}/CMakeLists.txt")
    message(FATAL_ERROR "VST 3 SDK not found at ${SMTG_SDK_PATH}")
endif()

# Configure SDK
set(SMTG_ENABLE_USE_OF_DEPENDENCY_SYSTEM_LIBRARIES FALSE)

# Add VST3 SDK
add_subdirectory("${SMTG_SDK_PATH}" vst3sdk)

# Plugin source files
set(PLUGIN_SOURCES
    src/processor.cpp
    src/controller.cpp
    src/entry.cpp
    src/lfo.cpp
)

set(PLUGIN_HEADERS
    include/processor.h
    include/controller.h
    include/constants.h
    include/carousel.h
    include/lfo.h
)

# Use the VST3 SDK's helper to add the plugin
if(COMMAND smtg_add_vst3_library)
    message(STATUS "Using SMTG VST3 library helper")
    
    smtg_add_vst3_library(
        TARGET carouselreverb
        SOURCES ${PLUGIN_SOURCES}
        HEADERS ${PLUGIN_HEADERS}
    )
    
    # Set plugin output name and location
    set_target_properties(carouselreverb PROPERTIES
        OUTPUT_NAME "carouselreverb"
    )
else()
    message(FATAL_ERROR "SMTG VST3 helper function not found. SDK may not be properly configured.")
endif()

# Ensure we capture the output
if(TARGET carouselreverb)
    message(STATUS "CarouselReverb plugin target configured")
    message(STATUS "Plugin output directory: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
else()
    message(FATAL_ERROR "Failed to create carouselreverb target")
endif()
