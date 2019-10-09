

#include <stdio.h>
#include <Windows.h>
#include "amgame.h"
#include "game_state.h"
#include "common.h"
#include "chara__common.h"
#include "chara_attacker.h"
#include "input_manager.h"
#include "image_manager.h"
#include "sound_manager.h"
#include "anima.h"
#include "chara__hit_rect.h"
#include "chara_enemy.h"
#include "chara_block.h"
#include "chara__manager.h"
#include "chara_player2_bullet.h"
#include "chara_player2_sub.h"
#include "chara_player2_recovered_bullet.h"
#include "player_1_2_bridge.h"
#include "stage_data_reader.h"
#include "ui_mgr.h"
#include "camera.h"
#include "new_effect_hp_heal.h"

#include <cassert>
#include "forDebug.h"


//ファイル読み込み用
#define FILE_NAME_MAX	FILENAME_MAX
#define FILE_LINE_MAX	99
#define FILE_NAME_PLAYER_DATA	"_data\\2p_data.txt"

//ゲーム用
#define INVINCIBLE_TIME		60		//無敵時間
#define M_DELAY				2		//サブウェポン遅延時間
#define M_SPEED				18		//サブウェポン移動速度





static constexpr int BULLET_MAX = 64;
static constexpr int BULLET_SPEED = 32;
//////////////////////


//プレイヤーのデータ(初期化時にファイルから読み込む)
Player2Att player2AttBase = {0};


typedef enum{
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_JUMP,
	KEY_RESURRECTION,
	KEY_MAIN_ATK,
	KEY_SUB_ATK,
	KEY_GUARD,
	KEY_AVOID_L,
	KEY_AVOID_R,
}Keys;

static int keyConfig[] = {
	PAD_UP,
	PAD_DOWN,
	PAD_LEFT,
	PAD_RIGHT,
	PAD_UP,			//jump
	PAD_X,			//resurrection
	PAD_B,			//mainatk
	PAD_Y,			//subatk
	PAD_A,			//guard
	PAD_L,			//avoid_left
	PAD_R,			//avoid_right
};

typedef enum{
	INIT,
	IDLE,
	WALK,
	JUMP,
	MAIN_ATTACK,
	MAIN_ATTACK_AFTER,
	SUB_ATTACK,
	GUARD,
	AVOID,
	DAMAGE_STOP,
	DAMAGE,
	KNOCK_OUT,
	DEAD,
	RESURRECTION,
	SHOOT_RECOVERED_BULLETS,
}Player2State;



extern int loadPlayer2DataFromFile();
extern int setPlayer2(int x, int y);
extern int player2Blow(double vx, double vy);
extern int player2HitDamage(int damage);
extern int player2HealHp(int recoveryAmount);
extern int getPlayer2X();
extern int player2Revive();


static int player2Update(pCharacter player);

static int player2Section(pCharacter player);
static int setAttackerMainAtk(pCharacter player);
static int setAttackerSubAtk(pCharacter player);
static int setRecoveredBullets(pCharacter player);
static int player2ChangeState(pCharacter player, Player2State nextState);
static int player2StateEnter(pCharacter player);
static int player2StateExit(pCharacter player);
static int player2Init(pCharacter player);
static int player2Idle(pCharacter player);
static int player2Move(pCharacter player);
static int player2Jump(pCharacter player);
static int player2MainAttack(pCharacter player);
static int player2MainAttackAfter(pCharacter player);
static int player2SubAttack(pCharacter player);
static int player2Guard(pCharacter player);
static int player2Avoid(pCharacter player);
static int player2DamageStop(pCharacter player);
static int player2Damage(pCharacter player);
static int player2KnockOut(pCharacter player);
static int player2Dead(pCharacter player);
static int player2Resurrection(pCharacter player);
static int player2ShootRecoveredBullets(pCharacter player);
static int player2PosUpdate(pCharacter player, double vx, double vy);
static int setAvoidPos(pCharacter player);
static bool isPlayer2StateDead(pCharacter player);
static bool isKey(Keys keyType);
//static bool isKeyUp(Keys keyType);//今は使わない
static bool isKeyDown(Keys keyType);



