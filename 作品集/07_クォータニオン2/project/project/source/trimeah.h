//メッシュデータです
#pragma once
#include "mymath/mymath.h"
#include <vector>
#include <Windows.h>

class Vertex
{
public:
  Vertex() = default;
  ~Vertex() = default;
public:
  Vector position;
};

class Triangle
{
public:
  Triangle() = default;
  ~Triangle() = default;
public:
  int index0;
  int index1;
  int index2;
};

class Mesh
{
public:
  Mesh() = default;
  ~Mesh() = default;
public:
  std::vector<Triangle> triangles;
  std::vector<Vertex> vertices;
};

class MeshDrawer
{
public:
  MeshDrawer() = default;
  ~MeshDrawer() = default;
public:
  void operator()(HDC hdc, const Mesh* mesh, Vector location, Quaternion rotation, Vector scaling, float focal, int width, int height);
};
