#!/bin/bash

## Please run this script in git bash as administrator

# Function to check if script is running with admin rights
# check_admin() {
#     if [ "$(id -u)" != "0" ]; then
#         echo "This script must be run as Administrator."
#         echo "Please try one of these methods:"
#         echo "1. Right-click on Git Bash and select 'Run as administrator'"
#         echo "2. Run 'sudo bash setup_glut.sh'"
#         echo "3. Open Command Prompt as Administrator, then run this script"
#         exit 1
#     fi
# }

# # Check for admin rights before proceeding
# check_admin

# Source directory
SOURCE_DIR="glutdlls37beta"

# Windows destination paths (using forward slashes for bash compatibility)
# VS_GL_PATH="C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS"
VS_GL_PATH="C:/Program Files (x86)/Microsoft Visual Studio/2019/Professional/VC/Auxiliary/VS"
WINDOWS_PATH="C:/Windows"

# Check if source directory exists
if [ ! -d "$SOURCE_DIR" ]; then
    echo "Error: Source directory $SOURCE_DIR not found!"
    exit 1
fi

# Function to check if file exists in source
check_file() {
    if [ ! -f "$SOURCE_DIR/$1" ]; then
        echo "Error: $1 not found in $SOURCE_DIR"
        return 1
    fi
    return 0
}

# Function to create directory if it doesn't exist
create_dir() {
    if [ ! -d "$1" ]; then
        mkdir -p "$1"
        if [ $? -ne 0 ]; then
            echo "Error: Unable to create directory $1"
            echo "Please check if you have administrator rights"
            exit 1
        fi
    fi
}

# Function to copy file with error handling
copy_file() {
    local src="$1"
    local dst="$2"
    cp "$src" "$dst"
    if [ $? -ne 0 ]; then
        echo "Error: Failed to copy $src to $dst"
        echo "Please check if you have administrator rights"
        exit 1
    fi
    echo "Successfully copied $(basename $src) to $dst"
}

# Check if all required files exist
required_files=("glut.h" "glut.lib" "glut32.lib" "glut.dll" "glut32.dll")
for file in "${required_files[@]}"; do
    check_file "$file" || exit 1
done

echo "Starting file copy operations..."

# Create necessary directories
create_dir "$VS_GL_PATH/include/gl"
create_dir "$VS_GL_PATH/lib/x64"
create_dir "$VS_GL_PATH/lib/x86"

# Copy files to their respective locations
echo "Copying files..."

# Copy glut.h
copy_file "$SOURCE_DIR/glut.h" "$VS_GL_PATH/include/gl/"

# Copy glut.lib
copy_file "$SOURCE_DIR/glut.lib" "$VS_GL_PATH/lib/x64/"

# Copy glut32.lib
copy_file "$SOURCE_DIR/glut32.lib" "$VS_GL_PATH/lib/x86/"

# Copy DLL files to SysWOW64
copy_file "$SOURCE_DIR/glut.dll" "$WINDOWS_PATH/SysWOW64/"
copy_file "$SOURCE_DIR/glut32.dll" "$WINDOWS_PATH/SysWOW64/"

# Copy glut32.dll to System32
copy_file "$SOURCE_DIR/glut32.dll" "$WINDOWS_PATH/System32/"

echo "All files copied successfully!"