/**
 * @file  easing.cpp
 * イージング
 *
 * @date 2015-06-28
 * @version 1.0
 *
 * Copyright (C) Amusement Media Academy All rights Resved.
 */

#define _USE_MATH_DEFINES // math.hのM_PIを使うため
#include <math.h>

/**
 *  イージング
 */

double Linear(double t, double b, double c, double d) {
    t /= d;
	return c * t + b;
}

double EaseInQuad(double t, double b, double c, double d) {
	t /= d;
	return c * t * t + b;
}
double EaseOutQuad(double t, double b, double c, double d) {
	t /= d;
	return -c * t * (t - 2) + b;
}
double EaseInOutQuad(double t, double b, double c, double d) {
	t /= d / 2.0;
  if (t < 1) {
    return c / 2.0 * t * t + b;
  }
	t--;
	return -c / 2.0 * (t * (t - 2) - 1) + b;
}

double EaseInCubic(double t, double b, double c, double d) {
	t /= d;
	return c*t*t*t + b;
}
double EaseOutCubic(double t, double b, double c, double d) {
	t /= d;
	t--;
	return c*(t*t*t + 1) + b;
}
double EaseInOutCubic(double t, double b, double c, double d) {
	t /= d/2.0;
  if(t < 1) {
    return c / 2.0 * t*t*t + b;
  }
	t -= 2;
	return c/2.0*(t*t*t + 2) + b;
}

double EaseInQuart(double t, double b, double c, double d) {
	t /= d;
	return c*t*t*t*t + b;
}
double EaseOutQuart(double t, double b, double c, double d) {
	t /= d;
	t--;
	return -c * (t*t*t*t - 1) + b;
}
double EaseInOutQuart(double t, double b, double c, double d) {
	t /= d/2.0;
  if(t < 1) {
    return c / 2.0 * t*t*t*t + b;
  }
	t -= 2;
	return -c/2.0 * (t*t*t*t - 2) + b;
}

double EaseInQuint(double t, double b, double c, double d) {
	t /= d;
	return c*t*t*t*t*t + b;
}
double EaseOutQuint(double t, double b, double c, double d) {
	t /= d;
	t--;
	return c*(t*t*t*t*t + 1) + b;
}
double EaseInOutQuint(double t, double b, double c, double d) {
	t /= d/2.0;
  if(t < 1) {
    return c / 2.0 * t*t*t*t*t + b;
  }
	t -= 2;
	return c/2.0*(t*t*t*t*t + 2) + b;
}

double EaseInSine(double t, double b, double c, double d) {
	return -c * cos(t/d * (M_PI/2.0)) + c + b;
}
double EaseOutSine(double t, double b, double c, double d) {
	return c * sin(t/d * M_PI/2.0) + b;
}
double EaseInOutSine(double t, double b, double c, double d) {
	return -c/2.0 * (cos(M_PI*t/d) - 1) + b;
}

double EaseInExpo(double t, double b, double c, double d) {
	return c * pow( 2, 10 * (t/d - 1) ) + b;
}
double EaseOutExpo(double t, double b, double c, double d) {
	return c * ( -pow( 2, -10 * t/d ) + 1 ) + b;
}
double EaseInOutExpo(double t, double b, double c, double d) {
	t /= d/2.0;
  if (t < 1) {
    return c / 2.0 * pow(2, 10 * (t - 1)) + b;
  }
	t--;
	return c/2.0 * (-pow( 2, -10 * t) + 2 ) + b;
}

double EaseInCirc(double t, double b, double c, double d) {
	t /= d;
	return -c * (sqrt(1 - t*t) - 1) + b;
}
double EaseOutCirc(double t, double b, double c, double d) {
	t /= d;
	t--;
	return c * sqrt(1 - t*t) + b;
}
double EaseInOutCirc(double t, double b, double c, double d) {
	t /= d/2.0;
  if (t < 1) {
    return -c / 2.0 * (sqrt(1 - t*t) - 1) + b;
  }
	t -= 2;
	return c/2 * (sqrt(1 - t*t) + 1) + b;
}
