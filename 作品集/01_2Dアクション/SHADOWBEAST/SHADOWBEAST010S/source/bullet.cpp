
#include <math.h>
#include <stdlib.h>
#include	"amgame.h"
#include "common.h"
#include "bullet.h"
#include "effect_manager.h"
#include "game_state.h"
#include "chara__render.h"


/*バレットシステム*/

typedef enum{
	BULLET_STATE_NONE,
	BULLET_STATE_ACTIVE,
	BULLET_STATE_DEAD,
};

typedef struct tagBullet{
	int state;
	Parameter cenX, cenY;
	double vx, vy;
	RECT hitArea;
	Parameter scale;
	Parameter angle;
	Parameter alpha;
	int time;
	int died;
	int finish;
	DispInfo dispInfo;
}Bullet;

Bullet noneBullet = {
	BULLET_STATE_NONE,{EFFECT_NONE},{EFFECT_NONE},
	0,0,{0,0,0,0},
{EFFECT_NONE},{EFFECT_NONE},{EFFECT_NONE},
0,0,0,dispInfoDefault,
};
Bullet arrayBullet[BULLET_NUM] = {noneBullet};

static int bulletFindSpace(){
	for(int i = 0; i < BULLET_NUM; i++){
		if(arrayBullet[i].state == BULLET_STATE_NONE){
			return i;
		}
	}
	return -1;
}

int setBullet(double x, double y, int blend, int handle){
	int space = bulletFindSpace();
	if(space >= 0){
		Bullet *bullet = &arrayBullet[space];
		int w = 0, h = 0;
		GetPictureSize(handle, &w, &h);
		int w2 = w / 2;
		int h2 = h / 2;
		*bullet = noneBullet;
		bullet->state = BULLET_STATE_ACTIVE;
		bullet->cenX.current = x;
		bullet->cenY.current = y;
		bullet->dispInfo.size.x = w;
		bullet->dispInfo.size.y = h;
		bullet->dispInfo.home.x = w2;
		bullet->dispInfo.home.y = h2;
		bullet->dispInfo.blend = blend;
		bullet->dispInfo.handle = handle;
		bullet->alpha = {EFFECT_NONE,255,255,255};
		bullet->scale = {EFFECT_NONE,1,1,1};
		int X = (int)x;
		int Y = (int)y;
		SetRect(&bullet->hitArea,
				X - w2, Y - h2,
				X + w2, Y + h2);
	}
	return space;
}
int setBulletCoord(int index, double x, double y){
	Bullet *bullet = &arrayBullet[index];
	bullet->cenX.current = x;
	bullet->cenY.current = y;
	int X = (int)x;
	int Y = (int)y;
	int w2 = bullet->dispInfo.size.x / 2;
	int h2 = bullet->dispInfo.size.y / 2;
	SetRect(&bullet->hitArea,
			X - w2, Y - h2,
			X + w2, Y + h2);
	return index;
}
int setBulletVelosity(int index, double vx, double vy){
	Bullet *bullet = &arrayBullet[index];
	bullet->vx = vx;
	bullet->vy = vy;
	return index;
}
int setBulletScale(int index, double scale){
	arrayBullet[index].scale.current = scale;
	return index;
}
int setBulletAngle(int index, double angle){
	arrayBullet[index].angle.current = angle;
	return index;
}
int setBulletAlpha(int index, double alpha){
	arrayBullet[index].alpha.current = alpha;
	return index;
}

