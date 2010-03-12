
include_directories(.. ${CMAKE_CURRENT_BINARY_DIR} ${EIGEN2_INCLUDE_DIR} ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR})

set(storage_SRCS
  storage.cpp
  item.cpp
  mesh.cpp
)

add_library(storage ${storage_SRCS})
