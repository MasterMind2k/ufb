project(UFB)

cmake_minimum_required(VERSION 2.6.0)

find_package(Qt4 REQUIRED)

if(WIN32)
  add_definitions(-DNOMINMAX)
endif(WIN32)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT CMAKE_BUILD_TYPE)

add_subdirectory(bge)
add_subdirectory(src)
