# Makefile for Filter Audio Unit
# Usage: make, make install, make clean, make test

# Configuration
PROJECT_NAME = FilterAudioUnit
BUNDLE_NAME = FilterAudioUnit.component
BUILD_DIR = build
INSTALL_DIR = $(HOME)/Library/Audio/Plug-Ins/Components

# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -arch x86_64 -arch arm64 -mmacosx-version-min=10.9 -fPIC
INCLUDES = -I/System/Library/Frameworks/AudioUnit.framework/Headers \
           -I/System/Library/Frameworks/AudioToolbox.framework/Headers \
           -I/System/Library/Frameworks/CoreFoundation.framework/Headers

FRAMEWORKS = -framework AudioUnit \
             -framework AudioToolbox \
             -framework CoreFoundation \
             -framework Carbon

LDFLAGS = -bundle $(FRAMEWORKS) -arch x86_64 -arch arm64 -mmacosx-version-min=10.9

# Source files
SOURCES = FilterAudioUnit.cpp
OBJECTS = $(BUILD_DIR)/FilterAudioUnit.o
EXECUTABLE = $(BUILD_DIR)/$(PROJECT_NAME)
COMPONENT = $(BUILD_DIR)/$(BUNDLE_NAME)

# Default target
all: $(COMPONENT)

# Create build directory
$(BUILD_DIR):
	@echo "Creating build directory..."
	@mkdir -p $(BUILD_DIR)

# Compile object files
$(OBJECTS): $(SOURCES) | $(BUILD_DIR)
	@echo "Compiling $(SOURCES)..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(SOURCES) -o $(OBJECTS)

# Link executable
$(EXECUTABLE): $(OBJECTS)
	@echo "Linking executable..."
	$(CXX) $(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS)

# Create component bundle
$(COMPONENT): $(EXECUTABLE)
	@echo "Creating component bundle..."
	@mkdir -p "$(COMPONENT)/Contents/MacOS"
	@mkdir -p "$(COMPONENT)/Contents/Resources"
	@cp $(EXECUTABLE) "$(COMPONENT)/Contents/MacOS/"
	@cp Info.plist "$(COMPONENT)/Contents/"
	@chmod +x "$(COMPONENT)/Contents/MacOS/$(PROJECT_NAME)"
	@echo "✅ Component bundle created: $(COMPONENT)"

# Install the Audio Unit
install: $(COMPONENT)
	@echo "Installing Audio Unit..."
	@mkdir -p "$(INSTALL_DIR)"
	@cp -R "$(COMPONENT)" "$(INSTALL_DIR)/"
	@echo "✅ Audio Unit installed to: $(INSTALL_DIR)/$(BUNDLE_NAME)"
	@echo "Restart your DAW to use the new Audio Unit."

# Validate the installed Audio Unit
validate:
	@echo "Validating Audio Unit..."
	@auval -v aufx Fltr Demo

# Test the Audio Unit
test: install
	@echo "Running Audio Unit tests..."
	@chmod +x test_au.sh
	@./test_au.sh

# Clean build files
clean:
	@echo "Cleaning build files..."
	@rm -rf $(BUILD_DIR)
	@echo "✅ Build files cleaned"

# Uninstall the Audio Unit
uninstall:
	@echo "Uninstalling Audio Unit..."
	@rm -rf "$(INSTALL_DIR)/$(BUNDLE_NAME)"
	@echo "✅ Audio Unit uninstalled"

# Show help
help:
	@echo "Filter Audio Unit Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  all        - Build the Audio Unit (default)"
	@echo "  install    - Build and install the Audio Unit"
	@echo "  validate   - Validate the installed Audio Unit"
	@echo "  test       - Run complete test suite"
	@echo "  clean      - Remove build files"
	@echo "  uninstall  - Remove installed Audio Unit"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make           # Build the Audio Unit"
	@echo "  make install   # Build and install"
	@echo "  make test      # Build, install, and test"
	@echo "  make clean     # Clean up"

# Debug target
debug: CXXFLAGS += -g -DDEBUG
debug: $(COMPONENT)
	@echo "✅ Debug build completed"

# Release target  
release: CXXFLAGS += -DNDEBUG
release: $(COMPONENT)
	@echo "✅ Release build completed"

# Show build info
info:
	@echo "Build Configuration:"
	@echo "  Project: $(PROJECT_NAME)"
	@echo "  Bundle: $(BUNDLE_NAME)"
	@echo "  Build Dir: $(BUILD_DIR)"
	@echo "  Install Dir: $(INSTALL_DIR)"
	@echo "  Compiler: $(CXX)"
	@echo "  Flags: $(CXXFLAGS)"
	@echo "  Frameworks: $(FRAMEWORKS)"

.PHONY: all install validate test clean uninstall help debug release info
