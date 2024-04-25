/**
 * @file shapes.cpp
 * @author Abhishekh Reddy (areddy42@umd.edu)
 * @brief Implementation of a simple 2D graphics engine to draw shapes using OpenGL
 * @version 0.1
 * @date 2024-03-16
 *
 * @copyright Copyright (c) 2024 Abhishekh Reddy
 *
 */

#include "shapes.hpp"

void TwoDE::generatePointsGrid(
  std::vector<unsigned int>& points,
  const TwoDE::vec2ui& view_size,
  int spacing
)
{
  for (unsigned int y {0}; y <= view_size.y - spacing; y += spacing) {
    for (unsigned int x {0}; x <= view_size.x - spacing; x += spacing) {
      points.push_back(x);
      points.push_back(y);
    }
  }
}

void TwoDE::generatePolygonPoints(
  std::vector<unsigned int>& points,
  TwoDE::vec2ui center_point,
  unsigned int sides,
  unsigned int side_length,
  bool alt_orientation,
  bool closed_loop
)
{
  double offset {alt_orientation ? M_PI_2 : 0.0};

  float final_theta {closed_loop ? 2 * M_PI : 2 * M_PI - (2 * M_PI) / sides};

  for (float theta {0}; theta <= final_theta; theta += (2 * M_PI) / sides) {
    points.push_back(int(center_point.x + side_length * cos(theta + offset)));
    points.push_back(int(center_point.y + side_length * sin(theta + offset)));
  }
}

TwoDE::PointsStatic::PointsStatic(
  const std::vector<unsigned int>& points,
  const TwoDE::vec2ui& view_size,
  const TwoDE::color4ui& color
)
: screen_x_size_ {view_size.x},
  screen_y_size_ {view_size.y}
{
  generateVertexAttributes(points, color);

  glGenBuffers(1, &vertex_buffer_);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(float), vertices_.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

TwoDE::PointsStatic::~PointsStatic()
{
  glDeleteBuffers(1, &vertex_buffer_);
}

void TwoDE::PointsStatic::bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0));

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

void TwoDE::PointsStatic::unbind()
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TwoDE::PointsStatic::generateVertexAttributes(
  const std::vector<unsigned int>& points,
  const TwoDE::color4ui& color
)
{
  vertices_.clear();
  bool is_x {true};

  for (auto i {points.begin()}; i < points.end()-1; i++) {
    vertices_.push_back(normalizePosition(*i, screen_x_size_));
    vertices_.push_back(normalizePosition(*(++i), screen_y_size_));

    vertices_.push_back(normalizeColor(color.r));
    vertices_.push_back(normalizeColor(color.g));
    vertices_.push_back(normalizeColor(color.b));
    vertices_.push_back(normalizeColor(color.a));

    // std::cout << "Normalized colors: " << std::endl;
    // std::cout << "(" << normalizeColor(color.r) << ", "
    //   << normalizeColor(color.g) << ", "
    //   << normalizeColor(color.b) << ", "
    //   << normalizeColor(color.a) << ")" << std::endl;
  }
}

TwoDE::PointsDynamic::PointsDynamic(
  const std::vector<unsigned int>& points,
  const vec2ui& view_size,
  const color4ui& color,
  int points_size
)
: view_size_ {view_size}, points_size_ {points_size}
{
  generateVertexAttributes(points, view_size, color);

  glGenBuffers(3, &vertex_buffer_handles_[0]);
  position_buffer_ = vertex_buffer_handles_[0];
  read_color_buffer_ = vertex_buffer_handles_[1];
  write_color_buffer_ = vertex_buffer_handles_[2];

  glBindBuffer(GL_ARRAY_BUFFER, position_buffer_);
  glBufferData(GL_ARRAY_BUFFER, positions_.size() * sizeof(float), positions_.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, write_color_buffer_);
  glBufferData(GL_ARRAY_BUFFER, colors_.size() * sizeof(float), colors_.data(), GL_STREAM_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, read_color_buffer_);
  glBufferData(GL_ARRAY_BUFFER, colors_.size() * sizeof(float), nullptr, GL_STREAM_DRAW);
  glBindBuffer(GL_COPY_READ_BUFFER, write_color_buffer_);
  glBindBuffer(GL_COPY_WRITE_BUFFER, read_color_buffer_);
  glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, colors_.size() * sizeof(float));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

TwoDE::PointsDynamic::~PointsDynamic()
{
  glDeleteBuffers(3, &vertex_buffer_handles_[0]);
}

void TwoDE::PointsDynamic::bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer_);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_COPY_READ_BUFFER, write_color_buffer_);
  glBindBuffer(GL_COPY_WRITE_BUFFER, read_color_buffer_);
  glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, colors_.size() * sizeof(float));

  glBindBuffer(GL_ARRAY_BUFFER, read_color_buffer_);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

