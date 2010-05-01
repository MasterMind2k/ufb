set(QT_USE_QTOPENGL TRUE)

include(${QT_USE_FILE})

include_directories(. ../../src ../../src/3rdparty/bullet/src ${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDES} ${EIGEN2_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR})

set(bge_multilighting_SRCS
  main.cpp
  lighting.cpp
  controller.cpp
  camera.cpp
  overlay.cpp
)

set(bge_multilighting_HDRS
  lighting.h
)

qt4_wrap_cpp(bge_multilighting_MOC ${bge_multilighting_HDRS})
qt4_add_resources(bge_multilighting_RC resources.qrc)

add_executable(bge_multilighting ${bge_multilighting_SRCS}
  ${bge_multilighting_MOC} ${bge_multilighting_RC})

target_link_libraries(bge_multilighting bge ${QT_LIBRARIES})
add_dependencies(bge_multilighting bge)

