#include "quaternion.h"
#include <cmath>

Quaternion::Quaternion()
  : x()
  , y()
  , z()
  , w()
{

}
Quaternion::Quaternion(const Quaternion &rhs)
  : x(rhs.x)
  , y(rhs.y)
  , z(rhs.z)
  , w(rhs.w)
{

}
Quaternion::Quaternion(float _x, float _y, float _z, float _w)
  : x(_x)
  , y(_y)
  , z(_z)
  , w(_w)
{

}
Quaternion Quaternion::inverse() const noexcept
{
  return Quaternion(-x, -y, -z, w);
}
Vector Quaternion::rotateVector(const Vector &target)
{
  Quaternion result = (*this) * (target) * (inverse());
  return Vector(result.x, result.y, result.z);
}

extern Quaternion operator*(const Quaternion &lhs, const Quaternion rhs)
{
  return Quaternion(
    lhs.w * rhs.x - lhs.z * rhs.y + lhs.y * rhs.z + lhs.x * rhs.w,
    lhs.z * rhs.x + lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w,
    -lhs.y * rhs.x + lhs.x * rhs.y + lhs.w * rhs.z + lhs.z * rhs.w,
    -lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z + lhs.w * rhs.w
  );
}
extern Quaternion operator*(const Quaternion &lhs, const Vector rhs)
{
  return lhs * Quaternion(rhs.x, rhs.y, rhs.z, 0.0f);
}
extern Quaternion operator*(const Vector &lhs, const Quaternion rhs)
{
  return Quaternion(lhs.x, lhs.y, lhs.z, 0.0f) * rhs;
}

Quaternion MakeQuaternion::axisAngle(Vector axis, float sheta)
{
  Vector normalizedAxis = axis.normalized();
  float sins2 = sin(sheta / 2.0f);
  float coss2 = cos(sheta / 2.0f);
  return Quaternion(
    normalizedAxis.x * sins2,
    normalizedAxis.y * sins2,
    normalizedAxis.z * sins2,
    coss2
  );
}
Quaternion MakeQuaternion::yawPitchRoll(float yaw, float pitch, float roll)
{
  Vector x(1.0f, 0.0f, 0.0f);
  Vector y(0.0f, 1.0f, 0.0f);
  Vector z(0.0f, 0.0f, 1.0f);
  auto xq = MakeQuaternion::axisAngle(x, yaw);
  auto yq = MakeQuaternion::axisAngle(y, pitch);
  auto zq = MakeQuaternion::axisAngle(z, roll);
  return xq * yq * zq;
}
