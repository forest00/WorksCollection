
//�e�w�b�_�[
#include "..\public\calc.h"
//���̑��̃w�b�_�[
#include "..\..\math_with_defines\math_with_defines.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>


//���̐����n���ꂽ��1�A
//���̐����n���ꂽ��-1�A
//0���n���ꂽ��0��Ԃ��֐�
extern int sign(double value)
{
	if (value > 0.0f)
	{
		return 1;
	}
	else if (value < 0.0f)
	{
		return -1;
	}
	else
	{
		return 0;
	}

}

//�����񒆂Ɋ܂܂��ŏ��̃^�[�Q�b�g�������k�������ɒu��������
extern void change_target_to_null(char *str, size_t chars, const char *target)
{
	char *found = strpbrk(str, target);
	if (found != nullptr)
	{
		*found = '\0';
	}
}

//bool�l�̔��]
extern bool bool_invers(bool b)
{
	return b ? false : true;
}

//[0.0, 1.0)�̈�l����
extern double uniform_rand()
{
	return (double)(rand()) / (double)(RAND_MAX);
}

//[minimum, maximum)�̈�l����
extern double uniform_rand(double minimum, double maximum)
{
	if (minimum > maximum)
	{
		swap(&minimum, &maximum);
	}
	return uniform_rand() * (maximum - minimum) + minimum;
}



void swap(int *x, int *y)
{
	int z = *x;
	*x = *y;
	*y = z;
}
void swap(double *x, double *y)
{
	double z = *x;
	*x = *y;
	*y = z;
}

double lerp(double x, double y, double t)
{
	return x + (y - x) * t;
}

double clamp(double x, double min, double max)
{
	if (x < min)
	{
		return min;
	}
	else if (x > max)
	{
		return max;
	}
	return x;
}

double wrap(double x, double min, double max)
{
	if (x < min)
	{
		return max;
	}
	else if (x > max)
	{
		return min;
	}
	return x;
}

double follow(double x, double y, double v)
{
	double v_ = y - x;
	double d = fabs(v_);
	if (d > v)
	{
		v_ /= d;
		v_ *= v;
	}
	x += v_;
	return x;
}





