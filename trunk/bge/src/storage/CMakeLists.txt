add_subdirectory(loader)

set(storage_SRCS
  manager.cpp
  item.cpp
  mesh.cpp
  texture.cpp
  shader.cpp
  shaderprogram.cpp
  material.cpp
)

add_library(storage ${storage_SRCS})
target_link_libraries(storage loader driver)
add_dependencies(storage loader driver)
