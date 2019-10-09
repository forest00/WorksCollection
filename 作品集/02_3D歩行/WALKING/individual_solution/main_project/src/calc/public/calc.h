#pragma once

//ここでは、さまざまな計算をしてくれます




//正の数が渡されたら1、
//負の数が渡されたら-1、
//0が渡されたら0を返す関数
extern int sign(double value);

//文字列中に含まれる最初のターゲット文字をヌル文字に置き換える
extern void change_target_to_null(char *str, size_t chars, const char *target);

//bool値の反転
extern  bool bool_invers(bool b);

//[0.0, 1.0)の一様乱数
extern  double uniform_rand();

//[minimum, maximum)の一様乱数
extern  double uniform_rand(double minimum, double maximum);


//スワップ
void swap(int *x, int *y);
void swap(double *x, double *y);

//線形補間
double lerp(double x, double y, double t);

//クランプ
double clamp(double x, double min, double max);

//ラッピング
double wrap(double x, double min, double max);

//追従
double follow(double x, double y, double v);

