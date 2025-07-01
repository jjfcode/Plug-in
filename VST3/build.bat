@echo off
REM filepath: c:\Code\Plug-in\VST3\build.bat
echo Building Filter VST3 Plugin...

REM Create build directory
if not exist "build" mkdir build
cd build

REM Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64

REM Build the plugin
cmake --build . --config Release

REM Copy to VST3 plugins folder
if exist "VST3\Release\FilterVST3.vst3" (
    echo Installing plugin...
    xcopy "VST3\Release\FilterVST3.vst3" "%USERPROFILE%\AppData\Local\Programs\Common\VST3\" /E /I /Y
    echo Plugin installed successfully!
) else (
    echo Build failed - plugin not found
)

pause
