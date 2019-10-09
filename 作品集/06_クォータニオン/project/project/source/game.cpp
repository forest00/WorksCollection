#include "game.h"
#include "mymath/mymath.h"
#include <ctime>

static float focal;
static int width;
static int height;

static POINT worldTo2dScreen(Vector worldLocation/*, float focal, int width, int height*/)
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

static void drawSquare(HDC hdc, POINT p1, POINT p2, POINT p3, POINT p4)
{
  MoveToEx(hdc, p1.x, p1.y, nullptr);
  LineTo(hdc, p2.x, p2.y);
  LineTo(hdc, p3.x, p3.y);
  LineTo(hdc, p4.x, p4.y);
  LineTo(hdc, p1.x, p1.y);
}

extern int initializeGame(HWND hWnd)
{
  focal = 500;
  RECT clientRect;
  GetClientRect(hWnd, &clientRect);
  width = clientRect.right - clientRect.left;
  height = clientRect.bottom - clientRect.top;
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
  Vector location2(-300, 0, 500);
  auto rotation1 = MakeQuaternion::yawPitchRoll(t, t * 2.0f, t * 4.0f);
  auto rotation2 = MakeQuaternion::yawPitchRoll(t * 2.0f, t * 4.0f, t);
  float extent1 = 50.0f;
  float extent2 = 150.0f;
  Vector v1(extent1, extent1, extent1);
  Vector v2(extent1, -extent1, extent1);
  Vector v3(-extent1, -extent1, extent1);
  Vector v4(-extent1, extent1, extent1);
  Vector v5(extent1, extent1, -extent1);
  Vector v6(extent1, -extent1, -extent1);
  Vector v7(-extent1, -extent1, -extent1);
  Vector v8(-extent1, extent1, -extent1);
  Vector v9(0.0f, 0.0f, 0.0f);
  Vector v10(extent2, 0.0f, 0.0f);
  Vector v11(0.0f, extent2, 0.0f);
  Vector v12(0.0f, 0.0f, extent2);
  v1 = rotation1.rotateVector(v1);
  v2 = rotation1.rotateVector(v2);
  v3 = rotation1.rotateVector(v3);
  v4 = rotation1.rotateVector(v4);
  v5 = rotation1.rotateVector(v5);
  v6 = rotation1.rotateVector(v6);
  v7 = rotation1.rotateVector(v7);
  v8 = rotation1.rotateVector(v8);
  v9 = rotation2.rotateVector(v9);
  v10 = rotation2.rotateVector(v10);
  v11 = rotation2.rotateVector(v11);
  v12 = rotation2.rotateVector(v12);
  v1 = v1 + location1;
  v2 = v2 + location1;
  v3 = v3 + location1;
  v4 = v4 + location1;
  v5 = v5 + location1;
  v6 = v6 + location1;
  v7 = v7 + location1;
  v8 = v8 + location1;
  v9 = v9 + location2;
  v10 = v10 + location2;
  v11 = v11 + location2;
  v12 = v12 + location2;
  POINT p1 = worldTo2dScreen(v1);
  POINT p2 = worldTo2dScreen(v2);
  POINT p3 = worldTo2dScreen(v3);
  POINT p4 = worldTo2dScreen(v4);
  POINT p5 = worldTo2dScreen(v5);
  POINT p6 = worldTo2dScreen(v6);
  POINT p7 = worldTo2dScreen(v7);
  POINT p8 = worldTo2dScreen(v8);
  POINT p9 = worldTo2dScreen(v9);
  POINT p10 = worldTo2dScreen(v10);
  POINT p11 = worldTo2dScreen(v11);
  POINT p12 = worldTo2dScreen(v12);
  drawSquare(hdc, p1, p2, p3, p4);
  drawSquare(hdc, p5, p6, p7, p8);
  drawSquare(hdc, p1, p2, p6, p5);
  drawSquare(hdc, p2, p3, p7, p6);
  drawSquare(hdc, p1, p4, p8, p5);
  drawSquare(hdc, p3, p4, p8, p7);
  drawTriangle(hdc, p9, p10, p11);
  drawTriangle(hdc, p9, p12, p12);
  drawTriangle(hdc, p9, p12, p10);
  drawTriangle(hdc, p10, p11, p12);
  Sleep(32);
  InvalidateRect(hWnd, nullptr, TRUE);
  return 0;
}
