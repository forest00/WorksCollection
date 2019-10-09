
#include "common.h"
#include "chara__manager.h"
#include "input_manager.h"
#include "amgame.h"
#include "tree_sign.h"
#include "stage_data_reader_assistant.h"
#include "chara__hit_rect.h"
#include "camera.h"

#include "forDebug.h"

typedef struct tagTreeSign{
	bool isActive;			//アクティブである
	bool beingRead;			//読まれている
	bool hasRead;			//一回以上読まれた
	int elapsed;			//経過時間
	double positionX;		//位置x
	double positionY;		//位置y
	int imageIndex;			//自分が表示する画像の番号
	int hitRelativeX;		//あたり判定ズレx
	int hitRelativeY;		//あたり判定ズレy
	int hitSizeX;			//あたり判定幅x
	int hitSizeY;			//あたり判定幅y
	//当たり判定領域を取得
	int getHitArea(int &left, int &top, int &right, int &bottom){
		left = (int)(positionX) + hitRelativeX;
		top = (int)(positionY) + hitRelativeY;
		right = left + hitSizeX;
		bottom = top + hitSizeY;
		return 0;
	}
	int drawRelativeX;		//描画ズレx
	int drawRelativeY;		//描画ズレy
	int drawSizeX;			//描画幅x
	int drawSizeY;			//描画幅y
	//描画領域を取得
	int getDrawArea(int &left, int &top, int &right, int &bottom){
		left = (int)(positionX) + drawRelativeX;
		top = (int)(positionY) + drawRelativeY;
		right = left + drawSizeX;
		bottom = top + drawSizeY;
		return 0;
	}
	int alpha;
	int handle;				//描画ハンドル
	int update();			//更新関数
	int draw();				//描画関数
}TreeSign, *pTreeSign;

int TreeSign::update(){
	///フラグチェック
	if(!isActive){
		return -1;
	}
	///看板を読む処理↓
	//ボタンは押されたか
	if(isDownPad1(PAD_X) || isDownKeyBoard(KEYIN_RET)){
		//読まれている途中か
		if(beingRead){
			clearImageInGame(imageIndex);
			beingRead = false;
			player1->att.pl1.reading = false;
		} else{
			double l0 = 0.0, t0 = 0.0, r0 = 0.0, b0 = 0.0;
			getCharaHitArea(player1, &l0, &t0, &r0, &b0);
			int _l0 = (int)(l0);
			int _t0 = (int)(t0);
			int _r0 = (int)(r0);
			int _b0 = (int)(b0);
			int l1 = 0, t1 = 0, r1 = 0, b1 = 0;
			getHitArea(l1, t1, r1, b1);
			//1pとぶつかっているか
			if(isHitBox(_l0, _t0, _r0, _b0, l1, t1, r1, b1)){
				dispImageInGame(imageIndex);
				beingRead = true;
				hasRead = true;
				player1->att.pl1.reading = true;
			}
		}
	}
	///看板を読む処理↑
	alpha = min(elapsed * 5, 255);
	elapsed++;
	return 0;
}

int TreeSign::draw(){
	if(!isActive){
		return -1;
	}
	int x0 = (int)(positionX) + drawRelativeX;
	int y0 = (int)(positionY) + drawRelativeY;
	int x1 = x0 + drawSizeX;
	int y1 = y0 + drawSizeY;
	SetDrawMode(AMDRAW_ALPHABLEND, alpha);
	SetDrawBright(1000, 1000, 1000);
	correctByCamera(&x0, &y0);
	correctByCamera(&x1, &y1);
	DrawMemThExp(x0, y0, x1, y1, handle);
	#ifdef _DBGOPT_DRAW_RECT_
	{
		int hitL = 0, hitT = 0, hitR = 0, hitB = 0;
		getHitArea(hitL, hitT, hitR, hitB);
		correctByCamera(&hitL, &hitT);
		correctByCamera(&hitR, &hitB);
		drawBox(x0, y0, x1, y1, GetColor(255, 0, 0));
		drawBox(hitL, hitT, hitR, hitB, GetColor(160, 255, 0));
	}
	#endif
	return 0;
}


TreeSign treeSign[TREE_SIGN_MAX];


int initTreeSign(){
	for each(auto &iter in treeSign){
		iter.isActive = false;
	}
	return 0;
}

int updateTreeSign(){
	for each(auto &iter in treeSign){
		iter.update();
	}
	return 0;
}

int drawTreeSign(){
	for each(auto &iter in treeSign){
		iter.draw();
	}
	return 0;
}

int endTreeSign(){
	initTreeSign();
	return 0;
}


int setTreeSign(int index, double x, double y, int imageIndex){
	if(index < 0 || index >= TREE_SIGN_MAX){
		return -1;
	}
	TreeSign &t = treeSign[index];
	t.isActive = true;
	t.beingRead = false;
	t.hasRead = false;
	t.elapsed = 0;
	t.positionX = x;
	t.positionY = y;
	t.imageIndex = imageIndex;
	t.handle = imageMgrGetHandle(IMAGE_TREE_SIGN, 0);
	pBox hitBox = (pBox)getBoxData(HIT_TREE_SIGN);
	pBox drawBox = (pBox)getBoxData(DISP_TREE_SIGN);
	t.hitRelativeX = hitBox->relative.x;
	t.hitRelativeY = hitBox->relative.y;
	t.hitSizeX = hitBox->size.x;
	t.hitSizeY = hitBox->size.y;
	t.drawRelativeX = drawBox->relative.x;
	t.drawRelativeY = drawBox->relative.y;
	t.drawSizeX = drawBox->size.x;
	t.drawSizeY = drawBox->size.y;
	return 0;
}

int clearTreeSign(int index){
	if(index < 0 || index >= TREE_SIGN_MAX){
		return -1;
	}
	TreeSign t = treeSign[index];
	t.isActive = false;
	return 0;
}


//看板の数
extern int countSign(){
	int count = 0;
	for each(auto &iter in treeSign){
		if(iter.isActive){
			count++;
		}
	}
	return count;
}
//すでに読まれている看板の数
extern int countSignHasRead(){
	int count = 0;
	for each(auto &iter in treeSign){
		if(iter.isActive){
			if(iter.hasRead){
				count++;
			}
		}
	}
	return count;
}
