cmake_minimum_required(VERSION 3.16)

project(CarouselReverb VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set SMTG SDK path - download from Steinberg
set(SMTG_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vst3sdk" CACHE PATH "Path to VST 3 SDK")

if(NOT EXISTS "${SMTG_SDK_PATH}/CMakeLists.txt")
    message(STATUS "VST 3 SDK not found at ${SMTG_SDK_PATH}")
    message(STATUS "Please download VST 3 SDK from https://github.com/steinbergmedia/vst3sdk")
endif()

# Include VST 3 SDK
if(EXISTS "${SMTG_SDK_PATH}/CMakeLists.txt")
    add_subdirectory("${SMTG_SDK_PATH}" vst3sdk)
endif()

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

# Create VST 3 plugin
add_library(carouselreverb MODULE ${PLUGIN_SOURCES} ${PLUGIN_HEADERS})

target_include_directories(carouselreverb PRIVATE include)

# Link VST 3 framework
if(TARGET sdk)
    target_link_libraries(carouselreverb PRIVATE sdk)
endif()

# Platform-specific settings
if(MSVC)
    target_compile_options(carouselreverb PRIVATE /W4 /fp:fast)
    set_target_properties(carouselreverb PROPERTIES SUFFIX ".vst3")
elseif(APPLE)
    set_target_properties(carouselreverb PROPERTIES BUNDLE TRUE)
    set_target_properties(carouselreverb PROPERTIES BUNDLE_EXTENSION "vst3")
    set_target_properties(carouselreverb PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist")
else() # Linux
    target_compile_options(carouselreverb PRIVATE -Wall -Wextra -O3)
endif()
