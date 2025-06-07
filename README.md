# Controller Checker

A Windows application that provides real-time visualization of Xbox controller inputs. This tool is useful for testing and debugging controller functionality, displaying button states, trigger values, and thumbstick positions.

## Features

- Real-time visualization of up to 4 Xbox controllers
- Display of button states (A, B, X, Y)
- Analog trigger input visualization (LT, RT)
- Thumbstick position tracking
- 60 FPS refresh rate for smooth updates
- Support for multiple connected controllers

## Requirements

- Windows operating system
- Microsoft Visual Studio Build Tools 2022 or later
- Windows SDK 10.0.22621.0 or later
- Xbox controller(s) for testing

## Building the Application

1. Ensure you have Microsoft Visual Studio Build Tools 2022 installed
2. Open the project in Visual Studio Code
3. Use the "Build with MSVC" task (Ctrl+Shift+B) to compile the application

## Dependencies

The application uses the following Windows libraries:
- Xinput.lib - For controller input handling
- User32.lib - For window management
- Gdi32.lib - For graphics rendering

## Usage

1. Launch the application
2. Connect your Xbox controller(s)
3. The application will automatically detect and display the state of connected controllers
4. Each controller's state is displayed in its own section, showing:
   - Button states (A, B, X, Y)
   - Trigger values (LT, RT)
   - Thumbstick positions

## Development

The application is written in C++ using the Windows API. The main components are:
- `controller.cpp` - Contains the main application logic
- `.vscode/tasks.json` - Build configuration for Visual Studio Code
- `c_cpp_properties.json` - C++ configuration for Visual Studio Code

## License

This project is open source and available under the MIT License. 