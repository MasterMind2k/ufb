
include_directories(.. ${CMAKE_CURRENT_BINARY_DIR} ${EIGEN2_INCLUDE_DIR} ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR})

set(rendering_SRCS
  directrenderer.cpp
  renderer.cpp
  shadermanager.cpp
)

set(rendering_HDRS
  shader_p.h
)

#qt4_wrap_cpp(rendering_MOC ${rendering_QHDRS})

add_library(rendering ${rendering_SRCS} ${rendering_HDRS})
