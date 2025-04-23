# Setker

## Description
Setker is a C++ project that uses CMake as its build system. The project includes several modules organized into folders such as `commands` and `def`, with a main entry point in `main.cpp`.

## Project Structure
The main structure of the project is as follows:

```
src/
  main.cpp
  commands/
  def/
```

Additionally, the project generates a `build/` directory containing CMake-generated files and compiled binaries.

## Prerequisites
- A C++ compiler (e.g., GCC or Clang).
- CMake installed on your system.

## Build Instructions
1. Create a `build` directory in the project root if it doesn't exist:
   ```bash
   mkdir -p build
   ```
2. Navigate to the `build` directory:
   ```bash
   cd build
   ```
3. Run CMake to generate the build files:
   ```bash
   cmake ..
   ```
4. Compile the project:
   ```bash
   make
   ```

## Execution
After compilation, the generated binary will be available in the `build/` directory. You can run it with:
```bash
./Setker
```

## Contributions
If you wish to contribute to this project, please follow these steps:
1. Fork the repository.
2. Create a branch for your feature or bug fix.
3. Make your changes and commit them with a clear message.
4. Submit a pull request.

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.