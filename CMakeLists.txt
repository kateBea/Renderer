cmake_minimum_required(VERSION 3.22)
project(Renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        library/third_party/glew/include
        library/third_party/glfw/include
        library/third_party/soil/inc
        library/third_party/glm
        library/kate
)

add_subdirectory(library/third_party/glew/build/cmake)
add_subdirectory(library/third_party/glfw)

add_executable(renderer main.cpp)

target_link_libraries(renderer glfw glew_s)
