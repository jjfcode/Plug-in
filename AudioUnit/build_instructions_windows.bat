@echo off
echo Filter Audio Unit - Build Instructions for macOS
echo ===============================================
echo.
echo This Audio Unit can only be built and used on macOS.
echo.
echo REQUIREMENTS:
echo - macOS 10.9 or later
echo - Xcode with command line tools
echo - A Mac computer
echo.
echo BUILD INSTRUCTIONS:
echo.
echo 1. Transfer the AudioUnit folder to a Mac
echo.
echo 2. Open Terminal on the Mac and navigate to the AudioUnit folder:
echo    cd /path/to/AudioUnit
echo.
echo 3. Make the build script executable:
echo    chmod +x build.sh
echo.
echo 4. Run the build script:
echo    ./build.sh
echo.
echo 5. Or use the Makefile:
echo    make install
echo.
echo 6. The Audio Unit will be installed to:
echo    ~/Library/Audio/Plug-Ins/Components/FilterAudioUnit.component
echo.
echo 7. Restart your DAW (Logic Pro, GarageBand, etc.) to use the plugin
echo.
echo ALTERNATIVE FOR WINDOWS:
echo If you want to use audio plugins on Windows, consider:
echo.
echo 1. VST3 development with JUCE framework
echo 2. Using the Python version with a plugin host like:
echo    - REAPER (supports ReaScript/Python)
echo    - Carla (plugin host)
echo    - Blue Cat's Plug'n Script
echo.
echo The Python version (audio_filter_plugin.py) works great on Windows
echo for real-time audio processing.
echo.
pause
