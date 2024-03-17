/**
 * @file project2.cpp
 * @author Abhishekh Reddy (areddy42@umd.edu)
 * @brief Definitions for some helper functions used for Project 2
 * @version 0.1
 * @date 2024-03-16
 *
 * @copyright Copyright (c) 2024 Abhishekh Reddy
 *
 */

#include <cmath>

#include "shader.hpp"
#include "project2.hpp"

project2::OpenList::OpenList()
: std::priority_queue<project2::Node, std::vector<project2::Node>, std::greater<project2::Node>> {}
{}

void project2::OpenList::addNode(project2::Node& node)
{
  auto found_node {std::find(c.begin(), c.end(), node)};

  if (found_node != c.end() && *found_node <= node)
    return;

  push(node);
}

project2::ObstacleSpace::ObstacleSpace(
  const std::vector<unsigned int>& points,
  unsigned int clearance,
  const TwoDE::vec2ui& view_size)
: clearance_ {clearance},
  view_size_ {view_size}
{
  getCoefficients(points);
}

bool project2::ObstacleSpace::containsPoint(const Position& position)
{
  for (auto i {coeffients_.begin()}; i < coeffients_.end() - 3; i += 4) {
    float a {*i};
    float b {*(i + 1)};
    float c {*(i + 2)};
    float dir {*(i + 3)};

    float dist = dir * (a * position.x + b * position.y + c) / std::sqrt(std::pow(a, 2) + std::pow(b, 2));

    if (dist >= clearance_)
      return false;
  }

  return true;
}

void project2::ObstacleSpace::getCoefficients(const std::vector<unsigned int>& points)
{
  auto points_ {points};
  points_.push_back(*points.begin());
  points_.push_back(*(points.begin() + 1));

  for (auto i {points_.begin()}; i < points_.end() - 2; i += 2) {
    unsigned int x1 {*i};
    unsigned int y1 {*(i + 1)};
    unsigned int x2 {*(i + 2)};
    unsigned int y2 {*(i + 3)};

    float a {0.F};
    float b {-1.F};
    float c {0.F};
    float dir {1.F};

    if (x2 - x1 != 0) {
      a = (y2 - y1) / (x2 - x1);
    }
    else {
      a = 1.F;
      b = 0.F;
    }

    c = -1.F * (a * x1 + b * y1);

    if ((x1 == view_size_.x && x2 == view_size_.x) || (y1 == view_size_.y && y2 == view_size_.y) || (x1 > x2) || (y1 > y2))
      dir = -1.F;

    coeffients_.push_back(a);
    coeffients_.push_back(b);
    coeffients_.push_back(c);
    coeffients_.push_back(dir);
  }
}

bool project2::searchDijkstra(
  project2::Node& start_node,
  project2::Node& goal_node,
  std::vector<ObstacleSpace>& obstacles,
  std::deque<TwoDE::vec2ui>& position_buffer,
  const bool& continue_search,
  bool& search_complete)
{
  project2::OpenList open_list {};
  std::vector<project2::Node> closed_list {};

  open_list.addNode(start_node);
  bool goal_node_found {false};

  std::cout << std::endl << "Searching..." << std::endl;
  auto t_begin {std::chrono::high_resolution_clock::now()};

  while (!open_list.isEmpty() && continue_search) {
    auto current_node {open_list.getNode()};
    closed_list.push_back(current_node);

    // std::cout << "Exploring---- " << std::endl;
    // std::cout << current_node << std::endl;

    TwoDE::vec2ui current_node_pos {};
    current_node_pos.x = current_node.getPosition().x;
    current_node_pos.y = current_node.getPosition().y;

    position_buffer.push_back(current_node_pos);

    if (current_node == goal_node) {
      goal_node_found = true;
      goal_node = current_node;
      closed_list.push_back(current_node);
      break;
    }

    project2::Node child_node {};

    for (const auto& action: project2::actions_list) {
      if (!current_node.actionMove(action, child_node))
        continue;

      if (std::find(closed_list.begin(), closed_list.end(), child_node) != closed_list.end())
        continue;

      bool in_obstacle_space {project2::inObstacleSpace(child_node.getPosition(), obstacles)};

      if (in_obstacle_space)
        continue;

      open_list.addNode(child_node);
    }
  }
  auto t_end {std::chrono::high_resolution_clock::now()};

  // Wrap-up
  std::chrono::duration<float, std::ratio<1L, 1L>> exec_time {t_end - t_begin};

  if (!goal_node_found)
    return false;

  std::cout << "-- Goal node found --" << std::endl;
  std::cout << goal_node << std::endl << std::endl;
  std::cout << "Execution time: " << exec_time.count() << " seconds" << std::endl;
  backtrackPath(closed_list, position_buffer);
  search_complete = true;

  return true;
}

bool project2::inObstacleSpace(
  const project2::Position& point,
  std::vector<project2::ObstacleSpace>& obstacles_space)
{
  for (auto& obstacle_space: obstacles_space) {
    if (!obstacle_space.containsPoint(point))
      continue;

    return true;
  }

  return false;
}

void project2::backtrackPath(
  const std::vector<project2::Node>& closed_list,
  std::deque<TwoDE::vec2ui>& position_buffer)
{
  position_buffer.clear();
  auto next_position {closed_list.rbegin()->getFromPosition()};
  
  position_buffer.push_front(
    {closed_list.rbegin()->getPosition().x,
    closed_list.rbegin()->getPosition().y});

  for (auto i {closed_list.rbegin()}; i < closed_list.rend(); i++) {
    if (i->getPosition() != next_position)
      continue;

    position_buffer.push_front(
      {i->getPosition().x,
      i->getPosition().y});

    next_position = i->getFromPosition();
  }
}

unsigned int project2::initShader()
{
  auto vertex_shader_source {TwoDE::parseShader("../libs/project2d-engine/shaders/vertex.shader")};
  auto fragment_shader_source {TwoDE::parseShader("../libs/project2d-engine/shaders/fragment.shader")};

  auto vertex_shader {TwoDE::createShader(vertex_shader_source, GL_VERTEX_SHADER)};
  auto fragment_shader {TwoDE::createShader(fragment_shader_source, GL_FRAGMENT_SHADER)};

  unsigned int gl_program {glCreateProgram()};
  glAttachShader(gl_program, vertex_shader);
  glAttachShader(gl_program, fragment_shader);
  glLinkProgram(gl_program);

  // // Shader Debug
  // int success {};
  // glGetShaderiv(gl_program, GL_LINK_STATUS, &success);
  // if(!success)
  // {
  //     GLchar infoLog[512];
  //     glGetShaderInfoLog(gl_program, 512, NULL, infoLog);
  //     std::string infoLogstr {infoLog};
  //     std::cout << infoLogstr << std::endl;
  // }

  glValidateProgram(gl_program);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  glUseProgram(gl_program);

  return gl_program;
}
