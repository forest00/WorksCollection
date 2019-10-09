//概要：3次元のベクトル
//作成者：森　仁志
//作成日：2019/10/03
#pragma once

class Vector
{
public:
	Vector();
	Vector(const Vector &rhs);
	Vector(float _x, float _y, float _z);
public:
	//長さの二乗
	float lengthSquared() const noexcept;
	//長さ
	float length() const noexcept;
	//正規化されたものを返す
	Vector normalized() const noexcept;
	//内積
	float dot(const Vector &rhs) const noexcept;
	//外積
	Vector cross(const Vector &rhs) const noexcept;
public:
	float x;
	float y;
	float z;
};

//'+'
extern Vector operator+(const Vector &lhs, const Vector &rhs);
//'-'
extern Vector operator-(const Vector &lhs, const Vector &rhs);
//'*'
extern Vector operator*(const Vector &lhs, float rhs);
//'/'
extern Vector operator/(const Vector &lhs, float rhs);
