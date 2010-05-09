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

# This is a ugly hack. Don't know how to fix this :(
IF(CMAKE_BUILD_TYPE STREQUAL Debug)
  add_definitions(-DEIGEN_DONT_ALIGN)
ENDIF(CMAKE_BUILD_TYPE STREQUAL Debug)

add_subdirectory(src)
add_subdirectory(test)
