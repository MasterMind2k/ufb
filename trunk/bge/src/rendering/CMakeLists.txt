include_directories(.. ${CMAKE_CURRENT_BINARY_DIR} ${EIGEN2_INCLUDE_DIR} ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR})

set(rendering_SRCS
  renderer.cpp
  stage.cpp
  lightingstage.cpp
  bloomstage.cpp
)

add_library(rendering ${rendering_SRCS})
