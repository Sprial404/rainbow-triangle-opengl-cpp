# Rainbow Triangle (C++)

This repo is a lightweight OpenGL playground in C++.

At the moment, the executable target is named rainbow-triangle and opens a GLFW window with a fixed-step game-loop structure in place. It is a good starting point for building game-style rendering and input systems.

## What is in here

- CMake-based C/C++ project
- GLFW (fetched automatically with CPM during configure)
- GLAD (vendored in vendor/glad)
- Version info generated into config.h
- A basic main loop and input callback wiring in src/main.cpp

## Requirements

- CMake 4.1 or newer
- A C++ compiler toolchain
- OpenGL 4.1-capable drivers/runtime

## Build

From the project root:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Notes:

- Out-of-source builds are required (the project enforces this).
- Valid build types are Debug, Release, RelWithDebInfo, and MinSizeRel.

## Run

Executable name:

- rainbow-triangle

Typical run commands:

```bash
# macOS / Linux
./build/rainbow-triangle

# Windows (Visual Studio generator example)
.\build\Release\rainbow-triangle.exe
```

## Current status

This is currently a scaffold project rather than a finished demo. If you are expecting a rendered rainbow triangle today, that rendering pass has not been added yet.

## License

MIT License. See LICENSE.
