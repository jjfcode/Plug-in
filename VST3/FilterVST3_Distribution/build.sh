#!/bin/bash

echo "========================================="
echo "Building Filter VST3 Plugin (Cross-Platform)"
echo "========================================="

# Detect platform
if [[ "$OSTYPE" == "darwin"* ]]; then
    PLATFORM="macOS"
    GENERATOR="Xcode"
    VST3_SDK_PATH="/Users/${USER}/VST_SDK/vst3sdk"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    PLATFORM="Linux"
    GENERATOR="Unix Makefiles"
    VST3_SDK_PATH="/usr/local/VST_SDK/vst3sdk"
else
    echo "Unsupported platform: $OSTYPE"
    exit 1
fi

echo "Platform: $PLATFORM"
echo "Generator: $GENERATOR"

# Check if VST3 SDK exists
if [ ! -d "$VST3_SDK_PATH" ]; then
    echo "ERROR: VST3 SDK not found at $VST3_SDK_PATH"
    echo "Please download and extract the VST3 SDK to this location"
    exit 1
fi

echo "✅ VST3 SDK found at $VST3_SDK_PATH"

# Create build directory
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi

echo "Entering build directory..."
cd build

echo "========================================="
echo "Configuring with CMake..."
echo "========================================="

if [[ "$PLATFORM" == "macOS" ]]; then
    cmake .. -G "$GENERATOR" -DCMAKE_OSX_DEPLOYMENT_TARGET=10.14
else
    cmake .. -G "$GENERATOR"
fi

if [ $? -ne 0 ]; then
    echo "ERROR: CMake configuration failed"
    exit 1
fi

echo "========================================="
echo "Building the plugin..."
echo "========================================="
cmake --build . --config Release

if [ $? -ne 0 ]; then
    echo "ERROR: Build failed"
    exit 1
fi

echo "========================================="
echo "Installing plugin..."
echo "========================================="

# Check if plugin was built
if [[ "$PLATFORM" == "macOS" ]]; then
    PLUGIN_PATH="VST3/Release/FilterVST3.vst3"
    INSTALL_PATH="/Library/Audio/Plug-Ins/VST3"
else
    PLUGIN_PATH="VST3/Release/FilterVST3.vst3"
    INSTALL_PATH="/usr/local/lib/vst3"
fi

if [ -d "$PLUGIN_PATH" ]; then
    echo "✅ Plugin built successfully!"
    
    # Create VST3 plugins directory if it doesn't exist
    sudo mkdir -p "$INSTALL_PATH"
    
    echo "Installing to VST3 plugins folder..."
    sudo cp -R "$PLUGIN_PATH" "$INSTALL_PATH/"
    
    echo "========================================="
    echo "✅ SUCCESS! Plugin installed!"
    echo "========================================="
    echo ""
    echo "Plugin location:"
    echo "$INSTALL_PATH/FilterVST3.vst3"
    echo ""
    echo "You can now use the plugin in:"
    echo "- REAPER"
    echo "- Logic Pro X"
    echo "- Pro Tools"
    echo "- Ableton Live"
    echo "- And other VST3-compatible DAWs"
    echo ""
else
    echo "❌ Build failed - plugin file not found"
    echo "Expected: $PLUGIN_PATH"
    echo ""
    echo "Please check the build output above for errors."
fi

echo "=========================================" 