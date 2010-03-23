
include_directories(.. ${CMAKE_CURRENT_BINARY_DIR} ${EIGEN2_INCLUDE_DIR} ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(loader)

set(storage_SRCS
  storagemanager.cpp
  item.cpp
  mesh.cpp
  texture.cpp
  shader.cpp
  shaderprogram.cpp
)

add_library(storage ${storage_SRCS})
target_link_libraries(storage loader)
add_dependencies(storage loader)
