/**
 * @file node_dijkstra.hpp
 * @author Abhishekh Reddy (areddy42@umd.edu)
 * @brief Declaration of the node object and its behavior
 * @version 0.1
 * @date 2024-03-16
 *
 * @copyright Copyright (c) 2024 Abhishekh Reddy
 *
 */
#pragma once

#include <iostream>
#include <vector>
#include <array>

#define X_MIN_MM 0
#define X_MAX_MM 1200
#define Y_MIN_MM 0
#define Y_MAX_MM 500

#define ACTION_DISPLACEMENT_MM 1
#define ACTION_COST_STRAIGHT 1.0
#define ACTION_COST_DIAGONAL 1.4

namespace project2 {
enum class Action {
  UP = 1,
  UP_RIGHT = 2,
  RIGHT = 3,
  DOWN_RIGHT = 4,
  DOWN = 5,
  DOWN_LEFT = 6,
  LEFT = 7,
  UP_LEFT = 8
};

const std::array<Action, 8> actions_list {
  Action::UP,
  Action::UP_RIGHT,
  Action::RIGHT,
  Action::DOWN_RIGHT,
  Action::DOWN,
  Action::DOWN_LEFT,
  Action::LEFT,
  Action::UP_LEFT
};

struct Position {
  Position()
  : x {0}, y {0} {}

  Position(unsigned int x_val, unsigned int y_val)
  : x {x_val}, y {y_val} {}

  Position(const Position& position_val)
  : x {position_val.x},
    y {position_val.y} {}

  bool operator==(const Position& _position) const
  {
    return (x == _position.x && y == _position.y);
  }

  bool operator!=(const Position& _position) const
  {
    return (x != _position.x && y != _position.y);
  }

  friend std::ostream& operator<<(std::ostream& os, const Position& position)
  {
    os << "x: " << position.x << ", y: " << position.y;

    return os;
  }

  unsigned int x {0};
  unsigned int y {0};
};

class Node
{
  public:
    Node();

    explicit Node(
      const Position& position
    );

    explicit Node(
      const Position& position,
      const Position& from_position,
      float distance);

    void setDistance(float new_distance) {distance_ = new_distance;}
    bool actionMove(Action action, Node& child_node);

    const project2::Position& getPosition() const {return position_;}
    const float getDistance() const {return distance_;}
    const project2::Position& getFromPosition() const {return from_position_;}

    bool operator==(const Node& compare_node) const
    {
      return (position_ == compare_node.position_);
    }

    bool operator!=(const Node& compare_node) const
    {
      return (position_ != compare_node.position_);
    }

    bool operator>(const Node& compare_node) const
    {
      return (distance_ > compare_node.distance_);
    }

    bool operator>=(const Node& compare_node) const
    {
      return (distance_ >= compare_node.distance_);
    }

    bool operator<(const Node& compare_node) const
    {
      return (distance_ < compare_node.distance_);
    }

    bool operator<=(const Node& compare_node) const
    {
      return (distance_ <= compare_node.distance_);
    }

    friend std::ostream& operator<<(std::ostream& os, const Node& node)
    {
      os << "Node " << node.position_ << " ---" << std::endl;
      os << "From: " << node.from_position_ << std::endl;
      os << "Distance: " << node.distance_ << std::endl;

      return os;
    }

  private:
    float distance_;
    Position from_position_;
    Position position_;
};
}
