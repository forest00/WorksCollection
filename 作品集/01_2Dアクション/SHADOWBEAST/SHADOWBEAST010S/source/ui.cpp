

#include "amgame.h"
#include "ui.h"

static double lerp(double x, double y, double t){
	return x + t * (y - x);
}

static int hpUpdate(Hp *hp){
	double d = hp->current - hp->drawCurrent;
	hp->drawCurrent += (d / 8.0);
	return 0;
}

static int uiHpGaugeMeterDraw(Vec *pos, HpBarGaugeMeter *meter, int currentHp, int maxHp){
	double t = (double)currentHp / (double)maxHp;
	int meterWidth = meter->endOffset.x - meter->startOffset.x;
	int handle = meter->handle;
	Vec startPos = {}, endPos = {};
	startPos.x = pos->x + meter->startOffset.x;
	startPos.y = pos->y + meter->startOffset.y;
	endPos.x = pos->x + meter->endOffset.x;
	endPos.y = pos->y + meter->endOffset.y;
	SetDrawArea(startPos.x, startPos.y, startPos.x + (int)(t * meterWidth), endPos.y);
	DrawMemThExp(startPos.x, startPos.y, endPos.x, endPos.y, handle);
	SetDrawArea(0, 0, 1280, 720);
	return 0;
}

static int uiHpGaugeMeterDraw(Vec *pos, HpCircleGaugeMeter *meter, int currentHp, int maxHp){
	double t = (double)currentHp / (double)maxHp;
	Vec drawCenterPos = {};
	drawCenterPos.x = pos->x + meter->offset.x;
	drawCenterPos.y = pos->y + meter->offset.y;
	int r = meter->radius;
	double angle = lerp(meter->emptyRad, meter->fullRad, t);
	angle -= (M_PI_2 + M_PI_4);
	double q = sqrt(r * r + r * r);
	int x1 = (int)(drawCenterPos.x + q * cos(angle + M_PI_2 * 0));//左上
	int y1 = (int)(drawCenterPos.y + q * sin(angle + M_PI_2 * 0));
	int x2 = (int)(drawCenterPos.x + q * cos(angle + M_PI_2 * 1));//右上
	int y2 = (int)(drawCenterPos.y + q * sin(angle + M_PI_2 * 1));
	int x3 = (int)(drawCenterPos.x + q * cos(angle + M_PI_2 * 3));//左下
	int y3 = (int)(drawCenterPos.y + q * sin(angle + M_PI_2 * 3));
	int x4 = (int)(drawCenterPos.x + q * cos(angle + M_PI_2 * 2));//右下
	int y4 = (int)(drawCenterPos.y + q * sin(angle + M_PI_2 * 2));
	int handle = meter->handle;
	DrawMemThFlex(x1, y1, x2, y2, x3, y3, x4, y4, handle);
	return 0;
}

static int uiHpGaugeFrameDraw(Vec *pos, int frameHandle){
	Vec drawPos = *pos;
	DrawMemTh(drawPos.x, drawPos.y, frameHandle);
	return 0;
}

extern int uiDrawPre(){
	SetDrawMode(AMDRAW_NOBLEND, 0);
	SetDrawBright(1000, 1000, 1000);
	return 0;
}

//棒ゲージ-------------------------------------------------↓
extern int uiHpGaugeUpdate(HpBarGaugeUi *ui){
	hpUpdate(&ui->hp);
	return 0;
}


extern int uiHpGaugeDraw(HpBarGaugeUi *ui){
	SetDrawMode(AMDRAW_NOBLEND, 0);
	SetDrawBright(1000, 1000, 1000);
	uiHpGaugeMeterDraw(&ui->pos, &ui->meter, (int)(ui->hp.drawCurrent), ui->hp.max);
	uiHpGaugeFrameDraw(&ui->pos, ui->frameHandle);
	return 0;
}
//棒ゲージ-------------------------------------------------↑

//円ゲージ-------------------------------------------------↓
extern int uiHpGaugeUpdate(HpCircleGaugeUi *ui){
	hpUpdate(&ui->hp);
	return 0;
}

extern int uiHpGaugeDraw(HpCircleGaugeUi *ui){
	SetDrawMode(AMDRAW_NOBLEND, 0);
	SetDrawBright(1000, 1000, 1000);
	uiHpGaugeFrameDraw(&ui->pos, ui->frameHandle);
	uiHpGaugeMeterDraw(&ui->pos, &ui->meter, (int)(ui->hp.drawCurrent), ui->hp.max);
	return 0;
}
//円ゲージ-------------------------------------------------↑



//顔アイコン-------------------------------------------------↓
extern int uiFaceIconUpdate(FaceIconUi *ui){
	ui;
	return 0;
}

extern int uiFaceIconDraw(FaceIconUi *ui){
	int x1 = ui->pos.x;
	int y1 = ui->pos.y;
	int x2 = x1 + ui->size.x;
	int y2 = y1 + ui->size.y;
	int handle = ui->handle;
	DrawMemThExp(x1, y1, x2, y2, handle);
	return 0;
}
//顔アイコン-------------------------------------------------↑