int loadPlayer2DataFromFile(){

	FILE *in = nullptr;
	char fileName[FILE_NAME_MAX] = "\0";
	char comment[FILE_LINE_MAX] = "\0";
	int opened = -1;
	strncpy_s(fileName, FILE_NAME_PLAYER_DATA, FILE_NAME_MAX);
	opened = fopen_s(&in, fileName, "r");
	if(opened){
		assert(!opened && "2P用のファイルが開けません");
		exit(0);
	} else{
		fscanf_s(in, "%lf", &player2AttBase.gravity);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%lf", &player2AttBase.walkSpeedBase);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &player2AttBase.maxHp);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%lf", &player2AttBase.jumpPowBase);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &player2AttBase.attackBase);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%lf", &player2AttBase.avoidPowBase);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &player2AttBase.stopLimit);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &player2AttBase.stiffnessTimeLimit);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &player2AttBase.subAtkCoolTime);
		fgets(comment, FILE_LINE_MAX, in);
		{
			int mode = 0;
			fscanf_s(in, "%d", &mode);
			switch(mode){
			case 0:
				keyInputModeDetermine2p(AMINPUT_KEYBOARD);
				break;
			case 1:
				keyInputModeDetermine2p(AMINPUT_PAD1);
				break;
			case 2:
				keyInputModeDetermine2p(AMINPUT_PAD2);
				break;
			default:
				break;
			}
		}
		fclose(in);
	}
	return 0;
}
int setPlayer2(int x, int y){
	pCharacter pl = player2;
	charaInit(pl);
	pl->isActive = true;
	charaColSet(pl, HIT_PLAYER2_G);
	charaOffSet(pl, x, y);
	pl->att.pl2 = player2AttBase;
	pl->att.pl2.hp = pl->att.pl2.maxHp;
	charaFuncSet(pl, player2Update);
	player2ChangeState(pl, INIT);
	pl->imageFileId = IMAGE_2P_IDLE;
	return 0;
}
int player2Blow(double vx, double vy){
	pCharacter pl2 = player2;
	Player2Att *att = &pl2->att.pl2;
	if(att->state == AVOID){//回避中チェック
		return -1;
	}
	if(isPlayer2StateDead(pl2)){//死亡チェック
		return -1;
	}
	if(att->invincibleFlag && att->invincibleCount){//無敵チェック
		return -1;
	}
	att->knockBackX = vx;
	att->knockBackY = vy;
	player2ChangeState(pl2, DAMAGE_STOP);
	return 0;
}
int player2HitDamage(int damage){
	pCharacter pl2 = player2;
	Player2Att *att = &pl2->att.pl2;
	if(damage > 0){
		if(att->state == AVOID){//回避中チェック
			return -1;
		}
		if(isPlayer2StateDead(pl2)){//死亡チェック
			return -1;
		}
		if(att->invincibleFlag){//無敵チェック
			return -1;
		}
	}
	att->hp -= damage;
	att->hp = (int)Clamp((double)att->hp, 0.0, (double)att->maxHp);
	//無敵になる
	att->invincibleFlag = true;
	att->invincibleCount = 0;
	return 0;
}
int player2HealHp(int recoveryAmount)
{
	pCharacter pl2 = player2;
	Player2Att *att = &pl2->att.pl2;
	att->hp += recoveryAmount;
	att->hp = (int)Clamp((double)att->hp, 0.0, (double)att->maxHp);
	double effectX = pl2->pos.x + pl2->hit.relative.x + pl2->hit.size.x / 2;
	double effectY = pl2->pos.y + pl2->hit.relative.y + pl2->hit.size.y;
	hpHealEffectSet(effectX, effectY);
	return 0;
}
int getPlayer2X(){
	pCharacter pl2 = player2;
	return (int)(pl2->pos.x) + pl2->hit.size.x / 2;
}
int getPlayer2FactMove(double *moveX, double *moveY){
	pCharacter pl2 = player2;
	*moveX = pl2->pos.x - pl2->beforePos.x;
	*moveY = pl2->pos.y - pl2->beforePos.y;
	return 0;
}
//復活した瞬間だけtrueを返す
bool isPlayer2Resurrection(){
	pCharacter pl2 = player2;
	Player2Att *att = &pl2->att.pl2;
	return att->afterResurrection >= 0 && att->afterResurrection <= 3;
}
bool isPlayer2Dead(){
	pCharacter pl2 = player2;
	return isPlayer2StateDead(pl2);
}
int player2Revive(){
	if(player2->att.pl2.state == (int)(DEAD)){
		player2ChangeState(player2, INIT);
		player2->att.pl2.hp = player2->att.pl2.maxHp / 2;
	}
	player2->att.pl2.afterResurrection = 0;
	return 0;
}

