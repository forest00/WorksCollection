

#include <stdio.h>
#include <Windows.h>
#include "amgame.h"

#include "game_state.h"
#include "common.h"

#include "chara_attacker.h"
#include "chara_player1.h"
#include "chara__manager.h"
#include "camera.h"

#include "forDebug.h"


static int updateRecoveredBullet(pCharacter bullet){
	Player2MainAtt *att = &bullet->att.pl2Main;
	int attackerIndex = att->attackerIndex;
	double x, y;
	getCharaOffSet(bullet, &x, &y);
	x += att->vx;
	y += att->vy;
	attackerOffset(attackerIndex, x, y);
	//1P�Ƃ̓����蔻��
	pCharacter hitting = checkAttackerHitCharacters(attackerIndex, player1, PLAYER1_NUM);
	if(hitting != nullptr){
		//����������񕜂�����
		player1HealHp(att->attack);
		clearAttacker(att->attackerIndex);
		//������
		charaClear(bullet);
		//�G�t�F�N�g�H
		///set effect
	}
	charaOffSet(bullet, x, y);
	//��ʊO�ɏo��������遫
	correctByCamera(&x, &y);
	pBox box = (pBox)getBoxData(HIT_PLAYER2_MAIN_A);
	if(x < -(box->relative.x) - (box->size.x) || x > DISP_W + (box->relative.x) + (box->size.x)){
		clearAttacker(att->attackerIndex);
		charaClear(bullet);
	}
	//��ʊO�ɏo��������遪
	return 0;
}

int setPlayer2RecoveredBullet(int x, int y, int vx, int vy, int attack){
	int nonActive = charaFindNonActive(player2Bullet, PLAYER2_MAIN_NUM);
	if(nonActive < 0){
		return -1;
	}
	pCharacter actor = &player2Bullet[nonActive];
	charaInit(actor);
	const ImageFileId fileId = IMAGE_2P_RECOVERED_BULLETS;
	Player2MainAtt *att = &actor->att.pl2Main;
	actor->isActive = true;
	actor->imageFileId = fileId;
	actor->draw = dispInfoDefault;
	actor->draw.handle = imageMgrGetHandle(fileId, 0);
	actor->draw.flipFlagX = (vx < 0);
	charaOffSet(actor, x, y);
	charaFuncSet(actor, updateRecoveredBullet);
	att->vx = vx;
	att->vy = vy;
	att->attack = attack;
	att->attackerIndex = setAttacker(x, y, HIT_PLAYER2_MAIN_A);
	getImageSize(fileId, &(actor->draw.size.x), &(actor->draw.size.y));
	//�`��ʒu�̃Y��
	if(vx < 0){
		actor->draw.relative = {-150,-75};
	} else{
		actor->draw.relative = {-50,-75};
	}
	return nonActive;
}
