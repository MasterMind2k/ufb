
add_subdirectory(assets)

set(states_SRCS
  menu.cpp
  game.cpp
)

add_library(states STATIC ${states_SRCS})
target_link_libraries(states assets)
