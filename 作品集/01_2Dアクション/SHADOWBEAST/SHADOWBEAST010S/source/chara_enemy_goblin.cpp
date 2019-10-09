
#include <Windows.h>
#include "amgame.h"
#include "game_state.h"
#include "chara_enemy.h"
#include "image_manager.h"
#include "sound_manager.h"
#include "chara_block.h"
#include "chara_attacker.h"
#include "chara__hit_rect.h"
#include "chara_enemy_goblin.h"
#include "chara_enemy_wyvern.h"
#include "chara_enemy_death.h"
#include "in_game_effect.h"
#include "chara__manager.h"
#include "stage_data_reader.h"
#include "common.h"
#include "camera.h"
#include "new_effect_goblin_death_dust.h"

#include "forDebug.h"



/*
ゴブリン
*/



extern int goblinSet(double x, double y);
extern int goblinBlow(pCharacter gob, double vx, double vy);
extern int goblinHitDamage(pCharacter gob, int damage);


static int goblinUpdate(pCharacter gob);

static int goblinChangeState(pCharacter gob, EnemyState nextState);
static int goblinStateEnter(pCharacter gob);
static int goblinStateExit(pCharacter gob);
static int goblinInit(pCharacter gob);
static int goblinSection(pCharacter gob);
static int goblinWait(pCharacter gob);
static int goblinWalk(pCharacter gob);
static int goblinSearch(pCharacter gob);
static int goblinAttackStart(pCharacter gob);
static int goblinAttack(pCharacter gob);
static int goblinDamageStop(pCharacter gob);
static int goblinDamage(pCharacter gob);
static int goblinDeath(pCharacter gob);
static int goblinPosUpdate(pCharacter gob, double vx, double vy);

