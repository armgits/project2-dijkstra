add_library (project2d-engine
  src/shapes.cpp
)

target_include_directories(project2d-engine PUBLIC
  ./include
)

target_link_libraries(project2d-engine PUBLIC
  glad-opengl4
  # imgui-opengl3
)
