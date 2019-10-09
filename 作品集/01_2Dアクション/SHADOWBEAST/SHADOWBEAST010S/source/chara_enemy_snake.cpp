
#include <Windows.h>
#include "amgame.h"

#include "game_state.h"
#include "chara_enemy.h"
#include "chara__common.h"

#include "image_manager.h"
#include "chara_block.h"
#include "chara_attacker.h"
#include "chara__hit_rect.h"
#include "chara_enemy_snake.h"
#include "chara_enemy_wyvern.h"
#include "chara_enemy_death.h"
#include "in_game_effect.h"
#include "chara__manager.h"
#include "stage_data_reader.h"

#include "common.h"
#include "camera.h"

#include "forDebug.h"


/*
スネーク
*/




extern int snakeSet(double x, double y);
extern int snakeHitDamage(pCharacter wyv, int damage);
extern int snakeBlow(pCharacter wyv, double vx, double vy);


static int snakeUpdate(pCharacter snk);

static int snakeChangeState(pCharacter snk, EnemyState nextState);
static int snakeStateEnter(pCharacter snk);
static int snakeStateExit(pCharacter snk);
static int snakeInit(pCharacter snk);
static int snakeSection(pCharacter snk);
static int snakeWait(pCharacter snk);
static int snakeWalk(pCharacter snk);
static int snakeSearch(pCharacter snk);
static int SnakeAttackStart(pCharacter snk);
static int SnakeAttack(pCharacter snk);
static int snakeDamageStop(pCharacter snk);
static int snakeDamage(pCharacter snk);
static int snakeDeath(pCharacter snk);
static int snakePosUpdate(pCharacter snk, double vx, double vy);

