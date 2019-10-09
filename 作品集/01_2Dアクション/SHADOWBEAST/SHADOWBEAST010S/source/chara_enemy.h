#pragma once

#include "chara__common.h"


#define ENEMY_DEATH_DELAY		16		//雑魚が死んでから消えるまでの遅延時間
#define BOSS_DEATH_DELAY		48		//ボスが死んでから消えるまでの遅延時間

typedef enum{
	ENEMY_STATE_NONE,
	ENEMY_STATE_INIT,
	ENEMY_STATE_WAIT,
	ENEMY_STATE_WALK,
	ENEMY_STATE_SEARCH,
	ENEMY_STATE_ATTACK,
	ENEMY_STATE_ATTACK_AFTER,
	ENEMY_STATE_BOSS_ATTACK1_BEFORE,
	ENEMY_STATE_BOSS_ATTACK1_MAIN,
	ENEMY_STATE_BOSS_ATTACK1_AFTER,
	ENEMY_STATE_BOSS_ATTACK2_BEFORE,
	ENEMY_STATE_BOSS_ATTACK2_MAIN,
	ENEMY_STATE_BOSS_ATTACK2_AFTER,
	ENEMY_STATE_BOSS_ATTACK3_BEFORE,
	ENEMY_STATE_BOSS_ATTACK3_MAIN,
	ENEMY_STATE_BOSS_ATTACK3_AFTER,
	ENEMY_STATE_DAMAGE_STOP,
	ENEMY_STATE_DAMAGE,
	ENEMY_STATE_DEATH,
	ENEMY_STATE_SHOULD_CLEAR,
}EnemyState;

extern GoblinAtt goblinAttBase;
extern WyvernAtt wyvernAttBase;
extern SnakeAtt snakeAttBase;
extern TitanAtt titanAttBase;
extern SeaSnakeAtt seaSnakeAttBase;

int loadEnemyDataFromFile();

//(x,y)の位置に敵を出す
int setEnemy(EnemyType enemyType, double x, double y);

//敵の数を数える
int checkActiveEnemy();

//敵にダメージを与える
int enemyHitDamage(pCharacter enemy, int damage);

//敵を吹っ飛ばす
int enemyBlow(pCharacter enemy, double vx, double vy);

//1pの情報を得る・操作する

int player1BlowEnemyBridge(double vx, double vy);

int player1HitDamageEnemyBridge(int damage);

int getPlayer1XEnemyBridge();

bool isPlayer1DeadEnemyBridge();

//2pの情報を得る・操作する

int player2BlowEnemyBridge(double vx, double vy);

int player2HitDamageEnemyBridge(int damage);

int getPlayer2XEnemyBridge();

bool isPlayer2DeadEnemyBridge();

//タワーの情報を得る・操作する

int getTower1XEnemyBridge();

int towerHitDamageEnemyBridge(pCharacter target, int damage);
