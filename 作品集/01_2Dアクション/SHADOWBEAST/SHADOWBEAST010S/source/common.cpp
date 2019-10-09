#include "common.h"
#include "amgame.h"
#include <stdlib.h>


int BoxToRect(int x, int y, const pBox in, pRect out){
	out->left = x + in->relative.x;
	out->top = y + in->relative.y;
	out->right = out->left + in->size.x;
	out->bottom = out->top + in->size.y;
	return 0;
}

int drawBox(int left, int top, int right, int bottom, int color){
	DrawFBox(left, top, left + 1, bottom, color);
	DrawFBox(right, top, right - 1, bottom, color);
	DrawFBox(left, top, right, top + 1, color);
	DrawFBox(left, bottom, right, bottom - 1, color);
	return 0;
}
int drawBox(int x, int y, const pBox in, int color){
	int left = x + in->relative.x;
	int top = y + in->relative.y;
	int right = left + in->size.x;
	int bottom = top + in->size.y;
	return drawBox(left, top, right, bottom, color);
}
int drawBox(int x, int y, const Box *in, int color){
	int left = x + in->relative.x;
	int top = y + in->relative.y;
	int right = left + in->size.x;
	int bottom = top + in->size.y;
	return drawBox(left, top, right, bottom, color);
}
int drawBox(const pRect in, int color){
	return drawBox(in->left, in->top, in->right, in->bottom, color);
}
int drawBox(const Rect *in, int color){
	return drawBox(in->left, in->top, in->right, in->bottom, color);
}

bool isHitBox(int left0, int top0, int right0, int bottom0,
			  int left1, int top1, int right1, int bottom1) {
	bool x = false, y = false;
	// x•ûŒü‚Ì”»’è
	if(left0 < right0 || left1 < right1){
		x = left0 < right1 && left1 < right0;
	} else{
		x = false;
	}
	// y•ûŒü‚Ì”»’è
	if(top0 < bottom0 || top1 < bottom1){
		y = top0 < bottom1 && top1 < bottom0;
	} else{
		y = false;
	}
	return x && y;
}

bool isHitBox(int left0, int top0, int right0, int bottom0,
			  int x1, int y1, const pBox in1){
	pRect r1 = {0};
	BoxToRect(x1, y1, in1, r1);
	return isHitBox(left0, top0, right0, bottom0,
					r1->left, r1->top, r1->right, r1->bottom);
}
bool isHitBox(int left0, int top0, int right0, int bottom0,
			  int x1, int y1, const Box *in1){
	pRect r1 = {0};
	BoxToRect(x1, y1, (pBox)in1, r1);
	return isHitBox(left0, top0, right0, bottom0,
					r1->left, r1->top, r1->right, r1->bottom);
}

bool isHitBox(int left0, int top0, int right0, int bottom0,
			  const pRect in1){
	return isHitBox(left0, top0, right0, bottom0,
					in1->left, in1->top, in1->right, in1->bottom);
}
bool isHitBox(int left0, int top0, int right0, int bottom0,
			  const Rect *in1){
	return isHitBox(left0, top0, right0, bottom0,
					in1->left, in1->top, in1->right, in1->bottom);
}

bool isHitBox(int x0, int y0, const pBox in0,
			  int x1, int y1, const pBox in1){
	pRect r0 = {0}, r1 = {0};
	BoxToRect(x0, y0, in0, r0);
	BoxToRect(x1, y1, in1, r1);
	return isHitBox(r0->left, r0->top, r0->right, r0->bottom,
					r1->left, r1->top, r1->right, r1->bottom);
}
bool isHitBox(int x0, int y0, const Box *in0,
			  int x1, int y1, const pBox in1){
	pRect r0 = {0}, r1 = {0};
	BoxToRect(x0, y0, (pBox)in0, r0);
	BoxToRect(x1, y1, (pBox)in1, r1);
	return isHitBox(r0->left, r0->top, r0->right, r0->bottom,
					r1->left, r1->top, r1->right, r1->bottom);
}
bool isHitBox(int x0, int y0, const Box *in0,
			  int x1, int y1, const Box *in1){
	pRect r0 = {0}, r1 = {0};
	BoxToRect(x0, y0, (pBox)in0, r0);
	BoxToRect(x1, y1, (pBox)in1, r1);
	return isHitBox(r0->left, r0->top, r0->right, r0->bottom,
					r1->left, r1->top, r1->right, r1->bottom);
}

bool isHitBox(int x0, int y0, const pBox in0,
			  const pRect in1){
	pRect r0 = {0};
	BoxToRect(x0, y0, in0, r0);
	return isHitBox(r0->left, r0->top, r0->right, r0->bottom,
					in1->left, in1->top, in1->right, in1->bottom);
}
bool isHitBox(int x0, int y0, const Box *in0,
			  const pRect in1){
	pRect r0 = {0};
	BoxToRect(x0, y0, (pBox)in0, r0);
	return isHitBox(r0->left, r0->top, r0->right, r0->bottom,
					in1->left, in1->top, in1->right, in1->bottom);
}
bool isHitBox(int x0, int y0, const pBox in0,
			  const Rect *in1){
	pRect r0 = {0};
	BoxToRect(x0, y0, in0, r0);
	return isHitBox(r0->left, r0->top, r0->right, r0->bottom,
					in1->left, in1->top, in1->right, in1->bottom);
}
bool isHitBox(int x0, int y0, const Box *in0,
			  const Rect *in1){
	pRect r0 = {0};
	BoxToRect(x0, y0, (pBox)in0, r0);
	return isHitBox(r0->left, r0->top, r0->right, r0->bottom,
					in1->left, in1->top, in1->right, in1->bottom);
}

bool isHitBox(const pRect in0,
			  const pRect in1){
	return isHitBox(in0->left, in0->top, in0->right, in0->bottom,
					in1->left, in1->top, in1->right, in1->bottom);
}
bool isHitBox(const Rect *in0,
			  const pRect in1){
	return isHitBox(in0->left, in0->top, in0->right, in0->bottom,
					in1->left, in1->top, in1->right, in1->bottom);
}
bool isHitBox(const pRect in0,
			  const Rect *in1){
	return isHitBox(in0->left, in0->top, in0->right, in0->bottom,
					in1->left, in1->top, in1->right, in1->bottom);
}
bool isHitBox(const Rect *in0,
			  const Rect *in1){
	return isHitBox(in0->left, in0->top, in0->right, in0->bottom,
					in1->left, in1->top, in1->right, in1->bottom);
}

double toRadian(double angle){
	return (angle / 180.0 * PI);
}

void swap(int *x, int *y) {
	int z = *x;
	*x = *y;
	*y = z;
}
void swap(double *x, double *y) {
	double z = *x;
	*x = *y;
	*y = z;
}

double spin(double x, double min, double max) {
	double w = max - min;
	x -= floor((x - min) / w)*w;
	return x;
}

double Lerp(double x, double y, double t) {
	return x + (y - x)*t;
}

double Clamp(double x, double min, double max) {
	if(x < min){
		return min;
	} else if(x > max){
		return max;
	}
	return x;
}

double Wrap(double x, double min, double max) {
	if(x < min){
		return max;
	} else if(x > max){
		return min;
	}
	return x;
}

double Follow(double x, double y, double v) {
	double v_ = y - x;
	double d = fabs(v_);
	if(d > v){
		v_ /= d;
		v_ *= v;
	}
	x += v_;
	return x;
}

double rand0to1(){
	return (double)rand() / RAND_MAX;
}