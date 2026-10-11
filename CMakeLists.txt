cmake_minimum_required(VERSION 3.20)

project(particlesim LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
)

FetchContent_Declare(
    eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3.4.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(raylib)

FetchContent_GetProperties(eigen)

if(NOT eigen_POPULATED)
    FetchContent_Populate(eigen)
endif()

add_library(eigen_headers INTERFACE)
add_library(Eigen3::Eigen ALIAS eigen_headers)

target_include_directories(eigen_headers SYSTEM INTERFACE
    "${eigen_SOURCE_DIR}"
)

add_executable(particlesim
    src/main.cpp
    src/userinterface/ui.cpp
    src/userinterface/ui.hpp
    src/userinterface/button.cpp
    src/userinterface/button.hpp
    src/statistics/statistics.cpp
    src/statistics/statistics.hpp
    src/particlestate/ParticlesState.cpp
    src/particlestate/ParticlesState.hpp
    src/pairwise_algorithm/pairwise.cpp
    src/pairwise_algorithm/pairwise.hpp
    src/barneshut_algorithm/barneshut.cpp
    src/barneshut_algorithm/barneshut.hpp
    src/barneshut_algorithm/node.cpp
    src/barneshut_algorithm/node.hpp
    src/barneshut_algorithm/quadtree.cpp
    src/barneshut_algorithm/quadtree.hpp
    src/integrator/Euler.cpp
    src/integrator/Euler.hpp
    src/integrator/Verlet.cpp
    src/integrator/Verlet.hpp
    src/integrator/RK4.cpp
    src/integrator/RK4.hpp
)

target_include_directories(particlesim PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(particlesim PRIVATE
    raylib
    Eigen3::Eigen
)

find_package(OpenMP QUIET COMPONENTS CXX)

if(OpenMP_CXX_FOUND)
    target_link_libraries(particlesim PRIVATE OpenMP::OpenMP_CXX)
    target_compile_definitions(particlesim PRIVATE USE_OPENMP)
endif()

add_custom_command(TARGET particlesim POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory
        "$<TARGET_FILE_DIR:particlesim>/assets"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/src/assets"
        "$<TARGET_FILE_DIR:particlesim>/assets"
    VERBATIM
)