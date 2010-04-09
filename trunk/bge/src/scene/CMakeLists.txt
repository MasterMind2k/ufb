
include_directories(.. ${CMAKE_CURRENT_BINARY_DIR} ${EIGEN2_INCLUDE_DIR} ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR})

set(scene_SRCS
  object.cpp
  camera.cpp
  light.cpp
  partition.cpp
)

add_library(scene ${scene_SRCS})
