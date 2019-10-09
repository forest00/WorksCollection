#pragma once

#define BULLET_NUM 1024


/* バレットシステム */

typedef enum{
	BULLET_E_OUTOF_DISPLAY,
	BULLET_E_HIT_OTHER,
}BulletEventType;

typedef enum{
	BULLET_C_NO_MOTION,
	BULLET_C_FADE,
	BULLET_C_FADE_UP,
}BulletClearType;

int setBullet(double x, double y, int blend, int handle);

int setBulletCoord(int index, double x, double y);
int setBulletVelosity(int index, double vx, double vy);
int setBulletScale(int index, double scale);
int setBulletAngle(int index, double angle);
int setBulletAlpha(int index, double alpha);

int getBulletCoord(int index, double *x, double *y);
int getBulletVelosity(int index, double *vx, double *vy);
int getBulletScale(int index, double *scale);
int getBulletAngle(int index, double *angle);
int getBulletAlpha(int index, double *alpha);

bool getBulletEvent(int index, BulletEventType type, int option);

int clearBullet(int index, BulletClearType type, int option, int time);

int clearBulletAll();

int updateBullet();

int renderBullet();




