

#include <stdio.h>
#include <Windows.h>
#include "amgame.h"

#include "game_state.h"
#include "common.h"

#include "chara_attacker.h"
#include "chara_enemy.h"
#include "chara__manager.h"
#include "camera.h"

#include "forDebug.h"


static int updateBullet(pCharacter bullet){
	Player2MainAtt *att = &bullet->att.pl2Main;
	int attackerIndex = att->attackerIndex;
	double x, y;
	getCharaOffSet(bullet, &x, &y);
	x += att->vx;
	y += att->vy;
	attackerOffset(attackerIndex, x, y);
	//弾と敵の当たり判定
	pCharacter hitting = checkAttackerHitCharacters(attackerIndex, enemies, ENEMY_NUM);
	if(hitting != nullptr){
		double kbx = 1, kby = -1;
		if(att->vx < 0){
			kbx = -kbx;
		}
		enemyBlow(hitting, kbx, kby);
		enemyHitDamage(hitting, att->attack);
		clearAttacker(att->attackerIndex);
		charaClear(bullet);
	}
	charaOffSet(bullet, x, y);
	//画面外に出たら消える↓
	correctByCamera(&x, &y);
	pBox box = (pBox)getBoxData(HIT_PLAYER2_MAIN_A);
	if(x < -(box->relative.x) - (box->size.x) || x > DISP_W + (box->relative.x) + (box->size.x)){
		clearAttacker(att->attackerIndex);
		charaClear(bullet);
	}
	//画面外に出たら消える↑
	return 0;
}

int setPlayer2Bullet(int x, int y, int vx, int vy, int attack){
	int nonActive = charaFindNonActive(player2Bullet, PLAYER2_MAIN_NUM);
	if(nonActive < 0){
		return -1;
	}
	pCharacter actor = &player2Bullet[nonActive];
	charaInit(actor);
	const ImageFileId fileId = IMAGE_2P_MAIN_ATK_BULLET;
	Player2MainAtt *att = &actor->att.pl2Main;
	actor->isActive = true;
	actor->imageFileId = fileId;
	actor->draw = dispInfoDefault;
	actor->draw.handle = imageMgrGetHandle(fileId, 0);
	actor->draw.flipFlagX = (vx < 0);
	charaOffSet(actor, x, y);
	charaFuncSet(actor, updateBullet);
	att->vx = vx;
	att->vy = vy;
	att->attack = attack;
	att->attackerIndex = setAttacker(x, y, HIT_PLAYER2_MAIN_A);
	getImageSize(fileId, &(actor->draw.size.x), &(actor->draw.size.y));
	//描画位置のズレ
	if(vx < 0){
		actor->draw.relative = {-150,-75};
	} else{
		actor->draw.relative = {-50,-75};
	}
	return nonActive;
}
