# FilterVST3 - Cross-Platform VST3 Plugin

A cross-platform VST3 audio filter plugin that works on Windows, macOS, and Linux.

## Features

- **Low-Pass Filter**: Smooth high-frequency attenuation
- **High-Pass Filter**: Low-frequency cutoff
- **Stereo Processing**: Full stereo support
- **Real-time Processing**: Zero-latency audio processing
- **State Persistence**: Save/load filter settings
- **Cross-Platform**: Works on Windows, macOS, and Linux

## Prerequisites

### Windows
- Visual Studio 2022 (Community or higher)
- CMake 3.15 or higher
- VST3 SDK

### macOS
- Xcode 12 or higher
- CMake 3.15 or higher
- VST3 SDK

### Linux
- GCC 7+ or Clang 6+
- CMake 3.15 or higher
- VST3 SDK

## Installation

### 1. Download VST3 SDK
Download the VST3 SDK from [Steinberg Developer Portal](https://developer.steinberg.help/display/VST)

### 2. Extract VST3 SDK

**Windows:**
```
C:\VST_SDK\vst3sdk\
```

**macOS:**
```
/Users/[username]/VST_SDK/vst3sdk/
```

**Linux:**
```
/usr/local/VST_SDK/vst3sdk/
```

## Building

### Windows
```batch
cd VST3
build.bat
```

### macOS/Linux
```bash
cd VST3
chmod +x build.sh
./build.sh
```

## Manual Build Process

### Windows
```batch
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

### macOS
```bash
mkdir build
cd build
cmake .. -G "Xcode" -DCMAKE_OSX_DEPLOYMENT_TARGET=10.14
cmake --build . --config Release
```

### Linux
```bash
mkdir build
cd build
cmake .. -G "Unix Makefiles"
cmake --build . --config Release
```

## Installation Locations

### Windows
```
C:\Users\[username]\AppData\Local\Programs\Common\VST3\FilterVST3.vst3\
```

### macOS
```
/Library/Audio/Plug-Ins/VST3/FilterVST3.vst3/
```

### Linux
```
/usr/local/lib/vst3/FilterVST3.vst3/
```

## Testing

### REAPER
1. Open REAPER
2. Go to `Options` → `Preferences` → `Plug-ins` → `VST`
3. Add the VST3 path for your platform
4. Click "Re-scan"
5. Add the plugin to a track

### Other DAWs
- **Logic Pro X** (macOS): Automatically scans system VST3 folder
- **Pro Tools**: Supports VST3 on all platforms
- **Ableton Live**: Supports VST3 on all platforms
- **FL Studio**: Supports VST3 on Windows

## Plugin Parameters

- **Filter Type**: Switch between Low-Pass (0) and High-Pass (1)
- **Cutoff Frequency**: Adjustable from 50Hz to 8000Hz (logarithmic)

## Development

### Project Structure
```
VST3/
├── CMakeLists.txt          # Cross-platform build configuration
├── FilterVST3.h           # Plugin header
├── FilterVST3.cpp         # Plugin implementation
├── resource/
│   └── FilterVST3.rc      # Windows resource file
├── build.bat              # Windows build script
├── build.sh               # Unix build script
└── README_CROSS_PLATFORM.md
```

### Key Features
- **Platform Detection**: Automatic platform-specific settings
- **Universal Binary**: macOS supports both Intel and Apple Silicon
- **Cross-Platform Code**: No platform-specific dependencies in core code
- **Automatic Installation**: Build scripts handle installation

## Troubleshooting

### Common Issues

**VST3 SDK not found:**
- Ensure VST3 SDK is extracted to the correct location
- Check the path in CMakeLists.txt matches your installation

**Build fails on macOS:**
- Ensure Xcode command line tools are installed
- Check macOS deployment target compatibility

**Plugin not detected by DAW:**
- Verify installation path is correct
- Restart DAW after installation
- Check DAW's VST3 scanning settings

### Platform-Specific Notes

**Windows:**
- Requires Visual Studio 2022
- Uses Windows resource files for metadata

**macOS:**
- Supports both Intel (x86_64) and Apple Silicon (arm64)
- Uses bundle format instead of DLL

**Linux:**
- Tested with GCC and Clang
- May require additional audio development libraries

## License

This project is provided as-is for educational purposes.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test on multiple platforms
5. Submit a pull request

## Support

For issues and questions:
1. Check the troubleshooting section
2. Verify your build environment
3. Test on a clean system
4. Report detailed error messages 