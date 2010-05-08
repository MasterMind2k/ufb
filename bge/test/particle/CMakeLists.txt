set(QT_USE_QTOPENGL TRUE)

include(${QT_USE_FILE})

include_directories(. ../../src ${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDES} ${EIGEN2_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR})

set(bge_particle_SRCS
  main.cpp
  explosions.cpp
  explosion.cpp
)

set(bge_particle_HDRS
  explosions.h
)

qt4_wrap_cpp(bge_particle_MOC ${bge_particle_HDRS})

add_executable(bge_particle ${bge_particle_SRCS} ${bge_particle_MOC})

target_link_libraries(bge_particle bge ${QT_LIBRARIES})
add_dependencies(bge_particle bge)

