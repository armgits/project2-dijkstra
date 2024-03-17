/**
 * @file shapes.hpp
 * @author Ahbishekh Reddy (areddy42@umd.edu)
 * @brief Simple library for 2D graphics engine using OpenGL
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
#include <deque>
#include <cmath>
#include <glad/gl.h>

namespace TwoDE {

struct vec2f {
  vec2f() : x {0}, y {0} {}

  vec2f(float _x, float _y)
  : x {_x}, y {_y} {}

  vec2f(const vec2f& _vec)
  : x {_vec.x}, y {_vec.y} {}

  friend std::ostream& operator<<(std::ostream& os, const vec2f& vec)
  {
    os << "(" << vec.x << ", " << vec.y << ")";

    return os;
  }

  float x;
  float y;
};

struct vec2ui {
  vec2ui() : x {0}, y {0} {}

  vec2ui(unsigned int  _x, unsigned int  _y)
  : x {_x}, y {_y} {}

  vec2ui(const vec2ui& _vec)
  : x {_vec.x}, y {_vec.y} {}

  friend std::ostream& operator<<(std::ostream& os, const vec2ui& vec)
  {
    os << "(" << vec.x << ", " << vec.y << ")";

    return os;
  }

  unsigned int  x;
  unsigned int  y;
};

struct vec3f {
  vec3f() : x {0}, y {0}, z{0} {}

  vec3f(float _x, float _y, float _z)
  : x {_x}, y {_y}, z{_z} {}

  vec3f(const vec3f& _vec)
  : x {_vec.x}, y {_vec.y}, z {_vec.z} {}

  friend std::ostream& operator<<(std::ostream& os, const vec3f& vec)
  {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";

    return os;
  }

  float x;
  float y;
  float z;
};

struct color4ui {
  color4ui() : r {0}, g {0}, b {0}, a {255} {}

  color4ui(unsigned int _x, unsigned int _y, unsigned int _z, unsigned int _a = 255)
  : r {_x}, g {_y}, b {_z}, a {_a} {}

  color4ui(const color4ui& color)
  : r {color.r}, g {color.g}, b {color.b}, a {color.a} {}

  friend std::ostream& operator<<(std::ostream& os, const color4ui& color)
  {
    os << "(" << color.r << ", " << color.g << ", " << color.b << ")";

    return os;
  }

  unsigned int r;
  unsigned int g;
  unsigned int b;
  unsigned int a;
};

void generatePointsGrid(
  std::vector<unsigned int>& points,
  const vec2ui& view_size,
  int spacing);

void generatePolygonPoints(
  std::vector<unsigned int>& points,
  vec2ui center_point,
  unsigned int sides,
  unsigned int side_length,
  bool alt_orientation = false);

class PointsStatic
{
  public:
    PointsStatic(
      const std::vector<unsigned int>& points,
      const vec2ui& view_size,
      const color4ui& color);

    ~PointsStatic();

    virtual void generateVertexAttributes(
      const std::vector<unsigned int>& points,
      const TwoDE::color4ui& color);

    virtual void bind();
    virtual void unbind();
    virtual int size() {return (vertices_.size() / 6);}

  protected:
    float normalizePosition(unsigned int val, unsigned int max_screen_dim)
    {
      return (val - 0.F) * (1.F - (-1.F)) / (max_screen_dim - 0.F) + (-1.F);
    }

    float normalizeColor(unsigned int val)
    {
      return (val - 0.F) * (1.F - 0.F) / (255 - 0.F) + 0.F;
    }

    unsigned int vertex_buffer_;
    std::vector<float> vertices_;

    unsigned int screen_x_size_;
    unsigned int screen_y_size_;
};

class PointsDynamic
{
  public:
    PointsDynamic(
      const std::vector<unsigned int>& points,
      const vec2ui& view_size,
      const color4ui& color,
      int points_size = 1);

    ~PointsDynamic();

    virtual void generateVertexAttributes(
      const std::vector<unsigned int>& points,
      const vec2ui& view_size,
      const TwoDE::color4ui& color);

    virtual void bind();
    virtual void unbind();
    virtual int size() {return (positions_.size() / 2);}
    void setPointColor(unsigned int x, unsigned int y, color4ui color);
    void setPointsColor(
      std::deque<vec2ui>& points,
      color4ui color,
      int num_points = 0);

  protected:
    float normalizePosition(unsigned int val, unsigned int max_screen_dim)
    {
      return (val - 0.F) * (1.F - (-1.F)) / (max_screen_dim - 0.F) + (-1.F);
    }

    float normalizeColor(unsigned int val)
    {
      return (val - 0.F) * (1.F - 0.F) / (255 - 0.F) + 0.F;
    }

    unsigned long getBufferOffsetXY(
      unsigned int x,
      unsigned int y,
      unsigned long attrib_size_bytes)
    {
      return (x + view_size_.x * y) * attrib_size_bytes;
    }

    unsigned int vertex_buffer_handles_[3];
    unsigned int position_buffer_;
    unsigned int write_color_buffer_;
    unsigned int read_color_buffer_;

    std::vector<float> positions_;
    std::vector<float> colors_;
    int points_size_;

    TwoDE::vec2ui view_size_;
};

class PolygonSimpleStatic : public PointsStatic
{
  public:
    PolygonSimpleStatic(
      const std::vector<unsigned int>& points,
      const vec2ui& view_size,
      const color4ui& color
    );

    ~PolygonSimpleStatic();

    void bind() override;
    void unbind() override;
    int size() override {return indices_.size() - 4;}

  protected:
    virtual void generateIndices();
    std::vector<unsigned int> indices_;
    unsigned int index_buffer_;
};

// class Polygon : public PolygonSimpleStatic
// {
//   public:
//     Polygon(
//       const std::vector<unsigned int>& points,
//       unsigned int screen_w = 1200,
//       unsigned int screen_h = 500
//     );

//   private:
//     void generateIndices() override;
// };
}
