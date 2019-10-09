
//親ヘッダー
#include "..\public\calc.h"
//その他のヘッダー
#include "..\..\math_with_defines\math_with_defines.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>


//正の数が渡されたら1、
//負の数が渡されたら-1、
//0が渡されたら0を返す関数
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

//文字列中に含まれる最初のターゲット文字をヌル文字に置き換える
extern void change_target_to_null(char *str, size_t chars, const char *target)
{
	char *found = strpbrk(str, target);
	if (found != nullptr)
	{
		*found = '\0';
	}
}

//bool値の反転
extern bool bool_invers(bool b)
{
	return b ? false : true;
}

//[0.0, 1.0)の一様乱数
extern double uniform_rand()
{
	return (double)(rand()) / (double)(RAND_MAX);
}

//[minimum, maximum)の一様乱数
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





