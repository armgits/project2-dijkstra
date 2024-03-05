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
class NodeDijkstra
{
  public:
    NodeDijkstra();

    explicit NodeDijkstra(
      int index,
      int parent_index,
      float cost_to_come,
      const Position& position);

    std::vector<NodeDijkstra> getChildrenNodes();
    void setIndex(int index_val) {index_ = index_val;}

  private:
    void actionMove(Action action, std::vector<NodeDijkstra>& children_nodes);

    int index_;
    int parent_index_;
    float cost_to_come_;
    Position position_;
};

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
  Action::UP_LEFT};

struct Position {
  Position(int x_val, int y_val)
  : x {x_val}, y {y_val} {}

  Position(const Position& position_val)
  : x {position_val.x},
    y {position_val.y} {}

  bool operator==(const Position& _position)
  {
    return (x == _position.x && y == _position.y);
  }

  bool operator!=(const Position& _position)
  {
    return (x != _position.x && y != _position.y);
  }

  int x {0};
  int y {0};
};
}
