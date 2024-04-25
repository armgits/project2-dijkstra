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
#include <unordered_map>

#include "shader.hpp"
#include "project2.hpp"

project2::OpenList::OpenList()
{}

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
  float dist {};
  for (const auto& line: lines_) {
    if (position.x < 0 + clearance_ || position.x > view_size_.x - clearance_
      || position.y < 0 + clearance_ || position.y > view_size_.y - clearance_) {

      return true;
    }

    dist = -1.F * ((line.x_diff * (position.y - line.y1)) - ((position.x - line.x1) * line.y_diff)) * line.distance_inv;
    if (dist >= clearance_) {
      return false;
    }
  }

  return true;
}

void project2::ObstacleSpace::getCoefficients(const std::vector<unsigned int>& points)
{
  auto points_ {points};

  points_.push_back(*points.begin());
  points_.push_back(*(points.begin() + 1));

  for (auto i {points_.begin()}; i < points_.end() - 2; i += 2) {
    auto x1 {static_cast<float>(*i)};
    auto y1 {static_cast<float>(*(i + 1))};
    auto x2 {static_cast<float>(*(i + 2))};
    auto y2 {static_cast<float>(*(i + 3))};

    lines_.push_back({x1, y1, x2, y2});
  }

}

bool project2::searchDijkstra(
  project2::Node& start_node,
  project2::Node& goal_node,
  std::vector<ObstacleSpace>& obstacles,
  std::deque<TwoDE::vec2ui>& explored_nodes,
  std::deque<TwoDE::vec2ui>& backtracked_path,
  const bool& continue_search,
  bool& search_complete)
{
  project2::OpenList open_list {};
  std::unordered_map<project2::Position, project2::Node> closed_list {};

  open_list.push(start_node);
  bool goal_node_found {false};

  std::cout << '\n' << "Searching..." << '\n';
  auto t_begin {std::chrono::high_resolution_clock::now()};

  while (!open_list.empty() && continue_search) {
    auto current_node {open_list.top()};
    open_list.pop();
    closed_list.insert({current_node.getPosition(), current_node});

    TwoDE::vec2ui current_node_pos {};
    current_node_pos.x = current_node.getPosition().x;
    current_node_pos.y = current_node.getPosition().y;

    explored_nodes.push_back(current_node_pos);

    if (current_node == goal_node) {
      goal_node_found = true;
      goal_node = current_node;
      closed_list.insert({current_node.getPosition(), current_node});
      break;
    }

    project2::Node child_node {};

    for (const auto& action: project2::actions_list) {
      if (!current_node.actionMove(action, child_node))
        continue;

      bool in_obstacle_space {project2::inObstacleSpace(child_node.getPosition(), obstacles)};

      if (in_obstacle_space)
        continue;

      if (closed_list.find(child_node.getPosition()) != closed_list.end())
        continue;

      auto found_open_list_node {open_list.find(child_node)};

      if (found_open_list_node == open_list.end()) {
        open_list.push(child_node);
        continue;
      }

      if (found_open_list_node != open_list.end() && child_node < *found_open_list_node) {
        open_list.erase(found_open_list_node);
        open_list.push(child_node);
      }
    }
  }
  auto t_end {std::chrono::high_resolution_clock::now()};

  // Wrap-up
  std::chrono::duration<float, std::ratio<1L, 1L>> exec_time {t_end - t_begin};

  if (!goal_node_found)
    return false;

  std::cout << '\n' << "-- Goal node found --" << '\n';
  std::cout << goal_node << '\n' << '\n';
  std::cout << "Execution time: " << exec_time.count() << " seconds" << '\n';
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
  const project2::Node& start_node,
  const project2::Node& goal_node,
  const std::unordered_map<project2::Position, project2::Node>& closed_list,
  std::deque<TwoDE::vec2ui>& backtracked_path)
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
