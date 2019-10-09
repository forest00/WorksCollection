#include "vector2d.h"
#include "random.h"
#define _USE_MATH_DEFINES
#include <math.h>
#undef _USE_MATH_DEFINES

extern Vector2D Vector2D_Literal(double x, double y)
{
	Vector2D a;
	a.x = x;
	a.y = y;
	return a;
}

extern Vector2D Vector2D_UniformNormal()
{
	Vector2D a;
	double r = UniformRandom() * M_PI * 2.0;
	a.x = cos(r);
	a.y = sin(r);
	return a;
}

extern Vector2D Add(Vector2D a, Vector2D b)
{
	Vector2D c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	return c;
}

extern Vector2D Sub(Vector2D a, Vector2D b)
{
	Vector2D c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	return c;
}

extern Vector2D Multi(Vector2D a, double b)
{
	Vector2D c;
	c.x = a.x * b;
	c.y = a.y * b;
	return c;
}

extern Vector2D Div(Vector2D a, double b)
{
	Vector2D c;
	c.x = a.x / b;
	c.y = a.y / b;
	return c;
}

extern double Dot(Vector2D a, Vector2D b)
{
	return a.x * b.x + a.y * b.y;
}

extern double LengthSquare(Vector2D a)
{
	return Dot(a, a);
}

extern double Length(Vector2D a)
{
	return sqrt(LengthSquare(a));
}

extern Vector2D Rot(Vector2D a, double theta)
{
	Vector2D b;
	b.x = a.x * cos(theta) - a.y * sin(theta);
	b.y = a.y * sin(theta) + a.y * cos(theta);
	return b;
}
