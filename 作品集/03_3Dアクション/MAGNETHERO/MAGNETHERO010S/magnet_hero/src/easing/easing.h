#pragma once
/**
 * @file  easing.h
 * イージング
 *
 * @date 2015-06-28
 * @version 1.0
 *
 * Copyright (C) Amusement Media Academy All rights Resved.
 */
double Linear(double t, double b, double c, double d);

/**
 *  イージング
 *	@param t カレントタイム
 *	@param b 開始値
 *	@param c 全体の変化量
 *	@param d デュレーション
 *	@return  計算結果
 */
double EaseInQuad(double t, double b, double c, double d);
double EaseOutQuad(double t, double b, double c, double d);
double EaseInOutQuad(double t, double b, double c, double d);

double EaseInCubic(double t, double b, double c, double d);
double EaseOutCubic(double t, double b, double c, double d);
double EaseInOutCubic(double t, double b, double c, double d);

double EaseInQuart(double t, double b, double c, double d);
double EaseOutQuart(double t, double b, double c, double d);
double EaseInOutQuart(double t, double b, double c, double d);

double EaseInQuint(double t, double b, double c, double d);
double EaseOutQuint(double t, double b, double c, double d);
double EaseInOutQuint(double t, double b, double c, double d);

double EaseInSine(double t, double b, double c, double d);
double EaseOutSine(double t, double b, double c, double d);
double EaseInOutSine(double t, double b, double c, double d);

double EaseInExpo(double t, double b, double c, double d);
double EaseOutExpo(double t, double b, double c, double d);
double EaseInOutExpo(double t, double b, double c, double d);

double EaseInCirc(double t, double b, double c, double d);
double EaseOutCirc(double t, double b, double c, double d);
double EaseInOutCirc(double t, double b, double c, double d);