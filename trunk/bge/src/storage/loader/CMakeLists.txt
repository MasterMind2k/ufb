
include_directories(../.. ${CMAKE_CURRENT_BINARY_DIR} ${EIGEN2_INCLUDE_DIR} ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR})

set(loader_SRCS
  abstractloader.cpp
  threeds.cpp
  texture.cpp
  shader.cpp
  obj.cpp
  audio.cpp
)

add_library(loader ${loader_SRCS})
target_link_libraries(loader storage)
