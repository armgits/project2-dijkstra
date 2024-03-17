#version 330 core
#extension GL_ARB_separate_shader_objects : require

in vec4 fragment_color;
out vec4 color;

void main()
{
  // color = vec4(0.0, 1.0, 0.0, 1.0);
  color = fragment_color;
}
