
set(assets_SRCS
  menuoverlay.cpp
  menucontroller.cpp
  controller.cpp
  hud.cpp
)

add_library(assets STATIC ${assets_SRCS})
target_link_libraries(assets states objects)
