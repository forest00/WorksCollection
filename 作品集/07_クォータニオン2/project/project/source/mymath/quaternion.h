//概要：クォータニオン
//作成者：森　仁志
//作成日：2019/10/03
#pragma once
#include "vector.h"

class Quaternion
{
public:
  Quaternion();
  Quaternion(const Quaternion &rhs);
  Quaternion(float _x, float _y, float _z, float _w);
public:
  //逆回転
  Quaternion inverse() const noexcept;
  //ベクトルを回転させる
  //Unity上での"Quaternion * Vector"と同じ
  Vector rotateVector(const Vector &target);
public:
  float x;
  float y;
  float z;
  float w;
};

//'*'
extern Quaternion operator*(const Quaternion &lhs, const Quaternion rhs);
extern Quaternion operator*(const Quaternion &lhs, const Vector rhs);
extern Quaternion operator*(const Vector &lhs, const Quaternion rhs);

class MakeQuaternion
{
public:
  //回転軸と回転角度からクォータニオン生成
  static Quaternion axisAngle(Vector axis, float sheta);
  //ヨーピッチロールのみっつからクォータニオン生成
  static Quaternion yawPitchRoll(float yaw, float pitch, float roll);
};
