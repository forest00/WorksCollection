

#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include "amgame.h"
#include "game_state.h"
#include "chara__manager.h"
#include "chara__common.h"
#include "input_manager.h"
#include "image_manager.h"
#include "chara_castle.h"
#include "chara_item.h"
#include "stage_data_reader.h"

#include "forDebug.h"


#define INVINCIBLE_TIME 8
static bool itemSetFlag = false;
static XYd itemSetV = {0,0};

static int stage = 0;
static CastleAtt castleMainAttBase = {
	1500,		//hp
	0
};
static CastleAtt castleRedAttBase = {
	1000,		//hp
	0
};
static CastleAtt castleGreenAttBase = {
	1000,		//hp
	0
};
static CastleAtt castleBlueAttBase = {
	1000,		//hp
	0
};


static int castleUpdate(pCharacter cstl);
static int castleSubUpdate(pCharacter cstlSub);
//城が崩壊しきったか調べる
//true...崩壊
static bool isCastleDead(){
	if(getSelectedStage() < 3){
		int count = 0;
		for(int i = 0; i < CASTLE_NUM; i++){
			if(castle[i].att.cstl.hp <= 0){
				count++;
			}
		}
		return count == CASTLE_NUM;
	} else{
		return castle[0].att.cstl.hp <= 0;
	}
}



static int setCastle(double x, double y){
	ImageFileId fileIdArray[CASTLE_NUM] = {
		IMAGE_CASTLE_BLUE,
		IMAGE_CASTLE_GREEN,
		IMAGE_CASTLE_RED,
		IMAGE_CASTLE_MAIN,
	};
	CastleAtt attBaseArray[CASTLE_NUM] = {
		castleBlueAttBase,
		castleGreenAttBase,
		castleRedAttBase,
		castleMainAttBase,
	};
	CharaHitAreaId hitAreaArray[CASTLE_NUM] = {
		HIT_CASTLE_BLUE,
		HIT_CASTLE_GREEN,
		HIT_CASTLE_RED,
		HIT_NONE,
	};
	int(*updateFuncPtr[CASTLE_NUM])(pCharacter) = {
		castleSubUpdate,
		castleSubUpdate,
		castleSubUpdate,
		castleUpdate,
	};
	int xDiff[CASTLE_NUM] = {32,32,32,0};
	int hTower = -1;
	int totalHp = 0;
	int w = 0, h = 0;
	getImageSize(IMAGE_CASTLE_MAIN, &w, &h);
	int towerX = (int)(x - w / 2 + 32);
	int towerY = (int)(y - h + 64);
	pCharacter cstl = nullptr;
	CastleAtt *att = nullptr;
	AnimaData animaNone = {1,1};

	for(unsigned int index = 0; index < CASTLE_NUM; index++){
		ImageFileId fileId = fileIdArray[index];
		hTower = imageMgrGetHandle(fileId, 0);
		cstl = &castle[index];
		att = &cstl->att.cstl;
		charaInit(cstl);
		cstl->isActive = true;
		cstl->imageFileId = fileId;
		charaSetDispSize(cstl, fileId);
		charaColSet(cstl, hitAreaArray[index]);
		int _x = towerX + xDiff[index];
		charaOffSet(cstl, _x, towerY);
		charaSetImageHandle(cstl, hTower);
		charaFuncSet(cstl, updateFuncPtr[index]);
		*att = attBaseArray[index];
		att->aliveFlag = true;
		att->hp = att->maxHp;
		animaSetup(&cstl->anim, 0, &animaNone);
		//城合計hp計算
		totalHp += att->maxHp;
	}


	setTowerMaxHp(totalHp);

	//変数のリセット
	itemSetFlag = false;
	return 0;
}

