#pragma once
//	
//	�~������uMagnet Hero�v
//	
//	�쐬��	�X	�m�u
//	
//	�X�V����
//	2019/01/25...�ɍ��W���������W�̕ϊ��֐���ǉ�
//	

//�����ł́A���܂��܂Ȍv�Z�����܂�

//DirectX
#include <d3d9.h>
#include <d3dx9.h>
//�Ƃ肠�������w�֐����C���N���[�h
#define _USE_MATH_DEFINES
#include <math.h>
#undef _USE_MATH_DEFINES

//���̐����n���ꂽ��1�A
//���̐����n���ꂽ��-1�A
//0���n���ꂽ��0��Ԃ��֐�
extern int sign(double value);
extern int signf(float value);

//�����񒆂Ɋ܂܂��ŏ��̃^�[�Q�b�g�������k�������ɒu��������
extern void change_target_to_null(char *str, size_t chars, const char *target);

//[0.0, 1.0)�̈�l����
extern double uniform_rand();
extern float uniform_randf();


//[minimum, maximum)�̈�l����
extern double uniform_rand(double minimum, double maximum);
extern float uniform_randf(float minimum, float maximum);


//�X���b�v
extern void swap(int *x, int *y);
extern void swap(float *x, float *y);
extern void swap(double *x, double *y);

//���`���
extern double lerp(double x, double y, double t);
extern float lerpf(float x, float y, float t);

//�N�����v
extern double clamp(double x, double min, double max);
extern float clampf(float x, float min, float max);

//���b�v
extern double wrap(double x, double min, double max);
extern float wrapf(float x, float min, float max);

//�Ǐ]
extern double follow(double x, double y, double v);
extern float followf(float x, float y, float v);


//�}�C�i�X�΂���v���X�΂̐��l�ɂ������̂�Ԃ��܂�
extern double angle_normalize(double angle);
extern float angle_normalizef(float angle);
//�ڕW�̊p�x�ɋ߂Â������̂�Ԃ��܂�
extern double angle_follow(double angle, double target, double turning_force);
extern float angle_followf(float angle, float target, float turning_force);


//�ɍ��W���璼�����W�ɕϊ�����
void to_xyz(float *x, float *y, float *z, float r, float theta, float phi);


//�������ꂽ�e�N�X�`����UV���W���v�Z
extern void find_uv_rect(
	int index,
	size_t x_divide_num,
	size_t y_divide_num,
	size_t all_frame_num,
	double *u_left,
	double *v_top,
	double *u_right,
	double *v_bottom
);
extern void find_uv_rectf(
	int index,
	size_t x_divide_num,
	size_t y_divide_num,
	size_t all_frame_num,
	float *u_left,
	float *v_top,
	float *u_right,
	float *v_bottom
);

/*
@fn ClosestPointPointSegment
*@brief �^����ꂽ����AB�ɑ΂��āA�_C����AB�ւ̍ŋߐړ_D���v�Z����
*@return D(t) = A + t * (B - A)�ɂ��\�����D�̈ʒu�ɑ΂���t��Ԃ�
*/
float ClosestPointPointSegment(const D3DXVECTOR3 &a, const D3DXVECTOR3 &b, const D3DXVECTOR3 &c, D3DXVECTOR3 *d);

/*
*@fn SquareDistancePointSegment
*@brief �_C�������AB�ւ̋����̓����v�Z����
*/
float SquareDistancePointSegment(const D3DXVECTOR3 &a, const D3DXVECTOR3 &b, const D3DXVECTOR3 &c);
