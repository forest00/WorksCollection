#pragma once

struct Vector2D
{
	double x;
	double y;
};

//リテラルを取得
extern Vector2D Vector2D_Literal(double x, double y);

//ランダムな単位ベクトルを取得
extern Vector2D Vector2D_UniformNormal();

//a + b
extern Vector2D Add(Vector2D a, Vector2D b);

//a - b
extern Vector2D Sub(Vector2D a, Vector2D b);

//a * (double)
extern Vector2D Multi(Vector2D a, double b);

//a / (double)
extern Vector2D Div(Vector2D a, double b);

//a * b
extern double Dot(Vector2D a, Vector2D b);

//長さの二乗
extern double LengthSquare(Vector2D a);

//長さ
extern double Length(Vector2D a);

//回転
extern Vector2D Rot(Vector2D a, double theta);
