cmake_minimum_required(VERSION 3.22)
project(Renderer)

# Declarations
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LIBRARIES glfw glew_s)

# Header files directories
include_directories(
    library/glew/include
    library/glfw/include
    library/glm
    library/stb
    src/
)

# Compiler flags
if (NOT MSVC)
    # Compile commands for Linux
    add_compile_options(-Wall -Wpedantic -Wextra -fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

# find OpenGL package in system
find_package(OpenGL REQUIRED)

# Add libraries from submodules
add_subdirectory(library/glfw)
add_subdirectory(library/glew/build/cmake)

# Renderer main app build
add_executable(renderer src/main.cpp)
target_link_libraries(renderer ${LIBRARIES})
target_compile_definitions(renderer PUBLIC GLEW_STATIC)

# ************************* Tests builds *************************
# Test Window
add_executable(window test/WindowTest.cpp)
target_link_libraries(window ${LIBRARIES})
target_compile_definitions(window PUBLIC GLEW_STATIC)