int snakeSet(double x, double y){
	int found = charaFindNonActive(enemies, ENEMY_NUM);
	if(found < 0){
		return -1;
	}
	pCharacter snk = &enemies[found];
	SnakeAtt *att = &snk->att.snk;
	ImageFileId fileId = IMAGE_SNAKE_WAIT;
	const int hsnk = imageMgrGetHandle(fileId, 0);
	charaInit(snk);
	snk->isActive = true;
	charaSetDispSize(snk, fileId);
	pBox snkBox = (pBox)getBoxData(HIT_SNAKE_G);
	snk->hit = *snkBox;
	*att = snakeAttBase;
	charaOffSet(snk, x - snk->hit.size.x / 2, y - snk->hit.size.y);
	charaColSet(snk, HIT_SNAKE_G);
	charaSetImageHandle(snk, hsnk);
	charaHome(snk, snk->hit.size.x / 2, snk->hit.size.y / 2);
	charaFuncSet(snk, snakeUpdate);
	snakeChangeState(snk, ENEMY_STATE_INIT);
	snakeUpdate(snk);
	return 0;
}
int snakeUpdate(pCharacter snk){
	snk->anim.time = getGameFrame();
	snakeSection(snk);

	int index = animaGetValueStep(&snk->anim);
	int handle = imageMgrGetHandle(snk->imageFileId, index);
	charaSetImageHandle(snk, handle);
	charaFlipX(snk, snk->dir == RIGHT);
	//玉を出す
	if(!(rand() % 3)){
		double _x = snk->pos.x + snk->hit.relative.x;
		double _y = snk->pos.y + snk->hit.relative.y;
		_x += rand() % snk->hit.size.x;
		_y += rand() % snk->hit.size.y;
		InGameEffectType _type = IN_GAME_EFFECT_DARK_BALL;
		inGameEffctSet(_type, _x, _y, EFFECT_FRONT);
	}
	return 0;
}
int snakeChangeState(pCharacter snk, EnemyState nextState){
	SnakeAtt *att = &snk->att.snk;
	//死んでいれば状態チェンジしない
	if(att->isDead){
		return -1;
	}
	snk->anim.started = getGameFrame();
	snakeStateExit(snk);

	att->state = nextState;

	snakeStateEnter(snk);
	return 0;
}
int snakeStateEnter(pCharacter snk){
	SnakeAtt *att = &snk->att.snk;
	switch(att->state){
		case ENEMY_STATE_INIT:
			snk->imageFileId = IMAGE_SNAKE_WAIT;
			break;
		case ENEMY_STATE_WAIT:
			snk->imageFileId = IMAGE_SNAKE_WAIT;
			break;
		case ENEMY_STATE_WALK:
			snk->imageFileId = IMAGE_SNAKE_WALK;
			break;
		case ENEMY_STATE_SEARCH:
			break;
		case ENEMY_STATE_ATTACK:
			SnakeAttackStart(snk);
			snk->imageFileId = IMAGE_SNAKE_ATTACK;
			break;
		case ENEMY_STATE_DAMAGE_STOP:
			snk->imageFileId = IMAGE_SNAKE_DAMAGE;
			att->stopCoord.x = snk->pos.x;
			att->stopCoord.y = snk->pos.y;
			att->stopCount = 0;
			break;
		case ENEMY_STATE_DAMAGE:
			charaOffSet(snk, att->stopCoord.x, att->stopCoord.y);
			charaOffSet(snk, att->stopCoord.x, att->stopCoord.y);
			break;
		case ENEMY_STATE_DEATH:
			att->isDead = true;
			att->deadCounter = 0;
			break;
		default:
			break;
	}
	AnimaData creature = {0};
	createAnimaData(&creature, snk->imageFileId);
	animaSetup(&snk->anim,
			   getGameFrame(),
			   &creature);
	return 0;
}
int snakeStateExit(pCharacter snk){
	SnakeAtt *att = &snk->att.snk;
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
int snakeSection(pCharacter snk){
	SnakeAtt *att = &snk->att.snk;
	switch(att->state){
		case ENEMY_STATE_INIT:
			snakeInit(snk);
			snakeSection(snk);
			break;
		case ENEMY_STATE_WAIT:
			snakeWait(snk);
			break;
		case ENEMY_STATE_WALK:
			snakeWalk(snk);
			break;
		case ENEMY_STATE_SEARCH:
			snakeSearch(snk);
			break;
		case ENEMY_STATE_ATTACK:
			SnakeAttack(snk);
			break;
		case ENEMY_STATE_DAMAGE_STOP:
			snakeDamageStop(snk);
			break;
		case ENEMY_STATE_DAMAGE:
			snakeDamage(snk);
			break;
		case ENEMY_STATE_DEATH:
			snakeDeath(snk);
			break;
		default:
			break;
	}
	return 0;
}
int snakeInit(pCharacter snk){
	if(snk->pos.x > WORLD_CENTER_X){
		snk->dir = LEFT;
	} else{
		snk->dir = RIGHT;
	}
	snakeChangeState(snk, ENEMY_STATE_WAIT);
	return 0;
}
int snakeWait(pCharacter snk){
	SnakeAtt *att = &snk->att.snk;
	XYd velocity = att->velocity;
	if(animaGetCount(&snk->anim) >= 1){
		bool hitFlag = false;
		if(!isPlayer1DeadEnemyBridge()){
			hitFlag |= (charaBulkHit(player1, PLAYER1_NUM, snk, HIT_SNAKE_SEARCH) >= 0);
		}
		if(!isPlayer2DeadEnemyBridge()){
			hitFlag |= (charaBulkHit(player2, PLAYER2_NUM, snk, HIT_SNAKE_SEARCH) >= 0);
		}
		hitFlag |= (charaBulkHit(castle, CASTLE_NUM, snk, HIT_SNAKE_SEARCH) >= 0);
		if(hitFlag){
			snakeChangeState(snk, ENEMY_STATE_ATTACK);
			return -1;
		}
		snakeChangeState(snk, ENEMY_STATE_SEARCH);
	}
	snakePosUpdate(snk, velocity.x, velocity.y);
	return 0;
}
int snakeWalk(pCharacter snk){
	SnakeAtt *att = &snk->att.snk;
	XYd velocity = att->velocity;
	switch(snk->dir){
		case LEFT:
			velocity.x -= att->walkSpeed;
			break;
		case RIGHT:
			velocity.x += att->walkSpeed;
			break;
		default:
			break;
	}
	if(animaGetCount(&snk->anim) >= 1){
		snakeChangeState(snk, ENEMY_STATE_SEARCH);
	}
	bool hitFlag = false;
	if(!isPlayer1DeadEnemyBridge()){
		hitFlag |= (charaBulkHit(player1, PLAYER1_NUM, snk, HIT_SNAKE_SEARCH) >= 0);
	}
	if(!isPlayer2DeadEnemyBridge()){
		hitFlag |= (charaBulkHit(player2, PLAYER2_NUM, snk, HIT_SNAKE_SEARCH) >= 0);
	}
	hitFlag |= (charaBulkHit(castle, CASTLE_NUM, snk, HIT_SNAKE_SEARCH) >= 0);
	if(hitFlag){
		snakeChangeState(snk, ENEMY_STATE_WAIT);
		return -1;
	}
	snakePosUpdate(snk, velocity.x, velocity.y);
	return 0;
}
int snakeSearch(pCharacter snk){
	static const int NUM = 3;
	const int X[NUM] = {
		getPlayer1XEnemyBridge(),
		getPlayer2XEnemyBridge(),
		getTower1XEnemyBridge()};//位置
	const bool F[NUM] = {
		isPlayer1DeadEnemyBridge(),
		isPlayer2DeadEnemyBridge(),
		false};//無視するフラグ
	const int snkX = (int)(snk->pos.x) + snk->hit.relative.x + snk->hit.size.x / 2;
	int nearestX = INT_MAX;
	int nearest = -1;
	for(int i = 0; i < NUM; i++){
		int dx = abs(X[i] - snkX);
		if(F[i]){
			dx = INT_MAX;
		}
		if(nearestX > dx){
			nearestX = dx;
			nearest = i;
		}
	}
	if(snkX > X[nearest]){
		snk->dir = LEFT;
	} else{
		snk->dir = RIGHT;
	}
	snakeChangeState(snk, ENEMY_STATE_WALK);
	return 0;
}
int SnakeAttackStart(pCharacter snk){
	SnakeAtt *att = &snk->att.snk;
	static const int NUM = 3;
	const int X[NUM] = {
		getPlayer1XEnemyBridge(),
		getPlayer2XEnemyBridge(),
		getTower1XEnemyBridge()};
	const int snkX = (int)(snk->pos.x) + snk->hit.size.x / 2;
	int nearestX = INT_MAX;
	int nearest = -1;
	for(int i = 0; i < NUM; i++){
		int dx = abs(X[i] - snkX);
		if(nearestX > dx){
			nearestX = dx;
			nearest = i;
		}
	}
	if(snkX > X[nearest]){
		snk->dir = LEFT;
	} else{
		snk->dir = RIGHT;
	}
	att->atkBeforeFrame = 0;
	att->atkRectIndex = -1;
	return 0;
}
int SnakeAttack(pCharacter snk){
	SnakeAtt *att = &snk->att.snk;
	XYd velocity = att->velocity;

	{
		int atkFrame = 8;//パンチ中、腕が前に突き出されるコマ
		if(att->atkBeforeFrame == (atkFrame - 1) &&
			(int)animaGetFrame(&snk->anim) == atkFrame){
			double atX = snk->pos.x;
			double atY = snk->pos.y;
			const pBox at = (pBox)getBoxData(HIT_SNAKE_ATTACK);
			if(snk->dir == LEFT){//左向き
				atX += snk->hit.relative.x;
				atX -= at->relative.x;
				atX -= at->size.x;
			} else{//右向き
				atX += snk->hit.size.x;
			}
			att->atkRectIndex = setAttacker(atX, atY, at);
		}
	}
	pCharacter hitting = nullptr;
	//1pとパンチの衝突判定
	hitting = checkAttackerHitCharacters(att->atkRectIndex, player1, PLAYER1_NUM);
	if(hitting != nullptr){
		XYd _velocity = {4,-3};
		if(snk->dir == LEFT){
			_velocity.x = -_velocity.x;
		}
		player1BlowEnemyBridge(_velocity.x, _velocity.y);
		player1HitDamageEnemyBridge(att->attack);
	}
	//2pとパンチの衝突判定
	hitting = checkAttackerHitCharacters(att->atkRectIndex, player2, PLAYER2_NUM);
	if(hitting != nullptr){
		XYd _velocity = {4,-3};
		if(snk->dir == LEFT){
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
		(int)animaGetFrame(&snk->anim) == 4){
		clearAttacker(att->atkRectIndex);
		att->atkRectIndex = -1;
	}
	if(animaGetCount(&snk->anim) >= 1){
		snakeChangeState(snk, ENEMY_STATE_WAIT);
	}
	snakePosUpdate(snk, velocity.x, velocity.y);
	att->atkBeforeFrame = (int)animaGetFrame(&snk->anim);
	return 0;
}
int snakeBlow(pCharacter snk, double vx, double vy){
	SnakeAtt *att = &snk->att.snk;
	att->knockBackX = vx;
	att->knockBackY = vy;
	bool stateDead = false;
	stateDead |= (att->state == ENEMY_STATE_DAMAGE_STOP);
	stateDead |= (att->state == ENEMY_STATE_DAMAGE);
	if(stateDead && (att->hp) <= 0){
		return -1;
	}
	snakeChangeState(snk, ENEMY_STATE_DAMAGE_STOP);
	return 0;
}
int snakeHitDamage(pCharacter snk, int damage){
	SnakeAtt *att = &snk->att.snk;
	int _hp = att->hp;
	att->hp -= damage;
	if(_hp > 0 && att->hp <= 0){
		return -1;
	}
	return 0;
}
int snakeDamageStop(pCharacter snk){
	SnakeAtt *att = &snk->att.snk;
	XYd velocity = att->velocity;
	if(att->stopCount >= att->stopLimit){
		snakeChangeState(snk, ENEMY_STATE_DAMAGE);
		return -1;
	}

	snakePosUpdate(snk, rand() % 12 - 6, rand() % 12 - 6);
	snakePosUpdate(snk, velocity.x, velocity.y);
	att->stopCount++;
	return 0;
}
int snakeDamage(pCharacter snk){
	SnakeAtt *att = &snk->att.snk;
	XYd velocity = att->velocity;

	double kbX = att->knockBackX;
	double kbY = att->knockBackY;

	kbX *= 0.96;
	kbY *= 0.96;

	snakePosUpdate(snk, kbX, kbY);
	att->knockBackX = kbX;
	att->knockBackY = kbY;

	if(animaGetCount(&snk->anim) >= 1){//アニメーションで被ダメ状態解除
		if((att->hp) <= 0){//HPがなければ消える状態へ
			snakeChangeState(snk, ENEMY_STATE_DEATH);
		} else{
			snakeChangeState(snk, ENEMY_STATE_WAIT);
		}
		return -1;
	}

	snakePosUpdate(snk, velocity.x, velocity.y);
	return 0;
}
int snakeDeath(pCharacter snk){
	SnakeAtt *att = &snk->att.snk;
	//非表示にする
	snk->draw.doHide = true;
	//エフェクトを出す
	if(att->deadCounter == 0){
		double x = snk->pos.x + snk->draw.relative.x;
		double y = snk->pos.y + snk->draw.relative.y;
		enemyDeadEffectSet(ENEMY_SNAKE, x, y, snk->dir);
	}
	//時間が来たら消える
	if(att->deadCounter > ENEMY_DEATH_DELAY){
		snk->isActive = false;
	}
	att->deadCounter++;
	return 0;
}
int snakePosUpdate(pCharacter snk, double vx, double vy){
	int result = 0;
	SnakeAtt *att = &snk->att.snk;
	double x, y;
	double gravity = att->gravity;
	getCharaOffSet(snk, &x, &y);

	vy += gravity;
	vx *= 0.7;
	vy *= 0.96;

	x += vx;
	y += vy;

	charaOffSet(snk, x, y);

	int hitInfo = charaBlockingByBlock((void*)snk);
	if(charaLanding(hitInfo)){
		//着地
		vy = 0;
		result = -1;
	}

	//世界の外に出たら死亡
	if(x < (0 - snk->hit.size.x) || x >(WORLD_WIDTH + snk->hit.size.x)){
		charaClear(snk);
	}

	att->velocity.x = vx;
	att->velocity.y = vy;
	return result;
}

