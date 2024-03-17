add_library (glad-opengl4
  src/gl.c
)

target_include_directories(glad-opengl4 PUBLIC
  ./include
)
