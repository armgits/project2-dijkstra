/**
 * @file main.cpp
 * @author Abhishekh Reddy (areddy42@umd.edu)
 * @brief Main function for Project 2
 * @version 0.1
 * @date 2024-03-16
 *
 * @copyright Copyright (c) 2024 Abhishekh Reddy
 *
 */

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <thread>

#include "project2.hpp"
#include "shapes.hpp"

int main()
{
  TwoDE::vec2ui window_size {1200, 500};

  // Initialize obstacle space
  std::vector<unsigned int> obstacle1_points {100, 100, 175, 100, 175, 500, 100, 500};
  std::vector<unsigned int> obstacle2_points {275, 0, 350, 0, 350, 400, 275, 400};
  std::vector<unsigned int> obstacle3_points {};
  TwoDE::generatePolygonPoints(obstacle3_points, {650, 250}, 6, 150, true, false);

  std::vector<unsigned int> obstacle4_1_points {900, 125, 900, 50, 1100, 50, 1100, 125};
  std::vector<unsigned int> obstacle4_2_points {1020, 125, 1100, 125, 1100, 375, 1020, 375};
  std::vector<unsigned int> obstacle4_3_points {1100, 375, 1100, 450, 900, 450, 900, 375};

  project2::ObstacleSpace obstacle_space1 {obstacle1_points, 5, window_size};
  project2::ObstacleSpace obstacle_space2 {obstacle2_points, 5, window_size};
  project2::ObstacleSpace obstacle_space3 {obstacle3_points, 5, window_size};
  project2::ObstacleSpace obstacle_space4_1 {obstacle4_1_points, 5, window_size};
  project2::ObstacleSpace obstacle_space4_2 {obstacle4_2_points, 5, window_size};
  project2::ObstacleSpace obstacle_space4_3 {obstacle4_3_points, 5, window_size};

  std::vector<project2::ObstacleSpace> obstacles_space {
    obstacle_space1,
    obstacle_space2,
    obstacle_space3,
    obstacle_space4_1,
    obstacle_space4_2,
    obstacle_space4_3};

  project2::Position start_node_pos {};
  project2::Position goal_node_pos {};

  // Get the user input
  bool in_obstacle_space {true};

  do {
    std::cout << '\n' << "-- Enter start node --" << '\n';
    std::cout << "x: ";
    std::cin >> start_node_pos.x;
    std::cout << "y: ";
    std::cin >> start_node_pos.y;

    in_obstacle_space = project2::inObstacleSpace(start_node_pos, obstacles_space);

    if (in_obstacle_space)
      std::cout << '\n' << "Start point is in obstacle space, try again..." << '\n';

  } while (!(start_node_pos.x > 0) || !(start_node_pos.x < window_size.x)
          || !(start_node_pos.y > 0) || !(start_node_pos.y < window_size.y)
          || in_obstacle_space);

  do {
    std::cout << '\n' << "-- Enter goal node --" << '\n';
    std::cout << "x: ";
    std::cin >> goal_node_pos.x;
    std::cout << "y: ";
    std::cin >> goal_node_pos.y;

    in_obstacle_space = project2::inObstacleSpace(goal_node_pos, obstacles_space);

    if (in_obstacle_space)
      std::cout << '\n' << "Goal point is in obstacle space, try again..." << '\n';

  } while (!(goal_node_pos.x > 0) || !(goal_node_pos.x < window_size.x)
          || !(goal_node_pos.y > 0) || !(goal_node_pos.y < window_size.y)
          || in_obstacle_space);

  // Initialize GLFW window
  GLFWwindow* window;

  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  window = glfwCreateWindow(window_size.x, window_size.y, "Project 2", NULL, NULL);

  if (window == NULL) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  // Load OpenGL
  if (!gladLoaderLoadGL())
  {
    std::cout << "Failed to initialize GLAD" << '\n';
    return -1;
  }

  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Initialize map draw objects
  std::vector<unsigned int> points_grid {};
  TwoDE::generatePointsGrid(points_grid, window_size, 25);
  TwoDE::PointsStatic map_points {points_grid, window_size, {65, 65, 65}};

  std::vector<unsigned int> new_points_grid {};
  TwoDE::generatePointsGrid(new_points_grid, window_size, 1);
  TwoDE::PointsDynamic map_graph {new_points_grid, window_size, {40, 40, 40}};

  TwoDE::PolygonSimpleStatic obstacle1 {obstacle1_points, window_size, {100, 100, 100}};
  TwoDE::PolygonSimpleStatic obstacle2 {obstacle2_points, window_size, {100, 100, 100}};
  TwoDE::PolygonSimpleStatic obstacle3 {obstacle3_points, window_size, {100, 100, 100}};
  TwoDE::PolygonSimpleStatic obstacle4_1 {obstacle4_1_points, window_size, {100, 100, 100}};
  TwoDE::PolygonSimpleStatic obstacle4_2 {obstacle4_2_points, window_size, {100, 100, 100}};
  TwoDE::PolygonSimpleStatic obstacle4_3 {obstacle4_3_points, window_size, {100, 100, 100}};

  // Initialize OpenGL shader and set some parameters for rendering
  auto gl_program {project2::initShader()};

  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glLineWidth(5);

  // Initial parameters for Dijkstra
  auto start_node {project2::Node(start_node_pos)};
  auto goal_node {project2::Node(goal_node_pos)};

  std::deque<TwoDE::vec2ui> position_buffer {};
  TwoDE::color4ui node_color {0, 0, 255};

  // Start Dijkstra search on a dedicated thread and update the buffer with
  // explored nodes.
  bool continue_search {true};
  bool search_complete {false};
  std::thread search_thread {project2::searchDijkstra,
    std::ref(start_node),
    std::ref(goal_node),
    std::ref(obstacles_space),
    std::ref(position_buffer),
    std::ref(continue_search),
    std::ref(search_complete)};

  // Render for visualization
  while(!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.075, 0.075, 0.075, 1.0);

    // Update the GPU vertex buffer with explored nodes and render.
    if (search_complete)
      node_color = {0, 255, 0};

    map_graph.setPointsColor(position_buffer, node_color, 100);
    map_graph.bind();
    glDrawArrays(GL_POINTS, 0, map_graph.size());

    // map_points.bind();
    // glDrawArrays(GL_POINTS, 0, map_points.size());

    obstacle1.bind();
    glDrawElements(GL_TRIANGLE_FAN, obstacle1.size(), GL_UNSIGNED_INT, nullptr);

    obstacle2.bind();
    glDrawElements(GL_TRIANGLE_FAN, obstacle2.size(), GL_UNSIGNED_INT, nullptr);

    obstacle3.bind();
    glDrawElements(GL_TRIANGLE_FAN, obstacle3.size(), GL_UNSIGNED_INT, nullptr);

    obstacle4_1.bind();
    glDrawElements(GL_TRIANGLE_FAN, obstacle4_1.size(), GL_UNSIGNED_INT, nullptr);
    obstacle4_2.bind();
    glDrawElements(GL_TRIANGLE_FAN, obstacle4_2.size(), GL_UNSIGNED_INT, nullptr);
    obstacle4_3.bind();
    glDrawElements(GL_TRIANGLE_FAN, obstacle4_3.size(), GL_UNSIGNED_INT, nullptr);

    glfwSwapBuffers(window);
  }

  // Wrap-up
  continue_search = false;

  search_thread.join();

  glDeleteShader(gl_program);
  glDeleteVertexArrays(1, &vao);
  glfwTerminate();

  return 0;
}
