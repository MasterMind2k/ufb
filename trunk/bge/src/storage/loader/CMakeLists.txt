
include_directories(../.. ${CMAKE_CURRENT_BINARY_DIR} ${EIGEN2_INCLUDE_DIR} ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR})

set(loader_SRCS
  modelloader.cpp
  loader3ds.cpp
)

add_library(loader ${loader_SRCS})
