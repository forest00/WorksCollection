//�T�v�F�N�H�[�^�j�I��
//�쐬�ҁF�X�@�m�u
//�쐬���F2019/10/03
#pragma once
#include "vector.h"

class Quaternion
{
public:
  Quaternion();
  Quaternion(const Quaternion &rhs);
  Quaternion(float _x, float _y, float _z, float _w);
public:
  //�t��]
  Quaternion inverse() const noexcept;
  //�x�N�g������]������
  //Unity��ł�"Quaternion * Vector"�Ɠ���
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
  //��]���Ɖ�]�p�x����N�H�[�^�j�I������
  static Quaternion axisAngle(Vector axis, float sheta);
  //���[�s�b�`���[���݂̂�����N�H�[�^�j�I������
  static Quaternion yawPitchRoll(float yaw, float pitch, float roll);
};
