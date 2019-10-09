#pragma once

struct Vector2D
{
	double x;
	double y;
};

//���e�������擾
extern Vector2D Vector2D_Literal(double x, double y);

//�����_���ȒP�ʃx�N�g�����擾
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

//�����̓��
extern double LengthSquare(Vector2D a);

//����
extern double Length(Vector2D a);

//��]
extern Vector2D Rot(Vector2D a, double theta);