int player2ChangeState(pCharacter player, Player2State nextState){
	Player2Att *att = &player->att.pl2;
	player->anim.started = getGameFrame();
	player2StateExit(player);

	att->beforeState = att->state;
	att->state = nextState;

	player2StateEnter(player);
	return 0;
}
int player2StateEnter(pCharacter player){
	Player2Att *att = &player->att.pl2;
	CharaHitAreaId hitId = HIT_PLAYER2_G;
	switch(att->state){
	case INIT:
		player->imageFileId = IMAGE_2P_IDLE;
		break;
	case IDLE:
		player->imageFileId = IMAGE_2P_IDLE;
		if(isKey(KEY_RIGHT) || isKey(KEY_LEFT)){
			player2ChangeState(player, WALK);
		}
		break;
	case WALK:
		player->imageFileId = IMAGE_2P_WALK;
		break;
	case JUMP:
		player->imageFileId = IMAGE_2P_JUMP;
		break;
	case MAIN_ATTACK:
		player->draw.relative.y = -35;
		player->imageFileId = IMAGE_2P_MAIN_ATTACK;
		att->alreadyMainAtk = false;
		break;
	case MAIN_ATTACK_AFTER:
		//硬直状態
		player->imageFileId = IMAGE_2P_IDLE;
		att->stiffnessCount = 0;
		break;
	case SUB_ATTACK:
		att->subAtkCoolCounter = 0;
		soundMgrPlay(SE_2P_RIFLE);
		player->imageFileId = IMAGE_2P_SUB_ATTACK;
		att->alreadySubAtk = false;
		break;
	case GUARD:
		player->imageFileId = IMAGE_2P_GUARD;
		break;
	case AVOID:
		setAvoidPos(player);
		player->dir = att->avoidDir;
		player->imageFileId = IMAGE_2P_AVOID;
		break;
	case DAMAGE_STOP:
		att->stopCoord.x = player->pos.x;
		att->stopCoord.y = player->pos.y;
		att->stopCount = 0;
		player->imageFileId = IMAGE_2P_DAMAGE;
		break;
	case DAMAGE:
		soundMgrPlay(SE_PLAYER_DAMAGE);
		charaOffSet(player, att->stopCoord.x, att->stopCoord.y);
		charaOffSet(player, att->stopCoord.x, att->stopCoord.y);
		player->imageFileId = IMAGE_2P_DAMAGE;
		break;
	case KNOCK_OUT:
		player->imageFileId = IMAGE_2P_KNOCK_OUT;
		break;
	case DEAD:
		player->imageFileId = IMAGE_2P_DEAD;
		countUpPlayer2Dead();
		break;
	case RESURRECTION:
		player->imageFileId = IMAGE_2P_RESURRECTION;
		att->alreadyResurrect = false;
	case SHOOT_RECOVERED_BULLETS:
		player->imageFileId = IMAGE_2P_SHOOT_RECOVERED_BULLETS;
		att->alreadyRecoveryAtk = false;
	default:
		break;
	}
	int dy = (player->hit.size.y - player->hit.relative.y);
	charaColSet(player, hitId);
	dy = (player->hit.size.y - player->hit.relative.y) - dy;
	charaOffSet(player, player->pos.x, player->pos.y + dy);
	AnimaData creature = {0};
	createAnimaData(&creature, player->imageFileId);
	animaSetup(&player->anim,
		getGameFrame(),
		&creature);
	return 0;
}
int player2StateExit(pCharacter player){
	Player2Att *att = &player->att.pl2;
	switch(att->state){
	case INIT:
		break;
	case IDLE:
		break;
	case WALK:
		break;
	case JUMP:
		break;
	case MAIN_ATTACK:
		player->draw.relative.y = 0;
		break;
	case MAIN_ATTACK_AFTER:
		break;
	case SUB_ATTACK:
		break;
	case GUARD:
		break;
	case AVOID:
		break;
	case DAMAGE_STOP:
		break;
	case DAMAGE:
		break;
	case KNOCK_OUT:
		break;
	case DEAD:
		break;
	case RESURRECTION:
		break;
	case SHOOT_RECOVERED_BULLETS:
		break;
	default:
		break;
	}
	return 0;
}

