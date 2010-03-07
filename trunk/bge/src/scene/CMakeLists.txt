
include_directories(.. ${CMAKE_CURRENT_BINARY_DIR} ${EIGEN2_INCLUDE_DIR} ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR})

set(scene_SRCS
  sceneobject.cpp
)

#qt4_wrap_cpp(scene_MOC ${scene_HDRS})

add_library(scene ${scene_SRCS})
