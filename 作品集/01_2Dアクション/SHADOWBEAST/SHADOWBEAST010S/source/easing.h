#pragma once
/**
 * @file  easing.h
 * �C�[�W���O
 *
 * @date 2015-06-28
 * @version 1.0
 *
 * Copyright (C) Amusement Media Academy All rights Resved.
 */
double Linear(double t, double b, double c, double d);

/**
 *  �C�[�W���O
 *	@param t �J�����g�^�C��
 *	@param b �J�n�l
 *	@param c �S�̂̕ω���
 *	@param d �f�����[�V����
 *	@return  �v�Z����
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