int player2Update(pCharacter player){
	Player2Att *att = &player->att.pl2;
#ifdef _DBGOPT_KEYBOARD_
	if(ForDebugDownKey('8')){
		player2ChangeState(player, INIT);
		att->hp = att->maxHp;
	}
#endif
	player->anim.time = getGameFrame();
	player2Section(player);
	att->afterResurrection++;

	ImageFileId fileId = player->imageFileId;
	int index = animaGetValueStep(&player->anim);
	int handle = imageMgrGetHandle(fileId, index);
	charaSetImageHandle(player, handle);
	charaSetDispSize(player, DISP_PLAYER2_NORMAL);
	charaFlipX(player, player->dir == RIGHT);

	//無敵中であれば、なんか処理
	if(att->invincibleFlag){
		att->invincibleCount++;
		if(att->invincibleCount >= att->invincibleLimit){
			att->invincibleFlag = false;
		}
	}

	//なんだこれ？
	att->subAtkCoolCounter++;

	//uiのため、HPを教える
	uiMgrTeach2pHp(att->hp, att->maxHp);

	//デバッグ用表示
	flashDebugInfo("クールタイム : %d", att->subAtkCoolCounter);

	return 0;
}
int player2Section(pCharacter player){
	Player2Att *att = &player->att.pl2;
	switch(att->state){
	case INIT:
		player2Init(player);
		break;
	case IDLE:
		player2Idle(player);
		break;
	case WALK:
		player2Move(player);
		break;
	case JUMP:
		player2Jump(player);
		break;
	case MAIN_ATTACK:
		player2MainAttack(player);
		break;
	case MAIN_ATTACK_AFTER:
		player2MainAttackAfter(player);
		break;
	case SUB_ATTACK:
		player2SubAttack(player);
		break;
	case GUARD:
		player2Guard(player);
		break;
	case AVOID:
		player2Avoid(player);
		break;
	case DAMAGE_STOP:
		player2DamageStop(player);
		break;
	case DAMAGE:
		player2Damage(player);
		break;
	case KNOCK_OUT:
		player2KnockOut(player);
		break;
	case DEAD:
		player2Dead(player);
		break;
	case RESURRECTION:
		player2Resurrection(player);
		break;
	case SHOOT_RECOVERED_BULLETS:
		player2ShootRecoveredBullets(player);
		break;
	default:
		player2ChangeState(player, INIT);
		break;
	}
	return 0;
}
//当たり判定飛ばし　メイン攻撃
int setAttackerMainAtk(pCharacter player){
	int x = int(player->pos.x);
	int y = int(player->pos.y);
	x += player->hit.relative.x + player->hit.size.x / 2;
	y += player->hit.relative.y + player->hit.size.y / 2;
	int vx = BULLET_SPEED;
	if(player->dir == LEFT){
		vx *= -1;
	}
	setPlayer2Bullet(x, y, vx, 0, player->att.pl2.attack);
	return 0;
}
//当たり判定飛ばし　サブ攻撃
int setAttackerSubAtk(pCharacter player){
	const pBox rect = (pBox)getBoxData(HIT_PLAYER2_SUB_A);
	int x = (int)(player->pos.x) + player->hit.relative.x;
	int y = (int)(player->pos.y) + player->hit.relative.y + player->hit.size.y / 2;
	switch(player->dir){
	case LEFT:
		x -= (rect->relative.x + rect->size.x);
		break;
	case RIGHT:
		x += player->hit.size.x;
		x -= rect->relative.x;
		break;
	default:
		break;
	}
	y -= 50;
	double vx = M_SPEED;
	if(player->dir == LEFT){
		vx *= -1;
	}
	player2SubAtkSet(x, y, vx, 0, M_DELAY);
	return 0;
}
//回復弾飛ばし
int setRecoveredBullets(pCharacter player){
	int x = int(player->pos.x);
	int y = int(player->pos.y);
	x += player->hit.relative.x + player->hit.size.x / 2;
	y += player->hit.relative.y + player->hit.size.y / 2;
	int vx = BULLET_SPEED;
	if(player->dir == LEFT){
		vx *= -1;
	}
	setPlayer2RecoveredBullet(x, y, vx, 0, player->att.pl2.attack);
	return 0;
}

