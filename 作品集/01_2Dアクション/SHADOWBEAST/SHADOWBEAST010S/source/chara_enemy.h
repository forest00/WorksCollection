#pragma once

#include "chara__common.h"


#define ENEMY_DEATH_DELAY		16		//�G��������ł��������܂ł̒x������
#define BOSS_DEATH_DELAY		48		//�{�X������ł��������܂ł̒x������

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

//(x,y)�̈ʒu�ɓG���o��
int setEnemy(EnemyType enemyType, double x, double y);

//�G�̐��𐔂���
int checkActiveEnemy();

//�G�Ƀ_���[�W��^����
int enemyHitDamage(pCharacter enemy, int damage);

//�G�𐁂���΂�
int enemyBlow(pCharacter enemy, double vx, double vy);

//1p�̏��𓾂�E���삷��

int player1BlowEnemyBridge(double vx, double vy);

int player1HitDamageEnemyBridge(int damage);

int getPlayer1XEnemyBridge();

bool isPlayer1DeadEnemyBridge();

//2p�̏��𓾂�E���삷��

int player2BlowEnemyBridge(double vx, double vy);

int player2HitDamageEnemyBridge(int damage);

int getPlayer2XEnemyBridge();

bool isPlayer2DeadEnemyBridge();

//�^���[�̏��𓾂�E���삷��

int getTower1XEnemyBridge();

int towerHitDamageEnemyBridge(pCharacter target, int damage);
