#!/bin/bash

# Build script for Audio Unit on macOS
# This script compiles the Audio Unit and creates a .component bundle

set -e

echo "Building Filter Audio Unit..."

# Configuration
PROJECT_NAME="FilterAudioUnit"
BUNDLE_NAME="FilterAudioUnit.component"
BUILD_DIR="build"
INSTALL_DIR="~/Library/Audio/Plug-Ins/Components"

# Create build directory
mkdir -p $BUILD_DIR

# Compile the Audio Unit
echo "Compiling source files..."
g++ -c -Wall -O2 \
    -arch x86_64 -arch arm64 \
    -mmacosx-version-min=10.9 \
    -I/System/Library/Frameworks/AudioUnit.framework/Headers \
    -I/System/Library/Frameworks/AudioToolbox.framework/Headers \
    -I/System/Library/Frameworks/CoreFoundation.framework/Headers \
    -fPIC \
    FilterAudioUnit.cpp \
    -o $BUILD_DIR/FilterAudioUnit.o

# Link the bundle
echo "Linking Audio Unit bundle..."
g++ -bundle \
    -arch x86_64 -arch arm64 \
    -mmacosx-version-min=10.9 \
    -framework AudioUnit \
    -framework AudioToolbox \
    -framework CoreFoundation \
    -framework Carbon \
    -o $BUILD_DIR/$PROJECT_NAME \
    $BUILD_DIR/FilterAudioUnit.o

# Create the component bundle structure
echo "Creating component bundle..."
COMPONENT_DIR="$BUILD_DIR/$BUNDLE_NAME"
mkdir -p "$COMPONENT_DIR/Contents/MacOS"
mkdir -p "$COMPONENT_DIR/Contents/Resources"

# Copy files to bundle
cp $BUILD_DIR/$PROJECT_NAME "$COMPONENT_DIR/Contents/MacOS/"
cp Info.plist "$COMPONENT_DIR/Contents/"

# Set permissions
chmod +x "$COMPONENT_DIR/Contents/MacOS/$PROJECT_NAME"

echo "Build completed successfully!"
echo "Component created at: $COMPONENT_DIR"

# Install the component (optional)
read -p "Do you want to install the Audio Unit? (y/n): " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "Installing Audio Unit..."
    mkdir -p "$INSTALL_DIR"
    cp -R "$COMPONENT_DIR" "$INSTALL_DIR/"
    echo "Audio Unit installed to: $INSTALL_DIR/$BUNDLE_NAME"
    echo "Restart your DAW to use the new Audio Unit."
fi

echo "Done!"
