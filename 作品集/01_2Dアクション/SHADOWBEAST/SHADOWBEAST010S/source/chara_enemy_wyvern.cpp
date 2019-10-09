


#include <Windows.h>
#include "amgame.h"

#include "game_state.h"
#include "chara_enemy.h"
#include "chara__common.h"

#include "image_manager.h"
#include "chara_block.h"
#include "chara_attacker.h"
#include "chara__hit_rect.h"
#include "chara_enemy_goblin.h"
#include "chara_enemy_wyvern.h"
#include "chara_enemy_wyvern_fire.h"
#include "chara_enemy_death.h"
#include "in_game_effect.h"
#include "chara__manager.h"
#include "stage_data_reader.h"

#include "common.h"
#include "camera.h"

#include "forDebug.h"



/*
ワイバーン
*/




extern int wyvernSet(double x, double y);
extern int wyvernBlow(pCharacter wyv, double vx, double vy);
extern int wyvernHitDamage(pCharacter wyv, int damage);


static int wyvernUpdate(pCharacter wyv);

static int wyvernChangeState(pCharacter wyv, EnemyState nextState);
static int wyvernStateEnter(pCharacter wyv);
static int wyvernStateExit(pCharacter wyv);
static int wyvernInit(pCharacter wyv);
static int wyvernSection(pCharacter wyv);
static int wyvernWait(pCharacter wyv);
static int wyvernWalk(pCharacter wyv);
static int wyvernSearch(pCharacter wyv);
static int wyvernAttackStart(pCharacter wyv);
static int wyvernAttack(pCharacter wyv);
static int wyvernDamageStop(pCharacter wyv);
static int wyvernDamage(pCharacter wyv);
static int wyvernDeath(pCharacter wyv);
static int wyvernPosUpdate(pCharacter wyv, double vx, double vy);

