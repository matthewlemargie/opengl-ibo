cmake_minimum_required(VERSION 3.10)

# Define the project name and version
project(MyProject VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define source files
file(GLOB_RECURSE SRC_FILES 
    src/*.cpp
    src/OpenGL/*.cpp
)

# Define include directories
include_directories(
    src
    src/OpenGL
)

# Create executable
add_executable(main ${SRC_FILES})

# Link the library (e.g., static or shared)
target_link_libraries(main PRIVATE glfw GLEW GL)
