
#include <Windows.h>
#include "amgame.h"

#include "game_state.h"
#include "chara_enemy.h"
#include "chara__common.h"

#include "image_manager.h"
#include "sound_manager.h"
#include "chara_player1.h"
#include "chara_player2.h"
#include "chara_castle.h"
#include "chara_block.h"
#include "chara_attacker.h"
#include "chara__hit_rect.h"
#include "chara_enemy_death.h"
#include "in_game_effect.h"
#include "chara__manager.h"
#include "stage_data_reader.h"
#include "ui_mgr.h"

#include "common.h"
#include "camera.h"

#include "forDebug.h"


/*
ボス1
*/



extern int boss1Set(double x, double y);
extern int boss1Blow(pCharacter boss, double vx, double vy);
extern int boss1HitDamage(pCharacter boss, int damage);
extern int getBoss1FileIdForEffect(ImageFileId *id, int *animIndex, bool *flipFlag);
static ImageFileId fileIdForEffect = IMAGE_NONE;//最後に更新されたボスの画像ファイルID
static int animIndexForEffect = 0;//最後に更新されたボスのアニメーション番号
static bool flipFlagForEffect = false;//最後に更新されたボスの横反転フラグ

static int bossUpdate(pCharacter bs);

static int bossChangeState(pCharacter bs, EnemyState nextState);
static int bossStateEnter(pCharacter bs);
static int bossStateExit(pCharacter bs);
static int bossInit(pCharacter bs);
static int bossSection(pCharacter bs);
static int bossWait(pCharacter bs);
static int bossWalk(pCharacter bs);
static int bossSearch(pCharacter bs);
static int boss1AttackWait(pCharacter bs);
static int boss1Attack1Start(pCharacter bs);
static int boss1Attack1(pCharacter bs);
static int boss1Attack1After(pCharacter bs);
static int boss1Attack2Start(pCharacter bs);
static int boss1Attack2(pCharacter bs);
static int boss1Attack2After(pCharacter bs);
static int boss1Attack3Start(pCharacter bs);
static int boss1Attack3(pCharacter bs);
static int boss1Attack3After(pCharacter bs);
static int bossDamageStop(pCharacter bs);
static int bossDamage(pCharacter bs);
static int bossDeath(pCharacter bs);
static int bossPosUpdate(pCharacter bs, double vx, double vy);

