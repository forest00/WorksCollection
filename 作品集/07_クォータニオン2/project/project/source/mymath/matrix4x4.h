//4*4の行列
//作成者：森　仁志
//作成日：2019/10/03
#pragma once
#include "vector.h"

class Matrix4x4
{
public:
  Matrix4x4();
  Matrix4x4(const Matrix4x4 &rhs);
  Matrix4x4(
    float __00, float __01, float __02, float __03,
    float __10, float __11, float __12, float __13,
    float __20, float __21, float __22, float __23,
    float __30, float __31, float __32, float __33
  );
public:
  //0~3のインデックスふたつにより要素取得
  float getElement(int row, int column);
public:
  union {
    float m[4][4];
    struct
    {
      float _00;
      float _01;
      float _02;
      float _03;
      float _10;
      float _11;
      float _12;
      float _13;
      float _20;
      float _21;
      float _22;
      float _23;
      float _30;
      float _31;
      float _32;
      float _33;
    };
  };
};

//'*'
extern Matrix4x4 operator*(const Matrix4x4 &lhs, const Matrix4x4 &rhs);

class MakeMatrix4x4
{
public:
  static Matrix4x4 identity();
};