int wyvernSet(double x, double y){
	int found = charaFindNonActive(enemies, ENEMY_NUM);
	if(found < 0){
		return -1;
	}
	pCharacter wyv = &enemies[found];
	WyvernAtt *att = &wyv->att.wyv;
	ImageFileId fileId = IMAGE_WYVERN_WAIT;
	const int hWyv = imageMgrGetHandle(fileId, 0);
	charaInit(wyv);
	wyv->isActive = true;
	charaSetDispSize(wyv, fileId);
	pBox WyvBox = (pBox)getBoxData(HIT_WYVERN_G);
	wyv->hit = *WyvBox;
	*att = wyvernAttBase;
	charaOffSet(wyv, x - wyv->hit.size.x / 2, y - wyv->hit.size.y / 2);
	charaColSet(wyv, HIT_WYVERN_G);
	charaSetImageHandle(wyv, hWyv);
	charaHome(wyv, wyv->hit.size.x / 2, wyv->hit.size.y / 2);
	charaFuncSet(wyv, wyvernUpdate);
	wyvernChangeState(wyv, ENEMY_STATE_INIT);
	wyvernUpdate(wyv);
	return 0;
}
int wyvernUpdate(pCharacter wyv){
	wyv->anim.time = getGameFrame();
	wyvernSection(wyv);

	int index = animaGetValueStep(&wyv->anim);
	int handle = imageMgrGetHandle(wyv->imageFileId, index);
	charaSetImageHandle(wyv, handle);
	charaFlipX(wyv, wyv->dir == RIGHT);
	//玉を出す
	if(!(rand() % 3)){
		double _x = wyv->pos.x + wyv->hit.relative.x;
		double _y = wyv->pos.y + wyv->hit.relative.y;
		_x += rand() % wyv->hit.size.x;
		_y += rand() % wyv->hit.size.y;
		InGameEffectType _type = IN_GAME_EFFECT_DARK_BALL;
		inGameEffctSet(_type, _x, _y, EFFECT_FRONT);
	}
	return 0;
}
int wyvernChangeState(pCharacter wyv, EnemyState nextState){
	WyvernAtt *att = &wyv->att.wyv;
	//死んでいれば状態チェンジしない
	if(att->isDead){
		return -1;
	}
	wyv->anim.started = getGameFrame();
	wyvernStateExit(wyv);

	att->state = nextState;

	wyvernStateEnter(wyv);
	return 0;
}
int wyvernStateEnter(pCharacter wyv){
	WyvernAtt *att = &wyv->att.wyv;
	switch(att->state){
		case ENEMY_STATE_INIT:
			wyv->imageFileId = IMAGE_WYVERN_WAIT;
			break;
		case ENEMY_STATE_WAIT:
			wyv->imageFileId = IMAGE_WYVERN_WAIT;
			break;
		case ENEMY_STATE_WALK:
			wyv->imageFileId = IMAGE_WYVERN_WALK;
			break;
		case ENEMY_STATE_SEARCH:
			wyv->imageFileId = IMAGE_WYVERN_WAIT;
			break;
		case ENEMY_STATE_ATTACK:
			wyvernAttackStart(wyv);
			wyv->imageFileId = IMAGE_WYVERN_ATTACK;
			break;
		case ENEMY_STATE_DAMAGE_STOP:
			wyv->imageFileId = IMAGE_WYVERN_DAMAGE;
			att->stopCoord.x = wyv->pos.x;
			att->stopCoord.y = wyv->pos.y;
			att->stopCount = 0;
			break;
		case ENEMY_STATE_DAMAGE:
			charaOffSet(wyv, att->stopCoord.x, att->stopCoord.y);
			charaOffSet(wyv, att->stopCoord.x, att->stopCoord.y);
			break;
		case ENEMY_STATE_DEATH:
			att->isDead = true;
			att->deadCounter = 0;
			break;
		default:
			break;
	}
	AnimaData creature = {0};
	createAnimaData(&creature, wyv->imageFileId);
	animaSetup(&wyv->anim,
			   getGameFrame(),
			   &creature);
	return 0;
}
int wyvernStateExit(pCharacter wyv){
	WyvernAtt *att = &wyv->att.wyv;
	switch(att->state){
		case ENEMY_STATE_INIT:
			break;
		case ENEMY_STATE_WAIT:
			break;
		case ENEMY_STATE_WALK:
			break;
		case ENEMY_STATE_SEARCH:
			break;
		case ENEMY_STATE_ATTACK:
			break;
		case ENEMY_STATE_DAMAGE_STOP:
			break;
		case ENEMY_STATE_DAMAGE:
			break;
		case ENEMY_STATE_DEATH:
			break;
		default:
			break;
	}
	return 0;
}
int wyvernSection(pCharacter wyv){
	WyvernAtt *att = &wyv->att.wyv;
	att->isHittingDamage = false;
	switch(att->state){
		case ENEMY_STATE_INIT:
			wyvernInit(wyv);
			wyvernSection(wyv);
			break;
		case ENEMY_STATE_WAIT:
			wyvernWait(wyv);
			break;
		case ENEMY_STATE_WALK:
			wyvernWalk(wyv);
			break;
		case ENEMY_STATE_SEARCH:
			wyvernSearch(wyv);
			break;
		case ENEMY_STATE_ATTACK:
			wyvernAttack(wyv);
			break;
		case ENEMY_STATE_DAMAGE_STOP:
			att->isHittingDamage = true;
			wyvernDamageStop(wyv);
			break;
		case ENEMY_STATE_DAMAGE:
			att->isHittingDamage = true;
			wyvernDamage(wyv);
			break;
		case ENEMY_STATE_DEATH:
			att->isHittingDamage = true;
			wyvernDeath(wyv);
			break;
		default:
			break;
	}
	return 0;
}
int wyvernInit(pCharacter wyv){
	if(wyv->pos.x > WORLD_CENTER_X){
		wyv->dir = LEFT;
	} else{
		wyv->dir = RIGHT;
	}
	wyvernChangeState(wyv, ENEMY_STATE_WAIT);
	return 0;
}
int wyvernWait(pCharacter wyv){
	WyvernAtt *att = &wyv->att.wyv;
	XYd velocity = att->velocity;

	if(animaGetCount(&wyv->anim) >= 1){
		bool hitFlag = false;
		if(!isPlayer1DeadEnemyBridge()){
			hitFlag |= (charaBulkHit(player1, PLAYER1_NUM, wyv, HIT_WYVERN_SEARCH) >= 0);
		}
		if(!isPlayer2DeadEnemyBridge()){
			hitFlag |= (charaBulkHit(player2, PLAYER2_NUM, wyv, HIT_WYVERN_SEARCH) >= 0);
		}
		hitFlag |= (charaBulkHit(castle, CASTLE_NUM, wyv, HIT_WYVERN_SEARCH) >= 0);
		if(hitFlag){
			wyvernChangeState(wyv, ENEMY_STATE_ATTACK);
			return -1;
		}
		wyvernChangeState(wyv, ENEMY_STATE_SEARCH);
	}
	wyvernPosUpdate(wyv, velocity.x, velocity.y);
	return 0;
}
int wyvernWalk(pCharacter wyv){
	WyvernAtt *att = &wyv->att.wyv;
	XYd velocity = att->velocity;
	switch(wyv->dir){
		case LEFT:
			velocity.x -= att->walkSpeed;
			break;
		case RIGHT:
			velocity.x += att->walkSpeed;
			break;
		default:
			break;
	}
	if(animaGetCount(&wyv->anim) >= 1){
		wyvernChangeState(wyv, ENEMY_STATE_SEARCH);
	}
	{
		//斜めにプレイヤーを探す
		//見つけたら攻撃
		int f = 0;
		double dx = wyv->pos.x;
		double dy = wyv->pos.y;
		Direction dir = wyv->dir;
		double speed = 20.0;
		XYd _velocity = {speed,speed};
		if(dir == LEFT){ _velocity.x *= -1; }
		double x = dx, y = dy;
		for(int i = 0; i < 20; i++){
			x += _velocity.x;
			y += _velocity.y;
			wyv->pos.x = x;
			wyv->pos.y = y;
			//1pとの当たり判定
			if(charaBulkHit(player1, PLAYER1_NUM, wyv, HIT_WYVERN_SEARCH) >= 0){
				//1p生存チェック
				if(!isPlayer1DeadEnemyBridge()){
					f = 1;
				}
				break;
			}
			//2pとの当たり判定
			if(charaBulkHit(player2, PLAYER2_NUM, wyv, HIT_WYVERN_SEARCH) >= 0){
				//2p生存チェック
				if(!isPlayer2DeadEnemyBridge()){
					f = 2;
				}
				break;
			}
			//城との当たり判定
			if(charaBulkHit(castle, CASTLE_NUM, wyv, HIT_WYVERN_SEARCH) >= 0){
				f = 3;
				break;
			}
		}
		wyv->pos.x = dx;
		wyv->pos.y = dy;
		att->atkDir = dir;
		if(f){
			wyvernChangeState(wyv, ENEMY_STATE_ATTACK);
			return -1;
		}
	}
	bool hitFlag = false;
	if(!isPlayer1DeadEnemyBridge()){
		hitFlag |= (charaBulkHit(player1, PLAYER1_NUM, wyv, HIT_WYVERN_SEARCH) >= 0);
	}
	if(!isPlayer2DeadEnemyBridge()){
		hitFlag |= (charaBulkHit(player2, PLAYER2_NUM, wyv, HIT_WYVERN_SEARCH) >= 0);
	}
	hitFlag |= (charaBulkHit(castle, CASTLE_NUM, wyv, HIT_WYVERN_SEARCH) >= 0);
	if(hitFlag){
		wyvernChangeState(wyv, ENEMY_STATE_WAIT);
		return -1;
	}
	wyvernPosUpdate(wyv, velocity.x, velocity.y);
	return 0;
}
int wyvernSearch(pCharacter wyv){
	static const int NUM = 3;
	const int X[NUM] = {
		getPlayer1XEnemyBridge(),
		getPlayer2XEnemyBridge(),
		getTower1XEnemyBridge()};//位置
	const bool F[NUM] = {
		isPlayer1DeadEnemyBridge(),
		isPlayer2DeadEnemyBridge(),
		false};//無視するフラグ
	const int wyvX = (int)(wyv->pos.x) + wyv->hit.relative.x + wyv->hit.size.x / 2;
	int nearestX = INT_MAX;
	int nearest = -1;
	for(int i = 0; i < NUM; i++){
		int dx = abs(X[i] - wyvX);
		if(F[i]){
			dx = INT_MAX;
		}
		if(nearestX > dx){
			nearestX = dx;
			nearest = i;
		}
	}
	if(wyvX > X[nearest]){
		wyv->dir = LEFT;
	} else{
		wyv->dir = RIGHT;
	}
	wyvernChangeState(wyv, ENEMY_STATE_WALK);
	return 0;
}
int wyvernAttackStart(pCharacter wyv){
	WyvernAtt *att = &wyv->att.wyv;
	//火を出す
	{
		int delay = 32;
		double X = wyv->pos.x + wyv->hit.relative.x + wyv->hit.size.x / 2.0;
		double Y = wyv->pos.y + wyv->hit.relative.y + wyv->hit.size.y / 4.0;
		setWyvernFire(X, Y, att->atkDir, delay, att->attack, (void*)wyv);
	}
	return 0;
}
int wyvernAttack(pCharacter wyv){


	if(animaGetCount(&wyv->anim) >= 1){
		wyvernChangeState(wyv, ENEMY_STATE_WAIT);
	}
	return 0;
}
int wyvernBlow(pCharacter wyv, double vx, double vy){
	WyvernAtt *att = &wyv->att.wyv;
	att->knockBackX = vx;
	att->knockBackY = vy;
	bool stateDead = false;
	stateDead |= (att->state == ENEMY_STATE_DAMAGE_STOP);
	stateDead |= (att->state == ENEMY_STATE_DAMAGE);
	if(stateDead && (att->hp) <= 0){
		return -1;
	}
	wyvernChangeState(wyv, ENEMY_STATE_DAMAGE_STOP);
	return 0;
}
int wyvernHitDamage(pCharacter wyv, int damage){
	WyvernAtt *att = &wyv->att.wyv;
	int _hp = att->hp;
	att->hp -= damage;
	if(_hp > 0 && att->hp <= 0){
		return -1;
	}
	return 0;
}
int wyvernDamageStop(pCharacter wyv){
	WyvernAtt *att = &wyv->att.wyv;
	XYd velocity = att->velocity;
	if(att->stopCount >= att->stopLimit){
		wyvernChangeState(wyv, ENEMY_STATE_DAMAGE);
		return -1;
	}

	wyvernPosUpdate(wyv, rand() % 12 - 6, rand() % 12 - 6);
	wyvernPosUpdate(wyv, velocity.x, velocity.y);
	att->stopCount++;
	return 0;
}
int wyvernDamage(pCharacter wyv){
	WyvernAtt *att = &wyv->att.wyv;
	XYd velocity = att->velocity;

	double kbX = att->knockBackX;
	double kbY = att->knockBackY;

	kbX *= 0.96;
	kbY *= 0.96;

	wyvernPosUpdate(wyv, kbX, kbY);
	att->knockBackX = kbX;
	att->knockBackY = kbY;

	wyvernPosUpdate(wyv, velocity.x, velocity.y);
	if(animaGetCount(&wyv->anim) >= 1){//アニメーションで被ダメ状態解除
		if((att->hp) <= 0){//HPがなければ消える状態へ
			wyvernChangeState(wyv, ENEMY_STATE_DEATH);
		} else{
			wyvernChangeState(wyv, ENEMY_STATE_WAIT);
		}
		return -1;
	}

	return 0;
}
int wyvernDeath(pCharacter wyv){
	WyvernAtt *att = &wyv->att.wyv;
	//非表示にする
	wyv->draw.doHide = true;
	//エフェクトを出す
	if(att->deadCounter == 0){
		double x = wyv->pos.x + wyv->draw.relative.x;
		double y = wyv->pos.y + wyv->draw.relative.y;
		enemyDeadEffectSet(ENEMY_WYVERN, x, y, wyv->dir);
	}
	//時間が来たら消える
	if(att->deadCounter > ENEMY_DEATH_DELAY){
		wyv->isActive = false;
	}
	att->deadCounter++;
	return 0;
}
int wyvernPosUpdate(pCharacter wyv, double vx, double vy){
	int result = 0;
	WyvernAtt *att = &wyv->att.wyv;
	double x, y;
	double gravity = att->gravity;
	getCharaOffSet(wyv, &x, &y);

	vy += gravity;
	vx *= 0.7;
	vy *= 0.96;

	x += vx;
	y += vy;

	charaOffSet(wyv, x, y);

	//地面に近づけば上に向かう
	{
		int hittingBlockIndex = charaBulkHit(
			block, BLOCK_NUM,
			wyv, HIT_WYVERN_SEARCH_GROUND);
		if(hittingBlockIndex >= 0){
			vy -= gravity;
			vy -= gravity;
		}
	}

	int hitInfo = charaBlockingByBlock((void*)wyv);
	if(charaLanding(hitInfo)){
		//着地
		vy = 0;
		result = -1;
	}

	//世界の外に出たら死亡
	if(x < (0 - wyv->hit.size.x) || x > (WORLD_WIDTH + wyv->hit.size.x)){
		charaClear(wyv);
	}

	att->velocity.x = vx;
	att->velocity.y = vy;
	return result;
}


