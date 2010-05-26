set(util_SRCS
  ai.cpp
  objectlist.cpp
)

add_library(util STATIC ${util_SRCS})
target_link_libraries(util objects)
