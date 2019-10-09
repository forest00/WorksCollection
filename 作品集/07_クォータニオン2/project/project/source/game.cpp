#include "game.h"
#include "mymath/mymath.h"
#include <ctime>
#include "trimeah.h"

static float focal;
static int width;
static int height;
static Mesh mesh1;

extern int initializeGame(HWND hWnd)
{
  focal = 500;
  RECT clientRect;
  GetClientRect(hWnd, &clientRect);
  width = clientRect.right - clientRect.left;
  height = clientRect.bottom - clientRect.top;
  //create mesh1
  for (int i = 0; i < 8; i++) {
    Triangle tri;
    tri.index0 = i * 3 + 0;
    tri.index1 = i * 3 + 1;
    tri.index2 = i * 3 + 2;
    mesh1.triangles.push_back(tri);
  }
  for (int i = 0; i < 8; i++)
  {
    Vertex v0;
    Vertex v1;
    Vertex v2;
    v0.position.x = 100;
    v0.position.y = 0;
    v1.position.x = 0;
    v1.position.y = 100;
    v2.position.x = -100;
    v2.position.y = 0;
    v0.position.z = v1.position.z = v2.position.z = (i - 4) * 10;
    v2.position.z *= 4.5f;
    mesh1.vertices.push_back(v0);
    mesh1.vertices.push_back(v1);
    mesh1.vertices.push_back(v2);
  }
  return 0;
}
extern int finalizeGame()
{
  return 0;
}
extern int updateGame()
{
  return 0;
}
extern int drawGame(HWND hWnd, HDC hdc)
{
  float t = (float)(clock()) / (float)(CLOCKS_PER_SEC);
  Vector location1(100, 0, 500);
  auto rotation1 = MakeQuaternion::yawPitchRoll(0, t, t);
  auto scaling1 = Vector(1, 1, 1);
  MeshDrawer draw;
  draw(hdc, &mesh1, location1, rotation1, scaling1, focal, width, height);
  Sleep(32);
  InvalidateRect(hWnd, nullptr, TRUE);
  return 0;
}
