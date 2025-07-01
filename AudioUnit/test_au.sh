#!/bin/bash

# Test script for Filter Audio Unit
# This script validates the Audio Unit and tests its functionality

echo "Testing Filter Audio Unit..."

# Check if Audio Unit is installed
AU_PATH="$HOME/Library/Audio/Plug-Ins/Components/FilterAudioUnit.component"

if [ ! -d "$AU_PATH" ]; then
    echo "❌ Audio Unit not found at: $AU_PATH"
    echo "Please build and install the Audio Unit first."
    exit 1
fi

echo "✅ Audio Unit found at: $AU_PATH"

# Validate the Audio Unit
echo ""
echo "Running Audio Unit validation..."
echo "This may take a few moments..."

if auval -v aufx Fltr Demo; then
    echo "✅ Audio Unit validation passed!"
else
    echo "❌ Audio Unit validation failed!"
    echo "Check the error messages above for details."
    exit 1
fi

# Test Audio Unit loading (requires additional tools)
echo ""
echo "Testing Audio Unit loading..."

# Create a simple test using afplay (if available)
if command -v afplay &> /dev/null; then
    echo "✅ afplay found - you can test audio playback"
    echo "Use: afplay /System/Library/Sounds/Ping.aiff"
else
    echo "⚠️  afplay not found - manual testing required"
fi

# Check system requirements
echo ""
echo "System Requirements Check:"
echo "macOS Version: $(sw_vers -productVersion)"
echo "Architecture: $(uname -m)"

# Minimum version check
MIN_VERSION="10.9"
CURRENT_VERSION=$(sw_vers -productVersion)

if [[ $(echo "$CURRENT_VERSION $MIN_VERSION" | tr " " "\n" | sort -V | head -n1) = "$MIN_VERSION" ]]; then
    echo "✅ macOS version is compatible (>= $MIN_VERSION)"
else
    echo "❌ macOS version is too old (< $MIN_VERSION)"
fi

# Framework check
echo ""
echo "Framework Check:"
FRAMEWORKS=(
    "/System/Library/Frameworks/AudioUnit.framework"
    "/System/Library/Frameworks/AudioToolbox.framework"
    "/System/Library/Frameworks/CoreFoundation.framework"
    "/System/Library/Frameworks/Carbon.framework"
)

for framework in "${FRAMEWORKS[@]}"; do
    if [ -d "$framework" ]; then
        echo "✅ $framework"
    else
        echo "❌ $framework"
    fi
done

# Bundle structure check
echo ""
echo "Bundle Structure Check:"
BUNDLE_ITEMS=(
    "$AU_PATH/Contents/MacOS/FilterAudioUnit"
    "$AU_PATH/Contents/Info.plist"
)

for item in "${BUNDLE_ITEMS[@]}"; do
    if [ -e "$item" ]; then
        echo "✅ $item"
    else
        echo "❌ $item"
    fi
done

# Permissions check
echo ""
echo "Permissions Check:"
if [ -x "$AU_PATH/Contents/MacOS/FilterAudioUnit" ]; then
    echo "✅ Executable has correct permissions"
else
    echo "❌ Executable permissions incorrect"
    echo "Fix with: chmod +x '$AU_PATH/Contents/MacOS/FilterAudioUnit'"
fi

# Code signing check (for distribution)
echo ""
echo "Code Signing Check:"
if codesign -v "$AU_PATH" 2>/dev/null; then
    echo "✅ Audio Unit is properly signed"
else
    echo "⚠️  Audio Unit is not signed (required for distribution)"
    echo "For development/testing, this is usually fine"
fi

echo ""
echo "Testing complete!"
echo ""
echo "Next steps:"
echo "1. Launch your favorite DAW (Logic Pro, GarageBand, etc.)"
echo "2. Look for 'Demo: Filter' in the Audio Effects menu"
echo "3. Insert the plugin on an audio track"
echo "4. Adjust the Filter Type and Cutoff Frequency parameters"
echo ""
echo "If the plugin doesn't appear:"
echo "- Restart your DAW completely"
echo "- Check Console.app for error messages"
echo "- Run this test script again"
