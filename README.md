# ECS Particle System Benchmark

This project demonstrates and benchmarks two approaches to implementing a particle system in C++:  
- An **Entity Component System (ECS)** using the [EnTT](https://github.com/skypjack/entt) library  
- A **simple array-based implementation**  

Both systems use [Raylib](https://www.raylib.com/) for graphics rendering. The primary goal is to compare performance and code structure between ECS and traditional approaches.

---

## Project Structure

- `ECSParticleSystem/` — Source code for both ECS and array-based particle systems
- Uses **C++20** standard
- Relies on **Raylib** for rendering and **EnTT** for ECS

---

## Getting Started

### Windows (Visual Studio 2022)

This project is set up for Visual Studio 2022.  
Follow these steps to set up Raylib and dependencies:

#### 1. Download Raylib

- Go to the [Raylib releases page](https://github.com/raysan5/raylib/releases)
- Download the latest **raylib-<version>_win64_msvc16.zip** (or similar)
- Extract the contents

#### 2. Add Raylib to Your Project

- Copy the `include` and `lib` folders from the Raylib package into your project directory (or reference them from their location)
- In Visual Studio:
  - Right-click your project > __Properties__
    - Under __C/C++ > General > Additional Include Directories__, add the path to Raylib's `include` folder
    - Under __Linker > General > Additional Library Directories__, add the path to Raylib's `lib` folder
    - Under __Linker > Input > Additional Dependencies__, add:    ```
      raylib.lib
  ```    - (Optional) Add `winmm.lib`, `gdi32.lib`, and `opengl32.lib` if required

#### 3. Add EnTT

- Download [EnTT](https://github.com/skypjack/entt) (header-only)
- Add the `single_include` or `src` directory to your project's include paths

#### 4. Build and Run

- Open the solution in Visual Studio 2022
- Build and run the project

---

### Other Platforms (CMake)

While this repository is tailored for Visual Studio, you can use CMake to build on other platforms:

1. **Install Raylib**  
   - Follow the [Raylib CMake instructions](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux#using-cmake) for your OS

2. **Install EnTT**  
   - Download and add the EnTT headers to your include path

3. **CMake Example**

---

## About

This project is designed to:
- Benchmark ECS (via EnTT) vs. a simple array-based particle system
- Visualize thousands of particles using Raylib
- Serve as a reference for integrating Raylib and EnTT in C++ projects

---

## License

This project uses open-source libraries:
- [Raylib](https://github.com/raysan5/raylib) (zlib/libpng license)
- [EnTT](https://github.com/skypjack/entt) (MIT license)

See individual library repositories for details.