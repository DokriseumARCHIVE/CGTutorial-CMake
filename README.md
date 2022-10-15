## Prerequisites
- cmake
- make

## How to clone
Use `git clone --recursive git@github.com:RicoGroth/CGTutorial.git` to clone. Otherwise, the submodules (glew, glfw, glm) won't be cloned.

## How to build
### Linux
Your current directory has to be `CGTutorial/`.  
1. Create the Makefile: `cmake -S . -B build`
2. Use make to build the project: `cd build && make`

You should end up with an executable `CGTutorial/build/CGTutorial`.

### Windows (Visual Studio)
1. Open the folder `CGTutorial` in Visual Studio. It should detect the CMakeLists.txt and build the solution automatically in Debug mode.
2. Choose the target called `CGTutorial.exe`.
---
Inspired by https://github.com/ArthurSonzogni/OpenGL_CMake_Skeleton
