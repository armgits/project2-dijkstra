#include "node_dijkstra.hpp"

project2::NodeDijkstra::NodeDijkstra()
: index_ {0},
  parent_index_ {0},
  cost_to_come_ {0},
  position_ {0, 0} {}

project2::NodeDijkstra::NodeDijkstra(
  int index,
  int parent_index,
  float cost_to_come,
  const Position& position
)
: index_ {index},
  parent_index_ {parent_index},
  cost_to_come_ {cost_to_come},
  position_ {position} {}

void project2::NodeDijkstra::actionMove(
  Action action,
  std::vector<NodeDijkstra>& children_nodes)
{
  float new_cost_to_come {cost_to_come_};
  Position new_position {position_};

  switch (action)
  {
  case (Action::UP):
      if (new_position.y >= Y_MAX_MM)
        return;

      new_position.y += ACTION_DISPLACEMENT_MM;
      new_cost_to_come += ACTION_COST_STRAIGHT;
      break;

  case (Action::UP_RIGHT):
      if (new_position.y >= Y_MAX_MM || new_position.x >= X_MAX_MM)
        return;

      new_position.y += ACTION_DISPLACEMENT_MM;
      new_position.x += ACTION_DISPLACEMENT_MM;
      new_cost_to_come += ACTION_COST_DIAGONAL;
      break;

  case (Action::RIGHT):
      if (new_position.x >= X_MAX_MM)
        return;

      new_position.x += ACTION_DISPLACEMENT_MM;
      new_cost_to_come += ACTION_COST_STRAIGHT;
      break;

  case (Action::DOWN_RIGHT):
      if (new_position.y <= Y_MIN_MM || new_position.x >= X_MAX_MM)
        return;

      new_position.y -= ACTION_DISPLACEMENT_MM;
      new_position.x += ACTION_DISPLACEMENT_MM;
      new_cost_to_come += ACTION_COST_DIAGONAL;
      break;

  case (Action::DOWN):
      if (new_position.y <= Y_MIN_MM)
        return;

      new_position.y -= ACTION_DISPLACEMENT_MM;
      new_cost_to_come += ACTION_COST_STRAIGHT;
      break;

  case (Action::DOWN_LEFT):
      if (new_position.y <= Y_MIN_MM || new_position.x <= X_MIN_MM)
        return;

      new_position.y -= ACTION_DISPLACEMENT_MM;
      new_position.y -= ACTION_DISPLACEMENT_MM;
      new_cost_to_come += ACTION_COST_DIAGONAL;
      break;

  case (Action::LEFT):
      if (new_position.x <= X_MIN_MM)
        return;

      new_position.x -= ACTION_DISPLACEMENT_MM;
      new_cost_to_come += ACTION_COST_STRAIGHT;
      break;

  case (Action::UP_LEFT):
      if (new_position.y >= Y_MAX_MM || new_position.x <= X_MIN_MM)
        return;

      new_position.y += ACTION_DISPLACEMENT_MM;
      new_position.x -= ACTION_DISPLACEMENT_MM;
      new_cost_to_come += ACTION_COST_DIAGONAL;
      break;

  default:
      return;
  }

  children_nodes.push_back(
    NodeDijkstra(0, index_, new_cost_to_come, new_position));
}

std::vector<project2::NodeDijkstra> project2::NodeDijkstra::getChildrenNodes()
{
  std::vector<NodeDijkstra> children_nodes {};

  for (const auto& action: actions_list) {
    actionMove(action, children_nodes);
  }

  return children_nodes;
}