int boss1Set(double x, double y){
	int found = charaFindNonActive(enemies, ENEMY_NUM);
	if(found < 0){
		return -1;
	}
	pCharacter bs = &enemies[found];
	TitanAtt *att = &bs->att.titan;
	ImageFileId fileId = IMAGE_BOSS1_WAIT;
	const int hbs = imageMgrGetHandle(fileId, 0);
	charaInit(bs);
	bs->isActive = true;
	pBox bsBox = (pBox)getBoxData(HIT_BOSS1_G);
	bs->hit = *bsBox;
	*att = titanAttBase;
	charaOffSet(bs,
				x - (bs->hit.relative.x + bs->hit.size.x / 2),
				y - (bs->hit.relative.y + bs->hit.size.y / 2));
	charaColSet(bs, HIT_BOSS1_G);
	charaSetImageHandle(bs, hbs);
	charaHome(bs, bs->hit.size.x / 2, bs->hit.size.y / 2);
	charaFuncSet(bs, bossUpdate);
	bossChangeState(bs, ENEMY_STATE_INIT);
	bossUpdate(bs);
	return 0;
}
int boss1Blow(pCharacter bs, double vx, double vy){
	TitanAtt *att = &bs->att.titan;
	//死亡判定
	bool stateDead = false;
	stateDead |= (att->state == ENEMY_STATE_DAMAGE_STOP);
	stateDead |= (att->state == ENEMY_STATE_DAMAGE);
	if(stateDead && (att->hp) <= 0){
		return -1;
	}
	att->damegeCount = 8;
	att->knockBackX = vx;
	att->knockBackY = vy;
	double _d = hypot(vx, vy);
	if(_d > 10){
		bossChangeState(bs, ENEMY_STATE_DAMAGE_STOP);
	}
	return 0;
}
int boss1HitDamage(pCharacter bs, int damage){
	TitanAtt *att = &bs->att.titan;
	int _hp = att->hp;
	att->hp -= damage;
	if(_hp > 0 && att->hp <= 0){
		//死亡した
		att->hp = 0;
		return -1;
	}
	return 0;
}
int getBoss1FileIdForEffect(ImageFileId *id, int *animIndex, bool *flipFlag){
	*id = fileIdForEffect;
	*animIndex = animIndexForEffect;
	*flipFlag = flipFlagForEffect;
	return 0;
}
int bossUpdate(pCharacter bs){
	TitanAtt *att = &bs->att.titan;
	bs->anim.time = getGameFrame();
	bossSection(bs);

	int index = animaGetValueStep(&bs->anim);
	int handle = imageMgrGetHandle(bs->imageFileId, index);
	animIndexForEffect = index;
	charaSetImageHandle(bs, handle);
	charaFlipX(bs, bs->dir == RIGHT);
	fileIdForEffect = bs->imageFileId;
	flipFlagForEffect = bs->draw.flipFlagX;
	//被ダメ状態処理
	if(att->damegeCount > 0){
		int _x = 0, _y = 0;
		_x = (int)(bs->pos.x);
		_x += bs->draw.relative.x;
		_x += bs->draw.size.x / 2;
		_x += rand() % 16;
		_y = (int)(bs->pos.y);
		_y += bs->draw.relative.y;
		_y += bs->draw.size.y / 2;
		_y += rand() % 16;
		InGameEffectType _type = IN_GAME_EFFECT_BOSS1_DAMAGE;
		inGameEffctSet(_type, _x, _y, EFFECT_FRONT);
		att->damegeCount--;
		if(att->damegeCount == 0){
			if((att->hp) <= 0){
				bossChangeState(bs, ENEMY_STATE_DAMAGE_STOP);
			}
		}
	}
	//揺らす
	if(att->quakeCount > 0){
		att->quakeCount--;
		if(att->quakeCount > 0){
			int w = 30;
			int w2 = w / 2;
			bs->draw.quake.x = (rand() % w) - w2;
			bs->draw.quake.y = (rand() % w) - w2;
		} else{
			bs->draw.quake = {};
		}
	}
	//玉を出す
	{
		double _x = bs->pos.x + bs->hit.relative.x;
		double _y = bs->pos.y + bs->hit.relative.y;
		_x += rand() % bs->hit.size.x;
		_y += rand() % bs->hit.size.y;
		InGameEffectType _type = IN_GAME_EFFECT_BLACK_GRAIN;
		inGameEffctSet(_type, _x, _y, EFFECT_BACK);
	}

	//ui
	uiMgrTeachBossHp(att->hp, titanAttBase.hp);

   return 0;
}
int bossChangeState(pCharacter bs, EnemyState nextState){
	TitanAtt *att = &bs->att.titan;
	//死んでいれば状態チェンジしない
	if(att->isDead){
		return -1;
	}
	bs->anim.started = getGameFrame();
	bossStateExit(bs);

	att->state = nextState;

	bossStateEnter(bs);
	return 0;
}
int bossStateEnter(pCharacter bs){
	TitanAtt *att = &bs->att.titan;
	charaSetDispSize(bs, DISP_BOSS1_NORMAL);
	switch(att->state){
		case ENEMY_STATE_INIT:
			bs->imageFileId = IMAGE_BOSS1_WAIT;
			break;
		case ENEMY_STATE_WAIT:
			bs->imageFileId = IMAGE_BOSS1_WAIT;
			break;
		case ENEMY_STATE_WALK:
			bs->imageFileId = IMAGE_BOSS1_WALK;
			break;
		case ENEMY_STATE_SEARCH:
			break;
		case ENEMY_STATE_BOSS_ATTACK1_BEFORE:
			att->attackWaitCount = 0;
			break;
		case ENEMY_STATE_BOSS_ATTACK1_MAIN:
			boss1Attack1Start(bs);
			bs->imageFileId = IMAGE_BOSS1_ATTACK1;
			break;
		case ENEMY_STATE_BOSS_ATTACK2_MAIN:
			boss1Attack2Start(bs);
			bs->imageFileId = IMAGE_BOSS1_ATTACK2;
			break;
		case ENEMY_STATE_BOSS_ATTACK3_MAIN:
			boss1Attack3Start(bs);
			bs->imageFileId = IMAGE_BOSS1_ATTACK3;
			break;
		case ENEMY_STATE_BOSS_ATTACK1_AFTER:
		case ENEMY_STATE_BOSS_ATTACK2_AFTER:
		case ENEMY_STATE_BOSS_ATTACK3_AFTER:
			bs->imageFileId = IMAGE_BOSS1_WAIT;
			att->stiffnessCount = 0;
			break;
		case ENEMY_STATE_DAMAGE_STOP:
			bs->imageFileId = IMAGE_BOSS1_DAMAGE;
			att->stopCoord.x = bs->pos.x;
			att->stopCoord.y = bs->pos.y;
			att->stopCount = 0;
			break;
		case ENEMY_STATE_DAMAGE:
			bs->imageFileId = IMAGE_BOSS1_DAMAGE;
			charaOffSet(bs, att->stopCoord.x, att->stopCoord.y);
			charaOffSet(bs, att->stopCoord.x, att->stopCoord.y);
			break;
		case ENEMY_STATE_DEATH:
			att->isDead = true;
			att->deadCounter = 0;
			break;
		default:
			break;
	}
	AnimaData creature = {0};
	createAnimaData(&creature, bs->imageFileId);
	animaSetup(&bs->anim,
			   getGameFrame(),
			   &creature);
	return 0;
}
int bossStateExit(pCharacter bs){
	TitanAtt *att = &bs->att.titan;
	switch(att->state){
		case ENEMY_STATE_INIT:
			break;
		case ENEMY_STATE_WAIT:
			break;
		case ENEMY_STATE_WALK:
			break;
		case ENEMY_STATE_SEARCH:
			break;
		case ENEMY_STATE_BOSS_ATTACK1_BEFORE:
			break;
		case ENEMY_STATE_BOSS_ATTACK1_MAIN:
			clearAttacker(att->atk1RectIndex);
			att->atk1RectIndex = -1;
			break;
		case ENEMY_STATE_BOSS_ATTACK2_MAIN:
			clearAttacker(att->atk2RectIndexL);
			clearAttacker(att->atk2RectIndexR);
			att->atk2RectIndexL = -1;
			att->atk2RectIndexR = -1;
			break;
		case ENEMY_STATE_BOSS_ATTACK3_MAIN:
			clearAttacker(att->atk3RectIndex);
			att->atk3RectIndex = -1;
			break;
		case ENEMY_STATE_BOSS_ATTACK1_AFTER:
		case ENEMY_STATE_BOSS_ATTACK2_AFTER:
		case ENEMY_STATE_BOSS_ATTACK3_AFTER:
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
int bossSection(pCharacter bs){
	TitanAtt *att = &bs->att.titan;
	switch(att->state){
		case ENEMY_STATE_INIT:
			bossInit(bs);
			bossSection(bs);
			break;
		case ENEMY_STATE_WAIT:
			bossWait(bs);
			break;
		case ENEMY_STATE_WALK:
			bossWalk(bs);
			break;
		case ENEMY_STATE_SEARCH:
			bossSearch(bs);
			bossChangeState(bs, ENEMY_STATE_WALK);
			break;
		case ENEMY_STATE_BOSS_ATTACK1_BEFORE:
			boss1AttackWait(bs);
			break;
		case ENEMY_STATE_BOSS_ATTACK1_MAIN:
			boss1Attack1(bs);
			break;
		case ENEMY_STATE_BOSS_ATTACK1_AFTER:
			boss1Attack1After(bs);
			break;
		case ENEMY_STATE_BOSS_ATTACK2_MAIN:
			boss1Attack2(bs);
			break;
		case ENEMY_STATE_BOSS_ATTACK2_AFTER:
			boss1Attack2After(bs);
			break;
		case ENEMY_STATE_BOSS_ATTACK3_MAIN:
			boss1Attack3(bs);
			break;
		case ENEMY_STATE_BOSS_ATTACK3_AFTER:
			boss1Attack3After(bs);
			break;
		case ENEMY_STATE_DAMAGE_STOP:
			bossDamageStop(bs);
			break;
		case ENEMY_STATE_DAMAGE:
			bossDamage(bs);
			break;
		case ENEMY_STATE_DEATH:
			bossDeath(bs);
			break;
		default:
			break;
	}
	return 0;
}
int bossInit(pCharacter bs){
	if(bs->pos.x > WORLD_CENTER_X){
		bs->dir = LEFT;
	} else{
		bs->dir = RIGHT;
	}
	bossChangeState(bs, ENEMY_STATE_WAIT);
	return 0;
}
int bossWait(pCharacter bs){
	TitanAtt *att = &bs->att.titan;
	XYd velocity = att->velocity;
	if(animaGetCount(&bs->anim) >= 1){
		bossChangeState(bs, ENEMY_STATE_SEARCH);
		return -1;
	}
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
int bossWalk(pCharacter bs){
	TitanAtt *att = &bs->att.titan;
	XYd velocity = att->velocity;
	switch(bs->dir){
		case LEFT:
			velocity.x -= att->walkSpeed;
			break;
		case RIGHT:
			velocity.x += att->walkSpeed;
			break;
		default:
			break;
	}
	if(animaGetCount(&bs->anim) >= 1){
		bossChangeState(bs, ENEMY_STATE_SEARCH);
	}
	//プレイヤーを感知したら攻撃前硬直へ移行し、その後攻撃↓
	bool hitFlag = false;
	if(!isPlayer1Dead()){
		hitFlag |= (charaBulkHit(player1, PLAYER1_NUM, bs, HIT_BOSS1_SEARCH) >= 0);
	}
	if(!isPlayer2Dead()){
		hitFlag |= (charaBulkHit(player2, PLAYER2_NUM, bs, HIT_BOSS1_SEARCH) >= 0);
	}
	if(isPlayer1Dead() || isPlayer2Dead()){
		hitFlag |= (charaBulkHit(castle, CASTLE_NUM, bs, HIT_BOSS1_SEARCH) >= 0);
	}
	if(hitFlag){
		bossChangeState(bs, ENEMY_STATE_BOSS_ATTACK1_BEFORE);
		return -1;
	}
	//プレイヤーを感知したら攻撃前硬直へ移行し、その後攻撃↑
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
int bossSearch(pCharacter bs){//方向決定
	static constexpr int NUM = 2;
	int X[NUM] = {getPlayer1X(),getPlayer2X()};//位置
	bool F[NUM] = {isPlayer1Dead(),isPlayer2Dead()};//無視するフラグ
	const int bsX = (int)(bs->pos.x) + bs->hit.relative.x + bs->hit.size.x / 2;
	int nearestX = INT_MAX;
	int nearest = -1;
	//1Pか2Pが戦闘不能ならオブジェクトを狙う↓
	if(F[0] || F[1]){
		F[0] = false;
		F[1] = true;
		X[0] = getCastleX();
	}
	//1Pか2Pが戦闘不能ならオブジェクトを狙う↑
	for(int i = 0; i < NUM; i++){
		int dx = abs(X[i] - bsX);
		if(F[i]){
			dx = INT_MAX;
		}
		if(nearestX > dx){
			nearestX = dx;
			nearest = i;
		}
	}
	if(bsX > X[nearest]){
		bs->dir = LEFT;
	} else{
		bs->dir = RIGHT;
	}
	return 0;
}
int boss1AttackWait(pCharacter bs){
	TitanAtt *att = &bs->att.titan;
	XYd velocity = att->velocity;
	if(att->attackWaitCount >= att->attackWaitLimit){
		//攻撃する
		EnemyState nextState;
		switch(att->atkControlller){
			case 0:
				nextState = ENEMY_STATE_BOSS_ATTACK1_MAIN;
				break;
			case 1:
				nextState = ENEMY_STATE_BOSS_ATTACK2_MAIN;
				break;
			case 2:
				nextState = ENEMY_STATE_BOSS_ATTACK3_MAIN;
				break;
			default:
				nextState = ENEMY_STATE_BOSS_ATTACK1_MAIN;
				break;
		}
		att->atkControlller++;///////////////////////////////////////////////////
		att->atkControlller %= 3;
		bossChangeState(bs, nextState);
	}

	att->attackWaitCount++;
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
int boss1Attack1Start(pCharacter bs){
	//攻撃1 : 棍棒振り下ろしの開始タイミングで一回だけ呼ばれる
	bossSearch(bs);
	//当たり判定矩形を出す
	TitanAtt *att = &bs->att.titan;
	pBox at = (pBox)getBoxData(HIT_BOSS1_ATTACK1);
	int x = (int)(bs->pos.x) + bs->hit.relative.x;
	int y = (int)(bs->pos.y) + bs->hit.relative.y;
	int w = bs->hit.size.x;
	int h = bs->hit.size.y;
	att->atk1RectIndex = setAttacker(x + w / 2, y, at);
	double startX = 0, startY = 0;
	double goalX = 0, goalY = 0;
	startX = x + w / 2;
	startY = y;
	if(bs->dir == LEFT){
		goalX = x;
	} else{
		goalX = x + w;
	}
	goalY = y + h;
	att->atk1X = {EASE_OUT_SINE,startX,startX,goalX};
	att->atk1Y = {EASE_IN_SINE,startY,startY,goalY};
	//ここまで
	//画像ずれ修正↓
	if(bs->dir == LEFT){
		charaSetDispSize(bs, DISP_BOSS1_ATTACK_L);
	} else{
		charaSetDispSize(bs, DISP_BOSS1_ATTACK_R);
	}
	//画像ずれ修正↑
	return 0;
}
int boss1Attack1(pCharacter bs){
	//ボス1振り下ろし攻撃メイン部分
	TitanAtt *att = &bs->att.titan;
	int time = bs->anim.time - bs->anim.started;
	int duration = bs->anim.duration;
	int atkIndex = att->atk1RectIndex;
	XYd velocity = att->velocity;
	int _start = (int)(16.0 / 20.0 * 60.0);//振り下ろし開始フレーム
	int _duration = 3;//振り下ろし期間
	int _time = time - _start;//経過時間
	//振り下ろしが始まったら当たり判定矩形を動かす
	if(time >= _start && _time <= _duration){
		if(time == _start){
			soundMgrPlay(SE_TITAN_ATTACK);
		}
		ParameterUpdate(&att->atk1X, time - _start, _duration);
		ParameterUpdate(&att->atk1Y, time - _start, _duration);
		attackerOffset(atkIndex, att->atk1X.current, att->atk1Y.current);
		//ここまで
		pCharacter hitting = nullptr;
		//1pとの衝突判定
		hitting = checkAttackerHitCharacters(atkIndex, player1, PLAYER1_NUM);
		if(hitting != nullptr){
			XYd _velocity = {4,-3};
			if(bs->dir == LEFT){
				_velocity.x = -_velocity.x;
			}
			player1Blow(_velocity.x, _velocity.y);
			player1HitDamage(att->attack);
		}
		//2pとの衝突判定
		hitting = checkAttackerHitCharacters(atkIndex, player2, PLAYER2_NUM);
		if(hitting != nullptr){
			XYd _velocity = {4,-3};
			if(bs->dir == LEFT){
				_velocity.x = -_velocity.x;
			}
			player2Blow(_velocity.x, _velocity.y);
			player2HitDamage(att->attack);
		}
		//タワーとの衝突判定
		hitting = checkAttackerHitCharacters(atkIndex, castle, CASTLE_NUM);
		if(hitting != nullptr){
			castleHitDamage(hitting, att->attack);
		}
		//衝突判定ここまで
	} else if(_time > _duration){
		clearAttacker(atkIndex);
	}
	//アニメーションが終わったら状態を切り替える
	if(time >= duration){
		bossChangeState(bs, ENEMY_STATE_BOSS_ATTACK1_AFTER);
	}
	//位置更新
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
int boss1Attack1After(pCharacter bs){
	TitanAtt *att = &bs->att.titan;
	XYd velocity = att->velocity;
	if(att->stiffnessCount>=att->stiffnessTimeLimit1){
		bossChangeState(bs, ENEMY_STATE_WALK);
	}
	att->stiffnessCount++;
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
int boss1Attack2Start(pCharacter bs){
	//攻撃2 : 棍棒薙ぎ払いの開始タイミングで一回だけ呼ばれる
	bossSearch(bs);
	//当たり判定矩形を出す↓
	TitanAtt *att = &bs->att.titan;
	int _x = (int)(bs->pos.x) + bs->hit.relative.x;
	int _y = (int)(bs->pos.y) + bs->hit.relative.y;
	int _w = bs->hit.size.x;
	int _h = bs->hit.size.y;
	pBox at = nullptr;
	if(bs->dir == LEFT){
		_y += _h / 2;
		at = (pBox)getBoxData(HIT_BOSS1_ATTACK2_L);
		att->atk2RectIndexL = setAttacker(_x, _y, at);
	} else{
		_x += _w;
		_y += _h / 2;
		at = (pBox)getBoxData(HIT_BOSS1_ATTACK2_R);
		att->atk2RectIndexR = setAttacker(_x, _y, at);
	}
	//当たり判定矩形を出す↑
	//画像ずれ修正↓
	if(bs->dir == LEFT){
		charaSetDispSize(bs, DISP_BOSS1_NAGIAT_L);
	} else{
		charaSetDispSize(bs, DISP_BOSS1_NAGIAT_R);
	}
	//画像ずれ修正↑
	//音出す↓
	soundMgrPlay(SE_TITAN_ATTACK);
	//音出す↑
	return 0;
}
int boss1Attack2(pCharacter bs){
	//ボス1薙ぎ払い攻撃メイン部分
	TitanAtt *att = &bs->att.titan;
	int atkIndexL = att->atk2RectIndexL;
	int atkIndexR = att->atk2RectIndexR;
	XYd velocity = att->velocity;
	int animFrame = animaGetValueStep(&bs->anim);
	int _start = 7;//薙ぎ払い開始
	int _end = 8;//薙ぎ払い終了
	if(animFrame >= _start && animFrame <= _end){
		pCharacter hitting = nullptr;
		//1pとの衝突判定
		hitting = checkAttackerHitCharacters(atkIndexL, player1, PLAYER1_NUM);
		if(hitting == nullptr){
			hitting = checkAttackerHitCharacters(atkIndexR, player1, PLAYER1_NUM);
		}
		if(hitting != nullptr){
			XYd _velocity = {4,-3};
			if(bs->dir == LEFT){
				_velocity.x = -_velocity.x;
			}
			player1Blow(_velocity.x, _velocity.y);
			player1HitDamage(att->attack);
		}
		//2pとの衝突判定
		hitting = checkAttackerHitCharacters(atkIndexL, player2, PLAYER2_NUM);
		if(hitting == nullptr){
			hitting = checkAttackerHitCharacters(atkIndexR, player2, PLAYER2_NUM);
		}
		if(hitting != nullptr){
			XYd _velocity = {4,-3};
			if(bs->dir == LEFT){
				_velocity.x = -_velocity.x;
			}
			player2Blow(_velocity.x, _velocity.y);
			player2HitDamage(att->attack);
		}
		//タワーとの衝突判定
		hitting = checkAttackerHitCharacters(atkIndexL, castle, CASTLE_NUM);
		if(hitting == nullptr){
			hitting = checkAttackerHitCharacters(atkIndexR, castle, CASTLE_NUM);
		}
		if(hitting != nullptr){
			castleHitDamage(hitting, att->attack);
		}
	} else if(animFrame > _end){
		clearAttacker(atkIndexL);
		clearAttacker(atkIndexR);
	}
	//アニメーションが終わったら状態を切り替える
	if(animaGetCount(&bs->anim) >= 1){
		bossChangeState(bs, ENEMY_STATE_BOSS_ATTACK2_AFTER);
	}
	//位置更新
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
int boss1Attack2After(pCharacter bs){
	TitanAtt *att = &bs->att.titan;
	XYd velocity = att->velocity;
	if(att->stiffnessCount >= att->stiffnessTimeLimit2){
		bossChangeState(bs, ENEMY_STATE_WALK);
	}
	att->stiffnessCount++;
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
int boss1Attack3Start(pCharacter bs){
	//攻撃3 : 突進の開始タイミングで一回だけ呼ばれる
	bossSearch(bs);
	//当たり判定矩形を出す
	TitanAtt *att = &bs->att.titan;
	pBox at = (pBox)getBoxData(HIT_BOSS1_ATTACK3);
	int x = (int)(bs->pos.x);
	int y = (int)(bs->pos.y);
	att->atk3RectIndex = setAttacker(x, y, at);
	//ここまで
	return 0;
}
int boss1Attack3(pCharacter bs){
	//ボス1突進攻撃メイン部分
	TitanAtt *att = &bs->att.titan;
	Anima *anim = &bs->anim;
	int time = bs->anim.time - bs->anim.started;
	int atkIndex = att->atk3RectIndex;
	XYd velocity = att->velocity;
	constexpr int _start = 32;//突進開始時
	const int _duration = (anim->duration) - _start - 16;//突進期間
	int _time = time - _start;//経過時間
	//時間内であれば行う処理
	if(time >= _start && _time <= _duration){
		//移動
		double speed = 12;
		velocity.x = speed;
		if(bs->dir == LEFT){
			velocity.x *= -1;
		}
		int x = (int)(bs->pos.x);
		int y = (int)(bs->pos.y);
		attackerOffset(atkIndex, x, y);
		//当たり判定
		pCharacter hitting = nullptr;
		//1pとの衝突判定
		hitting = checkAttackerHitCharacters(atkIndex, player1, PLAYER1_NUM);
		if(hitting != nullptr){
			XYd _velocity = {4,-3};
			if(bs->dir == LEFT){
				_velocity.x = -_velocity.x;
			}
			player1Blow(_velocity.x, _velocity.y);
			player1HitDamage(att->attack);
		}
		//2pとの衝突判定
		hitting = checkAttackerHitCharacters(atkIndex, player2, PLAYER2_NUM);
		if(hitting != nullptr){
			XYd _velocity = {4,-3};
			if(bs->dir == LEFT){
				_velocity.x = -_velocity.x;
			}
			player2Blow(_velocity.x, _velocity.y);
			player2HitDamage(att->attack);
		}
		//タワーとの衝突判定
		hitting = checkAttackerHitCharacters(atkIndex, castle, CASTLE_NUM);
		if(hitting != nullptr){
			castleHitDamage(hitting, att->attack);
		}
		//壁との衝突判定
		{
			int left = (int)(bs->pos.x) + bs->hit.relative.x;
			int right = left + bs->hit.size.x;
			if(left < 0 || right > WORLD_WIDTH){
				bossChangeState(bs, ENEMY_STATE_WAIT);
			}
		}
		//衝突判定ここまで
	} else if(_time > _duration){
		clearAttacker(atkIndex);
	}
	//時間内であれば行う処理ここまで
	//アニメーションが終わったら状態を切り替える
	if(animaGetCount(anim) >= 1){
		bossChangeState(bs, ENEMY_STATE_BOSS_ATTACK3_AFTER);
	}
	//位置更新
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
int boss1Attack3After(pCharacter bs){
	TitanAtt *att = &bs->att.titan;
	XYd velocity = att->velocity;
	if(att->stiffnessCount >= att->stiffnessTimeLimit3){
		bossChangeState(bs, ENEMY_STATE_WALK);
	}
	att->stiffnessCount++;
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
int bossDamageStop(pCharacter bs){
	TitanAtt *att = &bs->att.titan;
	XYd velocity = att->velocity;
	if(att->stopCount >= att->stopLimit){
		bossChangeState(bs, ENEMY_STATE_DAMAGE);
		return -1;
	}

	bossPosUpdate(bs, rand() % 12 - 6, rand() % 12 - 6);
	bossPosUpdate(bs, velocity.x, velocity.y);
	att->stopCount++;
	return 0;
}
int bossDamage(pCharacter bs){
	TitanAtt *att = &bs->att.titan;
	XYd velocity = att->velocity;

	double kbX = att->knockBackX;
	double kbY = att->knockBackY;

	kbX *= 0.96;
	kbY *= 0.96;

	bossPosUpdate(bs, kbX, kbY);
	att->knockBackX = kbX;
	att->knockBackY = kbY;

	if(animaGetCount(&bs->anim) >= 1){//アニメーションで被ダメ状態解除
		if((att->hp) <= 0){//HPがなければ消える状態へ
			bossChangeState(bs, ENEMY_STATE_DEATH);
		} else{
			bossChangeState(bs, ENEMY_STATE_WAIT);
		}
		return -1;
	}
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
int bossDeath(pCharacter bs){
	TitanAtt *att = &bs->att.titan;
	//非表示にする
	bs->draw.doHide = true;
	//エフェクトを出す
	if(att->deadCounter == 0){
		double x = bs->pos.x + bs->draw.relative.x;
		double y = bs->pos.y + bs->draw.relative.y;
		enemyDeadEffectSet(BOSS_TITAN, x, y, bs->dir);
	}
	//時間が来たら消える
	if(att->deadCounter > BOSS_DEATH_DELAY){
		bs->isActive = false;
	}
	att->deadCounter++;
	return 0;
}
int bossPosUpdate(pCharacter bs, double vx, double vy){
	int result = 0;
	TitanAtt *att = &bs->att.titan;
	double x, y;
	double gravity = att->gravity;
	getCharaOffSet(bs, &x, &y);

	vy += gravity;
	vx *= 0.7;
	vy *= 0.96;

	x += vx;
	y += vy;

	//世界の果てに行かせない↓
	if(x < 0 - bs->hit.relative.x){
		x = 0 - bs->hit.relative.x;
	}
	if(x >(WORLD_WIDTH - bs->hit.relative.x - bs->hit.size.x)){
		x = WORLD_WIDTH - bs->hit.relative.x - bs->hit.size.x;
	}
	//世界の果てに行かせない↑

	charaOffSet(bs, x, y);

	int hitInfo = charaBlockingByBlock((void*)bs);
	if(charaLanding(hitInfo)){
		//着地
		vy = 0;
		result = -1;
	}


	att->velocity.x = vx;
	att->velocity.y = vy;
	return result;
}

