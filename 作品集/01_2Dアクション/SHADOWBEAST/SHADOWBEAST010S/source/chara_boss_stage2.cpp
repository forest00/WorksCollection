
#include <Windows.h>
#include "amgame.h"

#include "game_state.h"
#include "chara_enemy.h"

#include "image_manager.h"
#include "sound_manager.h"
#include "chara_player1.h"
#include "chara_player2.h"
#include "chara_castle.h"
#include "chara_block.h"
#include "chara_attacker.h"
#include "chara__hit_rect.h"
#include "chara_boss_stage2_thunder.h"
#include "chara_boss_stage2_water.h"
#include "chara_enemy_death.h"
#include "in_game_effect.h"
#include "chara__manager.h"
#include "stage_data_reader.h"
#include "ui_mgr.h"

#include "common.h"
#include "camera.h"

#include "forDebug.h"

#define BOSS2_ATTACK_KIND_MAX 5

/*
ボス1
*/



extern int boss2Set(double x, double y);
extern int boss2Blow(pCharacter boss, double vx, double vy);
extern int boss2HitDamage(pCharacter boss, int damage);

static int bossUpdate(pCharacter bs);

static EnemyState decideAttackMethod(pCharacter bs);
static int bossChangeState(pCharacter bs, EnemyState nextState);
static int bossStateEnter(pCharacter bs);
static int bossStateExit(pCharacter bs);
static int bossInit(pCharacter bs);
static int bossSection(pCharacter bs);
static int bossWait(pCharacter bs);
static int bossWalk(pCharacter bs);
static int bossSearch(pCharacter bs);
static int boss2Attack1Before(pCharacter bs);
static int boss2Attack1Start(pCharacter bs);
static int boss2Attack1Main(pCharacter bs);
static int boss2Attack1After(pCharacter bs);
static int boss2Attack2Before(pCharacter bs);
static int boss2Attack2Start(pCharacter bs);
static int boss2Attack2Main(pCharacter bs);
static int boss2Attack2After(pCharacter bs);
static int boss2Attack3Before(pCharacter bs);
static int boss2Attack3Start(pCharacter bs);
static int boss2Attack3Main(pCharacter bs);
static int boss2Attack3After(pCharacter bs);
static int bossDamageStop(pCharacter bs);
static int bossDamage(pCharacter bs);
static int bossDeath(pCharacter bs);
static int bossShouldClear(pCharacter bs);
static int bossPosUpdate(pCharacter bs, double vx, double vy);
//ENEMY_STATE_SHOULD_CLEAR

int boss2Set(double x, double y){
	int found = charaFindNonActive(enemies, ENEMY_NUM);
	if(found < 0){
		return -1;
	}
	pCharacter bs = &enemies[found];
	SeaSnakeAtt *att = &bs->att.seaSnk;
	ImageFileId fileId = IMAGE_BOSS2_MOTION_WAIT;
	const int hbs = imageMgrGetHandle(fileId, 0);
	charaInit(bs);
	bs->isActive = true;
	charaSetDispSize(bs, DISP_BOSS2_NORMAL);
	pBox bsBox = (pBox)getBoxData(HIT_BOSS2_NORMAL_L);
	bs->hit = *bsBox;
	*att = seaSnakeAttBase;
	charaOffSet(bs,
		x - (bs->hit.relative.x + bs->hit.size.x / 2),
		y - (bs->hit.relative.y + bs->hit.size.y / 2));
	charaColSet(bs, HIT_BOSS2_NORMAL_L);
	charaSetImageHandle(bs, hbs);
	charaHome(bs, bs->hit.size.x / 2, bs->hit.size.y / 2);
	charaFuncSet(bs, bossUpdate);
	bossChangeState(bs, ENEMY_STATE_INIT);
	bossUpdate(bs);
	return 0;
}
int boss2Blow(pCharacter bs, double vx, double vy){
	SeaSnakeAtt *att = &bs->att.seaSnk;
	//死亡判定
	bool stateDead = false;
	stateDead |= (att->state == ENEMY_STATE_DAMAGE_STOP);
	stateDead |= (att->state == ENEMY_STATE_DAMAGE);
	if(stateDead && (att->hp) <= 0){
		return -1;
	}
	att->damageStopCounter = 8;
	att->knockBackX = vx;
	att->knockBackY = vy;
	double _d = hypot(vx, vy);
	if(_d > 1000){
		bossChangeState(bs, ENEMY_STATE_DAMAGE_STOP);
	}
	return 0;
}
int boss2HitDamage(pCharacter bs, int damage){
	SeaSnakeAtt *att = &bs->att.seaSnk;
	int _hp = att->hp;
	att->hp -= damage;
	if(_hp > 0 && att->hp <= 0){
		//死亡した
		att->hp = 0;
		bossChangeState(bs, ENEMY_STATE_DEATH);
		return -1;
	}
	return 0;
}

