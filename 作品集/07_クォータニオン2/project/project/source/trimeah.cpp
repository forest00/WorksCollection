#include "trimeah.h"


static POINT worldTo2dScreen(Vector worldLocation, float focal, int width, int height)
{
  POINT result;
  result.x = (LONG)(worldLocation.x * focal / worldLocation.z + width / 2.0f);
  result.y = (LONG)(worldLocation.y * focal / worldLocation.z + height / 2.0f);
  return result;
}

static void drawLine(HDC hdc, POINT start, POINT stop)
{
  MoveToEx(hdc, start.x, start.y, nullptr);
  LineTo(hdc, stop.x, stop.y);
}
static void drawTriangle(HDC hdc, POINT p1, POINT p2, POINT p3)
{
  MoveToEx(hdc, p1.x, p1.y, nullptr);
  LineTo(hdc, p2.x, p2.y);
  LineTo(hdc, p3.x, p3.y);
  LineTo(hdc, p1.x, p1.y);
}

void MeshDrawer::operator()(HDC hdc, const Mesh* mesh, Vector location, Quaternion rotation, Vector scaling, float focal, int width, int height)
{
  if (mesh == nullptr)
  {
    return;
  }
  auto vnum = mesh->vertices.size();
  auto tnum = mesh->triangles.size();
  for (auto i = 0u; i < tnum; i++)
  {
    auto& tri = mesh->triangles[i];
    if (tri.index0 > vnum) {
      return;
    }
    if (tri.index1 > vnum) {
      return;
    }
    if (tri.index2 > vnum) {
      return;
    }
    Vector v0 = rotation.rotateVector(mesh->vertices[tri.index0].position);
    Vector v1 = rotation.rotateVector(mesh->vertices[tri.index1].position);
    Vector v2 = rotation.rotateVector(mesh->vertices[tri.index2].position);
    v0.x = v0.x * scaling.x;
    v0.y = v0.y * scaling.y;
    v0.z = v0.z * scaling.z;
    v1.x = v1.x * scaling.x;
    v1.y = v1.y * scaling.y;
    v1.z = v1.z * scaling.z;
    v2.x = v2.x * scaling.x;
    v2.y = v2.y * scaling.y;
    v2.z = v2.z * scaling.z;
    v0 = v0 + location;
    v1 = v1 + location;
    v2 = v2 + location;
    POINT p0 = worldTo2dScreen(v0, focal, width, height);
    POINT p1 = worldTo2dScreen(v1, focal, width, height);
    POINT p2 = worldTo2dScreen(v2, focal, width, height);
    drawTriangle(hdc, p0, p1, p2);
  }
}
