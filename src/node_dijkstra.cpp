/**
 * @file node_dijkstra.cpp
 * @author Abhishekh Reddy (areddy42@umd.edu)
 * @brief Implementation of node object and more
 * @version 1.0
 * @date 2024-03-16
 *
 * @copyright Copyright (c) 2024 Abhishekh Reddy
 *
 */

#include "node_dijkstra.hpp"

project2::Node::Node()
: position_ {0, 0},
  from_position_ {0, 0},
  distance_ {0} {}

project2::Node::Node(
  const project2::Position& position
)
: position_ {position},
  from_position_ {},
  distance_ {} {}

project2::Node::Node(
  const project2::Position& position,
  const project2::Position& from_position,
  float distance
)
: position_ {position},
  from_position_ {from_position},
  distance_ {distance} {}

bool project2::Node::actionMove(
  project2::Action action,
  project2::Node& child_node)
{
  float new_distance {distance_};
  project2::Position new_position {position_};

  switch (action)
  {
  case (project2::Action::UP):
      if (new_position.y >= Y_MAX_MM)
        return false;

      new_position.y += ACTION_DISPLACEMENT_MM;
      new_distance += ACTION_COST_STRAIGHT;
      break;

  case (project2::Action::UP_RIGHT):
      if (new_position.y >= Y_MAX_MM || new_position.x >= X_MAX_MM)
        return false;

      new_position.y += ACTION_DISPLACEMENT_MM;
      new_position.x += ACTION_DISPLACEMENT_MM;
      new_distance += ACTION_COST_DIAGONAL;
      break;

  case (project2::Action::RIGHT):
      if (new_position.x >= X_MAX_MM)
        return false;

      new_position.x += ACTION_DISPLACEMENT_MM;
      new_distance += ACTION_COST_STRAIGHT;
      break;

  case (project2::Action::DOWN_RIGHT):
      if (new_position.y <= Y_MIN_MM || new_position.x >= X_MAX_MM)
        return false;

      new_position.y -= ACTION_DISPLACEMENT_MM;
      new_position.x += ACTION_DISPLACEMENT_MM;
      new_distance += ACTION_COST_DIAGONAL;
      break;

  case (project2::Action::DOWN):
      if (new_position.y <= Y_MIN_MM)
        return false;

      new_position.y -= ACTION_DISPLACEMENT_MM;
      new_distance += ACTION_COST_STRAIGHT;
      break;

  case (project2::Action::DOWN_LEFT):
      if (new_position.y <= Y_MIN_MM || new_position.x <= X_MIN_MM)
        return false;

      new_position.y -= ACTION_DISPLACEMENT_MM;
      new_position.x -= ACTION_DISPLACEMENT_MM;
      new_distance += ACTION_COST_DIAGONAL;
      break;

  case (project2::Action::LEFT):
      if (new_position.x <= X_MIN_MM)
        return false;

      new_position.x -= ACTION_DISPLACEMENT_MM;
      new_distance += ACTION_COST_STRAIGHT;
      break;

  case (project2::Action::UP_LEFT):
      if (new_position.y >= Y_MAX_MM || new_position.x <= X_MIN_MM)
        return false;

      new_position.y += ACTION_DISPLACEMENT_MM;
      new_position.x -= ACTION_DISPLACEMENT_MM;
      new_distance += ACTION_COST_DIAGONAL;
      break;

  default:
      return false;
  }

  child_node = project2::Node(new_position, position_, new_distance);
  return true;
}

// std::vector<project2::Node> project2::Node::getChildrenNodes()
// {
//   std::vector<project2::Node> children_nodes {};

//   int index_offset {0};
//   for (const auto& action: actions_list) {
//     if (!actionMove(action, children_nodes))
//       continue;

//     // TODO: Fix a bug at this line
//     children_nodes.back().index_ += index_offset;
//     index_offset++;
//   }

//   return children_nodes;
// }
