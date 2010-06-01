set(objects_SRCS
  object.cpp
  fighter.cpp
  exhaust.cpp
  asteroid.cpp
  bullet.cpp
  explosion.cpp
  powerup.cpp
)

add_library(objects ${objects_SRCS})
target_link_libraries(objects util)
