
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
#include "chara_block.h"
#include "chara_enemy.h"
#include "chara__manager.h"
#include "chara_player1_sub.h"
#include "player_1_2_bridge.h"
#include "stage_data_reader.h"
#include "camera.h"
#include "in_game_effect.h"
#include "in_game_simple_effect.h"
#include "ui_mgr.h"
#include "new_effect_hp_heal.h"
#include "new_effect_rain_effect.h"
#include "new_effect_shield.h"
#include "new_effect_shield_break.h"
#include "new_effect_confusion_star.h"
#include "chara_boss_stage2_thunder.h"
#include <cassert>
#include "forDebug.h"

//ファイル読み込み用
#define FILE_NAME_MAX	FILENAME_MAX
#define FILE_LINE_MAX	99
#define FILE_NAME_PLAYER_DATA	"_data\\1p_data.txt"

//ゲーム用
#define INVINCIBLE_TIME			60		//無敵時間
#define WIRE_DELAY				2		//ワイヤー遅延時間
#define WIRE_SPEED				28		//ワイヤー移動速度
#define WIRE_RANGE				500		//ワイヤー移動範囲
#define GUARD_INVINCIBLE_TIME	50		//ガード用無敵時間
#define GUARD_SHIELD_ENDURANCE	5		//ガードの耐久力


//プレイヤーのデータ(初期化時にファイルから読み込む)
Player1Att player1AttBase = { 0 };


