#pragma once

#include "chara__common.h"

typedef enum{
	PLAYER1_NUM						= 1		,//1P
	PLAYER1_SUB_NUM					= 1		,//1Pのサブウェポン
	PLAYER2_NUM						= 1		,//2P
	PLAYER2_MAIN_NUM				= 3		,//2Pのメインウェポン
	PLAYER2_SUB_NUM					= 32	,//2Pのサブウェポン
	BLOCK_NUM						= 32	,//地形用ブロック
	CASTLE_NUM						= 4		,//お城
	HP_HEAL_ITEM_NUM				= 8		,//hp回復アイテム
	ATTACKER_NUM					= 256	,//攻撃用矩形
	WYVERN_FIRE_NUM					= 64	,//ワイバーンの吐く火
	ENEMY_NUM						= 64	,//敵
	THUNDER_NUM						= 1600	,//雷
	IN_GAME_EFFECT_NUM				= 65536	,//エフェクト
	IN_GAME_SIMPLE_EFFECT_NUM		= 256	,//簡易エフェクト
	FOG_GENERATOR_NUM				= 1		,//霧
	//デバッグ用↓
	TEST_NUM						= 1,		//デバッグ用マウス位置表示マン
}CHARACTER_NUM;

//CHARACTER型の配列を用意


extern pCharacter player1;
extern pCharacter player1Sub;
extern pCharacter player2;
extern pCharacter player2Bullet;
extern pCharacter player2Sub;
extern pCharacter block;
extern pCharacter castle;
extern pCharacter hpHealItem;
extern pCharacter attackers;
extern pCharacter enemies;
extern pCharacter enemyHoles;
extern pCharacter thunder;
extern pCharacter wyvernFires;
extern pCharacter backEffects;
extern pCharacter simpleEffects;
extern pCharacter frontEffects;
extern pCharacter fogGenerator;


int loadCharacterDataFromFile();

int initCharacters();

int preCharacters();

int CharactersUpdate();

int drawCharacters();

int endCharacters();
