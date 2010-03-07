project(BGE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

cmake_minimum_required(VERSION 2.6.0)

find_package(Qt4 REQUIRED)
find_package(Eigen2 REQUIRED)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT CMAKE_BUILD_TYPE)

add_subdirectory(src)
