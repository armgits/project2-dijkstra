cmake_minimum_required(VERSION 3.11)

project(project2)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(libs)


set(executable_list
  src/node_dijkstra.cpp
  src/project2.cpp
  src/main.cpp
)

add_executable(project2 ${executable_list})

target_include_directories(project2 PUBLIC
  ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(project2 PUBLIC
  glad-opengl4
  project2d-engine
  # imgui-opengl3
  glfw
  OpenGL
)
