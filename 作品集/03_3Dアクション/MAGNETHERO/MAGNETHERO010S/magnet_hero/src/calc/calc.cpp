//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	
//	最終更新		2018/12/13
//	


//親ヘッダー
#include "calc.h"
//その他のヘッダー
#include "..\mylib\mylib.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>



static void rand_init()
{
	static bool initialized = false;
	if (initialized == false)
	{
		initialized = true;
		srand((unsigned int)time(nullptr));
	}
}


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
extern int signf(float value)
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
	UNREFERENCED_PARAMETER(chars);
	char *found = strpbrk(str, target);
	if (found != nullptr)
	{
		*found = '\0';
	}
}

//[0.0, 1.0)の一様乱数
extern double uniform_rand()
{
	rand_init();
	return (double)(rand()) / (double)(RAND_MAX);
}
extern float uniform_randf()
{
	rand_init();
	return (float)(rand()) / (float)(RAND_MAX);
}

//[minimum, maximum)の一様乱数
extern double uniform_rand(double minimum, double maximum)
{
	rand_init();
	if (minimum > maximum)
	{
		swap(&minimum, &maximum);
	}
	return uniform_rand() * (maximum - minimum) + minimum;
}
extern float uniform_randf(float minimum, float maximum)
{
	rand_init();
#if 0
	if (minimum > maximum)
	{
		swap(&minimum, &maximum);
	}
#endif
	return uniform_randf() * (maximum - minimum) + minimum;
}



extern void swap(int *x, int *y)
{
	int z = *x;
	*x = *y;
	*y = z;
}
extern void swap(float *x, float *y)
{
	float z = *x;
	*x = *y;
	*y = z;
}
extern void swap(double *x, double *y)
{
	double z = *x;
	*x = *y;
	*y = z;
}

extern double lerp(double x, double y, double t)
{
	return x + (y - x) * t;
}
extern float lerpf(float x, float y, float t)
{
	return x + (y - x) * t;
}

extern double clamp(double x, double min, double max)
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
extern float clampf(float x, float min, float max)
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

extern double wrap(double x, double min, double max)
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
extern float wrapf(float x, float min, float max)
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

extern double follow(double x, double y, double v)
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
extern float followf(float x, float y, float v)
{
	float v_ = y - x;
	float d = fabsf(v_);
	if (d > v)
	{
		v_ /= d;
		v_ *= v;
	}
	x += v_;
	return x;
}



//マイナスπからプラスπの数値にしたものを返します
extern double angle_normalize(double angle)
{
	double answer = angle;
	const double c_2_pi = M_PI * 2.0;
	const double fl = floor((angle + M_PI) / c_2_pi);
	answer -= c_2_pi * fl;
	return answer;
}
extern float angle_normalizef(float angle)
{
	float answer = angle;
	const float c_2_pi = (float)(M_PI) * 2.0f;
	const float fl = floorf((angle + (float)(M_PI)) / c_2_pi);
	answer -= c_2_pi * fl;
	return answer;
}

//目標の角度に近づけたものを返します
extern double angle_follow(double angle, double target, double turning_force)
{
	double sub = target - angle;
	sub = angle_normalize(sub);
	if (fabs(sub) <= turning_force)
	{
		return angle_normalize(target);
	}
	else
	{
		if (sub >= 0.0)
		{
			return angle_normalize(angle + turning_force);
		}
		else
		{
			return angle_normalize(angle - turning_force);
		}
	}
}
extern float angle_followf(float angle, float target, float turning_force)
{
	float sub = target - angle;
	sub = angle_normalizef(sub);
	if (fabs(sub) <= turning_force)
	{
		return angle_normalizef(target);
	}
	else
	{
		if (sub >= 0.0)
		{
			return angle_normalizef(angle + turning_force);
		}
		else
		{
			return angle_normalizef(angle - turning_force);
		}
	}
}


//極座標から直交座標に変換する
void to_xyz(float *x, float *y, float *z, float r, float theta, float phi)
{
	*x = r * sinf(theta) * cosf(phi);
	*y = r * cosf(theta);
	*z = r * sinf(theta) * sinf(phi);
}



//分割されたテクスチャのUV座標を計算
extern void find_uv_rect(
	int index,
	size_t x_divide_num,
	size_t y_divide_num,
	size_t all_frame_num,
	double *u_left,
	double *v_top,
	double *u_right,
	double *v_bottom
)
{
	index %= all_frame_num;
	if (u_left)
	{
		*u_left = (double)(index % x_divide_num) / (double)(x_divide_num);
	}
	if (v_top)
	{
		*v_top = (double)(index / x_divide_num) / (double)(y_divide_num);
	}
	if (u_right)
	{
		*u_right = (double)((index % x_divide_num) + 1) / (double)(x_divide_num);
	}
	if (v_bottom)
	{
		*v_bottom = (double)((index / x_divide_num) + 1) / (double)(y_divide_num);
	}
}
extern void find_uv_rectf(
	int index,
	size_t x_divide_num,
	size_t y_divide_num,
	size_t all_frame_num,
	float *u_left,
	float *v_top,
	float *u_right,
	float *v_bottom
)
{
	index %= all_frame_num;
	if (u_left)
	{
		*u_left = (float)(index % x_divide_num) / (float)(x_divide_num);
	}
	if (v_top)
	{
		*v_top = (float)(index / x_divide_num) / (float)(y_divide_num);
	}
	if (u_right)
	{
		*u_right = (float)((index % x_divide_num) + 1) / (float)(x_divide_num);
	}
	if (v_bottom)
	{
		*v_bottom = (float)((index / x_divide_num) + 1) / (float)(y_divide_num);
	}
}

float ClosestPointPointSegment(const D3DXVECTOR3 &a, const D3DXVECTOR3 &b, const D3DXVECTOR3 &c, D3DXVECTOR3 *d)
{
	D3DXVECTOR3 ab = b - a;
	D3DXVECTOR3 ac = c - a;
	float t = D3DXVec3Dot(&ac, &ab) / D3DXVec3LengthSq(&ab);
	//clamp
	if (t < 0.0f)
	{
		t = 0.0f;
	}
	if (t > 1.0f)
	{
		t = 1.0f;
	}
	if (d)
	{
		*d = a + t * ab;
	}
	return t;
}

float SquareDistancePointSegment(const D3DXVECTOR3 &a, const D3DXVECTOR3 &b, const D3DXVECTOR3 &c)
{
	D3DXVECTOR3 ab = b - a;
	D3DXVECTOR3 ac = c - a;
	D3DXVECTOR3 bc = c - b;
	float t = D3DXVec3Dot(&ac, &ab);
	//cがabの外側に射影される
	if (t < 0.0f)
	{
		return D3DXVec3Dot(&ac, &ac);//aがcに最も近い
	}
	float threshold = D3DXVec3Dot(&ab, &ab);
	if (t > threshold)
	{
		return D3DXVec3Dot(&bc, &bc);//bがcに最も近い
	}
	//cがabの内側に射影される
	return D3DXVec3Dot(&ac, &ac) - t * t / threshold;
}
