cmake_minimum_required(VERSION 3.9)
project(LinuxOverlay)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release)

add_compile_options(-Wno-multichar -std=c++17 -Wall -Wextra -g)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# load memflow project
include(ExternalProject)
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/rs)
ExternalProject_Add(
    memflow
    GIT_REPOSITORY "https://github.com/memflow/memflow"
    GIT_SHALLOW true
    GIT_PROGRESS true
    GIT_TAG "0.1.5"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cargo build --all-features --workspace
    COMMAND cargo build --release --all-features --workspace
    BINARY_DIR "${CMAKE_BINARY_DIR}/rs/src/memflow"
    INSTALL_COMMAND ""
    LOG_BUILD ON
)

find_package(Threads)
find_package(OpenGL REQUIRED)

file(GLOB SRC_FILES
    "./*.cpp"
    "./imgui/*.cpp"
)

include_directories(memflow)

add_subdirectory(glfw-master)
add_executable(LinuxOverlay
    ${SRC_FILES}
)

add_dependencies(
    LinuxOverlay
    memflow
)

target_link_libraries(LinuxOverlay 
    glfw
    OpenGL::GL
    debug "${CMAKE_BINARY_DIR}/rs/src/memflow/target/debug/libmemflow_ffi.a"
    debug "${CMAKE_BINARY_DIR}/rs/src/memflow/target/debug/libmemflow_win32_ffi.a"
    optimized "${CMAKE_BINARY_DIR}/rs/src/memflow/target/release/libmemflow_win32_ffi.a"
    optimized "${CMAKE_BINARY_DIR}/rs/src/memflow/target/release/libmemflow_ffi.a"
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    m
)

target_include_directories(
    LinuxOverlay PUBLIC

    "${CMAKE_BINARY_DIR}/rs/src/memflow/memflow-ffi"
    "${CMAKE_BINARY_DIR}/rs/src/memflow/memflow-win32-ffi"
)