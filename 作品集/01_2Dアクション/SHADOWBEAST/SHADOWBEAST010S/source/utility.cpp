
#include	<Windows.h>
#include	"amgame.h"
#include	"AmHelper.h"
#include <math.h>

void fade(){

}

bool hitBox(const RECT *box0 , const RECT *box1) {
    bool xf = box0->right > box1->left && box1->right > box0->left;
    bool yf = box0->bottom > box1->top && box1->bottom > box0->top;
    return xf && yf;
}
void drawBox(const RECT *box , int c) {
    int x0 = box->left;
    int y0 = box->top;
    int x1 = box->right;
    int y1 = box->bottom;
    DrawFBox(x0 , y0 , x1 , y0 + 1 , c);
    DrawFBox(x0 , y1 , x1 , y1 - 1 , c);
    DrawFBox(x0 , y0 , x0 + 1 , y1 , c);
    DrawFBox(x1 , y0 , x1 - 1 , y1 , c);
}


void swap(int *x , int *y) {
    int z = *x;
    *x = *y;
    *y = z;
}
void swap(double *x , double *y) {
    double z = *x;
    *x = *y;
    *y = z;
}

double spin(double x , double min , double max) {
    double w = max - min;
    x -= floor((x - min) / w)*w;
    return x;
}

double Lerp(double x , double y , double t) {
    return x + (y - x)*t;
}

double Clamp(double x , double min , double max) {
    if(x < min){
        return min;
    } else if(x > max){
        return max;
    }
    return x;
}

double Wrap(double x , double min , double max) {
    if(x < min){
        return max;
    } else if(x > max){
        return min;
    }
    return x;
}

double Follow(double x , double y , double v) {
    double v_ = y - x;
    double d = fabs(v_);
    if(d > v){
        v_ /= d;
        v_ *= v;
    }
    x += v_;
    return x;
}






