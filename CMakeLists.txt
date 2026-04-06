cmake_minimum_required(VERSION 3.16)

project(CarouselReverb VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set SMTG SDK path
set(SMTG_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vst3sdk" CACHE PATH "Path to VST 3 SDK")

if(NOT EXISTS "${SMTG_SDK_PATH}/CMakeLists.txt")
    message(FATAL_ERROR "VST 3 SDK not found at ${SMTG_SDK_PATH}\nPlease download from https://github.com/steinbergmedia/vst3sdk")
endif()

# Include VST 3 SDK - this sets up all the necessary toolchains and functions
add_subdirectory("${SMTG_SDK_PATH}" vst3sdk)

# Plugin class name and metadata
set(plugin_name "CarouselReverb")
set(plugin_class "CarouselReverbProcessor")
set(plugin_target carouselreverb)

# Try to use SMTG helper if available, otherwise manual setup
if(COMMAND smtg_add_vst3_library)
    message(STATUS "Using SMTG_AddVST3Library helper function")
    
    smtg_add_vst3_library(
        TARGET ${plugin_target}
        SOURCES 
            src/processor.cpp
            src/controller.cpp
            src/entry.cpp
            src/lfo.cpp
        UI_SOURCES
            # Add editor sources if/when VSTGUI is re-enabled
        HEADERS
            include/processor.h
            include/controller.h
            include/constants.h
            include/carousel.h
            include/lfo.h
        PLUGIN_NAME "${plugin_name}"
    )
else()
    message(STATUS "SMTG helper not available, using manual target setup")
    
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

    # Create VST 3 plugin library
    add_library(${plugin_target} MODULE ${PLUGIN_SOURCES} ${PLUGIN_HEADERS})

    target_include_directories(${plugin_target} PRIVATE 
        include
        ${SDK_ROOT}
        ${SDK_ROOT}/pluginterfaces
        ${SDK_ROOT}/public.sdk
    )

    # Link with VST base libraries
    target_link_libraries(${plugin_target} PRIVATE
        base
        pluginterfaces
        sdk
    )

    # Platform-specific settings
    if(MSVC)
        target_compile_options(${plugin_target} PRIVATE /W4 /fp:fast)
        set_target_properties(${plugin_target} PROPERTIES SUFFIX ".vst3")
    elseif(APPLE)
        set_target_properties(${plugin_target} PROPERTIES BUNDLE TRUE)
        set_target_properties(${plugin_target} PROPERTIES BUNDLE_EXTENSION "vst3")
    else() # Linux
        target_compile_options(${plugin_target} PRIVATE -Wall -Wextra -O3)
    endif()
endif()

# Ensure output directory
set_target_properties(${plugin_target} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/VST3"
)
