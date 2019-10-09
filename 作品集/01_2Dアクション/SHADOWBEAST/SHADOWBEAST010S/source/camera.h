#pragma once



int initCamera();

int moveCamera();

int getCamera(double *x, double *y, double *z, double *homeX, double *homeY);

int correctByCamera(double *x, double *y);
int correctByCamera(int *x, int *y);

int correctByCameraR(double *x, double *y);
int correctByCameraR(int *x, int *y);
