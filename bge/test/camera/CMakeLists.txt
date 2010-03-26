set(QT_USE_QTOPENGL TRUE)

include(${QT_USE_FILE})

include_directories(. ../../src ${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDES} ${EIGEN2_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR})

set(bge_camera_SRCS
  main.cpp
  movingcube.cpp
  cameras.cpp
  overlay.cpp
)

set(bge_camera_HDRS
  movingcube.h
  cameras.h
)

qt4_wrap_cpp(bge_camera_MOC ${bge_camera_HDRS})
qt4_add_resources(bge_camera_RC resources.qrc)

add_executable(bge_camera ${bge_camera_SRCS} ${bge_camera_MOC} ${bge_camera_RC})

target_link_libraries(bge_camera bge ${QT_LIBRARIES})

add_dependencies(bge_camera bge)
