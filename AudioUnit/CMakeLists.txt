# CMake configuration for building Audio Unit on macOS

cmake_minimum_required(VERSION 3.15)

project(FilterAudioUnit VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# macOS specific settings
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9")
set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")

# Find required frameworks
find_library(AUDIOUNIT_FRAMEWORK AudioUnit)
find_library(AUDIOTOOLBOX_FRAMEWORK AudioToolbox)
find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
find_library(CARBON_FRAMEWORK Carbon)

if(NOT AUDIOUNIT_FRAMEWORK)
    message(FATAL_ERROR "AudioUnit framework not found")
endif()

if(NOT AUDIOTOOLBOX_FRAMEWORK)
    message(FATAL_ERROR "AudioToolbox framework not found")
endif()

if(NOT COREFOUNDATION_FRAMEWORK)
    message(FATAL_ERROR "CoreFoundation framework not found")
endif()

if(NOT CARBON_FRAMEWORK)
    message(FATAL_ERROR "Carbon framework not found")
endif()

# Add the Audio Unit target
add_library(FilterAudioUnit MODULE
    FilterAudioUnit.cpp
    FilterAudioUnit.h
)

# Set bundle properties
set_target_properties(FilterAudioUnit PROPERTIES
    BUNDLE TRUE
    BUNDLE_EXTENSION "component"
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
    MACOSX_BUNDLE_BUNDLE_NAME "Filter Audio Unit"
    MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    MACOSX_BUNDLE_IDENTIFIER "com.demo.audiounit.filter"
)

# Link frameworks
target_link_libraries(FilterAudioUnit
    ${AUDIOUNIT_FRAMEWORK}
    ${AUDIOTOOLBOX_FRAMEWORK}
    ${COREFOUNDATION_FRAMEWORK}
    ${CARBON_FRAMEWORK}
)

# Compiler flags
target_compile_options(FilterAudioUnit PRIVATE
    -Wall
    -Wextra
    -O2
    -fPIC
)

# Installation
install(TARGETS FilterAudioUnit
    BUNDLE DESTINATION "~/Library/Audio/Plug-Ins/Components"
)

# Custom target for validation
add_custom_target(validate
    COMMAND auval -v aufx Fltr Demo
    DEPENDS FilterAudioUnit
    COMMENT "Validating Audio Unit"
)

# Print build information
message(STATUS "Building Filter Audio Unit")
message(STATUS "AudioUnit Framework: ${AUDIOUNIT_FRAMEWORK}")
message(STATUS "AudioToolbox Framework: ${AUDIOTOOLBOX_FRAMEWORK}")
message(STATUS "CoreFoundation Framework: ${COREFOUNDATION_FRAMEWORK}")
message(STATUS "Carbon Framework: ${CARBON_FRAMEWORK}")
message(STATUS "Target architectures: ${CMAKE_OSX_ARCHITECTURES}")
message(STATUS "Deployment target: ${CMAKE_OSX_DEPLOYMENT_TARGET}")
