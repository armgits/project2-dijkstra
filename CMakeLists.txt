cmake_minimum_required(VERSION 3.10)

project(project2)

set(executable_list
  src/node_dijkstra.cpp
  src/main.cpp
)

add_executable(project2 ${executable_list})
target_include_directories(project2 PUBLIC
  ${CMAKE_SOURCE_DIR}/include
)
