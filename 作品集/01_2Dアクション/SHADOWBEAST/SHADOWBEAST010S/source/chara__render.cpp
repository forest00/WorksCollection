
#include <math.h>
#include <Windows.h>
#include "amgame.h"

#include "camera.h"
#include "chara__render.h"
#include "option.h"








int charaDispInfoInit(pDrawInfo target) {
    *target = dispInfoDefault;
	return 0;
}


int charaDispInfoOffSet(pDrawInfo target, int dispX, int dispY) {
	target->pos.x = dispX;
	target->pos.y = dispY;
	return 0;
}

int charaDispInfoHome(pDrawInfo target, int cenX, int cenY) {
	target->home.x = cenX;
	target->home.y = cenY;
	return 0;
}


int charaDispInfoScale(pDrawInfo target, double scaleX, double scaleY) {
	target->scale.x = scaleX;
	target->scale.y = scaleY;
	return 0;
}

int charaDispInfoRot(pDrawInfo target, double angle) {
	target->angle = angle;
	return 0;
}

int charaDispInfoColor(pDrawInfo target, int blend, int alpha,
                       int red, int green, int blue){
    target->blend = blend;
    target->alpha = alpha;
    target->red = red;
    target->green = green;
    target->blue = blue;
    return 0;
}

int charaDispInfoHandle(pDrawInfo target, int handle) {
	target->handle = handle;
	return 0;
}


int charaDispInfoFlip(pDrawInfo target, bool flagX, bool flagY) {
	target->flipFlagX = flagX;
	target->flipFlagY = flagY;
	return 0;
}


int charaDispInfoRender(pDrawInfo target) {
	if(target->doHide){
		return -1;
	}
	int x = (int)(target->pos.x) + target->relative.x + target->quake.x;
	int y = (int)(target->pos.y) + target->relative.y + target->quake.y;
    int h = target->handle;
    int cx = x + target->home.x;
    int cy = y + target->home.y;
    double left = x;
    double top = y;
    double right = x + target->size.x;
    double bottom = y + target->size.y;
    if(target->flipFlagX){
        double temp = left;
        left = right;
        right = temp;
    }
    double sx = target->scale.x;
    double sy = target->scale.y;
    double ang = target->angle;

    double dx0 = (left - cx)*sx;
    double dy0 = (top - cy)*sy;
    double dx1 = (right - cx)*sx;
    double dy1 = (bottom - cy)*sy;

    double d0 = hypot(dx0, dy0);
    double d1 = hypot(dx1, dy0);
    double d2 = hypot(dx0, dy1);
    double d3 = hypot(dx1, dy1);

    double a0 = atan2(dy0, dx0) + ang;
    double a1 = atan2(dy0, dx1) + ang;
    double a2 = atan2(dy1, dx0) + ang;
    double a3 = atan2(dy1, dx1) + ang;

    double x1 = cx + cos(a0)*d0;//左上
    double y1 = cy + sin(a0)*d0;
    double x2 = cx + cos(a1)*d1;//右上
    double y2 = cy + sin(a1)*d1;
    double x3 = cx + cos(a2)*d2;//左下
    double y3 = cy + sin(a2)*d2;
    double x4 = cx + cos(a3)*d3;//右下
    double y4 = cy + sin(a3)*d3;

	double gameBright = 1000.0;//getOptionBright();
    //カメラ位置に応じて表示座標を変換
    correctByCamera(&x1, &y1);
    correctByCamera(&x2, &y2);
    correctByCamera(&x3, &y3);
    correctByCamera(&x4, &y4);
    SetDrawMode(target->blend, target->alpha);
    SetDrawBright((int)(target->red / 255.0 * gameBright),
                  (int)(target->green / 255.0 * gameBright),
                  (int)(target->blue / 255.0 * gameBright));
    DrawMemFlex((int)x1, (int)y1,
                (int)x2, (int)y2,
                (int)x3, (int)y3,
                (int)x4, (int)y4,
                h);
    return 0;
}













