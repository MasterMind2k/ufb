set(QT_USE_QTOPENGL TRUE)

include(${QT_USE_FILE})

include_directories(. ../bge/src ${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDES} ${EIGEN2_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(states)

set(ufb_SRCS
  main.cpp
)

set(ufb_HDRS
)

#qt4_add_resources(ufb_RC resources.qrc)

qt4_wrap_cpp(ufb_MOC ${ufb_HDRS})
qt4_wrap_ui(ufb_UI ${ufb_UIS})

add_executable(ufb ${ufb_SRCS} ${ufb_MOC} ${ufb_RC})

target_link_libraries(ufb states bge ${QT_LIBRARIES})
add_dependencies(ufb states bge)