static int updateBoat(pCharacter boat){
	CastleAtt *att = &boat->att.cstl;

	//無敵中であれば、なんか処理
	if(att->invincibleFlag){
		att->invincibleCount++;
		if(att->invincibleCount >= att->invincibleLimit){
			att->invincibleFlag = false;
		}
	}

	//フラグがオンであればアイテム出現
	if(itemSetFlag){
		itemSetFlag = false;
		int x = 0, y = 0;
		XYd _velocity = itemSetV;
		x = (int)(boat->pos.x) + boat->draw.relative.x;
		y = (int)(boat->pos.y) + boat->draw.relative.y;
		x += (int)(boat->draw.size.x / 2);
		hpHealItemSet(x, y, _velocity.x, _velocity.y);
	}

	//すでに壊されていれば、アニメーション
	if((att->hp) <= 0){
		if(att->aliveFlag){
			//アニメーション開始処理↓
			AnimaData animaData = {0};
			createAnimaData(&animaData, boat->imageFileId);
			animaSetup(&boat->anim,
					   getGameFrame(),
					   &animaData);
			att->aliveFlag = false;
			//アニメーション開始処理↑
			//当たり判定も消す
			charaColSet(boat, HIT_NONE);
		}
		//アニメーション処理↓
		{
			boat->anim.time = getGameFrame();
			if(animaGetCount(&boat->anim) < 1){
				int index = animaGetValueStep(&boat->anim);
				int handle = imageMgrGetHandle(boat->imageFileId, index);
				charaSetImageHandle(boat, handle);
			}
		}
		//アニメーション処理↑
	}

	//hで回復
	if(ForDebugDownKey('H')){
		castleHitDamage(boat, 1000);
	}

	return 0;
}

static int setBoat(){
	pCharacter boat = &castle[0];
	ImageFileId fileId = IMAGE_SHIP_BREAK;
	CastleAtt *att = &boat->att.cstl;
	boat->isActive = true;
	//HP↓
	att->maxHp = 2000;
	att->hp = att->maxHp;
	setTowerMaxHp(att->hp);
	//HP↑
	//ハンドルをセット↓
	int handle = imageMgrGetHandle(fileId, 0);
	charaSetImageHandle(boat, handle);
	boat->imageFileId = fileId;
	//ハンドルをセット↑
	double xX = (double)WORLD_WIDTH / DISP_W;
	double yY = (double)WORLD_HEIGHT / DISP_H;
	//表示領域をセット↓
	charaSetDispSize(boat, DISP_BOAT_STAGE_3_4_5);
	boat->draw.relative.x = (int)(boat->draw.relative.x  * xX);
	boat->draw.relative.y = (int)(boat->draw.relative.y * yY);
	boat->draw.size.x = (int)(boat->draw.size.x * xX);
	boat->draw.size.y = (int)(boat->draw.size.y * yY);
	//表示領域をセット↑
	//あたり判定情報セット↓
	charaColSet(boat, HIT_BOAT_STAGE_3_4_5);
	boat->hit.relative.x = (int)(boat->hit.relative.x * xX);
	boat->hit.relative.y = (int)(boat->hit.relative.y * yY);
	boat->hit.size.x = (int)(boat->hit.size.x * xX);
	boat->hit.size.y = (int)(boat->hit.size.y * yY);
	//あたり判定情報セット↑
	att->aliveFlag = true;
	charaFuncSet(boat, updateBoat);
	//変数のリセット
	itemSetFlag = false;
	return 0;
}

int setProtectTarget(double x, double y){
	if(getSelectedStage() < 3){
		return setCastle(x, y);
	} else{
		return setBoat();
	}
}

