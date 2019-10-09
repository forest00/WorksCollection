

#include "amgame.h"
#include "image_manager.h"
#include "ui.h"




struct UserInterface{
	HpBarGaugeUi bar1p;
	HpBarGaugeUi bar2p;
	FaceIconUi face1p;
	FaceIconUi face2p;
	HpCircleGaugeUi circleCastle;
	bool isBossStage;
	HpBarGaugeUi barBoss;
};

static UserInterface ui = {};


static double rad(double x){
	return M_PI * (x / 180.0);
}

extern int uiMgrTeach1pHp(int currentHp, int maxHp){
	ui.bar1p.hp.current = currentHp;
	ui.bar1p.hp.max = maxHp;
	return 0;
}

extern int uiMgrTeach2pHp(int currentHp, int maxHp){
	ui.bar2p.hp.current = currentHp;
	ui.bar2p.hp.max = maxHp;
	return 0;
}

extern int uiMgrTeachCastleHp(int currentHp, int maxHp){
	ui.circleCastle.hp.current = currentHp;
	ui.circleCastle.hp.max = maxHp;
	return 0;
}

extern int uiMgrTeachBossHp(int currentHp, int maxHp){
	ui.barBoss.hp.current = currentHp;
	ui.barBoss.hp.max = maxHp;
	return 0;
}

extern int uiMgrTeachBossStage(bool isBossStage){
	ui.isBossStage = isBossStage;
	return 0;
}


extern int uiMgrInit(){
	//set up
	HpBarGaugeUi *pl1 = &ui.bar1p;
	HpBarGaugeUi *pl2 = &ui.bar2p;
	FaceIconUi *face1p = &ui.face1p;
	FaceIconUi *face2p = &ui.face2p;
	HpCircleGaugeUi *circle = &ui.circleCastle;
	HpBarGaugeUi *boss = &ui.barBoss;
	//1p bar
	///pos
	pl1->pos.x = 80;														//左上位置
	pl1->pos.y = 20;														//
																			///meter
	pl1->meter.startOffset.x = 41;											//オフセット
	pl1->meter.startOffset.y = 15;											//
	pl1->meter.endOffset.x = 420;											//
	pl1->meter.endOffset.y = 28;											//
	pl1->meter.handle = imageMgrGetHandle(IMAGE_HP_METER,0);				//画像ハンドル
																			///frame
	pl1->frameHandle = imageMgrGetHandle(IMAGE_HP_BAR_FRAME_1P, 0);			//画像ハンドル

	//2p bar
	///pos
	pl2->pos.x = 80;														//左上位置
	pl2->pos.y = 120;														//
																			///meter
	pl2->meter.startOffset.x = 41;											//オフセット
	pl2->meter.startOffset.y = 15;											//
	pl2->meter.endOffset.x = 420;											//
	pl2->meter.endOffset.y = 28;											//
	pl2->meter.handle = imageMgrGetHandle(IMAGE_HP_METER, 0);				//画像ハンドル
																			///frame
	pl2->frameHandle = imageMgrGetHandle(IMAGE_HP_BAR_FRAME_2P, 0);			//画像ハンドル

	//1p icon
	face1p->handle = imageMgrGetHandle(IMAGE_1P_FACE_ICON, 0);				//画像のハンドル
	face1p->size.x = 70;													//画像の大きさ
	face1p->size.y = 100;													//画像の大きさ
	face1p->pos.x = pl1->pos.x - face1p->size.x;							//ｘ座標
	face1p->pos.y = pl1->pos.y;												//ｙ座標

	//2p icon
	face2p->handle = imageMgrGetHandle(IMAGE_2P_FACE_ICON, 0);				//画像のハンドル
	face2p->size.x = 70;													//画像の大きさ
	face2p->size.y = 100;													//画像の大きさ
	face2p->pos.x = pl2->pos.x - face2p->size.x;							//ｘ座標
	face2p->pos.y = pl2->pos.y;												//ｙ座標

	//拠点
	circle->pos.x = (1280 - 150) / 2;											//ｘ座標((画面幅-画像幅)/2)
	circle->pos.y = 20;															//ｙ座標
	circle->meter.offset.x = 100;												//メーターのオフセットｘ
	circle->meter.offset.y = 100;												//メーターのオフセットｙ
	circle->meter.emptyRad = rad(-180.0 - 22.0);								//ｈｐゼロの時の角度
	circle->meter.fullRad = rad(22.0);											//ｈｐ最大の時の角度
	circle->meter.radius = 100;													//半径
	circle->meter.handle = imageMgrGetHandle(IMAGE_BASE_HP_NEEDLE, 0);			//針のハンドル
	circle->frameHandle = imageMgrGetHandle(IMAGE_BASE_HP_FRAME, 0);			//枠のハンドル

	//boss
	///pos
	boss->pos.x = 780;														//左上位置
	boss->pos.y = 20;														//
																			///meter
	boss->meter.startOffset.x = 41;											//オフセット
	boss->meter.startOffset.y = 15;											//
	boss->meter.endOffset.x = 420;											//
	boss->meter.endOffset.y = 28;											//
	boss->meter.handle = imageMgrGetHandle(IMAGE_HP_METER, 0);				//画像ハンドル
																			///frame
	boss->frameHandle = imageMgrGetHandle(IMAGE_HP_BAR_FRAME_2P, 0);		//画像ハンドル
	return 0;
}

extern int uiMgrUpdate(){
	uiHpGaugeUpdate(&ui.bar1p);
	uiHpGaugeUpdate(&ui.bar2p);
	uiFaceIconUpdate(&ui.face1p);
	uiFaceIconUpdate(&ui.face2p);
	uiHpGaugeUpdate(&ui.circleCastle);
	if(ui.isBossStage){
		uiHpGaugeUpdate(&ui.barBoss);
	}
	return 0;
}

extern int uiMgrDraw(){
	uiDrawPre();
	uiHpGaugeDraw(&ui.bar1p);
	uiHpGaugeDraw(&ui.bar2p);
	uiFaceIconDraw(&ui.face1p);
	uiFaceIconDraw(&ui.face2p);
	uiHpGaugeDraw(&ui.circleCastle);
	if(ui.isBossStage){
		uiHpGaugeDraw(&ui.barBoss);
	}
	return 0;
}

extern int uiMgrEnd(){
	ui = {};
	return 0;
}

