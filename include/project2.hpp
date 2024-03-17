/**
 * @file project2.hpp
 * @author Abhishekh Reddy (areddy42@umd.edu)
 * @brief Declaration of helper functions for project 2
 * @version 0.1
 * @date 2024-03-16
 *
 * @copyright Copyright (c) 2024 Abhishekh Reddy
 *
 */

#pragma once

#include <algorithm>
#include <queue>
#include <functional>
#include <chrono>

#include "shapes.hpp"
#include "node_dijkstra.hpp"

namespace project2 {

class OpenList : private std::priority_queue<project2::Node,
                                             std::vector<project2::Node>,
                                             std::greater<project2::Node>>
{
  public:
    OpenList();
    void addNode(project2::Node& node);

    bool isEmpty() {return empty();}
    project2::Node getNode()
    {
      auto node {top()};
      pop();
      return node;
    }
};

class ObstacleSpace
{
  public:
    ObstacleSpace(
      const std::vector<unsigned int>& points,
      unsigned int clearance,
      const TwoDE::vec2ui& view_size);

    bool containsPoint(const Position& position);

  private:
    void getCoefficients(const std::vector<unsigned int>& points);
    std::vector<float> coeffients_;
    unsigned int clearance_;
    TwoDE::vec2ui view_size_;
};

void initializeGLFW();
void initializeGL();

bool searchDijkstra(
  Node& start_node,
  Node& goal_node,
  std::vector<ObstacleSpace>& obstacles,
  std::deque<TwoDE::vec2ui>& position_buffer,
  const bool& continue_search,
  bool& search_complete);

bool inObstacleSpace(
  const Position& point,
  std::vector<ObstacleSpace>& obstacles_space);

void backtrackPath(
  const std::vector<project2::Node>& closed_list,
  std::deque<TwoDE::vec2ui>& position_buffer);

unsigned int initShader();

}
