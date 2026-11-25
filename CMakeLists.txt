cmake_minimum_required(VERSION 3.10.0)
project(study VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wno-unknown-warning-option)
endif()

if(MSVC)
    add_compile_options(/utf-8)
endif()

find_package(Eigen3 REQUIRED)

add_executable(main main.cpp)

add_executable(kdtree2 kdtree2.cpp)
target_link_libraries(kdtree2 PRIVATE Eigen3::Eigen)

add_executable(kdtree3 kdtree3.cpp)
target_link_libraries(kdtree3 PRIVATE Eigen3::Eigen)

add_executable(pointcloud_motion_undistort pointcloud_motion_undistort.cpp)
target_link_libraries(pointcloud_motion_undistort PRIVATE Eigen3::Eigen)