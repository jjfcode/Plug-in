#!/bin/bash

echo "========================================="
echo "Creating Cross-Platform VST3 Distribution"
echo "========================================="

# Create distribution directory
DIST_DIR="FilterVST3_Distribution"
mkdir -p "$DIST_DIR"

echo "Creating distribution package..."

# Copy source files
echo "📁 Copying source files..."
cp -r CMakeLists.txt "$DIST_DIR/"
cp -r FilterVST3.h "$DIST_DIR/"
cp -r FilterVST3.cpp "$DIST_DIR/"
cp -r resource "$DIST_DIR/"
cp -r build.sh "$DIST_DIR/"
cp -r build.bat "$DIST_DIR/"
cp -r README_CROSS_PLATFORM.md "$DIST_DIR/"
cp -r CROSS_PLATFORM_SUMMARY.md "$DIST_DIR/"

# Create platform-specific README files
echo "📝 Creating platform-specific instructions..."

# Windows instructions
cat > "$DIST_DIR/INSTALL_WINDOWS.txt" << 'EOF'
=== INSTALACIÓN EN WINDOWS ===

1. Asegúrate de tener instalado:
   - Visual Studio 2022 (Community o superior)
   - CMake 3.15 o superior
   - VST3 SDK en C:\VST_SDK\vst3sdk\

2. Abre Command Prompt como administrador

3. Navega a esta carpeta y ejecuta:
   build.bat

4. El plugin se instalará automáticamente en:
   %USERPROFILE%\AppData\Local\Programs\Common\VST3\

5. Reinicia tu DAW y el plugin estará disponible
EOF

# macOS instructions
cat > "$DIST_DIR/INSTALL_MACOS.txt" << 'EOF'
=== INSTALACIÓN EN macOS ===

1. Asegúrate de tener instalado:
   - Xcode 12 o superior
   - Command Line Tools: xcode-select --install
   - CMake 3.15 o superior: brew install cmake
   - VST3 SDK en ~/VST_SDK/vst3sdk/

2. Abre Terminal

3. Navega a esta carpeta y ejecuta:
   chmod +x build.sh
   ./build.sh

4. El plugin se instalará automáticamente en:
   /Library/Audio/Plug-Ins/VST3/

5. Reinicia tu DAW y el plugin estará disponible
EOF

# Linux instructions
cat > "$DIST_DIR/INSTALL_LINUX.txt" << 'EOF'
=== INSTALACIÓN EN LINUX ===

1. Asegúrate de tener instalado:
   - GCC 7+ o Clang 6+
   - CMake 3.15 o superior
   - VST3 SDK en /usr/local/VST_SDK/vst3sdk/

2. Abre Terminal

3. Navega a esta carpeta y ejecuta:
   chmod +x build.sh
   ./build.sh

4. El plugin se instalará automáticamente en:
   /usr/local/lib/vst3/

5. Reinicia tu DAW y el plugin estará disponible
EOF

# Create main README
cat > "$DIST_DIR/README.txt" << 'EOF'
=========================================
FilterVST3 - Plugin de Audio Cross-Platform
=========================================

Este plugin VST3 funciona en Windows, macOS y Linux.

ARCHIVOS INCLUIDOS:
- CMakeLists.txt: Configuración de compilación
- FilterVST3.h/.cpp: Código fuente del plugin
- build.bat: Script de compilación para Windows
- build.sh: Script de compilación para macOS/Linux
- resource/: Archivos de recursos (Windows)
- INSTALL_*.txt: Instrucciones específicas por plataforma

INSTRUCCIONES RÁPIDAS:

WINDOWS:
1. Instalar Visual Studio 2022 y VST3 SDK
2. Ejecutar: build.bat

macOS:
1. Instalar Xcode y VST3 SDK
2. Ejecutar: chmod +x build.sh && ./build.sh

LINUX:
1. Instalar GCC/Clang y VST3 SDK
2. Ejecutar: chmod +x build.sh && ./build.sh

CARACTERÍSTICAS:
- Filtro Low-Pass y High-Pass
- Procesamiento estéreo
- Compatible con todos los DAWs VST3
- Código fuente cross-platform

SOPORTE:
Consulta los archivos INSTALL_*.txt para instrucciones detalladas.
EOF

echo "✅ Distribución creada en: $DIST_DIR/"
echo ""
echo "📦 Contenido del paquete:"
ls -la "$DIST_DIR/"
echo ""
echo "🚀 Para distribuir:"
echo "1. Comprime la carpeta $DIST_DIR"
echo "2. Envía a usuarios de Windows, Mac y Linux"
echo "3. Cada usuario compilará en su plataforma"
echo ""
echo "=========================================" 