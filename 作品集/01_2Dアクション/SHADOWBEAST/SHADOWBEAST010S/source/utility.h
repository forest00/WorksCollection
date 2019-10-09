#pragma once


typedef struct {
    double current;
    double target;
    double min;
    double max;
} VALUE;

bool hitBox(const RECT *box0, const RECT *box1);
void drawBox(const RECT *box, int c);

void swap(int *x, int *y);
void swap(double *x, double *y);

double spin(double x, double min, double max);

double Lerp(double x, double y, double t);

double Clamp(double x, double min, double max);

double Wrap(double x, double min, double max);

double Follow(double x, double y, double v);


