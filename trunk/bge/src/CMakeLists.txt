set(QT_USE_QTOPENGL TRUE)

include(${QT_USE_FILE})

include_directories(. 3rdparty/bullet/src ${CMAKE_CURRENT_BINARY_DIR} ${EIGEN2_INCLUDE_DIR} ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(driver)
add_subdirectory(rendering)
add_subdirectory(scene)
add_subdirectory(storage)
add_subdirectory(3rdparty)

set(bge_SRCS
  canvas.cpp
  recorder.cpp
  abstractoverlay.cpp
  motionstate.cpp
  gamestate.cpp
)

set(bge_QHDRS
  canvas.h
  recorder.h
)

set(bge_HDRS
  global.h
  abstractcontroller.h
)

qt4_wrap_cpp(bge_MOC ${bge_QHDRS})
qt4_add_resources(bge_RC bge_resources.qrc)

add_library(bge ${bge_SRCS} ${bge_MOC} ${bge_HDRS} ${bge_RC})
target_link_libraries(bge scene rendering driver storage BulletDynamics BulletCollision LinearMath ${QT_LIBRARIES})
add_dependencies(bge LinearMath BulletDynamics BulletCollision rendering driver storage scene)
