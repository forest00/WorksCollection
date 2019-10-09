#include "vector.h"
#include <cmath>

Vector::Vector()
	: x()
	, y()
	, z()
{

}
Vector::Vector(const Vector &rhs)
	: x(rhs.x)
	, y(rhs.y)
	, z(rhs.z)
{

}
Vector::Vector(float _x, float _y, float _z)
	: x(_x)
	, y(_y)
	, z(_z)
{

}
float Vector::lengthSquared() const noexcept
{
	return x * x + y * y + z * z;
}
float Vector::length() const noexcept
{
	return sqrtf(x * x + y * y + z * z);
}
Vector Vector::normalized() const noexcept
{
	return *this / length();
}
float Vector::dot(const Vector &rhs) const noexcept
{
	return x * rhs.x + y * rhs.y + z * rhs.z;
}
Vector Vector::cross(const Vector &rhs) const noexcept
{
	return Vector(
		y * rhs.z - z * rhs.y,
		z * rhs.x - x * rhs.z,
		x * rhs.y - y * rhs.x
	);
}
extern Vector operator+(const Vector &lhs, const Vector &rhs)
{
	return Vector(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
extern Vector operator-(const Vector &lhs, const Vector &rhs)
{
	return Vector(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
extern Vector operator*(const Vector &lhs, float rhs)
{
	return Vector(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}
extern Vector operator/(const Vector &lhs, float rhs)
{
	return Vector(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}
