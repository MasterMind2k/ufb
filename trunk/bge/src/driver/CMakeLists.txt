
include_directories(.. ${CMAKE_CURRENT_BINARY_DIR} ${EIGEN2_INCLUDE_DIR} ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR})

set(driver_SRCS
  abstractdriver.cpp
  gl1.cpp
)

set(driver_HDRS
)

add_library(driver ${driver_SRCS} ${driver_HDRS})