void TwoDE::PointsDynamic::unbind()
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TwoDE::PointsDynamic::generateVertexAttributes(
  const std::vector<unsigned int>& points,
  const vec2ui& view_size,
  const TwoDE::color4ui& color
)
{
  for (auto i {points.begin()}; i < points.end()-1; i++) {
    positions_.push_back(normalizePosition(*i, view_size_.x));
    positions_.push_back(normalizePosition(*(++i), view_size_.y));
  }

  // std::cout << "Positions size: " << positions_.size() << std::endl;

  for (int i {0}; i <= positions_.size()-1; i++) {
    colors_.push_back(normalizeColor(color.r));
    colors_.push_back(normalizeColor(color.g));
    colors_.push_back(normalizeColor(color.b));
    colors_.push_back(normalizeColor(color.a));
  }

  // std::cout << "Colors size: " << colors_.size() << std::endl;
}

void TwoDE::PointsDynamic::setPointColor(
  unsigned int x,
  unsigned int y,
  TwoDE::color4ui color)
{
  std::array<float, 4> new_color {
    normalizeColor(color.r),
    normalizeColor(color.g),
    normalizeColor(color.b),
    normalizeColor(color.a)
  };

  auto offset {getBufferOffsetXY(x, y, new_color.size() * sizeof(float))};

  // std::cout << "Offset: " << offset << std::endl;
  // std::cout << "x: " << x << ", " << "y: " << y << std::endl;
  // std::cout << new_color.size() << " * " << (x + view_size_.x * y) << " * " << sizeof(float) << std::endl;

  glBindBuffer(GL_ARRAY_BUFFER, write_color_buffer_);
  glBufferSubData(GL_ARRAY_BUFFER, offset, new_color.size() * sizeof(float), new_color.data());
}

void TwoDE::PointsDynamic::setPointsColor(
  std::deque<TwoDE::vec2ui>& points,
  TwoDE::color4ui color,
  int num_points)
{
  if (points.empty() || num_points <= 0)
    return;

  if (points.begin() + num_points >= points.end())
    return;

  // Reset the write buffer data before updating...
  glBindBuffer(GL_ARRAY_BUFFER, write_color_buffer_);
  glBufferData(GL_ARRAY_BUFFER, colors_.size() * sizeof(float), nullptr, GL_STREAM_DRAW);
  glBindBuffer(GL_COPY_READ_BUFFER, read_color_buffer_);
  glBindBuffer(GL_COPY_WRITE_BUFFER, write_color_buffer_);
  glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, colors_.size() * sizeof(float));

  glBindBuffer(GL_ARRAY_BUFFER, write_color_buffer_);
  float * buffer_map {(float *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)};

  for (int i {0}; i <= num_points; i++) {
    auto point {points.begin()};
    auto offset {getBufferOffsetXY(point->x, point->y, 4)};
    points.pop_front();

    // std::cout << "Setting blue to: " << "(" << point->x << ", " << point->y << ")" << std::endl;
    auto color_pos {buffer_map + offset};
    *color_pos = normalizeColor(color.r);
    *(color_pos + 3) = normalizeColor(color.a);
    *(color_pos + 1) = normalizeColor(color.g);
    *(color_pos + 2) = normalizeColor(color.b);
  }

  glUnmapBuffer(GL_ARRAY_BUFFER);
}

TwoDE::PolygonSimpleStatic::PolygonSimpleStatic(
  const std::vector<unsigned int>& points,
  const TwoDE::vec2ui& view_size,
  const TwoDE::color4ui& color)
: PointsStatic(points, view_size, color)
{
  generateIndices();
  glGenBuffers(1, &index_buffer_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

TwoDE::PolygonSimpleStatic::~PolygonSimpleStatic()
{
  glDeleteBuffers(1, &index_buffer_);
}

void TwoDE::PolygonSimpleStatic::bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
  glEnableVertexAttribArray(0);

  // TODO: Fix the color bug...
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
}

void TwoDE::PolygonSimpleStatic::unbind()
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TwoDE::PolygonSimpleStatic::generateIndices()
{
  for (int i {0}; i < (vertices_.size() / 6); i++) {
    indices_.push_back(0);
    indices_.push_back(i + 1);
    indices_.push_back(i + 2);
  }
}

// TwoDE::Polygon::Polygon(
//   const std::vector<unsigned int>& points,
//   unsigned int screen_w,
//   unsigned int screen_h
// )
// : PolygonSimple(PointsStatic, screen_w, screen_h)
// {
//   generateIndices();
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
//   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
// }

// void TwoDE::Polygon::generateIndices()
// {
//   indices_.clear();

//   bool alt {false};
//   for (int i {0}; i < (vertices_.size() - 2); i++) {
//     if (!alt) {
//       indices_.push_back(i);
//       indices_.push_back(i + 1);
//       alt = true;
//     }
//     else {
//       indices_.push_back(i + 1);
//       indices_.push_back(i);
//       alt = false;
//     }

//     indices_.push_back(i + 2);
//   }

//   std::cout << "Indices: " << std::endl;
//   for (const auto& index: indices_) {
//     std::cout << index << std::endl;
//   }
// }
