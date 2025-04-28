VisibleSim
==================

## Getting Started
A [__User Manual__](doc/UserManual.md) and a [__Technical Reference__](doc/TechnicalReference.md) are available in the `doc` directory, to get users and contributors started with VisibleSim.

## Installation

Dependencies:
 - GLUT
 - GLEW
 - SBCL - to compile Meld programs
 - MUPARSER - to use CSG

### Compilation
VisibleSim is written in C++17 and therefore requires a compatible compiler such as gcc9 or a recent version of clang.

### Installing Submodules
There are a number of other repositories included as [git submodules] in VisibleSim and used as utilities (e.g. a configuration generator). They need to be fetched from their respective repositories before they can be used. This can be done with the following commands, from the project's root:
```shell
git submodule init
git submodule update
```

### Mac Installation

1\. Install dependencies:
```shell
brew install freeglut
brew install glew
brew install sbcl
brew install muparser
brew install cmake
brew install --cask xquartz

[Optional]
brew install ffmpeg # for video export
brew install sfml # for BlinkyBlocks sound simulation
```
2\. `make`

### Ubuntu Installation

1\. Install dependencies:
```shell
sudo apt-get update
sudo apt-get install cmake
sudo apt-get install build-essential
sudo apt-get install freeglut3-dev
sudo apt-get install libglew-dev
sudo apt-get install sbcl
sudo apt-get install libmuparser-dev

# [Optional]
sudo apt-get install doxygen # for html documentation 
sudo apt-get install ffmpeg # for video export
sudo apt-get install libsfml-dev # for BlinkyBlocks sound simulation
```
2\. Open the project in **VSCode** with the following extensions installed:
    - [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) for managing the build process.
    - [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) for IntelliSense, debugging, and code navigation.

Configure the CMake project by selecting a kit and specifying the build directory. Use the command palette (`Ctrl+Shift+P` or `Cmd+Shift+P` on macOS) to access CMake commands such as `CMake: Configure` and `CMake: Build`.

[git submodules]:https://git-scm.com/book/en/v2/Git-Tools-Submodules

## Windows Installation
For the best experience, use **WSL** (Windows Subsystem for Linux https://learn.microsoft.com/en-us/windows/wsl/install) with Ubuntu. Alternatively, you can use a virtual machine running Linux.