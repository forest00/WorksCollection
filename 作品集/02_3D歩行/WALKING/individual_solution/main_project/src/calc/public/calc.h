#pragma once

//�����ł́A���܂��܂Ȍv�Z�����Ă���܂�




//���̐����n���ꂽ��1�A
//���̐����n���ꂽ��-1�A
//0���n���ꂽ��0��Ԃ��֐�
extern int sign(double value);

//�����񒆂Ɋ܂܂��ŏ��̃^�[�Q�b�g�������k�������ɒu��������
extern void change_target_to_null(char *str, size_t chars, const char *target);

//bool�l�̔��]
extern  bool bool_invers(bool b);

//[0.0, 1.0)�̈�l����
extern  double uniform_rand();

//[minimum, maximum)�̈�l����
extern  double uniform_rand(double minimum, double maximum);


//�X���b�v
void swap(int *x, int *y);
void swap(double *x, double *y);

//���`���
double lerp(double x, double y, double t);

//�N�����v
double clamp(double x, double min, double max);

//���b�s���O
double wrap(double x, double min, double max);

//�Ǐ]
double follow(double x, double y, double v);

