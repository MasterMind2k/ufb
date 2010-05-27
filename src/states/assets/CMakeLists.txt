set(assets_SRCS
  menuoverlay.cpp
  menucontroller.cpp
  controller.cpp
  hud.cpp
  statehandler.cpp
)

set(assets_HDRS
  controller.h
)

qt4_wrap_cpp(assets_MOCS ${assets_HDRS})

add_library(assets STATIC ${assets_SRCS} ${assets_MOCS})
target_link_libraries(assets states objects util)
