cmake_minimum_required(VERSION 3.10)

project(Partycles)

set(TARGET party)
add_executable(${TARGET}
  party_main.cpp
)
target_link_libraries(${TARGET}
  raylib
)
