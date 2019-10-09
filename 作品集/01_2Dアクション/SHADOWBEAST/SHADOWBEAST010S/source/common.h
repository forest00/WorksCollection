#pragma once

#define PI 3.141592653589793

typedef enum{
	LEFT,
	RIGHT,
}Direction;

typedef struct tagXYi{
	int x;
	int y;
}XYi, *pXYi;

typedef struct tagXYd{
	double x;
	double y;
}XYd, *pXYd;

typedef struct tagBox{
	XYi relative;
	XYi size;
}Box, *pBox;

typedef struct tagRect{
	int left;
	int top;
	int right;
	int bottom;
}Rect, *pRect;

//utitlityä÷êîíËã`

int BoxToRect(int x, int y, const pBox in, pRect out);

int drawBox(int left, int top, int right, int bottom, int color);
int drawBox(int x, int y, const pBox in, int color);
int drawBox(int x, int y, const Box *in, int color);
int drawBox(const pRect in, int color);
int drawBox(const Rect *in, int color);

bool isHitBox(int left0, int top0, int right0, int bottom0,
			  int left1, int top1, int right1, int bottom1);
bool isHitBox(int left0, int top0, int right0, int bottom0,
			  int x1, int y1, const pBox in1);
bool isHitBox(int left0, int top0, int right0, int bottom0,
			  int x1, int y1, const Box *in1);
bool isHitBox(int left0, int top0, int right0, int bottom0,
			  const pRect in1);
bool isHitBox(int left0, int top0, int right0, int bottom0,
			  const Rect *in1);
bool isHitBox(int x0, int y0, const pBox in0,
			  int x1, int y1, const pBox in1);
bool isHitBox(int x0, int y0, const Box *in0,
			  int x1, int y1, const pBox in1);
bool isHitBox(int x0, int y0, const Box *in0,
			  int x1, int y1, const Box *in1);
bool isHitBox(int x0, int y0, const pBox in0,
			  const pRect in1);
bool isHitBox(int x0, int y0, const Box *in0,
			  const pRect in1);
bool isHitBox(int x0, int y0, const pBox in0,
			  const Rect *in1);
bool isHitBox(int x0, int y0, const Box *in0,
			  const Rect *in1);
bool isHitBox(const pRect in0,
			  const pRect in1);
bool isHitBox(const Rect *in0,
			  const pRect in1);
bool isHitBox(const pRect in0,
			  const Rect *in1);
bool isHitBox(const Rect *in0,
			  const Rect *in1);

double toRadian(double angle);

void swap(int *x, int *y);
void swap(double *x, double *y);

double spin(double x, double min, double max);

double Lerp(double x, double y, double t);

double Clamp(double x, double min, double max);

double Wrap(double x, double min, double max);

double Follow(double x, double y, double v);

double rand0to1();