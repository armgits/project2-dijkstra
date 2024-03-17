#version 330 core
#extension GL_ARB_separate_shader_objects : require

layout(location = 0) in vec4 vertex_position;
layout(location = 1) in vec4 vertex_color;
out vec4 fragment_color;

void main()
{
  gl_PointSize = 3;
  fragment_color = vertex_color;
  gl_Position = vertex_position;
};
