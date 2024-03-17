/**
 * @file shader.hpp
 * @author Abhishekh Reddy (areddy42@umd.edu)
 * @brief Simple library for parsing GLSL shaders
 * @version 0.1
 * @date 2024-03-16
 *
 * @copyright Copyright (c) 2024 Abhishekh Reddy
 *
 */
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glad/gl.h>

namespace TwoDE {

std::string parseShader(const std::string& filename)
{
  std::ifstream shader_stream {filename};
  std::stringstream parsed_stream;

  std::string shader_line {};
  while (std::getline(shader_stream, shader_line)) {
    parsed_stream << shader_line << '\n';
  }

  return parsed_stream.str();
}

unsigned int createShader(const std::string& shader_src_str, int shader_type)
{
  unsigned int shader {glCreateShader(shader_type)};
  const char * shader_src {shader_src_str.c_str()};
  glShaderSource(shader, 1, &shader_src, nullptr);
  glCompileShader(shader);

  // // TODO: Implement error handling
  // // Debug
  // int success {};
  // glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  // if(!success)
  // {
  //     GLchar infoLog[512];
  //     glGetShaderInfoLog(shader, 512, NULL, infoLog);
  //     std::string infoLogstr {infoLog};
  //     std::cout << infoLogstr << std::endl;
  // }

  return shader;
}

}
