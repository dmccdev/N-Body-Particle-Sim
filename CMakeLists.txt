cmake_minimum_required(VERSION 3.20)

project(particlesim LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
include(FetchContent)

# Raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)

FetchContent_MakeAvailable(raylib)

# Eigen (header-only)
FetchContent_Declare(
    Eigen3
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3.4.0
    GIT_SHALLOW TRUE
    SOURCE_SUBDIR cmake-not-used
)

FetchContent_MakeAvailable(Eigen3)

add_library(Eigen3::Eigen INTERFACE IMPORTED GLOBAL)

set_target_properties(Eigen3::Eigen PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${eigen3_SOURCE_DIR}"
)

# OpenMP (required)
find_package(OpenMP REQUIRED)

# Executable
add_executable(particlesim
    src/main.cpp

    # User interface
    src/userinterface/ui.hpp
    src/userinterface/ui.cpp
    src/userinterface/button.cpp
    src/userinterface/button.hpp

    # Statistics
    src/statistics/statistics.cpp
    src/statistics/statistics.hpp

    # Pairwise algorithm
    src/pairwise_algorithm/pairwise.hpp
    src/pairwise_algorithm/pairwise.cpp

    # Particle state
    src/particlestate/ParticlesState.hpp
    src/particlestate/ParticlesState.cpp

    # Barnes-Hut algorithm
    src/barneshut_algorithm/barneshut.hpp
    src/barneshut_algorithm/barneshut.cpp
    src/barneshut_algorithm/node.hpp
    src/barneshut_algorithm/node.cpp
    src/barneshut_algorithm/quadtree.cpp
    src/barneshut_algorithm/quadtree.hpp

    # Integrators
    src/integrator/Verlet.cpp
    src/integrator/Verlet.hpp
    src/integrator/RK4.cpp
    src/integrator/RK4.hpp
    src/integrator/Euler.cpp
    src/integrator/Euler.hpp
)

# Include directories
target_include_directories(particlesim PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(particlesim PRIVATE
    raylib
    Eigen3::Eigen
    OpenMP::OpenMP_CXX
)

# OpenMP compile definitions
target_compile_definitions(particlesim PRIVATE USE_OPENMP)