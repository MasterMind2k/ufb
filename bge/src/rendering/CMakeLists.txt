
include_directories(.. ${CMAKE_CURRENT_BINARY_DIR} ${EIGEN2_INCLUDE_DIR} ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR})

set(rendering_SRCS
  renderer.cpp
  shadermanager.cpp
  buffermanager.cpp
  vbomanager.cpp
)

set(rendering_HDRS
  shader_p.h
  buffer_p.h
)

#qt4_wrap_cpp(rendering_MOC ${rendering_QHDRS})

add_library(rendering ${rendering_SRCS} ${rendering_HDRS})
