#pragma once


/*
1pのｈｐを教える関数
毎フレーム呼び出して1pの現在ｈｐと最大ｈｐを教えてやります
*/
extern int uiMgrTeach1pHp(int currentHp, int maxHp);

/*
2pのｈｐを教える関数
*/
extern int uiMgrTeach2pHp(int currentHp, int maxHp);

/*
拠点のｈｐを教える関数
*/
extern int uiMgrTeachCastleHp(int currentHp, int maxHp);

/*
ボスのｈｐを教える関数
*/
extern int uiMgrTeachBossHp(int currentHp, int maxHp);

/*
ボスステージであることを教える関数
*/
extern int uiMgrTeachBossStage(bool isBossStage);


/*
uiをすべて初期化する関数
*/
extern int uiMgrInit();

/*
uiをすべて更新する関数
*/
extern int uiMgrUpdate();

/*
uiをすべて描画する関数
*/
extern int uiMgrDraw();

/*
uiをすべて消去する関数
*/
extern int uiMgrEnd();

