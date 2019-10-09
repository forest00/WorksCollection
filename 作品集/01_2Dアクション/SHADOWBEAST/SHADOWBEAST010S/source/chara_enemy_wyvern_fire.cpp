


#include <Windows.h>
#include "amgame.h"

#include "game_state.h"
#include "chara_enemy.h"
#include "chara__common.h"

#include "image_manager.h"
#include "chara_player1.h"
#include "chara_player2.h"
#include "chara_castle.h"
#include "chara_attacker.h"
#include "chara__hit_rect.h"
#include "chara_enemy_goblin.h"
#include "chara_enemy_wyvern.h"
#include "chara__manager.h"
#include "in_game_effect.h"

#include "camera.h"

#include "forDebug.h"

static const double FIRE_SPEED = 8.5;

static int moveWyvernFire(pCharacter fire) {
	WyvernFireAtt *att = &fire->att.wyvFire;
	if(att->time < att->delay){
		//遅延中は何もしない
		att->time++;
		return -1;
	}
	//エフェクト出す
	{
		InGameEffectType _type = IN_GAME_EFFECT_NONE;
		_type = IN_GAME_EFFECT_WYVERN_REMAIN_FIRE;
		XYd pos = fire->pos;
		inGameEffctSet(_type, pos.x, pos.y, EFFECT_BACK);
		inGameEffctSet(_type, pos.x, pos.y, EFFECT_BACK);
		inGameEffctSet(_type, pos.x, pos.y, EFFECT_BACK);
		_type = IN_GAME_EFFECT_WYVERN_NOT_LEFT_FIRE;
		inGameEffctSet(_type, pos.x, pos.y, EFFECT_BACK);
		inGameEffctSet(_type, pos.x, pos.y, EFFECT_BACK);
	}
	//エフェクトここまで

	//地面に当たったら消える、または親となるワイバーンが死んだら消える
	int hittingIndex = 0;
	pCharacter hitting = nullptr;
	double _x = 0, _y = 0;
	getCharaOffSet(fire, &_x, &_y);
	_x = fire->pos.x + att->velocity.x;
	_y = fire->pos.y + att->velocity.y;
	charaOffSet(fire, _x, _y);
	hittingIndex = charaBulkHit(block, BLOCK_NUM, fire);
	pCharacter parrent = (pCharacter)(att->parrentPtr);
	bool isParrentDamage = parrent->att.wyv.isHittingDamage;
	bool shouldDisappear = hittingIndex >= 0 || isParrentDamage;
	if(shouldDisappear){
		hitting = &block[hittingIndex];
		charaClear(fire);
		return -1;
	}
	//ここまで

	//プレイヤー1と火
	hittingIndex = charaBulkHit(player1, PLAYER1_NUM, fire);
	if(hittingIndex >= 0){
		XYd _velocity = {4,-3};
		if(att->dir == LEFT){
			_velocity.x = -_velocity.x;
		}
		player1Blow(_velocity.x, _velocity.y);
		player1HitDamage(att->attack);
		return -1;
	}
	//プレイヤー2と火
	hittingIndex = charaBulkHit(player2, PLAYER2_NUM, fire);
	if(hittingIndex >= 0){
		XYd _velocity = {4,-3};
		if(att->dir == LEFT){
			_velocity.x = -_velocity.x;
		}
		player2Blow(_velocity.x, _velocity.y);
		player2HitDamage(att->attack);
		return -1;
	}
	//タワーと火
	hittingIndex = charaBulkHit(castle, CASTLE_NUM, fire);
	if(hittingIndex >= 0){
		hitting = &castle[hittingIndex];
		castleHitDamage(hitting, att->attack);
		return -1;
	}
	att->time++;
	return 0;
}


int setWyvernFire(double x, double y, Direction dir, int delay, int attack, void *parrent) {
	XYd velocity = {FIRE_SPEED,FIRE_SPEED};
	if(dir == LEFT){
		velocity.x *= -1;
	}
	const pBox pf = (pBox)getBoxData(HIT_WYVERN_FIRE_A);
	int found = charaFindNonActive(wyvernFires, WYVERN_FIRE_NUM);
	if(found < 0){
		return -1;
	}
	Character *actor = &wyvernFires[found];
	charaInit(actor);
	actor->isActive = true;
	actor->pos.x = x;
	actor->pos.y = y;
	actor->hit = *pf;
	//アニメーション設定
	{
		actor->imageFileId = IMAGE_NONE;
		int sizeX = actor->draw.size.x;
		int sizeY = actor->draw.size.y;
		charaHome(actor, sizeX / 2, sizeY / 2);
		actor->draw.relative.x = -sizeX / 2;
		actor->draw.relative.y = -sizeY / 2;
	}
	WyvernFireAtt *att = &actor->att.wyvFire;
	att->used = true;
	att->dir = dir;
	att->time = 0;
	att->delay = delay;
	att->velocity.x = velocity.x;
	att->velocity.y = velocity.y;
	att->attack = attack;
	att->parrentPtr = parrent;
	charaFuncSet(actor, moveWyvernFire);
	return 0;
}
