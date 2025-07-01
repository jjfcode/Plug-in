@echo off
echo =========================================
echo Building Filter VST3 Plugin
echo =========================================

REM Check if VST3 SDK exists
if not exist "C:\VST_SDK" (
    echo ERROR: VST3 SDK not found at C:\VST_SDK
    echo Please make sure the VST3 SDK is extracted to C:\VST_SDK
    pause
    exit /b 1
)

echo ✅ VST3 SDK found at C:\VST_SDK

REM Create build directory
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

echo Entering build directory...
cd build

echo =========================================
echo Configuring with CMake...
echo =========================================
cmake .. -G "Visual Studio 17 2022" -A x64

if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake configuration failed
    echo Make sure you have Visual Studio 2022 installed
    pause
    exit /b 1
)

echo =========================================
echo Building the plugin...
echo =========================================
cmake --build . --config Release

if %ERRORLEVEL% neq 0 (
    echo ERROR: Build failed
    pause
    exit /b 1
)

echo =========================================
echo Installing plugin...
echo =========================================

REM Check if plugin was built
if exist "VST3\Release\FilterVST3.vst3" (
    echo ✅ Plugin built successfully!
    
    REM Create VST3 plugins directory if it doesn't exist
    if not exist "%USERPROFILE%\AppData\Local\Programs\Common\VST3" (
        mkdir "%USERPROFILE%\AppData\Local\Programs\Common\VST3"
    )
    
    echo Installing to VST3 plugins folder...
    xcopy "VST3\Release\FilterVST3.vst3" "%USERPROFILE%\AppData\Local\Programs\Common\VST3\" /E /I /Y
    
    echo =========================================
    echo ✅ SUCCESS! Plugin installed!
    echo =========================================
    echo.
    echo Plugin location:
    echo %USERPROFILE%\AppData\Local\Programs\Common\VST3\FilterVST3.vst3
    echo.
    echo You can now use the plugin in:
    echo - REAPER
    echo - FL Studio
    echo - Ableton Live
    echo - Cubase
    echo - Studio One
    echo - And other VST3-compatible DAWs
    echo.
) else (
    echo ❌ Build failed - plugin file not found
    echo Expected: VST3\Release\FilterVST3.vst3
    echo.
    echo Please check the build output above for errors.
)

echo =========================================
pause
