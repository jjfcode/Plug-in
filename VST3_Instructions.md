# VST3 Plugin for Windows - Build Instructions

## Creating a VST3 version for Windows

Since Audio Units (.au) only work on macOS, here's how to create a VST3 version for Windows.

## Requirements for VST3

- **Windows 10/11**
- **Visual Studio 2019/2022** (Community Edition is free)
- **VST3 SDK** from Steinberg
- **CMake** 3.15 or later

## Download VST3 SDK

1. Go to: https://www.steinberg.net/developers/
2. Download the VST3 SDK
3. Extract to: `C:\VST3_SDK\`

## Build Instructions

```powershell
# Clone JUCE framework (easier alternative)
git clone https://github.com/juce-framework/JUCE.git
cd JUCE

# Or download VST3 SDK manually
# Extract VST3 SDK to C:\VST3_SDK\
```

## Alternative: Use JUCE Framework (Recommended)

JUCE makes it much easier to create cross-platform audio plugins.

### Step 1: Download JUCE
- Go to: https://juce.com/get-juce/download
- Download JUCE (free for personal use)
- Install the Projucer

### Step 2: Create JUCE Project
1. Open Projucer
2. Create new "Audio Plug-in" project
3. Set the plugin name: "FilterPlugin"
4. Add our filter code

### Step 3: Build
- Open generated Visual Studio project
- Build for VST3 format

## Immediate Solution: Use our Python version

For immediate use on Windows, our Python version (`audio_filter_plugin.py`) works perfectly and provides:
- Real-time audio processing
- GUI controls
- Same filter algorithms
- Cross-platform compatibility

## Plugin Hosts that work with Python

1. **REAPER** - Supports ReaScript (Python scripts)
2. **Carla** - Can load Python audio scripts
3. **Blue Cat's Plug'n Script** - Python scripting plugin

Would you like me to:
1. Create a VST3 version using JUCE?
2. Create a REAPER ReaScript version?
3. Show you how to use the Python version with plugin hosts?
