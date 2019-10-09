//�T�v�F3�����̃x�N�g��
//�쐬�ҁF�X�@�m�u
//�쐬���F2019/10/03
#pragma once

class Vector
{
public:
	Vector();
	Vector(const Vector &rhs);
	Vector(float _x, float _y, float _z);
public:
	//�����̓��
	float lengthSquared() const noexcept;
	//����
	float length() const noexcept;
	//���K�����ꂽ���̂�Ԃ�
	Vector normalized() const noexcept;
	//����
	float dot(const Vector &rhs) const noexcept;
	//�O��
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
