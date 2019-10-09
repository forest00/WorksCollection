#pragma once
//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	
//	更新履歴
//	2019/01/25...極座標→直交座標の変換関数を追加
//	

//ここでは、さまざまな計算をします

//DirectX
#include <d3d9.h>
#include <d3dx9.h>
//とりあえず数学関数をインクルード
#define _USE_MATH_DEFINES
#include <math.h>
#undef _USE_MATH_DEFINES

//正の数が渡されたら1、
//負の数が渡されたら-1、
//0が渡されたら0を返す関数
extern int sign(double value);
extern int signf(float value);

//文字列中に含まれる最初のターゲット文字をヌル文字に置き換える
extern void change_target_to_null(char *str, size_t chars, const char *target);

//[0.0, 1.0)の一様乱数
extern double uniform_rand();
extern float uniform_randf();


//[minimum, maximum)の一様乱数
extern double uniform_rand(double minimum, double maximum);
extern float uniform_randf(float minimum, float maximum);


//スワップ
extern void swap(int *x, int *y);
extern void swap(float *x, float *y);
extern void swap(double *x, double *y);

//線形補間
extern double lerp(double x, double y, double t);
extern float lerpf(float x, float y, float t);

//クランプ
extern double clamp(double x, double min, double max);
extern float clampf(float x, float min, float max);

//ラップ
extern double wrap(double x, double min, double max);
extern float wrapf(float x, float min, float max);

//追従
extern double follow(double x, double y, double v);
extern float followf(float x, float y, float v);


//マイナスπからプラスπの数値にしたものを返します
extern double angle_normalize(double angle);
extern float angle_normalizef(float angle);
//目標の角度に近づけたものを返します
extern double angle_follow(double angle, double target, double turning_force);
extern float angle_followf(float angle, float target, float turning_force);


//極座標から直交座標に変換する
void to_xyz(float *x, float *y, float *z, float r, float theta, float phi);


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
*@brief 与えられた線分ABに対して、点CからABへの最近接点Dを計算する
*@return D(t) = A + t * (B - A)により表されるDの位置に対するtを返す
*/
float ClosestPointPointSegment(const D3DXVECTOR3 &a, const D3DXVECTOR3 &b, const D3DXVECTOR3 &c, D3DXVECTOR3 *d);

/*
*@fn SquareDistancePointSegment
*@brief 点Cから線分ABへの距離の二乗を計算する
*/
float SquareDistancePointSegment(const D3DXVECTOR3 &a, const D3DXVECTOR3 &b, const D3DXVECTOR3 &c);
