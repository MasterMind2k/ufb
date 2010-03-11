set(QT_USE_QTOPENGL TRUE)

include(${QT_USE_FILE})

include_directories(. ${CMAKE_CURRENT_BINARY_DIR} ${EIGEN2_INCLUDE_DIR} ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(rendering)
add_subdirectory(scene)
add_subdirectory(storage)

set(bge_SRCS
  canvas.cpp
)

set(bge_HDRS
  canvas.h
)

qt4_wrap_cpp(bge_MOC ${bge_HDRS})

add_library(bge ${bge_SRCS} ${bge_MOC})
target_link_libraries(bge scene rendering storage ${QT_LIBRARIES})
add_dependencies(bge rendering storage scene)