int goblinSet(double x, double y){
	int found = charaFindNonActive(enemies, ENEMY_NUM);
	if(found < 0){
		return -1;
	}
	pCharacter gob = &enemies[found];
	GoblinAtt *att = &gob->att.gob;
	ImageFileId fileId = IMAGE_GOBLIN_WAIT;
	const int hGob = imageMgrGetHandle(fileId, 0);
	charaInit(gob);
	gob->isActive = true;
	charaSetDispSize(gob, fileId);
	pBox gobBox = (pBox)getBoxData(HIT_GOBLIN_G);
	gob->hit = *gobBox;
	*att = goblinAttBase;
	charaOffSet(gob, x - gob->hit.size.x / 2, y - gob->hit.size.y / 2);
	charaColSet(gob, HIT_GOBLIN_G);
	charaSetImageHandle(gob, hGob);
	charaHome(gob, gob->hit.size.x / 2, gob->hit.size.y / 2);
	charaFuncSet(gob, goblinUpdate);
	goblinChangeState(gob, ENEMY_STATE_INIT);
	goblinUpdate(gob);
	return 0;
}
int goblinBlow(pCharacter gob, double vx, double vy){
	GoblinAtt *att = &gob->att.gob;
	att->knockBackX = vx;
	att->knockBackY = vy;
	bool stateDead = false;
	stateDead |= (att->state == ENEMY_STATE_DAMAGE_STOP);
	stateDead |= (att->state == ENEMY_STATE_DAMAGE);
	if(stateDead && (att->hp) <= 0){
		return -1;
	}
	goblinChangeState(gob, ENEMY_STATE_DAMAGE_STOP);
	return 0;
}
int goblinHitDamage(pCharacter gob, int damage){
	GoblinAtt *att = &gob->att.gob;
	int _hp = att->hp;
	att->hp -= damage;
	if(_hp > 0 && att->hp <= 0){
		return -1;
	}
	return 0;
}
int goblinUpdate(pCharacter gob){
	gob->anim.time = getGameFrame();
	goblinSection(gob);

	int index = animaGetValueStep(&gob->anim);
	int handle = imageMgrGetHandle(gob->imageFileId, index);
	charaSetImageHandle(gob, handle);
	charaFlipX(gob, gob->dir == RIGHT);
	//玉を出す
	if(!(rand() % 3)){
		double _x = gob->pos.x + gob->hit.relative.x;
		double _y = gob->pos.y + gob->hit.relative.y;
		_x += rand() % gob->hit.size.x;
		_y += rand() % gob->hit.size.y;
		InGameEffectType _type = IN_GAME_EFFECT_DARK_BALL;
		inGameEffctSet(_type, _x, _y, EFFECT_FRONT);
	}
	return 0;
}
int goblinChangeState(pCharacter gob, EnemyState nextState){
	GoblinAtt *att = &gob->att.gob;
	//死んでいれば状態チェンジしない
	if(att->isDead){
		return -1;
	}
	gob->anim.started = getGameFrame();
	goblinStateExit(gob);

	att->state = nextState;

	goblinStateEnter(gob);
	return 0;
}
int goblinStateEnter(pCharacter gob){
	GoblinAtt *att = &gob->att.gob;
	switch(att->state){
		case ENEMY_STATE_INIT:
			gob->imageFileId = IMAGE_GOBLIN_WAIT;
			break;
		case ENEMY_STATE_WAIT:
			gob->imageFileId = IMAGE_GOBLIN_WAIT;
			break;
		case ENEMY_STATE_WALK:
			gob->imageFileId = IMAGE_GOBLIN_WALK;
			break;
		case ENEMY_STATE_SEARCH:
			break;
		case ENEMY_STATE_ATTACK:
			goblinAttackStart(gob);
			gob->imageFileId = IMAGE_GOBLIN_ATTACK;
			//画像の大きさが変わってるため、それに対応
			if(gob->dir == LEFT){
				gob->draw.relative.x = -27;
			} else{
				gob->draw.relative.x = -3;
			}
			gob->draw.relative.y = -19;
			break;
		case ENEMY_STATE_DAMAGE_STOP:
			gob->imageFileId = IMAGE_GOBLIN_DAMAGE;
			att->stopCoord = gob->pos;
			att->stopCount = 0;
			break;
		case ENEMY_STATE_DAMAGE:
			charaOffSet(gob, att->stopCoord.x, att->stopCoord.y);
			charaOffSet(gob, att->stopCoord.x, att->stopCoord.y);
			break;
		case ENEMY_STATE_DEATH:
			att->isDead = true;
			att->deadCounter = 0;
			break;
		default:
			break;
	}
	charaSetDispSize(gob, gob->imageFileId);
	AnimaData creature = {0};
	createAnimaData(&creature, gob->imageFileId);
	animaSetup(&gob->anim,
			   getGameFrame(),
			   &creature);
	return 0;
}
int goblinStateExit(pCharacter gob){
	GoblinAtt *att = &gob->att.gob;
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
			clearAttacker(att->atkRectIndex);
			att->atkRectIndex = -1;
			gob->draw.relative.x = 0;
			gob->draw.relative.y = 0;
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
int goblinSection(pCharacter gob){
	GoblinAtt *att = &gob->att.gob;
	switch(att->state){
		case ENEMY_STATE_INIT:
			goblinInit(gob);
			goblinSection(gob);
			break;
		case ENEMY_STATE_WAIT:
			goblinWait(gob);
			break;
		case ENEMY_STATE_WALK:
			goblinWalk(gob);
			break;
		case ENEMY_STATE_SEARCH:
			goblinSearch(gob);
			break;
		case ENEMY_STATE_ATTACK:
			goblinAttack(gob);
			break;
		case ENEMY_STATE_DAMAGE_STOP:
			goblinDamageStop(gob);
			break;
		case ENEMY_STATE_DAMAGE:
			goblinDamage(gob);
			break;
		case ENEMY_STATE_DEATH:
			goblinDeath(gob);
			break;
		default:
			break;
	}
	return 0;
}
int goblinInit(pCharacter gob){
	if(gob->pos.x > WORLD_CENTER_X){
		gob->dir = LEFT;
	} else{
		gob->dir = RIGHT;
	}
	goblinChangeState(gob, ENEMY_STATE_WAIT);
	return 0;
}
int goblinWait(pCharacter gob){
	GoblinAtt *att = &gob->att.gob;
	XYd velocity = att->velocity;
	if(animaGetCount(&gob->anim) >= 1){
		bool hitFlag = false;
		if(!isPlayer1DeadEnemyBridge()){
			hitFlag |= (charaBulkHit(player1, PLAYER1_NUM, gob, HIT_GOBLIN_SEARCH) >= 0);
		}
		if(!isPlayer2DeadEnemyBridge()){
			hitFlag |= (charaBulkHit(player2, PLAYER2_NUM, gob, HIT_GOBLIN_SEARCH) >= 0);
		}
		hitFlag |= (charaBulkHit(castle, CASTLE_NUM, gob, HIT_GOBLIN_SEARCH) >= 0);
		if(hitFlag){
			goblinChangeState(gob, ENEMY_STATE_ATTACK);
			return -1;
		}
		goblinChangeState(gob, ENEMY_STATE_SEARCH);
	}
	goblinPosUpdate(gob, velocity.x, velocity.y);
	return 0;
}
int goblinWalk(pCharacter gob){
	GoblinAtt *att = &gob->att.gob;
	XYd velocity = att->velocity;
	switch(gob->dir){
		case LEFT:
			velocity.x -= att->walkSpeed;
			break;
		case RIGHT:
			velocity.x += att->walkSpeed;
			break;
		default:
			break;
	}
	if(animaGetCount(&gob->anim) >= 1){
		goblinChangeState(gob, ENEMY_STATE_SEARCH);
	}
	bool hitFlag = false;
	if(!isPlayer1DeadEnemyBridge()){
		hitFlag |= (charaBulkHit(player1, PLAYER1_NUM, gob, HIT_GOBLIN_SEARCH) >= 0);
	}
	if(!isPlayer2DeadEnemyBridge()){
		hitFlag |= (charaBulkHit(player2, PLAYER2_NUM, gob, HIT_GOBLIN_SEARCH) >= 0);
	}
	hitFlag |= (charaBulkHit(castle, CASTLE_NUM, gob, HIT_GOBLIN_SEARCH) >= 0);
	if(hitFlag){
		goblinChangeState(gob, ENEMY_STATE_WAIT);
		return -1;
	}
	goblinPosUpdate(gob, velocity.x, velocity.y);
	return 0;
}
int goblinSearch(pCharacter gob){
	static const int NUM = 3;
	const int X[NUM] = {
		getPlayer1XEnemyBridge(),
		getPlayer2XEnemyBridge(),
		getTower1XEnemyBridge()};//位置
	const bool F[NUM] = {
		isPlayer1DeadEnemyBridge(),
		isPlayer2DeadEnemyBridge(),
		false};//無視するフラグ
	const int gobX = (int)(gob->pos.x) + gob->hit.relative.x + gob->hit.size.x / 2;
	int nearestX = INT_MAX;
	int nearest = -1;
	for(int i = 0; i < NUM; i++){
		int dx = abs(X[i] - gobX);
		if(F[i]){
			dx = INT_MAX;
		}
		if(nearestX > dx){
			nearestX = dx;
			nearest = i;
		}
	}
	if(gobX > X[nearest]){
		gob->dir = LEFT;
	} else{
		gob->dir = RIGHT;
	}
	goblinChangeState(gob, ENEMY_STATE_WALK);
	return 0;
}
int goblinAttackStart(pCharacter gob){
	GoblinAtt *att = &gob->att.gob;
	static const int NUM = 3;
	const int X[NUM] = {
		getPlayer1XEnemyBridge(),
		getPlayer2XEnemyBridge(),
		getTower1XEnemyBridge()};
	const int gobX = (int)(gob->pos.x) + gob->hit.size.x / 2;
	int nearestX = INT_MAX;
	int nearest = -1;
	for(int i = 0; i < NUM; i++){
		int dx = abs(X[i] - gobX);
		if(nearestX > dx){
			nearestX = dx;
			nearest = i;
		}
	}
	if(gobX > X[nearest]){
		gob->dir = LEFT;
	} else{
		gob->dir = RIGHT;
	}
	att->atkBeforeFrame = 0;
	att->atkRectIndex = -1;
	return 0;
}
int goblinAttack(pCharacter gob){
	GoblinAtt *att = &gob->att.gob;
	XYd velocity = att->velocity;

	{
		int atkFrame = 10;//パンチ中、腕が前に突き出されるコマ
		if(att->atkBeforeFrame == (atkFrame - 1) &&
			(int)animaGetFrame(&gob->anim) == atkFrame){
			double atX = gob->pos.x;
			double atY = gob->pos.y;
			const pBox at = (pBox)getBoxData(HIT_GOBLIN_ATTACK);
			if(gob->dir == LEFT){//左向き

			} else{//右向き
				atX += gob->hit.size.x;
			}
			att->atkRectIndex = setAttacker(atX, atY, at);
			soundMgrPlay(SE_GOBLIN_PUNCH);
		}
	}
	pCharacter hitting = nullptr;
	//1pとパンチの衝突判定
	hitting = checkAttackerHitCharacters(att->atkRectIndex, player1, PLAYER1_NUM);
	if(hitting != nullptr){
		XYd _velocity = {4,-3};
		if(gob->dir == LEFT){
			_velocity.x = -_velocity.x;
		}
		player1BlowEnemyBridge(_velocity.x, _velocity.y);
		player1HitDamageEnemyBridge(att->attack);
	}
	//2pとパンチの衝突判定
	hitting = checkAttackerHitCharacters(att->atkRectIndex, player2, PLAYER2_NUM);
	if(hitting != nullptr){
		XYd _velocity = {4,-3};
		if(gob->dir == LEFT){
			_velocity.x = -_velocity.x;
		}
		player2BlowEnemyBridge(_velocity.x, _velocity.y);
		player2HitDamageEnemyBridge(att->attack);
	}
	//タワーとパンチの衝突判定
	hitting = checkAttackerHitCharacters(att->atkRectIndex, castle, CASTLE_NUM);
	if(hitting != nullptr){
		towerHitDamageEnemyBridge(hitting, att->attack);
	}
	if(att->atkBeforeFrame == 3 &&
		(int)animaGetFrame(&gob->anim) == 4){
		clearAttacker(att->atkRectIndex);
		att->atkRectIndex = -1;
	}
	if(animaGetCount(&gob->anim) >= 1){
		goblinChangeState(gob, ENEMY_STATE_WAIT);
	}
	goblinPosUpdate(gob, velocity.x, velocity.y);
	att->atkBeforeFrame = (int)animaGetFrame(&gob->anim);
	return 0;
}
int goblinDamageStop(pCharacter gob){
	GoblinAtt *att = &gob->att.gob;
	XYd velocity = att->velocity;
	if(att->stopCount >= att->stopLimit){
		goblinChangeState(gob, ENEMY_STATE_DAMAGE);
		return -1;
	}

	goblinPosUpdate(gob, rand() % 12 - 6, rand() % 12 - 6);
	goblinPosUpdate(gob, velocity.x, velocity.y);
	att->stopCount++;
	return 0;
}
int goblinDamage(pCharacter gob){
	GoblinAtt *att = &gob->att.gob;

	double kbX = att->knockBackX;
	double kbY = att->knockBackY;

	kbX *= 0.96;
	kbY *= 0.96;

	goblinPosUpdate(gob, kbX, kbY);
	att->knockBackX = kbX;
	att->knockBackY = kbY;

	if(animaGetCount(&gob->anim) >= 1){//アニメーションで被ダメ状態解除
		if((att->hp) <= 0){//HPがなければ消える状態へ
			goblinChangeState(gob, ENEMY_STATE_DEATH);
		} else{
			goblinChangeState(gob, ENEMY_STATE_WAIT);
		}
		return -1;
	}

	return 0;
}
int goblinDeath(pCharacter gob){
	GoblinAtt *att = &gob->att.gob;
	//非表示にする
	gob->draw.doHide = true;
	//エフェクトを出す
	if(att->deadCounter == 0){
		double x = gob->pos.x + gob->draw.relative.x;
		double y = gob->pos.y + gob->draw.relative.y;
		enemyDeadEffectSet(ENEMY_GOBLIN, x, y, gob->dir);
		x += gob->draw.size.x / 2;
		y += gob->draw.size.y / 2;
		newEffectGoblinDeathDustSet(x, y, gob->dir);
	}
	//時間が来たら消える
	if(att->deadCounter > ENEMY_DEATH_DELAY){
		gob->isActive = false;
	}
	att->deadCounter++;
	return 0;
}
int goblinPosUpdate(pCharacter gob, double vx, double vy){
	int result = 0;
	GoblinAtt *att = &gob->att.gob;
	double x, y;
	double gravity = att->gravity;
	getCharaOffSet(gob, &x, &y);

	vy += gravity;
	vx *= 0.7;
	vy *= 0.96;

	x += vx;
	y += vy;

	charaOffSet(gob, x, y);

	int hitInfo = charaBlockingByBlock((void*)gob);
	if(charaLanding(hitInfo)){
		//着地
		vy = 0;
		result = -1;
	}

	//世界の外に出たら死亡
	if(x < (0 - gob->hit.size.x) || x > (WORLD_WIDTH + gob->hit.size.x)){
		charaClear(gob);
	}

	att->velocity.x = vx;
	att->velocity.y = vy;
	return result;
}

