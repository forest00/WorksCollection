#include "matrix4x4.h"
#include <cassert>

Matrix4x4::Matrix4x4()
  : _00()
  , _01()
  , _02()
  , _03()
  , _10()
  , _11()
  , _12()
  , _13()
  , _20()
  , _21()
  , _22()
  , _23()
  , _30()
  , _31()
  , _32()
  , _33()
{

}
Matrix4x4::Matrix4x4(const Matrix4x4 &rhs)
  : _00(rhs._00)
  , _01(rhs._01)
  , _02(rhs._02)
  , _03(rhs._03)
  , _10(rhs._10)
  , _11(rhs._11)
  , _12(rhs._12)
  , _13(rhs._13)
  , _20(rhs._20)
  , _21(rhs._21)
  , _22(rhs._22)
  , _23(rhs._23)
  , _30(rhs._30)
  , _31(rhs._31)
  , _32(rhs._32)
  , _33(rhs._33)
{

}
Matrix4x4::Matrix4x4(
  float __00, float __01, float __02, float __03,
  float __10, float __11, float __12, float __13,
  float __20, float __21, float __22, float __23,
  float __30, float __31, float __32, float __33
)
  : _00(__00)
  , _01(__01)
  , _02(__02)
  , _03(__03)
  , _10(__10)
  , _11(__11)
  , _12(__12)
  , _13(__13)
  , _20(__20)
  , _21(__21)
  , _22(__22)
  , _23(__23)
  , _30(__30)
  , _31(__31)
  , _32(__32)
  , _33(__33)
{

}
float Matrix4x4::getElement(int row, int column)
{
  assert(0 <= row && row < 4);
  assert(0 <= column && column < 4);
  return m[row][column];
}

extern Matrix4x4 operator*(const Matrix4x4 &lhs, const Matrix4x4 &rhs)
{
  return Matrix4x4(
    lhs._00 * rhs._00 + lhs._01 * rhs._10 + lhs._02 * rhs._20 + lhs._03 * rhs._30,
    lhs._00 * rhs._01 + lhs._01 * rhs._11 + lhs._02 * rhs._21 + lhs._03 * rhs._31,
    lhs._00 * rhs._02 + lhs._01 * rhs._12 + lhs._02 * rhs._22 + lhs._03 * rhs._32,
    lhs._00 * rhs._03 + lhs._01 * rhs._13 + lhs._02 * rhs._23 + lhs._03 * rhs._33,
    lhs._10 * rhs._00 + lhs._11 * rhs._10 + lhs._12 * rhs._20 + lhs._13 * rhs._30,
    lhs._10 * rhs._01 + lhs._11 * rhs._11 + lhs._12 * rhs._21 + lhs._13 * rhs._31,
    lhs._10 * rhs._02 + lhs._11 * rhs._12 + lhs._12 * rhs._22 + lhs._13 * rhs._32,
    lhs._10 * rhs._03 + lhs._11 * rhs._13 + lhs._12 * rhs._23 + lhs._13 * rhs._33,
    lhs._20 * rhs._00 + lhs._21 * rhs._10 + lhs._22 * rhs._20 + lhs._23 * rhs._30,
    lhs._20 * rhs._01 + lhs._21 * rhs._11 + lhs._22 * rhs._21 + lhs._23 * rhs._31, 
    lhs._20 * rhs._02 + lhs._21 * rhs._12 + lhs._22 * rhs._22 + lhs._23 * rhs._32,
    lhs._20 * rhs._03 + lhs._21 * rhs._13 + lhs._22 * rhs._23 + lhs._23 * rhs._33,
    lhs._30 * rhs._00 + lhs._31 * rhs._10 + lhs._32 * rhs._20 + lhs._33 * rhs._30,
    lhs._30 * rhs._01 + lhs._31 * rhs._11 + lhs._32 * rhs._21 + lhs._33 * rhs._31, 
    lhs._30 * rhs._02 + lhs._31 * rhs._12 + lhs._32 * rhs._22 + lhs._33 * rhs._32,
    lhs._30 * rhs._03 + lhs._31 * rhs._13 + lhs._32 * rhs._23 + lhs._33 * rhs._33
  );
}

Matrix4x4 MakeMatrix4x4::identity()
{
  return Matrix4x4(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  );
}