int player2Init(pCharacter player){
	Player2Att *att = &player->att.pl2;
	player2ChangeState(player, IDLE);
	att->afterResurrection = 0;
	att->walkSpeed = att->walkSpeedBase;
	att->jumpPow = att->jumpPowBase;
	att->attack = att->attackBase;
	att->avoidPow = att->avoidPowBase;
	att->invincibleFlag = false;
	att->invincibleLimit = INVINCIBLE_TIME;
	return 0;
}
int player2Idle(pCharacter player){
	//情報取得
	Player2Att *att = &player->att.pl2;
	XYd velocity = att->velocity;

	//動く
	if(isKey(KEY_RIGHT)){
		player2ChangeState(player, WALK);
	}
	if(isKey(KEY_LEFT)){
		player2ChangeState(player, WALK);
	}
	if(isKeyDown(KEY_JUMP)){
		att->velocity.y = att->jumpPow;
		player2ChangeState(player, JUMP);
		return -1;
	}
	if(isKeyDown(KEY_MAIN_ATK)){
		player2ChangeState(player, MAIN_ATTACK);
		return -1;
	}
	if(isKeyDown(KEY_SUB_ATK)){
		if(att->subAtkCoolCounter >= att->subAtkCoolTime){
			player2ChangeState(player, SUB_ATTACK);
			return -1;
		}
	}
	if(isKeyDown(KEY_RESURRECTION)){
		player2ChangeState(player, RESURRECTION);
		return -1;
	}
	if(isKeyDown(KEY_GUARD)){
		//ガードキーを押したら回復弾を撃つ
		///player2ChangeState(player, GUARD);
		player2ChangeState(player, SHOOT_RECOVERED_BULLETS);
		return -1;
	}
	if(isKey(KEY_AVOID_L)){
		att->avoidDir = LEFT;
		player2ChangeState(player, AVOID);
		return -1;
	}
	if(isKey(KEY_AVOID_R)){
		att->avoidDir = RIGHT;
		player2ChangeState(player, AVOID);
		return -1;
	}
	if(!isCharaStandingOnBlock((void*)player)){
		player2ChangeState(player, JUMP);
		return -1;
	}

	player2PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
//プレイヤー2横移動中
int player2Move(pCharacter player){
	Player2Att *att = &player->att.pl2;
	XYd velocity = att->velocity;
	bool idleFlag = true;

	if(isKey(KEY_RIGHT)){
		idleFlag = false;
		velocity.x += att->walkSpeed / 20;
		player->dir = RIGHT;
	} else if(isKey(KEY_LEFT)){
		idleFlag = false;
		velocity.x -= att->walkSpeed / 20;
		player->dir = LEFT;
	}
	if(isKeyDown(KEY_JUMP)){
		att->velocity.y = att->jumpPow;
		player2ChangeState(player, JUMP);
		return -1;
	}
	if(isKeyDown(KEY_MAIN_ATK)){
		player2ChangeState(player, MAIN_ATTACK);
		return -1;
	}
	if(isKeyDown(KEY_SUB_ATK)){
		if(att->subAtkCoolCounter >= att->subAtkCoolTime){
			player2ChangeState(player, SUB_ATTACK);
			return -1;
		}
	}
	if(isKeyDown(KEY_RESURRECTION)){
		player2ChangeState(player, RESURRECTION);
		return -1;
	}
	if(isKeyDown(KEY_GUARD)){
		//ガードキーを押したら回復弾を撃つ
		///player2ChangeState(player, GUARD);
		player2ChangeState(player, SHOOT_RECOVERED_BULLETS);
		return -1;
	}
	if(isKey(KEY_AVOID_L)){
		att->avoidDir = LEFT;
		player2ChangeState(player, AVOID);
		return -1;
	}
	if(isKey(KEY_AVOID_R)){
		att->avoidDir = RIGHT;
		player2ChangeState(player, AVOID);
		return -1;
	}
	if(idleFlag){
		player2ChangeState(player, IDLE);
	}
	if(!isCharaStandingOnBlock((void*)player)){
		player2ChangeState(player, JUMP);
		return -1;
	}


	player2PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
int player2Jump(pCharacter player){
	//情報取得
	Player2Att *att = &player->att.pl2;
	XYd velocity = att->velocity;

	//動く
	if(isKey(KEY_RIGHT)){
		velocity.x += att->walkSpeed / 20;
		player->dir = RIGHT;
	} else if(isKey(KEY_LEFT)){
		velocity.x -= att->walkSpeed / 20;
		player->dir = LEFT;
	}
	if(isKeyDown(KEY_MAIN_ATK)){
		player2ChangeState(player, MAIN_ATTACK);
		return -1;
	}
	if(isKeyDown(KEY_SUB_ATK)){
		if(att->subAtkCoolCounter >= att->subAtkCoolTime){
			player2ChangeState(player, SUB_ATTACK);
			return -1;
		}
	}

	if(player2PosUpdate(player, velocity.x, velocity.y)){
		player2ChangeState(player, IDLE);
	}

	return 0;
}
int player2MainAttack(pCharacter player){
	Player2Att *att = &player->att.pl2;
	XYd velocity = att->velocity;
	Anima *anim = &(player->anim);
	//弾飛ばし↓
	constexpr int attackFrame = 5;
	const int animFrame = animaGetFrame(anim);
	if(animFrame >= attackFrame){
		if(att->alreadyMainAtk == false){
			att->alreadyMainAtk = true;
			setAttackerMainAtk(player);
		}
	}
	//弾飛ばし↑
	//アニメーション終了で状態チェンジ↓
	if(animaGetCount(&player->anim) >= 1){
		//攻撃後硬直状態へ
		player2PosUpdate(player, velocity.x, velocity.y);
		player2ChangeState(player, MAIN_ATTACK_AFTER);
		return -1;
	}
	//アニメーション終了で状態チェンジ↑
	player2PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
int player2MainAttackAfter(pCharacter player){
	Player2Att *att = &player->att.pl2;
	XYd velocity = att->velocity;

	//時間経過で状態チェンジ
	if(att->stiffnessCount >= att->stiffnessTimeLimit){
		if(isCharaStandingOnBlock((void*)player)){
			player2ChangeState(player, IDLE);
		} else{
			player2ChangeState(player, JUMP);
		}
		return -1;
	}
	player2PosUpdate(player, velocity.x, velocity.y);
	att->stiffnessCount++;
	return 0;
}

int player2SubAttack(pCharacter player){
	Player2Att *att = &player->att.pl2;
	XYd velocity = att->velocity;
	const int shootTiming = 10;
	int animFrame = animaGetFrame(&player->anim);
	int animLimit = player->anim.limit;
	//弾を出すべきタイミングか
	if(animFrame >= shootTiming){
		//まだ弾を出していないか
		if(!(att->alreadySubAtk)){
			att->alreadySubAtk = true;
			setAttackerSubAtk(player);
		}
	}
	//アニメ終了で状態チェンジ↓
	if(animFrame >= animLimit){
		player2ChangeState(player, IDLE);
	}
	//アニメ終了で状態チェンジ↑
	player2PosUpdate(player, velocity.x, velocity.y);
	return 0;
}/////////

int player2Guard(pCharacter player){
	Player2Att *att = &player->att.pl2;
	XYd velocity = att->velocity;
	//最後の一コマで止まる処理
	Anima *anim = &player->anim;
	unsigned int frame = animaGetFrame(anim);
	if(frame >= anim->limit){
		anim->started = anim->time - anim->duration + 1;
	}
	//キー入力を監視する
	if(!isKey(KEY_GUARD)){
		player2ChangeState(player, IDLE);
		return -1;
	}
	player2PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
int player2Avoid(pCharacter player){
	Player2Att *att = &player->att.pl2;
	XYd velocity = att->velocity;
	double v = att->avoidPosX.current;
	ParameterUpdate(&att->avoidPosX,
		player->anim.time - player->anim.started,
		player->anim.duration);
	v = att->avoidPosX.current - v;

	if(animaGetCount(&player->anim) >= 1){

		player2ChangeState(player, IDLE);
		return -1;
	}


	player2PosUpdate(player, velocity.x + v, velocity.y);
	return 0;
}
int player2DamageStop(pCharacter player){
	Player2Att *att = &player->att.pl2;
	XYd velocity = att->velocity;
	if(att->stopCount >= att->stopLimit){
		if(att->hp > 0){
			player2ChangeState(player, DAMAGE);
		} else{
			player2ChangeState(player, KNOCK_OUT);
		}
		return -1;
	}

	player2PosUpdate(player, rand() % 16 - 8, rand() % 16 - 8);
	player2PosUpdate(player, velocity.x, velocity.y);
	att->stopCount++;
	return 0;
}
int player2Damage(pCharacter player){
	Player2Att *att = &player->att.pl2;
	XYd velocity = att->velocity;
	double kbX = att->knockBackX;
	double kbY = att->knockBackY;

	kbX *= 0.96;
	kbY *= 0.96;


	player2PosUpdate(player, kbX, kbY);

	if(player2PosUpdate(player, velocity.x, velocity.y)){
		player2ChangeState(player, IDLE);
		return -1;
	}

	att->knockBackX = kbX;
	att->knockBackY = kbY;
	return 0;
}
int player2KnockOut(pCharacter player){
	Player2Att *att = &player->att.pl2;
	XYd velocity = att->velocity;
	//アニメーション終了で状態チェンジ↓
	if(animaGetCount(&player->anim) >= 1){
		player2ChangeState(player, DEAD);
		return -1;
	}
	//アニメーション終了で状態チェンジ↑
	player2PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
int player2Dead(pCharacter player){
	Player2Att *att = &player->att.pl2;
	XYd velocity = att->velocity;

	player2PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
int player2Resurrection(pCharacter player){
	Player2Att *att = &player->att.pl2;
	XYd velocity = att->velocity;
	//特定のコマの時、1Pと重なっていれば復活させる↓
	{
		Anima *anim = &(player->anim);
		const int animFrame = animaGetFrame(anim);
		constexpr int resurrectFrame = 8;
		if(animFrame >= resurrectFrame){
			if(att->alreadyResurrect == false){
				att->alreadyResurrect = true;
				//1Pと衝突チェック
				if(charaHit(player, player1)){
					//復活
					bridgePlayer1Revive();
				}
			}
		}
	}
	//特定のコマの時、2Pと重なっていれば復活させる↑
	//アニメーション終了で状態チェンジ↓
	if(animaGetCount(&player->anim) >= 1){
		player2ChangeState(player, IDLE);
		return -1;
	}
	//アニメーション終了で状態チェンジ↑
	player2PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
int player2ShootRecoveredBullets(pCharacter player){
	Player2Att *att = &player->att.pl2;
	XYd velocity = att->velocity;
	const int shootTiming = 10;
	int animFrame = animaGetFrame(&player->anim);
	int animLimit = player->anim.limit;
	//弾を出すべきタイミングか
	if(animFrame >= shootTiming){
		//まだ弾を出していないか
		if(!(att->alreadyRecoveryAtk)){
			att->alreadyRecoveryAtk = true;
			setRecoveredBullets(player);
		}
	}
	//アニメ終了で状態チェンジ↓
	if(animFrame >= animLimit){
		player2ChangeState(player, IDLE);
	}
	//アニメ終了で状態チェンジ↑
	player2PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
int player2PosUpdate(pCharacter player, double vx, double vy){
	int result = 0;
	Player2Att *att = &player->att.pl2;
	double x, y;
	double gravity = att->gravity;
	getCharaOffSet(player, &x, &y);

	vy += gravity;
	vx *= 0.65;
	vy *= 0.97;

	x += vx;
	y += vy;


	//世界の果てに行かせない↓
	if(x < 0 - player->hit.relative.x){
		x = 0 - player->hit.relative.x;
	}
	if(x > (WORLD_WIDTH - player->hit.relative.x - player->hit.size.x)){
		x = WORLD_WIDTH - player->hit.relative.x - player->hit.size.x;
	}
	//世界の果てに行かせない↑

	charaOffSet(player, x, y);
	if(isKeyDown(KEY_DOWN)){
		player->beforePos.y++;
		player->pos.y++;
	}

	int hitInfo = charaBlockingByBlock((void*)player);
	if(charaLanding(hitInfo)){
		//着地
		vy = 0;
		result = -1;
	}
	att->velocity.x = vx;
	att->velocity.y = vy;
	return result;
}
int setAvoidPos(pCharacter player){
	Player2Att *att = &player->att.pl2;
	double x, y, v;
	getCharaOffSet(player, &x, &y);
	v = (1 - (att->avoidDir == LEFT)) * 2 - 1;
	v *= att->avoidPow;
	att->avoidPosX.start = x;
	att->avoidPosX.stop = x + v;
	att->avoidPosX.current = att->avoidPosX.start;
	att->avoidPosX.easing = EASE_OUT_CIRC;
	return 0;
}
bool isPlayer2StateDead(pCharacter player){
	Player2State state = (Player2State)player->att.pl2.state;
	if(state == KNOCK_OUT){
		return true;
	}
	if(state == DEAD){
		return true;
	}
	return false;
}

static bool isKey(Keys keyType){
	return isFreshPad2(keyConfig[keyType]);
}

//static bool isKeyUp(Keys keyType){
//    return isKeyUp_2p(keyConfig[keyType]);
//}

static bool isKeyDown(Keys keyType){
	return isDownPad2(keyConfig[keyType]);
}

