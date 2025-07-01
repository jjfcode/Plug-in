cmake_minimum_required(VERSION 3.15)
project(FilterVST3 VERSION 1.0.0)

# Set VST3 SDK path
set(VST3_SDK_ROOT "C:/VST_SDK/vst3sdk")

# Add VST3 SDK
add_subdirectory(${VST3_SDK_ROOT} ${CMAKE_BINARY_DIR}/VST3_SDK)

# Create VST3 plugin
smtg_add_vst3plugin(FilterVST3
    FilterVST3.h
    FilterVST3.cpp
)

# Link VST3 SDK
target_link_libraries(FilterVST3
    PRIVATE
        sdk
        base
)

# Set output directory to VST3 folder
smtg_target_configure_version_file(FilterVST3)

# Installation
if(SMTG_MAC)
    smtg_target_set_bundle(FilterVST3
        BUNDLE_IDENTIFIER "com.demo.filtervst3"
        COMPANY_NAME "Demo Company"
    )
elseif(SMTG_WIN)
    target_sources(FilterVST3 PRIVATE resource/FilterVST3.rc)
endif(SMTG_MAC)

# Batch file for building the plugin
file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/build.bat" "@echo off\n")
file(APPEND "${CMAKE_CURRENT_SOURCE_DIR}/build.bat" "REM filepath: c:\\Code\\Plug-in\\VST3\\build.bat\n")
file(APPEND "${CMAKE_CURRENT_SOURCE_DIR}/build.bat" "echo Building Filter VST3 Plugin...\n")
file(APPEND "${CMAKE_CURRENT_SOURCE_DIR}/build.bat" "\nREM Create build directory\n")
file(APPEND "${CMAKE_CURRENT_SOURCE_DIR}/build.bat" "if not exist \"build\" mkdir build\n")
file(APPEND "${CMAKE_CURRENT_SOURCE_DIR}/build.bat" "cd build\n")
file(APPEND "${CMAKE_CURRENT_SOURCE_DIR}/build.bat" "\nREM Configure with CMake\n")
file(APPEND "${CMAKE_CURRENT_SOURCE_DIR}/build.bat" "cmake .. -G \"Visual Studio 17 2022\" -A x64\n")
file(APPEND "${CMAKE_CURRENT_SOURCE_DIR}/build.bat" "\nREM Build the plugin\n")
file(APPEND "${CMAKE_CURRENT_SOURCE_DIR}/build.bat" "cmake --build . --config Release\n")
file(APPEND "${CMAKE_CURRENT_SOURCE_DIR}/build.bat" "\nREM Copy to VST3 plugins folder\n")
file(APPEND "${CMAKE_CURRENT_SOURCE_DIR}/build.bat" "if exist \"VST3\\Release\\FilterVST3.vst3\" (\n")
file(APPEND "${CMAKE_CURRENT_SOURCE_DIR}/build.bat" "    echo Installing plugin...\n")
file(APPEND "${CMAKE_CURRENT_SOURCE_DIR}/build.bat" "    xcopy \"VST3\\Release\\FilterVST3.vst3\" \"%USERPROFILE%\\AppData\\Local\\Programs\\Common\\VST3\\\" /E /I /Y\n")
file(APPEND "${CMAKE_CURRENT_SOURCE_DIR}/build.bat" "    echo Plugin installed successfully!\n")
file(APPEND "${CMAKE_CURRENT_SOURCE_DIR}/build.bat" ") else (\n")
file(APPEND "${CMAKE_CURRENT_SOURCE_DIR}/build.bat" "    echo Build failed - plugin not found\n")
file(APPEND "${CMAKE_CURRENT_SOURCE_DIR}/build.bat" ")\n")
file(APPEND "${CMAKE_CURRENT_SOURCE_DIR}/build.bat" "\npause\n")