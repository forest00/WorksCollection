
#include <time.h>
#include <math.h>
#include <Windows.h>
#include "amgame.h"

#include "game_state.h"
#include "input_manager.h"
#include "type_parameter.h"

#include "chara__common.h"
#include "chara__manager.h"
#include "common.h"
#include "stage_data_reader.h"

#include "forDebug.h"

static int margin = 0;
static double xLower = 0;
static double yLower = 0;
static double zLower = 0;
static double xUpper = 0;
static double yUpper = 0;
static double zUpper = 0;


typedef struct{
	Parameter x, y, z;
	XYd home;
}CAMERA;


static CAMERA camera = {
	{EASE_OUT_CIRC,0,0,0},
	{EASE_OUT_CIRC,0,0,0},
	{EASE_OUT_CIRC,1,1,1},
	{0,0},
};

static void calcCameraArea(Character _array[], int num, double *x0, double *y0, double *x1, double *y1){
	double left, top, right, bottom;
	for(int i = 0; i < num; i++){
		pCharacter chara = &_array[i];
		if(getCharaExist(chara)){
			getCharaHitArea(chara, &left, &top, &right, &bottom);
			*x0 = min(*x0, left);
			*y0 = min(*y0, top);
			*x1 = max(*x1, right);
			*y1 = max(*y1, bottom);
		}
	}
}


int initCamera(){
	margin = 200;
	xLower = 0;
	yLower = 0;
	zLower = 1.0;
	xUpper = WORLD_WIDTH - DISP_W;
	yUpper = 0;
	zUpper = (double)(WORLD_WIDTH) / DISP_W;
	return 0;
}


int correctByCamera(double *x, double *y);

int moveCamera(){
	double x0 = (double)INT_MAX;
	double y0 = (double)INT_MAX;
	double x1 = (double)INT_MIN;
	double y1 = (double)INT_MIN;
	calcCameraArea(player1, PLAYER1_NUM, &x0, &y0, &x1, &y1);
	calcCameraArea(player2, PLAYER2_NUM, &x0, &y0, &x1, &y1);
	calcCameraArea(enemies, ENEMY_NUM, &x0, &y0, &x1, &y1);
	calcCameraArea(enemyHoles, ENEMY_NUM, &x0, &y0, &x1, &y1);
	calcCameraArea(thunder, THUNDER_NUM, &x0, &y0, &x1, &y1);
	calcCameraArea(hpHealItem, HP_HEAL_ITEM_NUM, &x0, &y0, &x1, &y1);
	x0 -= margin;
	x1 += margin;
	if(x0 < 0){ x0 = 0; }
	if(x1 > WORLD_WIDTH){ x1 = WORLD_WIDTH; }
	double w = x1 - x0;
	double h = y1 - y0;
	camera.x.stop = x0 + w / 2.0 - DISP_CENTER_X;
	camera.y.stop = 0;
	camera.z.stop = max(w / DISP_W, h / DISP_H);
	camera.x.stop = Clamp(camera.x.stop, xLower, xUpper);
	camera.y.stop = Clamp(camera.y.stop, yLower, yUpper);
	camera.z.stop = Clamp(camera.z.stop, zLower, zUpper);
	ParameterUpdate(&camera.x, 1, 64);
	ParameterUpdate(&camera.y, 1, 64);
	ParameterUpdate(&camera.z, 1, 64);
	camera.x.start = camera.x.current;
	camera.y.start = camera.y.current;
	camera.z.start = camera.z.current;
	camera.home.x = Lerp(0, DISP_W, camera.x.current / xUpper);
	camera.home.y = GROUND_Y;
	return 0;
}

int getCamera(double *x, double *y, double *z, double *homeX, double *homeY){
	*x = camera.x.current;
	*y = camera.y.current;
	*z = camera.z.current;
	*homeX = camera.home.x;
	*homeY = camera.home.y;
	return 0;
}

int correctByCamera(double *x, double *y){
	const double cx = camera.x.current;
	const double cy = camera.y.current;
	const double cz = camera.z.current;
	const double homeX = camera.home.x;
	const double homeY = camera.home.y;
	*x -= cx;
	*y -= cy;
	double dx = *x - homeX;
	double dy = *y - homeY;
	double t = 1.0 / cz;
	*x = homeX + dx * t;
	*y = homeY + dy * t;
	return 0;
}
int correctByCamera(int *x, int *y){
	double dx = *x;
	double dy = *y;
	correctByCamera(&dx, &dy);
	*x = (int)(dx);
	*y = (int)(dy);
	return 0;
}

int correctByCameraR(double *x, double *y){
	const double cx = camera.x.current;
	const double cy = camera.y.current;
	const double cz = camera.z.current;
	const double homeX = camera.home.x;
	const double homeY = camera.home.y;
	double dx = *x - homeX;
	double dy = *y - homeY;
	double t = cz;
	*x = homeX + dx * t;
	*y = homeY + dy * t;
	*x += cx;
	*y += cy;
	return 0;
}
int correctByCameraR(int *x, int *y){
	double dx = *x;
	double dy = *y;
	correctByCameraR(&dx, &dy);
	*x = (int)(dx);
	*y = (int)(dy);
	return 0;
}