int getBulletCoord(int index, double *x, double *y){
	Bullet *bullet = &arrayBullet[index];
	*x = bullet->cenX.current;
	*y = bullet->cenY.current;
	return 0;
}
int getBulletVelosity(int index, double *vx, double *vy){
	Bullet *bullet = &arrayBullet[index];
	*vx = bullet->vx;
	*vy = bullet->vy;
	return 0;
}
int getBulletScale(int index, double *scale){
	*scale = arrayBullet[index].scale.current;
	return index;
}
int getBulletAngle(int index, double *angle){
	*angle = arrayBullet[index].angle.current;
	return index;
}
int getBulletAlpha(int index, double *alpha){
	*alpha = arrayBullet[index].alpha.current;
	return index;
}
bool getBulletEvent(int index, BulletEventType type, int option){
	Bullet *bullet = &arrayBullet[index];
	bool flag = false;
	switch(type){
		case BULLET_E_OUTOF_DISPLAY:
			if(option & (1 << 0))flag |= (bullet->hitArea.top < 0);
			if(option & (1 << 1))flag |= (bullet->hitArea.bottom > DISP_H);
			if(option & (1 << 2))flag |= (bullet->hitArea.left < 0);
			if(option & (1 << 3))flag |= (bullet->hitArea.right > DISP_W);
			break;
		case BULLET_E_HIT_OTHER:
			break;
		default:
			break;
	}
	return flag;
}

int clearBullet(int index, BulletClearType type, int option, int time){
	Bullet *bullet = &arrayBullet[index];
	bullet->state = BULLET_STATE_DEAD;
	bullet->died = getGameFrame();
	bullet->finish = time;
	double alpha = bullet->alpha.current;
	double angle = bullet->angle.current;
	double scale = bullet->scale.current;
	switch(type){
		case BULLET_C_NO_MOTION:
			ParameterSet(&bullet->alpha, EFFECT_NONE, 0, 0, 0);
			break;
		case BULLET_C_FADE:
			ParameterSet(&bullet->alpha, EASE_LINEAR, alpha, alpha, 0);
			break;
		case BULLET_C_FADE_UP:
			ParameterSet(&bullet->cenY, EASE_OUT_QUAD,
						 bullet->cenY.current, bullet->cenY.current,
						 bullet->cenY.current - option);
			ParameterSet(&bullet->scale, EASE_OUT_QUAD,
						 scale, scale, 0.0);
			break;
		default:
			break;
	}
	return 0;
}

int clearBulletAll(){
	for(int i = 0; i < BULLET_NUM; i++){
		clearBullet(i, BULLET_C_NO_MOTION, 0, 0);
	}
	return 0;
}

static void deadBullet_(Bullet *bullet){
	int t = bullet->time - bullet->died;
	int l = bullet->finish;
	ParameterUpdate(&bullet->cenX, t, l);
	ParameterUpdate(&bullet->cenY, t, l);
	ParameterUpdate(&bullet->angle, t, l);
	ParameterUpdate(&bullet->alpha, t, l);
	ParameterUpdate(&bullet->scale, t, l);
	if(t > bullet->finish){
		bullet->state = BULLET_STATE_NONE;
	}
}

static void updataBullet_(int index){
	Bullet *bullet = &arrayBullet[index];
	bullet->time = getGameFrame();
	switch(bullet->state){
		case BULLET_STATE_NONE:
			break;
		case BULLET_STATE_ACTIVE:
			setBulletCoord(index,
						   bullet->cenX.current + bullet->vx,
						   bullet->cenY.current + bullet->vy);
			break;
		case BULLET_STATE_DEAD:
			setBulletCoord(index,
						   bullet->cenX.current + bullet->vx,
						   bullet->cenY.current + bullet->vy);
			deadBullet_(bullet);
			break;
		default:
			break;
	}
}

int updateBullet(){
	for(int i = 0; i < BULLET_NUM; i++){
		updataBullet_(i);
	}
	return 0;
}

static void renderBullet_(int index){
	Bullet *bullet = &arrayBullet[index];
	DispInfo *dispInfo = &bullet->dispInfo;
	dispInfo->disp.x = bullet->cenX.current;
	dispInfo->disp.y = bullet->cenY.current;
	dispInfo->alpha = bullet->alpha.current;
	dispInfo->angle = bullet->angle.current;
	dispInfo->scale.x = bullet->scale.current;
	dispInfo->scale.y = bullet->scale.current;
	charaDispInfoRender(&bullet->dispInfo);
}

int renderBullet(){
	for(int i = 0; i < BULLET_NUM; i++){
		if(arrayBullet[i].state != BULLET_STATE_NONE){
			renderBullet_(i);
		}
	}
	return 0;
}
