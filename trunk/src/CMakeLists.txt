set(QT_USE_QTOPENGL TRUE)

include(${QT_USE_FILE})

include_directories(../bge/src ../bge/src/3rdparty/bullet/src ${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDES} ${EIGEN2_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(objects)
add_subdirectory(states)
add_subdirectory(util)

set(ufb_SRCS
  main.cpp
)

set(ufb_HDRS
)

qt4_add_resources(ufb_RC internal.qrc)

qt4_wrap_cpp(ufb_MOC ${ufb_HDRS})
qt4_wrap_ui(ufb_UI ${ufb_UIS})

add_custom_target(resources
                   rcc -binary resources.qrc -o ${CMAKE_CURRENT_BINARY_DIR}/resources.rcc
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(ufb ${ufb_SRCS} ${ufb_MOC} ${ufb_RC})

target_link_libraries(ufb states bge ${QT_LIBRARIES})
add_dependencies(ufb states bge resources)