//ウミヘビ更新
int bossUpdate(pCharacter bs){
	SeaSnakeAtt *att = &bs->att.seaSnk;
	bs->anim.time = getGameFrame();

	bossSection(bs);

	int index = animaGetValueStep(&bs->anim);
	int handle = imageMgrGetHandle(bs->imageFileId, index);
	charaSetImageHandle(bs, handle);
	charaFlipX(bs, bs->dir == LEFT);
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
	uiMgrTeachBossHp(att->hp, seaSnakeAttBase.hp);

	return 0;
}
//ウミヘビ攻撃方法決定
EnemyState decideAttackMethod(pCharacter bs){
	SeaSnakeAtt *att = &bs->att.seaSnk;
	int ctrl = att->atkControlller;
	att->atkControlller++;
	att->atkControlller %= BOSS2_ATTACK_KIND_MAX;
	switch(ctrl){
	case 0:
		return ENEMY_STATE_BOSS_ATTACK1_BEFORE;
		break;
	case 1:
		return ENEMY_STATE_BOSS_ATTACK2_BEFORE;
		break;
	case 2:
		return ENEMY_STATE_BOSS_ATTACK2_BEFORE;
		break;
	case 3:
		return ENEMY_STATE_BOSS_ATTACK2_BEFORE;
		break;
	case 4:
		return ENEMY_STATE_BOSS_ATTACK3_BEFORE;
		break;
	default:
		return ENEMY_STATE_BOSS_ATTACK1_BEFORE;
		break;
	}
}
int bossChangeState(pCharacter bs, EnemyState nextState){
	SeaSnakeAtt *att = &bs->att.seaSnk;
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
	SeaSnakeAtt *att = &bs->att.seaSnk;
	charaSetDispSize(bs, DISP_BOSS2_NORMAL);
	bs->imageFileId = IMAGE_BOSS2_MOTION_WAIT;
	if(bs->dir == LEFT){
		charaColSet(bs, HIT_BOSS2_NORMAL_L);
	} else{
		charaColSet(bs, HIT_BOSS2_NORMAL_R);
	}
	switch(att->state){
	case ENEMY_STATE_INIT:
		break;
	case ENEMY_STATE_WAIT:
		bs->imageFileId = IMAGE_BOSS2_MOTION_WAIT;
		break;
	case ENEMY_STATE_WALK:
		bs->imageFileId = IMAGE_BOSS2_MOTION_WALK;
		if(bs->dir == LEFT){
			charaSetDispSize(bs, DISP_BOSS2_WALK_L);
			charaColSet(bs, HIT_BOSS2_WALK_L);
		} else{
			charaSetDispSize(bs, DISP_BOSS2_WALK_R);
			charaColSet(bs, HIT_BOSS2_WALK_R);
		}
		break;
	case ENEMY_STATE_SEARCH:
		break;
	case ENEMY_STATE_BOSS_ATTACK1_BEFORE:
		att->stiffnessCounter = 0;
		break;
	case ENEMY_STATE_BOSS_ATTACK1_MAIN:
		boss2Attack1Start(bs);
		bs->imageFileId = IMAGE_BOSS2_MOTION_BITE;
		if(bs->dir == LEFT){
			charaSetDispSize(bs, DISP_BOSS2_BITE_L);
		} else{
			charaSetDispSize(bs, DISP_BOSS2_BITE_R);
		}
		break;
	case ENEMY_STATE_BOSS_ATTACK1_AFTER:
		bs->imageFileId = IMAGE_BOSS2_MOTION_WAIT;
		att->stiffnessCounter = 0;
		break;
	case ENEMY_STATE_BOSS_ATTACK2_BEFORE:
		att->stiffnessCounter = 0;
		break;
	case ENEMY_STATE_BOSS_ATTACK2_MAIN:
		if(bs->dir == LEFT){
			charaSetDispSize(bs, DISP_BOSS2_THUNDER_L);
		} else{
			charaSetDispSize(bs, DISP_BOSS2_THUNDER_R);
		}
		bs->imageFileId = IMAGE_BOSS2_MOTION_THUNDER;
		boss2Attack2Start(bs);
		break;
	case ENEMY_STATE_BOSS_ATTACK2_AFTER:
		bs->imageFileId = IMAGE_BOSS2_MOTION_WAIT;
		att->stiffnessCounter = 0;
		break;
	case ENEMY_STATE_BOSS_ATTACK3_BEFORE:
		att->stiffnessCounter = 0;
		break;
	case ENEMY_STATE_BOSS_ATTACK3_MAIN:
		boss2Attack3Start(bs);
		bs->imageFileId = IMAGE_BOSS2_MOTION_WATER;
		if(bs->dir == LEFT){
			charaSetDispSize(bs, DISP_BOSS2_WATER_L);
		} else{
			charaSetDispSize(bs, DISP_BOSS2_WATER_R);
		}
		break;
	case ENEMY_STATE_BOSS_ATTACK3_AFTER:
		bs->imageFileId = IMAGE_BOSS2_MOTION_WAIT;
		att->stiffnessCounter = 0;
		break;
	case ENEMY_STATE_DAMAGE_STOP:
		bs->imageFileId = IMAGE_BOSS2_MOTION_DAMAGE;
		att->stopCoord.x = bs->pos.x;
		att->stopCoord.y = bs->pos.y;
		att->stiffnessCounter = 0;
		if(bs->dir == LEFT){
			charaSetDispSize(bs, DISP_BOSS2_DAMAGE_L);
		} else{
			charaSetDispSize(bs, DISP_BOSS2_DAMAGE_R);
		}
		break;
	case ENEMY_STATE_DAMAGE:
		bs->imageFileId = IMAGE_BOSS2_MOTION_DAMAGE;
		charaOffSet(bs, att->stopCoord.x, att->stopCoord.y);
		charaOffSet(bs, att->stopCoord.x, att->stopCoord.y);
		break;
	case ENEMY_STATE_DEATH:
		bs->imageFileId = IMAGE_BOSS2_MOTION_DEATH;
		if(bs->dir == LEFT){
			charaSetDispSize(bs, DISP_BOSS2_DEATH_L);
		} else{
			charaSetDispSize(bs, DISP_BOSS2_DEATH_R);
		}
		break;
	case ENEMY_STATE_SHOULD_CLEAR:
		if(bs->dir == LEFT){
			charaSetDispSize(bs, DISP_BOSS2_DEATH_L);
		} else{
			charaSetDispSize(bs, DISP_BOSS2_DEATH_R);
			bs->draw.relative.x = 440;
		}
		//消えるべき
		bs->draw.doHide = true;
		att->isDead = true;
		att->deadCounter = 0;
		break;
	default:
		break;
	}
	AnimaData creature = {0};
	createAnimaData(&creature, bs->imageFileId);
	switch(att->state){
	case ENEMY_STATE_BOSS_ATTACK1_MAIN:
		creature.duration = att->biteAnimDuration;
		break;
	case ENEMY_STATE_BOSS_ATTACK2_MAIN:
		creature.duration = att->thunderAnimDuration;
		break;
	case ENEMY_STATE_BOSS_ATTACK3_MAIN:
		creature.duration = att->waterAnimDuration;
		break;
	case ENEMY_STATE_DAMAGE_STOP:
		creature.duration = att->damageStopTime;
		break;
	default:
		break;
	}
	animaSetup(&bs->anim,
		getGameFrame(),
		&creature);
	return 0;
}
int bossStateExit(pCharacter bs){
	SeaSnakeAtt *att = &bs->att.seaSnk;
	switch(att->state){
	case ENEMY_STATE_BOSS_ATTACK1_MAIN:
		clearAttacker(att->biteAtkIndex);
		att->biteAtkIndex = -1;
		break;
	default:
		break;
	}
	return 0;
}
int bossSection(pCharacter bs){
	SeaSnakeAtt *att = &bs->att.seaSnk;
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
		boss2Attack1Before(bs);
		break;
	case ENEMY_STATE_BOSS_ATTACK1_MAIN:
		boss2Attack1Main(bs);
		break;
	case ENEMY_STATE_BOSS_ATTACK1_AFTER:
		boss2Attack1After(bs);
		break;
	case ENEMY_STATE_BOSS_ATTACK2_BEFORE:
		boss2Attack2Before(bs);
		break;
	case ENEMY_STATE_BOSS_ATTACK2_MAIN:
		boss2Attack2Main(bs);
		break;
	case ENEMY_STATE_BOSS_ATTACK2_AFTER:
		boss2Attack2After(bs);
		break;
	case ENEMY_STATE_BOSS_ATTACK3_BEFORE:
		boss2Attack3Before(bs);
		break;
	case ENEMY_STATE_BOSS_ATTACK3_MAIN:
		boss2Attack3Main(bs);
		break;
	case ENEMY_STATE_BOSS_ATTACK3_AFTER:
		boss2Attack3After(bs);
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
	case ENEMY_STATE_SHOULD_CLEAR:
		bossShouldClear(bs);
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
	SeaSnakeAtt *att = &bs->att.seaSnk;
	XYd velocity = att->velocity;
	//アニメーションで次の状態へ↓
	if(animaGetCount(&bs->anim) >= 1){
		bossChangeState(bs, ENEMY_STATE_SEARCH);
		return -1;
	}
	//アニメーションで次の状態へ↑
	//プレイヤーを感知したら攻撃前硬直へ↓(サーチ範囲はデータに依存)
	bool hitFlag = false;
	{
		CharaHitAreaId hitId = HIT_NONE;
		if(bs->dir == LEFT){
			hitId = HIT_BOSS2_ATTACK_WALK_SEARCH_L;
		} else{
			hitId = HIT_BOSS2_ATTACK_WALK_SEARCH_R;
		}
		if(isPlayer1Dead() == false){
			hitFlag |= (charaBulkHit(player1, PLAYER1_NUM, bs, hitId) >= 0);
		}
		if(isPlayer2Dead() == false){
			hitFlag |= (charaBulkHit(player2, PLAYER2_NUM, bs, hitId) >= 0);
		}
		if(isPlayer1Dead() || isPlayer2Dead()){
			hitFlag |= (charaBulkHit(castle, CASTLE_NUM, bs, hitId) >= 0);
		}
	}
	if(hitFlag){
		EnemyState nextState = ENEMY_STATE_NONE;
		nextState = decideAttackMethod(bs);
		bossChangeState(bs, nextState);
		return -1;
	}
	//プレイヤーを感知したら攻撃前硬直へ↑
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
int bossWalk(pCharacter bs){
	SeaSnakeAtt *att = &bs->att.seaSnk;
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
	//アニメーションでウェイト状態へ↓
	if(animaGetCount(&bs->anim) >= 1){
		bossChangeState(bs, ENEMY_STATE_WAIT);
	}
	//アニメーションでウェイト状態へ↑

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
//噛みつき前硬直
int boss2Attack1Before(pCharacter bs){
	SeaSnakeAtt *att = &bs->att.seaSnk;
	XYd velocity = att->velocity;
	if(att->stiffnessCounter >= att->bitePreStiffnessTime){
		//攻撃する
		EnemyState nextState;
		nextState = ENEMY_STATE_BOSS_ATTACK1_MAIN;
		bossChangeState(bs, nextState);
	}
	att->stiffnessCounter++;
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
//噛みつき前硬直終わり　攻撃直前　一回のみ
int boss2Attack1Start(pCharacter bs){
	//攻撃1 : 噛みつきの開始タイミングで一回だけ呼ばれる
	//方向決定↓
	//bossSearch(bs);
	//方向決定↑
	////当たり判定直し↓
	//if (bs->dir == LEFT) {
	//	charaSetDispSize(bs, DISP_BOSS2_BITE_L);
	//} else {
	//	charaSetDispSize(bs, DISP_BOSS2_BITE_R);
	//}
	////当たり判定直し↑
	SeaSnakeAtt *att = &bs->att.seaSnk;
	pBox at = nullptr;
	//当たり判定矩形を準備&出す↓
	if(bs->dir == LEFT){
		at = (pBox)getBoxData(HIT_BOSS2_BITE_L);
	} else{
		at = (pBox)getBoxData(HIT_BOSS2_BITE_R);
	}
	int _x = (int)(bs->pos.x) + bs->hit.relative.x;
	int _y = (int)(bs->pos.y) + bs->hit.relative.y;
	int _w = bs->hit.size.x;
	int _h = bs->hit.size.y;
	_x += _w / 2;
	_y += _h / 2;
	att->biteAtkIndex = setAttacker(_x, _y, at);
	//当たり判定矩形を準備&出す↑
	//音を出す準備↓
	att->alreadyPlaySound = false;
	//音を出す準備↑
	return 0;
}
//噛みつき攻撃メイン
int boss2Attack1Main(pCharacter bs){
	//ボス2噛みつき攻撃メイン部分
	SeaSnakeAtt *att = &bs->att.seaSnk;
	Anima *anim = &bs->anim;
	XYd velocity = att->velocity;
	int atkIndex = att->biteAtkIndex;
	int animFrame = animaGetFrame(anim);					//現在コマ
	const int _attackStart = att->biteAtkStartFrame;		//攻撃開始コマ
	const int _attackEnd = att->biteAtkEndFrame;			//攻撃終了コマ
	if(animFrame >= _attackStart && animFrame <= _attackEnd){
		//音を出すための操作↓
		if(!att->alreadyPlaySound){
			att->alreadyPlaySound = true;
			soundMgrPlay(SE_TITAN_ATTACK);
		}
		//音を出すための操作↑
		pCharacter hitting = nullptr;
		//1pとの衝突判定
		hitting = checkAttackerHitCharacters(atkIndex, player1, PLAYER1_NUM);
		if(hitting != nullptr){
			XYd _velocity = {4,-3};
			if(bs->dir == LEFT){
				_velocity.x = -_velocity.x;
			}
			player1Blow(_velocity.x, _velocity.y);
			player1HitDamage(att->attackBite);
		}
		//2pとの衝突判定
		hitting = checkAttackerHitCharacters(atkIndex, player2, PLAYER2_NUM);
		if(hitting != nullptr){
			XYd _velocity = {4,-3};
			if(bs->dir == LEFT){
				_velocity.x = -_velocity.x;
			}
			player2Blow(_velocity.x, _velocity.y);
			player2HitDamage(att->attackBite);
		}
		//タワーとの衝突判定
		hitting = checkAttackerHitCharacters(atkIndex, castle, CASTLE_NUM);
		if(hitting != nullptr){
			castleHitDamage(hitting, att->attackBite);
		}
		//衝突判定ここまで
	} else if(animFrame > _attackEnd){
		clearAttacker(atkIndex);
	}
	//アニメーションが終わったら状態を切り替える
	if(animaGetCount(anim) >= 1){
		bossChangeState(bs, ENEMY_STATE_BOSS_ATTACK1_AFTER);
	}
	//位置更新
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
//噛みつき攻撃後硬直
int boss2Attack1After(pCharacter bs){
	SeaSnakeAtt *att = &bs->att.seaSnk;
	XYd velocity = att->velocity;
	if(att->stiffnessCounter >= att->bitePostStiffnessTime){
		//待機状態になる
		EnemyState nextState;
		nextState = ENEMY_STATE_WAIT;
		bossChangeState(bs, nextState);
	}
	att->stiffnessCounter++;
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
//雷攻撃前硬直
int boss2Attack2Before(pCharacter bs){
	SeaSnakeAtt *att = &bs->att.seaSnk;
	XYd velocity = att->velocity;
	if(att->stiffnessCounter >= att->thunderPreStiffnessTime){
		//攻撃する
		EnemyState nextState = ENEMY_STATE_NONE;
		nextState = ENEMY_STATE_BOSS_ATTACK2_MAIN;
		bossChangeState(bs, nextState);
	}

	att->stiffnessCounter++;
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
//雷攻撃前硬直終わり　攻撃直前　一回のみ
int boss2Attack2Start(pCharacter bs){
	//攻撃2 : 雷攻撃開始タイミングで一回だけ呼ばれる
	SeaSnakeAtt *att = &bs->att.seaSnk;
	//雷を落とす位置の決定↓
	att->alreadyThunder = false;
	int bsX = (int)(bs->pos.x) + bs->hit.relative.x;//左端
	int w = 512;
	int u = 0;
	int s = 0;
	if(bs->dir == RIGHT){
		bsX += bs->hit.size.x;//右端
		att->thunderOffsetX[0] = getPlayer1X() + (int)(rand0to1() * 80) + 40;
		att->thunderOffsetX[1] = getPlayer2X() - (int)(rand0to1() * 80) + 40;
		att->thunderOffsetX[2] = bsX + (int)(rand0to1() * w) - u;
		s = (rand() % 2) * 2 - 1;
		u += (int)((rand0to1() + 1) * 320 * s);
		w *= 2;
	} else{
		att->thunderOffsetX[0] = getPlayer2X() + (int)(rand0to1() * 80) + 40;
		att->thunderOffsetX[1] = getPlayer1X() - (int)(rand0to1() * 80) + 40;
		att->thunderOffsetX[2] = bsX - (int)(rand0to1() * w) - u;
		s = (rand() % 2) * 2 - 1;
		u += (int)((rand0to1() + 1) * 320 * s);
		w *= 2;
	}
	//雷を落とす位置の決定↑
	//音出すための操作↓
	bs->att.seaSnk.alreadyPlaySound = false;
	//音出すための操作↑
	bs->att.seaSnk.animStopCounter = 0;
	return 0;
}
//雷攻撃メイン
int boss2Attack2Main(pCharacter bs){
	//ボス2雷メイン部分
	SeaSnakeAtt *att = &bs->att.seaSnk;
	Anima *anim = &bs->anim;
	XYd velocity = att->velocity;
	int animFrame = animaGetFrame(anim);					//現在コマ
	const int _attackStart = att->thunderAtkStartFrame;		//攻撃開始コマ
	const int _attackEnd = att->thunderAtkEndFrame;			//攻撃終了コマ

	//雷を出す↓
	//まだ出してないか
	if(att->alreadyThunder == false){
		//出すべきタイミングか
		if(animFrame >= _attackStart && animFrame <= _attackEnd){
			att->alreadyThunder = true;
			//出す
			int attack = att->attackThunder;
			int delay = 45;
			for each(int &x in att->thunderOffsetX){
				setBossThunder(x, GROUND_Y, attack, delay);
			}
		}
	}
	//雷を出す↑
	//アニメーションが終わったら状態を切り替える
	if(animaGetCount(anim) >= 1){
		bossChangeState(bs, ENEMY_STATE_BOSS_ATTACK2_AFTER);
	}
	//位置更新
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
//雷攻撃終わり硬直
int boss2Attack2After(pCharacter bs){
	SeaSnakeAtt *att = &bs->att.seaSnk;
	XYd velocity = att->velocity;
	if(att->stiffnessCounter >= att->thunderPostStiffnessTime){
		if(att->atkControlller < 4){
			//
			att->atkControlller++;
			bossChangeState(bs, ENEMY_STATE_BOSS_ATTACK2_BEFORE);
		} else{
			//待機状態になる
			bossChangeState(bs, ENEMY_STATE_WAIT);
		}
	}
	att->stiffnessCounter++;
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
//水鉄砲攻撃前硬直
int boss2Attack3Before(pCharacter bs){
	SeaSnakeAtt *att = &bs->att.seaSnk;
	XYd velocity = att->velocity;
	if(att->stiffnessCounter >= att->waterPreStiffnessTime){
		//攻撃する
		EnemyState nextState = ENEMY_STATE_NONE;
		nextState = ENEMY_STATE_BOSS_ATTACK3_MAIN;
		bossChangeState(bs, nextState);
	}

	att->stiffnessCounter++;
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
//水鉄砲攻撃前硬直終わり　一回だけ
int boss2Attack3Start(pCharacter bs){
	////方向決定↓
	//bossSearch(bs);
	////方向決定↑
	////当たり判定直し↓
	//if (bs->dir == LEFT) {
	//	charaSetDispSize(bs, DISP_BOSS2_WATER_L);
	//} else {
	//	charaSetDispSize(bs, DISP_BOSS2_WATER_R);
	//}
	////当たり判定直し↑
	//音出すための操作↓
	bs->att.seaSnk.alreadyPlaySound = false;
	//音出すための操作↑
	return 0;
}
//水鉄砲攻撃メイン
int boss2Attack3Main(pCharacter bs){
	//ボス2雷メイン部分
	SeaSnakeAtt *att = &bs->att.seaSnk;
	Anima *anim = &bs->anim;
	XYd velocity = att->velocity;
	int animFrame = animaGetFrame(anim);					//現在コマ
	const int _attackStart = att->waterAtkStartFrame;		//攻撃開始コマ
	const int _attackEnd = att->waterAtkEndFrame;			//攻撃終了コマ
	if(animFrame >= _attackStart && animFrame <= _attackEnd){
		//todo:水を出す
	}
	//アニメーションが終わったら状態を切り替える
	if(animaGetCount(anim) >= 1){
		bossChangeState(bs, ENEMY_STATE_BOSS_ATTACK3_AFTER);
	}
	//位置更新
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
//水鉄砲攻撃後硬直
int boss2Attack3After(pCharacter bs){
	SeaSnakeAtt *att = &bs->att.seaSnk;
	XYd velocity = att->velocity;
	if(att->stiffnessCounter >= att->waterPostStiffnessTime){
		//待機状態になる
		EnemyState nextState = ENEMY_STATE_NONE;
		nextState = ENEMY_STATE_WAIT;
		bossChangeState(bs, nextState);
	}
	att->stiffnessCounter++;
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
//被ダメ後硬直
int bossDamageStop(pCharacter bs){
	SeaSnakeAtt *att = &bs->att.seaSnk;
	XYd velocity = att->velocity;
	if(att->stiffnessCounter >= att->damageStopTime){
		bossChangeState(bs, ENEMY_STATE_DAMAGE);
		return -1;
	}
	bossPosUpdate(bs, rand() % 12 - 6, rand() % 12 - 6);
	bossPosUpdate(bs, velocity.x, velocity.y);
	att->stiffnessCounter++;
	return 0;
}
//被ダメ時更新処理
int bossDamage(pCharacter bs){
	SeaSnakeAtt *att = &bs->att.seaSnk;
	XYd velocity = att->velocity;

	double kbX = att->knockBackX;
	double kbY = att->knockBackY;

	kbX *= 0.96;
	kbY *= 0.96;

	bossPosUpdate(bs, kbX, kbY);
	att->knockBackX = kbX;
	att->knockBackY = kbY;

	//アニメーションで被ダメ状態解除↓
	if(animaGetCount(&bs->anim) >= 1){
		if((att->hp) <= 0){//HPがなければ消える状態へ
			bossChangeState(bs, ENEMY_STATE_DEATH);
		} else{
			bossChangeState(bs, ENEMY_STATE_WAIT);
		}
		return -1;
	}
	//アニメーションで被ダメ状態解除↑
	bossPosUpdate(bs, velocity.x, velocity.y);
	return 0;
}
int bossDeath(pCharacter bs){
	//アニメーションが終わったら状態チェンジ
	if(animaGetCount(&bs->anim) >= 1){
		bossChangeState(bs, ENEMY_STATE_SHOULD_CLEAR);
	}
	return 0;
}
//死亡アニメーションを終えた、消えるべき
int bossShouldClear(pCharacter bs){
	SeaSnakeAtt *att = &bs->att.seaSnk;
	//エフェクトを出す
	if(att->deadCounter == 0){
		double x = bs->pos.x + bs->draw.relative.x;
		double y = bs->pos.y + bs->draw.relative.y;
		enemyDeadEffectSet(BOSS_SEASNAKE, x, y, bs->dir);
	}
	//時間が来たら消える
	if(att->deadCounter > BOSS_DEATH_DELAY){
		bs->isActive = false;
	}
	att->deadCounter++;
	bs->isActive = false;
	att->deadCounter++;
	return 0;
}
int bossPosUpdate(pCharacter bs, double vx, double vy){
	int result = 0;
	SeaSnakeAtt *att = &bs->att.seaSnk;
	double x, y;
	double gravity = att->garavity;
	getCharaOffSet(bs, &x, &y);

	vy += gravity;
	vx *= 0.7;
	vy *= 0.96;

	x += vx;
	y += vy;

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