int castleUpdate(pCharacter cstl){
	CastleAtt *att = &cstl->att.cstl;

	//無敵中であれば、なんか処理
	if(att->invincibleFlag){
		att->invincibleCount++;
		if(att->invincibleCount >= att->invincibleLimit){
			att->invincibleFlag = false;
		}
	}

	//青、緑、赤がすべて壊されているか
	if(cstl->hit.size.x == 0){
		bool blueAliveFlag = castle[0].att.cstl.aliveFlag;
		bool greenAliveFlag = castle[1].att.cstl.aliveFlag;
		bool redAliveFlag = castle[2].att.cstl.aliveFlag;
		bool aliveFlag = redAliveFlag || greenAliveFlag || blueAliveFlag;
		if(!aliveFlag){
			//当たり判定を付ける
			charaColSet(cstl, HIT_CASTLE_MAIN);
			//無敵になる
			att->invincibleFlag = true;
			att->invincibleCount = 0;
			att->invincibleLimit = INVINCIBLE_TIME * 2;
		}
	}

	//フラグがオンであればアイテム出現
	if(itemSetFlag){
		itemSetFlag = false;
		int x = 0, y = 0;
		XYd _velocity = itemSetV;
		x = (int)(cstl->pos.x) + cstl->draw.relative.x;
		y = (int)(cstl->pos.y) + cstl->draw.relative.y;
		x += (int)(cstl->draw.size.x / 2);
		hpHealItemSet(x, y, _velocity.x, _velocity.y);
	}
	//hで回復
	if(ForDebugDownKey('H')){
		castleHitDamage(cstl, 1000);
	}

	return 0;
}
static int castleSubUpdate(pCharacter cstlSub){
	CastleAtt *att = &cstlSub->att.cstl;

	//無敵中であれば、なんか処理
	if(att->invincibleFlag){
		att->invincibleCount++;
		if(att->invincibleCount >= att->invincibleLimit){
			att->invincibleFlag = false;
		}
	}
	//すでに壊されていれば、アニメーション
	if((att->hp) <= 0){
		if(att->aliveFlag == true){
			AnimaData animaData = {0};
			createAnimaData(&animaData, cstlSub->imageFileId);
			animaSetup(&cstlSub->anim,
					   getGameFrame(),
					   &animaData);
			att->aliveFlag = false;
			//当たり判定も消す
			charaColSet(cstlSub, HIT_NONE);
		}
	}
	//アニメーション処理
	{
		cstlSub->anim.time = getGameFrame();
		if(animaGetCount(&cstlSub->anim) < 1){
			int index = animaGetValueStep(&cstlSub->anim);
			int handle = imageMgrGetHandle(cstlSub->imageFileId, index);
			charaSetImageHandle(cstlSub, handle);
		}
		else if(att->aliveFlag == false)
		{
			cstlSub->draw.alpha = 0;
		}
	}

	//hで回復
	if(ForDebugDownKey('H')){
		castleHitDamage(cstlSub, 1000);
	}
	return 0;
}


int getCastleX(){
	pCharacter twr = &castle[0];
	return (int)(twr->pos.x) + twr->draw.relative.x + twr->draw.size.x / 2;
}



int castleHitDamage(pCharacter target, int damage) {
	CastleAtt *att = &target->att.cstl;
	if(att->invincibleFlag){//無敵チェック
		return -1;
	}
	att->hp -= damage;
	att->hp = (int)Clamp((double)att->hp, 0.0, (double)att->maxHp);
	//無敵になる
	att->invincibleFlag = true;
	att->invincibleCount = 0;
	att->invincibleLimit = INVINCIBLE_TIME;
	if(isCastleDead()){
		intoGameOver();
	}
	addTowerDamege(damage);
	return 0;
}


int setItemOnCastle(double vx, double vy){
	itemSetFlag = true;
	itemSetV = {vx,vy};
	return 0;
}


void getCastleHp(int *hp, int *maxHp){
	*hp = 0;
	*maxHp = 0;
	if(getSelectedStage() < 3){
		for(int i = 0; i < CASTLE_NUM; i++){
			CastleAtt *att = &castle[i].att.cstl;
			*maxHp += att->maxHp;
			*hp += att->hp;
		}
	} else{
		*hp = castle[0].att.cstl.hp;
		*maxHp = castle[0].att.cstl.maxHp;
	}
}
