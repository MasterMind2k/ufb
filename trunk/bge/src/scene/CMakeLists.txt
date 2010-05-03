
include_directories(.. ${CMAKE_CURRENT_BINARY_DIR} ${EIGEN2_INCLUDE_DIR} ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR})

set(scene_SRCS
  object.cpp
  camera.cpp
  light.cpp
  particleemitter.cpp
  partition.cpp
  boundingvolume.cpp
)

add_library(scene ${scene_SRCS})
target_link_libraries(scene bge)
add_dependencies(scene bge)
