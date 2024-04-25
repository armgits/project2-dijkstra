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

struct TwoPoints {

  float x1;
  float y1;
  float x2;
  float y2;

  float x_diff;
  float y_diff;
  float distance;
  float distance_inv;

  TwoPoints()
  : x1 {0.F}, y1 {0.F}, x2 {0.F}, y2 {0.F}, x_diff {0.F},
    y_diff {0.F}, distance {0.F}
  {}

  TwoPoints(float x1, float y1, float x2, float y2)
  : x1 {x1}, y1 {y1}, x2 {x2}, y2 {y2}, x_diff {x2 - x1},
    y_diff {y2 - y1}
  {
    // float dir { (x1 < x2) || (y1 < y2) ? -1.F : 1.F };
    // float dir {1.F};
    distance = std::sqrt( std::pow(x_diff, 2) + std::pow(y_diff, 2) );
    distance_inv = 1 / distance;
  }

};

class OpenList : public std::priority_queue<project2::Node,
                                             std::vector<project2::Node>,
                                             std::greater<project2::Node>>
{
  public:
    OpenList();

    auto find(const project2::Node& node) {return std::find(c.begin(), c.end(), node);}
    void erase(const std::vector<project2::Node>::iterator& node) {c.erase(node);}
    auto end() {return c.end();}
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
    std::vector<project2::TwoPoints> lines_;
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
