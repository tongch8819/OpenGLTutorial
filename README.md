# OpenGL Tutorial

A simple guide to set up OpenGL development environment with Visual Studio.

## Prerequisites

- Microsoft Visual Studio 2019 Community Edition (or later)
- Git Bash

## Setup Instructions

1. Clone this repository
2. Navigate to the setup directory:
   ```bash
   cd Misc
   ```
3. Configure Visual Studio path:
   - Open `setup.sh`
   - Modify `VS_GL_PATH` to match your Visual Studio installation path
   - Default path is: `C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS`

4. Run the setup script:
   - Launch Git Bash as administrator (right-click → Run as administrator)
   - Execute the script:
     ```bash
     ./setup.sh
     ```

The script will automatically copy required GLUT files to appropriate system directories.

## What Gets Installed

The setup script installs the following files:
- `glut.h` → Visual Studio include directory
- `glut.lib` → Visual Studio x64 lib directory
- `glut32.lib` → Visual Studio x86 lib directory
- `glut.dll` and `glut32.dll` → System directories

## Troubleshooting

- If you get permission errors, make sure Git Bash is running as administrator
- Verify that the `VS_GL_PATH` in `setup.sh` matches your Visual Studio installation path

## License

This project is licensed under the MIT License - see the LICENSE file for details.