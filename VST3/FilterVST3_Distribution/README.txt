=========================================
FilterVST3 - Plugin de Audio Cross-Platform
=========================================

Este plugin VST3 funciona en Windows, macOS y Linux.

⚠️ REQUISITO OBLIGATORIO: VST3 SDK
Todos los usuarios deben descargar el VST3 SDK desde:
https://developer.steinberg.help/display/VST

ARCHIVOS INCLUIDOS:
- CMakeLists.txt: Configuración de compilación
- FilterVST3.h/.cpp: Código fuente del plugin
- build.bat: Script de compilación para Windows
- build.sh: Script de compilación para macOS/Linux
- resource/: Archivos de recursos (Windows)
- INSTALL_MACOS.txt: Instrucciones específicas para Mac

INSTRUCCIONES RÁPIDAS:

WINDOWS:
1. Descargar VST3 SDK a C:\VST_SDK\vst3sdk\
2. Instalar Visual Studio 2022
3. Ejecutar: build.bat

macOS:
1. Descargar VST3 SDK a ~/VST_SDK/vst3sdk/
2. Instalar Xcode y CMake
3. Ejecutar: chmod +x build.sh && ./build.sh

LINUX:
1. Descargar VST3 SDK a /usr/local/VST_SDK/vst3sdk/
2. Instalar GCC/Clang y CMake
3. Ejecutar: chmod +x build.sh && ./build.sh

CARACTERÍSTICAS:
- Filtro Low-Pass y High-Pass
- Procesamiento estéreo
- Compatible con todos los DAWs VST3
- Código fuente cross-platform

SOPORTE:
Consulta los archivos INSTALL_*.txt para instrucciones detalladas.

IMPORTANTE:
✅ Este paquete contiene el código fuente
✅ Cada usuario debe compilar en su plataforma
❌ NO se puede usar el binario de otra plataforma
⚠️ VST3 SDK es OBLIGATORIO para todas las plataformas 