typedef enum
{
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

typedef enum
{
	INIT,
	IDLE,
	WALK,
	JUMP,
	MAIN_ATTACK1,
	MAIN_ATTACK1_AFTER,
	MAIN_ATTACK2,
	MAIN_ATTACK3,
	SUB_ATTACK_EXEC,	//サブ攻撃行き(anim on)
	SUB_ATTACK_SUCCESS,	//サブ攻撃戻り(anim off)
	SUB_ATTACK_MISS,	//サブ攻撃戻り(anim off)
	SUB_ATTACK_AFTER,	//サブ攻撃腕戻し(anim on)
	GUARD,
	AVOID,
	DAMAGE_STOP,
	DAMAGE,
	KNOCK_OUT,
	DEAD,
	RESURRECTION,
	CONFUSION,
}Player1State;


extern int loadPlayer1DataFromFile();
extern int setPlayer1(int x, int y);
extern int player1Blow(double vx, double vy);
extern int player1HitDamage(int damage);
extern int player1HealHp(int recoveryAmount);
extern int getPlayer1X();
extern int getPlayer1FactMove(double *moveX, double *moveY);
extern bool isPlayer1Dead();
extern int player1Revive();

static int player1Update(pCharacter player);

static int player1Section(pCharacter player);
static int setAttackerMainAtk1(pCharacter player);
static int setAttackerMainAtk2(pCharacter player);
static int setAttackerMainAtk3(pCharacter player);
static int setAttackerSubAtk(pCharacter player);
static int player1ChangeState(pCharacter player, Player1State nextState);
static int player1StateEnter(pCharacter player);
static int player1StateExit(pCharacter player);
static int player1Init(pCharacter player);
static int player1Idle(pCharacter player);
static int player1Move(pCharacter player);
static int player1Jump(pCharacter player);
static int player1MainAttack1(pCharacter player);
static int player1MainAttack1After(pCharacter player);
static int player1MainAttack2(pCharacter player);
static int player1MainAttack3(pCharacter player);
static int player1SubAttackExec(pCharacter player);
static int player1SubAttackSuccess(pCharacter player);
static int player1SubAttackMiss(pCharacter player);
static int player1SubAttackAfter(pCharacter player);
static int player1Guard(pCharacter player);
static int player1Avoid(pCharacter player);
static int player1DamageStop(pCharacter player);
static int player1Damage(pCharacter player);
static int player1KnockOut(pCharacter player);
static int player1Dead(pCharacter player);
static int player1Resurrection(pCharacter player);
static int player1Confusion(pCharacter player);
static int player1PosUpdate(pCharacter player, double vx, double vy);
static int setAvoidPos(pCharacter player);
static bool isPlayer1StateDead(pCharacter player);
static bool isKey(Keys keyType);
//static bool isKeyUp(Keys keyType);//今は使わない
static bool isKeyDown(Keys keyType);


int loadPlayer1DataFromFile()
{
	FILE *in = nullptr;
	char fileName[FILE_NAME_MAX] = "\0";
	char comment[FILE_LINE_MAX] = "\0";
	int opened = -1;
	strncpy_s(fileName, FILE_NAME_PLAYER_DATA, FILE_NAME_MAX);
	opened = fopen_s(&in, fileName, "r");
	if (opened)
	{
		assert(!opened && "1P用のファイルが開けません");
		exit(0);
	}
	else
	{
		fscanf_s(in, "%lf", &player1AttBase.gravity);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%lf", &player1AttBase.walkSpeedBase);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &player1AttBase.maxHp);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%lf", &player1AttBase.jumpPowBase);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &player1AttBase.attackBase);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%lf", &player1AttBase.avoidPowBase);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &player1AttBase.stopLimit);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &player1AttBase.stiffnessTimeLimit);
		fgets(comment, FILE_LINE_MAX, in);
		{
			int mode = 0;
			fscanf_s(in, "%d", &mode);
			switch (mode)
			{
			case 0:
				keyInputModeDetermine1p(AMINPUT_KEYBOARD);
				break;
			case 1:
				keyInputModeDetermine1p(AMINPUT_PAD1);
				break;
			case 2:
				keyInputModeDetermine1p(AMINPUT_PAD2);
				break;
			default:
				break;
			}
		}
		fclose(in);
	}
	return 0;
}
int setPlayer1(int x, int y)
{
	pCharacter pl = player1;
	charaInit(pl);
	pl->isActive = true;
	charaColSet(pl, HIT_PLAYER1_G);
	charaOffSet(pl, x, y);
	pl->att.pl1 = player1AttBase;
	pl->att.pl1.hp = pl->att.pl1.maxHp;
	charaFuncSet(pl, player1Update);
	player1ChangeState(pl, INIT);
	pl->imageFileId = IMAGE_1P_IDLE;
	return 0;
}
int player1Blow(double vx, double vy)
{
	pCharacter pl1 = player1;
	Player1Att *att = &pl1->att.pl1;
	if (att->state == CONFUSION)
	{//混乱中チェック
//ここでプレイヤーを揺らすだけ揺らしたい
		if (att->invincibleFlag == false)
		{
			att->quakeCount = att->stopLimit;
		}
		return -1;
	}
	if (att->state == GUARD)
	{//ガード中チェック
		return -1;
	}
	if (att->state == AVOID)
	{//回避中チェック
		return -1;
	}
	if (isPlayer1StateDead(pl1))
	{//死亡チェック
		return -1;
	}
	if (att->invincibleFlag && att->invincibleCount)
	{//無敵チェック
		return -1;
	}
	//状態を変える
	att->knockBackX = vx;
	att->knockBackY = vy;
	player1ChangeState(pl1, DAMAGE_STOP);
	return 0;
}
int player1HitDamage(int damage)
{
	pCharacter pl1 = player1;
	Player1Att *att = &pl1->att.pl1;
	if (damage > 0)
	{
		if (att->state == CONFUSION)
		{//混乱中チェック
//ここでダメージだけ与えたい
			if (att->invincibleFlag == false)
			{
				att->hp -= damage;
				att->hp = (int)Clamp((double)att->hp, 0.0, (double)att->maxHp);
				//ついでに無敵になる
				att->invincibleFlag = true;
				att->invincibleCount = 0;
			}
			return -1;
		}
		if (att->state == GUARD)
		{//ガード中チェック
//ガード用変数を操作すべきか
			if (att->inGuardGuardCount <= 0)
			{
//カウンターを増やす
				att->inGuardTotalHitCount++;
				att->inGuardTotalHitDamage += damage;
				att->inGuardGuardCount = GUARD_INVINCIBLE_TIME;
			}
			return -1;
		}
		if (att->state == AVOID)
		{//回避中チェック
			return -1;
		}
		if (isPlayer1StateDead(pl1))
		{//死亡チェック
			return -1;
		}
		if (att->invincibleFlag)
		{//無敵チェック
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

int player1HealHp(int recoveryAmount)
{
	pCharacter pl1 = player1;
	Player1Att *att = &pl1->att.pl1;
	att->hp += recoveryAmount;
	att->hp = (int)Clamp((double)att->hp, 0.0, (double)att->maxHp);
	double effectX = pl1->pos.x + pl1->hit.relative.x + pl1->hit.size.x / 2;
	double effectY = pl1->pos.y + pl1->hit.relative.y + pl1->hit.size.y;
	hpHealEffectSet(effectX, effectY);
	return 0;
}

int getPlayer1X()
{
	pCharacter pl1 = player1;
	return (int)(pl1->pos.x) + pl1->hit.relative.x + pl1->hit.size.x / 2;
}
int getPlayer1FactMove(double *moveX, double *moveY)
{
	pCharacter pl1 = player1;
	*moveX = pl1->pos.x - pl1->beforePos.x;
	*moveY = pl1->pos.y - pl1->beforePos.y;
	return 0;
}
bool isPlayer1Dead()
{
	pCharacter pl1 = player1;
	return isPlayer1StateDead(pl1);
}
int player1Revive()
{
	if (player1->att.pl1.state == (int)(DEAD))
	{
		player1ChangeState(player1, INIT);
		player1->att.pl1.hp = player1->att.pl1.maxHp / 2;
	}
	return 0;
}

int player1ChangeState(pCharacter player, Player1State nextState)
{
	Player1Att *att = &player->att.pl1;
	player->anim.started = getGameFrame();
	player1StateExit(player);

	att->beforeState = att->state;
	att->state = nextState;

	player1StateEnter(player);
	return 0;
}
int player1StateEnter(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	CharaHitAreaId hitId = HIT_PLAYER1_G;
	switch (att->state)
	{
	case INIT:
		charaSetDispSize(player, DISP_PLAYER1_IDLE);
		player->imageFileId = IMAGE_1P_IDLE;
		break;
	case IDLE:
		charaSetDispSize(player, DISP_PLAYER1_IDLE);
		player->imageFileId = IMAGE_1P_IDLE;
		if (!(att->reading))
		{
			if (isKey(KEY_RIGHT) || isKey(KEY_LEFT))
			{
				player1ChangeState(player, WALK);
			}
		}
		break;
	case WALK:
		charaSetDispSize(player, DISP_PLAYER1_WALK);
		player->imageFileId = IMAGE_1P_WALK;
		break;
	case JUMP:
		charaSetDispSize(player, DISP_PLAYER1_JUMP);
		player->imageFileId = IMAGE_1P_JUMP;
		break;
	case MAIN_ATTACK1:
		charaSetDispSize(player, DISP_PLAYER1_MAIN_ATK);
		soundMgrPlay(SE_1P_SLASH);
		att->nextAttackFlag = false;
		att->nextAttackCounter = 0;
		player->imageFileId = IMAGE_1P_MAIN_ATTACK1;
		setAttackerMainAtk1(player);
		break;
	case MAIN_ATTACK1_AFTER:
		player->imageFileId = IMAGE_1P_IDLE;
		att->stiffnessCount = 0;
		break;
	case MAIN_ATTACK2:
		charaSetDispSize(player, DISP_PLAYER1_MAIN_ATK);
		soundMgrPlay(SE_1P_SLASH);
		att->nextAttackFlag = false;
		att->nextAttackCounter = 0;
		player->imageFileId = IMAGE_1P_MAIN_ATTACK2;
		setAttackerMainAtk2(player);
		break;
	case MAIN_ATTACK3:
		charaSetDispSize(player, DISP_PLAYER1_MAIN_ATK);
		soundMgrPlay(SE_1P_SLASH);
		player->draw.relative.y = -35;
		att->nextAttackFlag = false;
		att->nextAttackCounter = 0;
		player->imageFileId = IMAGE_1P_MAIN_ATTACK3;
		setAttackerMainAtk3(player);
		break;
	case SUB_ATTACK_EXEC:
		charaSetDispSize(player, DISP_PLAYER1_SUB_ATK);
		player->imageFileId = IMAGE_1P_SUB_ATTACK_BEFORE;
		att->alreadySubAttack = false;
		break;
	case SUB_ATTACK_SUCCESS:
		charaSetDispSize(player, DISP_PLAYER1_SUB_ATK);
		player->imageFileId = IMAGE_1P_SUB_ATTACK_AFTER;
		break;
	case SUB_ATTACK_MISS:
		charaSetDispSize(player, DISP_PLAYER1_SUB_ATK);
		player->imageFileId = IMAGE_1P_SUB_ATTACK_AFTER;
		break;
	case SUB_ATTACK_AFTER:
		charaSetDispSize(player, DISP_PLAYER1_SUB_ATK);
		player->imageFileId = IMAGE_1P_SUB_ATTACK_AFTER;
		break;
	case GUARD:
		charaSetDispSize(player, DISP_PLAYER1_GUARD);
		player->imageFileId = IMAGE_1P_GUARD;
		att->inGuardTotalHitCount = 0;
		att->inGuardTotalHitDamage = 0;
		att->inGuardGuardCount = 0;
		att->alreadyExpandShield = false;
		att->shieldHandle = -1;
		break;
	case AVOID:
		charaSetDispSize(player, DISP_PLAYER1_JUMP);
		setAvoidPos(player);
		player->dir = att->avoidDir;
		player->imageFileId = IMAGE_1P_AVOID;
		break;
	case DAMAGE_STOP:
		charaSetDispSize(player, DISP_PLAYER1_CONFUSION);
		att->stopCoord.x = player->pos.x;
		att->stopCoord.y = player->pos.y;
		att->stopCount = 0;
		player->imageFileId = IMAGE_1P_DAMAGE;
		break;
	case DAMAGE:
		charaSetDispSize(player, DISP_PLAYER1_CONFUSION);
		soundMgrPlay(SE_PLAYER_DAMAGE);
		charaOffSet(player, att->stopCoord.x, att->stopCoord.y);
		charaOffSet(player, att->stopCoord.x, att->stopCoord.y);
		player->imageFileId = IMAGE_1P_DAMAGE;
		break;
	case KNOCK_OUT:
		charaSetDispSize(player, DISP_PLAYER1_DEATH);
		player->imageFileId = IMAGE_1P_KNOCK_OUT;
		break;
	case DEAD:
		charaSetDispSize(player, DISP_PLAYER1_DEATH);
		player->imageFileId = IMAGE_1P_DEAD;
		countUpPlayer1Dead();
		break;
	case RESURRECTION:
		charaSetDispSize(player, DISP_PLAYER1_RESURRECTION);
		player->imageFileId = IMAGE_1P_RESURRECTION;
		att->alreadyResurrect = false;
		break;
	case CONFUSION:
		charaSetDispSize(player, DISP_PLAYER1_CONFUSION);
		player->imageFileId = IMAGE_1P_DAMAGE;
		//盾崩壊エフェクトを出す↓
		{
			double x = player->pos.x + player->hit.relative.x;
			double y = player->pos.y + player->hit.relative.y;
			double x_offset = 0;
			double y_offset = player->hit.size.y / 2;
			if (player->dir == LEFT)
			{
				x_offset = 0;
			}
			else
			{
				x_offset = player->hit.size.x;
			}
			x += x_offset;
			y += y_offset;
			newEffectShieldBreakSet(x, y);
		}
		//盾崩壊エフェクトを出す↑
		//混乱エフェクトを出す↓
		{
			double x = player->pos.x + player->hit.relative.x;
			double y = player->pos.y + player->hit.relative.y;
			double x_offset = player->hit.size.x / 2;
			double y_offset = 0;
			x += x_offset;
			y += y_offset;
			att->confusionStarHandle = newEffectConfusionStarSet(x, y);
		}
		//混乱エフェクトを出す↑
		break;
	default:
		break;
	}
	int dy = (player->hit.size.y - player->hit.relative.y);
	charaColSet(player, hitId);
	dy = (player->hit.size.y - player->hit.relative.y) - dy;
	charaOffSet(player, player->pos.x, player->pos.y + dy);
	AnimaData animaData = { 0 };
	createAnimaData(&animaData, player->imageFileId);
	animaSetup(&player->anim,
		getGameFrame(),
		&animaData);
	return 0;
}
int player1StateExit(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	switch (att->state)
	{
	case INIT:
		break;
	case IDLE:
		break;
	case WALK:
		break;
	case JUMP:
		break;
	case MAIN_ATTACK1:
		clearAttacker(att->attackerMainAtk1);
		break;
	case MAIN_ATTACK1_AFTER:
		break;
	case MAIN_ATTACK2:
		clearAttacker(att->attackerMainAtk2);
		break;
	case MAIN_ATTACK3:
		clearAttacker(att->attackerMainAtk3);
		break;
	case SUB_ATTACK_EXEC:
		break;
	case SUB_ATTACK_SUCCESS:
		att->subAtkIndex = -1;
		break;
	case SUB_ATTACK_MISS:
		att->subAtkIndex = -1;
		break;
	case SUB_ATTACK_AFTER:
		break;
	case GUARD:
		att->inGuardTotalHitCount = 0;
		att->inGuardTotalHitDamage = 0;
		att->inGuardGuardCount = 0;
		newEffectShieldDelete(att->shieldHandle);
		att->shieldHandle = -1;
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
	case CONFUSION:
		//混乱エフェクトを消す
		newEffectConfusionStarDelete(att->confusionStarHandle);
		break;
	default:
		break;
	}
	charaSetDispSize(player, DISP_PLAYER1_IDLE);
	return 0;
}

int player1Update(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
#ifdef _DBGOPT_KEYBOARD_
	if (ForDebugDownKey('7'))
	{
		player1ChangeState(player, INIT);
		att->hp = att->maxHp;
	}
#endif

	player->anim.time = getGameFrame();
	player1Section(player);

	int index = animaGetValueStep(&player->anim);
	int handle = imageMgrGetHandle(player->imageFileId, index);
	charaSetImageHandle(player, handle);
	charaFlipX(player, player->dir == RIGHT);
	//無敵中であれば、なんか処理↓
	if (att->invincibleFlag)
	{
		att->invincibleCount++;
		if (att->invincibleCount >= att->invincibleLimit)
		{
			att->invincibleFlag = false;
		}
	}
	//無敵中であれば、なんか処理↑
	//振動中であれば、なんか処理↓
	if (att->quakeCount > 0)
	{
		att->quakeCount--;
		if (att->quakeCount > 0)
		{
			int w = 20;
			int w2 = w / 2;
			player->draw.quake.x = (rand() % w) - w2;
			player->draw.quake.y = (rand() % w) - w2;
		}
		else
		{
			player->draw.quake = {};
		}
	}
	//振動中であれば、なんか処理↑


	if (att->inGuardGuardCount > 0)
	{
		att->inGuardGuardCount--;
	}

	//uiのため、HPを教える
	uiMgrTeach1pHp(att->hp, att->maxHp);

	flashDebugInfo("回数:%d", att->inGuardTotalHitCount);
	flashDebugInfo("ダメージ:%d", att->inGuardTotalHitDamage);
	flashDebugInfo("カウント:%d", att->inGuardGuardCount);

	return 0;
}
int player1Section(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	switch (att->state)
	{
	case INIT:
		player1Init(player);
		break;
	case IDLE:
		player1Idle(player);
		break;
	case WALK:
		player1Move(player);
		break;
	case JUMP:
		player1Jump(player);
		break;
	case MAIN_ATTACK1:
		player1MainAttack1(player);
		break;
	case MAIN_ATTACK1_AFTER:
		player1MainAttack1After(player);
		break;
	case MAIN_ATTACK2:
		player1MainAttack2(player);
		break;
	case MAIN_ATTACK3:
		player1MainAttack3(player);
		break;
	case SUB_ATTACK_EXEC:
		player1SubAttackExec(player);
		break;
	case SUB_ATTACK_SUCCESS:
		player1SubAttackSuccess(player);
		break;
	case SUB_ATTACK_MISS:
		player1SubAttackMiss(player);
		break;
	case SUB_ATTACK_AFTER:
		player1SubAttackAfter(player);
		break;
	case GUARD:
		player1Guard(player);
		break;
	case AVOID:
		player1Avoid(player);
		break;
	case DAMAGE_STOP:
		player1DamageStop(player);
		break;
	case DAMAGE:
		player1Damage(player);
		break;
	case KNOCK_OUT:
		player1KnockOut(player);
		break;
	case DEAD:
		player1Dead(player);
		break;
	case RESURRECTION:
		player1Resurrection(player);
		break;
	case CONFUSION:
		player1Confusion(player);
		break;
	default:
		player1ChangeState(player, INIT);
		break;
	}
	return 0;
}
int setAttackerMainAtk1(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	const pBox rect = (pBox)getBoxData(HIT_PLAYER1_MAIN1_A);
	int x = (int)(player->pos.x) + player->hit.relative.x;
	int y = (int)(player->pos.y) + player->hit.relative.y + player->hit.size.y / 2;
	switch (player->dir)
	{
	case RIGHT:
		x += player->hit.size.x;
		x -= rect->relative.x;
		break;
	case LEFT:
		x -= (rect->relative.x + rect->size.x);
		break;
	default:
		break;
	}
	att->attackerMainAtk1 = setAttacker(x, y, rect);
	return 0;
}
int setAttackerMainAtk2(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	const pBox rect = (pBox)getBoxData(HIT_PLAYER1_MAIN2_A);
	int x = (int)(player->pos.x) + player->hit.relative.x;
	int y = (int)(player->pos.y) + player->hit.relative.y + player->hit.size.y / 2;
	switch (player->dir)
	{
	case RIGHT:
		x += player->hit.size.x;
		x -= rect->relative.x;
		break;
	case LEFT:
		x -= (rect->relative.x + rect->size.x);
		break;
	default:
		break;
	}
	att->attackerMainAtk2 = setAttacker(x, y, rect);
	return 0;
}
int setAttackerMainAtk3(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	const pBox rect = (pBox)getBoxData(HIT_PLAYER1_MAIN3_A);
	int x = (int)(player->pos.x) + player->hit.relative.x;
	int y = (int)(player->pos.y) + player->hit.relative.y + player->hit.size.y / 2;
	switch (player->dir)
	{
	case RIGHT:
		x += player->hit.size.x;
		x -= rect->relative.x;
		break;
	case LEFT:
		x -= (rect->relative.x + rect->size.x);
		break;
	default:
		break;
	}
	att->attackerMainAtk3 = setAttacker(x, y, rect);
	return 0;
}
int setAttackerSubAtk(pCharacter player)
{
//鎖を出す
//ここでマジックナンバーを使っている
	Player1Att *att = &player->att.pl1;
	const pBox chainBox = (pBox)getBoxData(HIT_CHAIN);
	int x = (int)(player->pos.x) + player->hit.relative.x;
	int y = (int)(player->pos.y) + player->hit.relative.y + player->hit.size.y / 2;
	switch (player->dir)
	{
	case RIGHT:
		x += player->hit.size.x;
		x -= 32;///
		break;
	case LEFT:
		x -= chainBox->size.x;
		break;
	default:
		break;
	}
	y -= 36;///
	att->subAtkIndex = player1SubAtkSet(x, y, player->dir);
	if (att->subAtkIndex < 0)
	{
		return -1;
	}
	return 0;
}

int player1Init(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	player1ChangeState(player, IDLE);
	att->walkSpeed = att->walkSpeedBase;
	att->jumpPow = att->jumpPowBase;
	att->attack = att->attackBase;
	att->avoidPow = att->avoidPowBase;
	att->invincibleFlag = false;
	att->invincibleLimit = INVINCIBLE_TIME;
	return 0;
}
int player1Idle(pCharacter player)
{
//情報取得
	Player1Att *att = &player->att.pl1;
	XYd velocity = att->velocity;

	//動く↓
	if (!(att->reading))
	{
		if (isKey(KEY_RIGHT))
		{
			player1ChangeState(player, WALK);
		}
		if (isKey(KEY_LEFT))
		{
			player1ChangeState(player, WALK);
		}
		if (isKeyDown(KEY_JUMP))
		{
			att->velocity.y = att->jumpPow;
			player1ChangeState(player, JUMP);
			return -1;
		}
		if (isKeyDown(KEY_MAIN_ATK))
		{
			player1ChangeState(player, MAIN_ATTACK1);
			return -1;
		}
		if (isKeyDown(KEY_SUB_ATK))
		{
			player1ChangeState(player, SUB_ATTACK_EXEC);
			return -1;
		}
		if (isKeyDown(KEY_RESURRECTION))
		{
			player1ChangeState(player, RESURRECTION);
			return -1;
		}
		if (isKey(KEY_GUARD))
		{
			player1ChangeState(player, GUARD);
			return -1;
		}
		if (isKey(KEY_AVOID_L))
		{
			att->avoidDir = LEFT;
			player1ChangeState(player, AVOID);
			return -1;
		}
		if (isKey(KEY_AVOID_R))
		{
			att->avoidDir = RIGHT;
			player1ChangeState(player, AVOID);
			return -1;
		}
	}
	//動く↑
	if (!isCharaStandingOnBlock((void*)player))
	{
		player1ChangeState(player, JUMP);
		return -1;
	}
	player1PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
int player1Move(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	XYd velocity = att->velocity;
	bool idleFlag = true;
	//動く↓
	if (isKey(KEY_RIGHT))
	{
		idleFlag = false;
		velocity.x += att->walkSpeed / 20;
		player->dir = RIGHT;
	}
	else if (isKey(KEY_LEFT))
	{
		idleFlag = false;
		velocity.x -= att->walkSpeed / 20;
		player->dir = LEFT;
	}
	if (isKeyDown(KEY_JUMP))
	{
		att->velocity.y = att->jumpPow;
		player1ChangeState(player, JUMP);
		return -1;
	}
	if (isKeyDown(KEY_MAIN_ATK))
	{
		player1ChangeState(player, MAIN_ATTACK1);
		return -1;
	}
	if (isKeyDown(KEY_SUB_ATK))
	{
		player1ChangeState(player, SUB_ATTACK_EXEC);
		return -1;
	}
	if (isKeyDown(KEY_RESURRECTION))
	{
		player1ChangeState(player, RESURRECTION);
		return -1;
	}
	if (isKey(KEY_GUARD))
	{
		player1ChangeState(player, GUARD);
		return -1;
	}
	if (isKey(KEY_AVOID_L))
	{
		att->avoidDir = LEFT;
		player1ChangeState(player, AVOID);
		return -1;
	}
	if (isKey(KEY_AVOID_R))
	{
		att->avoidDir = RIGHT;
		player1ChangeState(player, AVOID);
		return -1;
	}
	if (att->reading)
	{
		idleFlag = true;
	}
	//動く↑
	if (!isCharaStandingOnBlock((void*)player))
	{
		player1ChangeState(player, JUMP);
		return -1;
	}
	if (idleFlag)
	{
		player1ChangeState(player, IDLE);
	}


	player1PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
int player1Jump(pCharacter player)
{
//情報取得
	Player1Att *att = &player->att.pl1;
	XYd velocity = att->velocity;

	//動く
	if (isKey(KEY_RIGHT))
	{
		velocity.x += att->walkSpeed / 20;
		player->dir = RIGHT;
	}
	else if (isKey(KEY_LEFT))
	{
		velocity.x -= att->walkSpeed / 20;
		player->dir = LEFT;
	}
	if (isKeyDown(KEY_MAIN_ATK))
	{
		player1ChangeState(player, MAIN_ATTACK1);
		return -1;
	}
	if (isKeyDown(KEY_SUB_ATK))
	{
		player1ChangeState(player, SUB_ATTACK_EXEC);
		return -1;
	}

	if (player1PosUpdate(player, velocity.x, velocity.y))
	{
		player1ChangeState(player, IDLE);
	}

	return 0;
}
int player1MainAttack1(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	const pBox rect = (pBox)getBoxData(HIT_PLAYER1_MAIN1_A);
	XYd velocity = att->velocity;
	int x = (int)(player->pos.x) + player->hit.relative.x;
	int y = (int)(player->pos.y) + player->hit.relative.y + player->hit.size.y / 2;
	switch (player->dir)
	{
	case RIGHT:
		x += player->hit.size.x;
		x -= rect->relative.x;
		break;
	case LEFT:
		x -= (rect->relative.x + rect->size.x);
		break;
	default:
		break;
	}
	attackerOffset(att->attackerMainAtk1, x, y);

	//敵との当たり判定
	if (animaGetFrame(&player->anim) > (player->anim.limit * 0.5))
	{
		pCharacter hitting = enemies - 1;
		bool hitFlag = false;
		XYd efcPos = { 0,0 };
		do
		{
			hitting = checkAttackerHitCharacters(att->attackerMainAtk1, hitting + 1, ENEMY_NUM);
			if (hitting == nullptr)
			{
				break;
			}
			else
			{
				double kbx = 1, kby = -1;
				if (player->dir == LEFT)
				{
					kbx = -kbx;
				}
				enemyBlow(hitting, kbx, kby);
				enemyHitDamage(hitting, att->attack);
				/*
				efcPos.x = hitting->pos.x + hitting->hit.relative.x + hitting->hit.size.x / 2;
				efcPos.y = hitting->pos.y + hitting->hit.relative.y + hitting->hit.size.y / 2;
				*/
				getAttackerCoord(att->attackerMainAtk1, &efcPos.x, &efcPos.y);
				hitFlag = true;
			}
		} while (1);
		if (hitFlag)
		{
			clearAttacker(att->attackerMainAtk1);
			att->attackerMainAtk1 = -1;
			setSimpleEffect(SIMPLE_EFFECT_BLOOD, efcPos.x, efcPos.y, player->dir, IMAGE_BLOOD_ANIM);
		}
	}

	if (isKey(KEY_MAIN_ATK))
	{
		att->nextAttackCounter++;
	}
	if (isKeyDown(KEY_MAIN_ATK) || att->nextAttackCounter > 8)
	{
		att->nextAttackFlag = true;
	}
	if (animaGetCount(&player->anim) >= 1)
	{
///if(att->nextAttackFlag){//次の攻撃へ
///	player1ChangeState(player, MAIN_ATTACK2);
///} else{//アイドル状態へ　この時、空中にいればジャンプ状態になる
///	if(isCharaStandingOnBlock((void*)player)){
///		player1ChangeState(player, IDLE);
///	} else{
///		player1ChangeState(player, JUMP);
///	}
///	return -1;
///}
//攻撃後硬直状態へ
		player1PosUpdate(player, velocity.x, velocity.y);
		player1ChangeState(player, MAIN_ATTACK1_AFTER);
		return -1;
	}
	player1PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
int player1MainAttack1After(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	XYd velocity = att->velocity;

	//時間経過で状態チェンジ
	if (att->stiffnessCount >= att->stiffnessTimeLimit)
	{
		if (isCharaStandingOnBlock((void*)player))
		{
			player1ChangeState(player, IDLE);
		}
		else
		{
			player1ChangeState(player, JUMP);
		}
		return -1;
	}
	player1PosUpdate(player, velocity.x, velocity.y);
	att->stiffnessCount++;
	return 0;
}
int player1MainAttack2(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	const pBox rect = (pBox)getBoxData(HIT_PLAYER1_MAIN2_A);
	XYd velocity = att->velocity;
	int x = (int)(player->pos.x) + player->hit.relative.x;
	int y = (int)(player->pos.y) + player->hit.relative.y + player->hit.size.y / 2;
	switch (player->dir)
	{
	case RIGHT:
		x += player->hit.size.x;
		x -= rect->relative.x;
		break;
	case LEFT:
		x -= (rect->relative.x + rect->size.x);
		break;
	default:
		break;
	}
	attackerOffset(att->attackerMainAtk1, x, y);

	//敵との当たり判定
	if (animaGetFrame(&player->anim) > (player->anim.limit * 0.5))
	{
		pCharacter hitting = enemies - 1;
		bool hitFlag = false;
		do
		{
			hitting = checkAttackerHitCharacters(att->attackerMainAtk1, hitting + 1, ENEMY_NUM);
			if (hitting == nullptr)
			{
				break;
			}
			else
			{
				double kbx = 1, kby = -1;
				if (player->dir == LEFT)
				{
					kbx = -kbx;
				}
				enemyBlow(hitting, kbx, kby);
				enemyHitDamage(hitting, att->attack);
				hitFlag = true;
			}
		} while (1);
		if (hitFlag)
		{
			clearAttacker(att->attackerMainAtk1);
		}
	}

	if (isKey(KEY_MAIN_ATK))
	{
		att->nextAttackCounter++;
	}
	if (isKeyDown(KEY_MAIN_ATK) || att->nextAttackCounter > 8)
	{
		att->nextAttackFlag = true;
	}
	if (animaGetCount(&player->anim) >= 1)
	{
		if (att->nextAttackFlag)
		{//次の攻撃へ
			player1ChangeState(player, MAIN_ATTACK3);
		}
		else
		{//アイドル状態へ　この時、空中にいればジャンプ状態になる
			if (isCharaStandingOnBlock((void*)player))
			{
				player1ChangeState(player, IDLE);
			}
			else
			{
				player1ChangeState(player, JUMP);
			}
			return -1;
		}
		player1PosUpdate(player, velocity.x, velocity.y);
		return 0;
	}
	player1PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
int player1MainAttack3(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	const pBox rect = (pBox)getBoxData(HIT_PLAYER1_MAIN3_A);
	XYd velocity = att->velocity;
	int x = (int)(player->pos.x) + player->hit.relative.x;
	int y = (int)(player->pos.y) + player->hit.relative.y + player->hit.size.y / 2;
	switch (player->dir)
	{
	case RIGHT:
		x += player->hit.size.x;
		x -= rect->relative.x;
		break;
	case LEFT:
		x -= (rect->relative.x + rect->size.x);
		break;
	default:
		break;
	}
	attackerOffset(att->attackerMainAtk1, x, y);

	//敵との当たり判定
	if (animaGetFrame(&player->anim) > (player->anim.limit * 0.5))
	{
		pCharacter hitting = enemies - 1;
		bool hitFlag = false;
		do
		{
			hitting = checkAttackerHitCharacters(att->attackerMainAtk1, hitting + 1, ENEMY_NUM);
			if (hitting == nullptr)
			{
				break;
			}
			else
			{
				double kbx = 1, kby = -1;
				if (player->dir == LEFT)
				{
					kbx = -kbx;
				}
				enemyBlow(hitting, kbx, kby);
				enemyHitDamage(hitting, att->attack);
				hitFlag = true;
			}
		} while (1);
		if (hitFlag)
		{
			clearAttacker(att->attackerMainAtk1);
		}
	}

	if (isKey(KEY_MAIN_ATK))
	{
		att->nextAttackCounter++;
	}
	if (isKeyDown(KEY_MAIN_ATK) || att->nextAttackCounter > 8)
	{
		att->nextAttackFlag = true;
	}
	if (animaGetCount(&player->anim) >= 1)
	{
		if (isCharaStandingOnBlock((void*)player))
		{
			player1ChangeState(player, IDLE);
		}
		else
		{
			player1ChangeState(player, JUMP);
		}
		return -1;
	}
	player1PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
int player1SubAttackExec(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	XYd velocity = att->velocity;
	//最後の一コマで止まる処理
	Anima *anim = &player->anim;
	unsigned int frame = animaGetFrame(anim);
	if (frame >= anim->limit)
	{
		anim->started = anim->time - anim->duration + 1;
		//サブ攻撃を出す
		if (!att->alreadySubAttack)
		{
			att->alreadySubAttack = true;
			setAttackerSubAtk(player);
		}
		//止まってる間に判定処理
		//サブ攻撃失敗
		if (isPlayer1SubAtkMiss(att->subAtkIndex))
		{
			player1ChangeState(player, SUB_ATTACK_MISS);
			return -1;
		}
		//サブ攻撃成功
		if (isPlayer1SubAtkSuccess(att->subAtkIndex))
		{
			player1ChangeState(player, SUB_ATTACK_SUCCESS);
			return -1;
		}
	}


	//位置更新
	player1PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
int player1SubAttackSuccess(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	XYd velocity = att->velocity;
	//最初の一コマで止まる処理
	Anima *anim = &player->anim;
	(anim->started) = (anim->time);
	//ワイヤーが消えたら状態移行
	if (!isPlayer1SubAtkActive(att->subAtkIndex))
	{
		player1ChangeState(player, SUB_ATTACK_AFTER);
		return -1;
	}
	player1PosUpdate(player, velocity.x, velocity.y);
	return 0;
}//////
int player1SubAttackMiss(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	XYd velocity = att->velocity;
	//最初の一コマで止まる処理
	Anima *anim = &player->anim;
	(anim->started) = (anim->time);
	//ワイヤーが消えたら状態移行
	if (!isPlayer1SubAtkActive(att->subAtkIndex))
	{
		player1ChangeState(player, SUB_ATTACK_AFTER);
		return -1;
	}
	player1PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
int player1SubAttackAfter(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	XYd velocity = att->velocity;
	//アニメーションで次の状態へ
	if (animaGetCount(&player->anim) >= 1)
	{
		player1ChangeState(player, IDLE);
	}
	player1PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
int player1Guard(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	XYd velocity = att->velocity;
	//最後の一コマで止まる処理
	Anima *anim = &player->anim;
	unsigned int frame = animaGetFrame(anim);
	if (frame >= anim->limit * 0.5)
	{
		anim->started = anim->time - anim->duration + 1;
		if (att->alreadyExpandShield == false)
		{
			//盾のエフェクトを出す↓
			double x = player->pos.x + player->hit.relative.x;
			double y = player->pos.y + player->hit.relative.y;
			double x_offset = 0;
			double y_offset = player->hit.size.y / 2;
			if (player->dir == LEFT)
			{
				x_offset = 0;
			}
			else
			{
				x_offset = player->hit.size.x;
			}
			x += x_offset;
			y += y_offset;
			att->shieldHandle = newEffectShieldSet(x, y);
			att->alreadyExpandShield = true;
			//盾のエフェクトを出す↑
		}

	}
	//ボタンが離されたら状態遷移↓
	if (!isKey(KEY_GUARD))
	{
		player1ChangeState(player, IDLE);
		return -1;
	}
	//ボタンが離されたら状態遷移↑
	//ガードの変数によって混乱↓
	if (att->inGuardTotalHitCount >= GUARD_SHIELD_ENDURANCE)
	{
		player1ChangeState(player, CONFUSION);
		return -1;
	}
	//ガードの変数によって混乱↑
	player1PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
int player1Avoid(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	XYd velocity = att->velocity;
	double v = att->avoidPosX.current;
	ParameterUpdate(&att->avoidPosX,
		player->anim.time - player->anim.started,
		player->anim.duration);
	v = att->avoidPosX.current - v;

	if (animaGetCount(&player->anim) >= 1)
	{

		player1ChangeState(player, IDLE);
		return -1;
	}


	player1PosUpdate(player, velocity.x + v, velocity.y);
	return 0;
}
int player1DamageStop(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	XYd velocity = att->velocity;
	if (att->stopCount >= att->stopLimit)
	{
		if (att->hp > 0)
		{
			player1ChangeState(player, DAMAGE);
		}
		else
		{
			player1ChangeState(player, KNOCK_OUT);
		}
		return -1;
	}
	int w = 16;
	int w2 = w / 2;
	player->draw.quake.x = (rand() % w) - w2;
	player->draw.quake.y = (rand() % w) - w2;
	//player1PosUpdate(player, rand() % 16 - 8, rand() % 16 - 8);
	player1PosUpdate(player, velocity.x, velocity.y);
	att->stopCount++;
	return 0;
}
int player1Damage(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	XYd velocity = att->velocity;
	double kbX = att->knockBackX;
	double kbY = att->knockBackY;

	kbX *= 0.96;
	kbY *= 0.96;


	player1PosUpdate(player, kbX, kbY);

	if (player1PosUpdate(player, velocity.x, velocity.y))
	{
		player1ChangeState(player, IDLE);
		return -1;
	}

	att->knockBackX = kbX;
	att->knockBackY = kbY;
	return 0;
}
int player1KnockOut(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	XYd velocity = att->velocity;
	//アニメーション終了で状態チェンジ↓
	if (animaGetCount(&player->anim) >= 1)
	{
		player1ChangeState(player, DEAD);
		return -1;
	}
	//アニメーション終了で状態チェンジ↑
	player1PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
int player1Dead(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	XYd velocity = att->velocity;

	player1PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
int player1Resurrection(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	XYd velocity = att->velocity;
	//特定のコマの時、2Pと重なっていれば復活させる↓
	{
		Anima *anim = &(player->anim);
		const int animFrame = animaGetFrame(anim);
		constexpr int resurrectFrame = 8;
		if (animFrame >= resurrectFrame)
		{
			if (att->alreadyResurrect == false)
			{
				att->alreadyResurrect = true;
				//2Pと衝突チェック
				if (charaHit(player, player2))
				{
//復活
					bridgePlayer2Revive();
				}
			}
		}
	}
	//特定のコマの時、2Pと重なっていれば復活させる↑
	//アニメーション終了で状態チェンジ↓
	if (animaGetCount(&player->anim) >= 1)
	{
		player1ChangeState(player, IDLE);
		return -1;
	}
	//アニメーション終了で状態チェンジ↑
	player1PosUpdate(player, velocity.x, velocity.y);
	return 0;
}
//混乱状態
int player1Confusion(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
	XYd velocity = att->velocity;
	//アニメーション終了で状態チェンジ↓
	if (animaGetCount(&player->anim) >= 1)
	{
		player1ChangeState(player, IDLE);
		return -1;
	}
	//アニメーション終了で状態チェンジ↑
	player1PosUpdate(player, velocity.x, velocity.y);
	//エフェクトを出す(?)
	return 0;
}
int player1PosUpdate(pCharacter player, double vx, double vy)
{
	int result = 0;
	Player1Att *att = &player->att.pl1;
	double x, y;
	double gravity = att->gravity;
	getCharaOffSet(player, &x, &y);

	vy += gravity;
	vx *= 0.65;
	vy *= 0.97;

	x += vx;
	y += vy;

	//世界の果てに行かせない↓
	if (x < 0 - player->hit.relative.x)
	{
		x = 0 - player->hit.relative.x;
	}
	if (x > (WORLD_WIDTH - player->hit.relative.x - player->hit.size.x))
	{
		x = WORLD_WIDTH - player->hit.relative.x - player->hit.size.x;
	}
	//世界の果てに行かせない↑

	charaOffSet(player, x, y);
	if (isKeyDown(KEY_DOWN))
	{
		player->beforePos.y++;
		player->pos.y++;
	}

	int hitInfo = charaBlockingByBlock((void*)player);
	if (charaLanding(hitInfo))
	{
//着地
		vy = 0;
		result = -1;
	}
	if (hitInfo & HIT_DIR_DOWN)
	{
//頭ごっつんこ
		vy = 0;
	}

	att->velocity = { vx,vy };
	return result;
}
int setAvoidPos(pCharacter player)
{
	Player1Att *att = &player->att.pl1;
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
bool isPlayer1StateDead(pCharacter player)
{
	Player1State state = (Player1State)player->att.pl1.state;
	if (state == KNOCK_OUT)
	{
		return true;
	}
	if (state == DEAD)
	{
		return true;
	}
	return false;
}

static bool isKey(Keys keyType)
{
	return isFreshPad1(keyConfig[keyType]);
}

//static bool isKeyUp(Keys keyType){
//    return isKeyUp_1p(keyConfig[keyType]);
//}

static bool isKeyDown(Keys keyType)
{
	return isDownPad1(keyConfig[keyType]);
}
