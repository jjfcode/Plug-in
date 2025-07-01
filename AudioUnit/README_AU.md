# Filter Audio Unit

A professional Audio Unit implementation of the Low Pass and High Pass filters for macOS.

## Requirements

- **macOS 10.9** or later
- **Xcode** with command line tools
- **CMake** 3.15 or later (optional, for CMake build)

## Building the Audio Unit

### Method 1: Using the build script (Recommended)

```bash
cd AudioUnit
chmod +x build.sh
./build.sh
```

### Method 2: Using CMake

```bash
cd AudioUnit
mkdir build
cd build
cmake ..
make
make install
```

### Method 3: Manual compilation

```bash
cd AudioUnit

# Compile
g++ -c -Wall -O2 \
    -arch x86_64 -arch arm64 \
    -mmacosx-version-min=10.9 \
    -I/System/Library/Frameworks/AudioUnit.framework/Headers \
    -I/System/Library/Frameworks/AudioToolbox.framework/Headers \
    -I/System/Library/Frameworks/CoreFoundation.framework/Headers \
    -fPIC \
    FilterAudioUnit.cpp \
    -o FilterAudioUnit.o

# Link
g++ -bundle \
    -arch x86_64 -arch arm64 \
    -mmacosx-version-min=10.9 \
    -framework AudioUnit \
    -framework AudioToolbox \
    -framework CoreFoundation \
    -framework Carbon \
    -o FilterAudioUnit \
    FilterAudioUnit.o

# Create bundle
mkdir -p FilterAudioUnit.component/Contents/MacOS
mkdir -p FilterAudioUnit.component/Contents/Resources
cp FilterAudioUnit FilterAudioUnit.component/Contents/MacOS/
cp Info.plist FilterAudioUnit.component/Contents/

# Install
cp -R FilterAudioUnit.component ~/Library/Audio/Plug-Ins/Components/
```

## Installation

The Audio Unit will be installed to:
```
~/Library/Audio/Plug-Ins/Components/FilterAudioUnit.component
```

## Validation

After installation, validate the Audio Unit:

```bash
auval -v aufx Fltr Demo
```

## Usage in DAW

1. **Logic Pro**: 
   - Insert > Audio FX > Demo > Filter

2. **GarageBand**: 
   - Smart Controls > Plug-ins > Audio Units > Demo > Filter

3. **Pro Tools** (with Audio Units support):
   - Insert > Audio Units > Demo > Filter

4. **Ableton Live** (with Audio Units support):
   - Audio Effects > Audio Units > Demo > Filter

## Parameters

- **Filter Type**: Switch between Low Pass (0) and High Pass (1)
- **Cutoff Frequency**: Adjustable from 50 Hz to 8000 Hz

## Filter Algorithms

### Low Pass Filter
```
y[n] = (1-α)·x[n] + α·y[n-1]
where α = 1 / (1 + fc/sample_rate)
```

### High Pass Filter
```
y[n] = α·(y[n-1] + x[n] - x[n-1])
where α = fc / (fc + sample_rate)
```

## Troubleshooting

### Audio Unit not appearing in DAW
1. Check installation path: `~/Library/Audio/Plug-Ins/Components/`
2. Restart your DAW completely
3. Run validation: `auval -v aufx Fltr Demo`
4. Check Console.app for error messages

### Build errors
1. Ensure Xcode command line tools are installed: `xcode-select --install`
2. Check that all frameworks are available
3. Verify macOS version compatibility

### Performance issues
1. The Audio Unit is optimized for real-time processing
2. Uses first-order filters for minimal CPU usage
3. Supports both Intel and Apple Silicon architectures

## Technical Details

- **Type**: Audio Effect (aufx)
- **Subtype**: Fltr
- **Manufacturer**: Demo
- **Version**: 1.0
- **Architectures**: Universal (x86_64 + arm64)
- **Minimum macOS**: 10.9
- **Processing**: Real-time, sample-by-sample
- **Channels**: Supports mono and stereo

## File Structure

```
AudioUnit/
├── FilterAudioUnit.h       # Header file with class definition
├── FilterAudioUnit.cpp     # Implementation with Audio Unit callbacks
├── Info.plist             # Bundle configuration
├── CMakeLists.txt          # CMake build configuration
├── build.sh               # Build script
└── README_AU.md           # This file
```

## Development Notes

This Audio Unit implements the exact same filter algorithms as the Python version, ensuring consistent behavior across platforms. The implementation follows Apple's Audio Unit programming guidelines and is compatible with all major DAWs that support Audio Units.

The code is structured to be easily extensible - additional filter types or parameters can be added by extending the parameter enumeration and implementing the corresponding processing functions.
