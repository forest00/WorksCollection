#pragma once

#include "chara__common.h"

typedef enum{
	PLAYER1_NUM						= 1		,//1P
	PLAYER1_SUB_NUM					= 1		,//1P�̃T�u�E�F�|��
	PLAYER2_NUM						= 1		,//2P
	PLAYER2_MAIN_NUM				= 3		,//2P�̃��C���E�F�|��
	PLAYER2_SUB_NUM					= 32	,//2P�̃T�u�E�F�|��
	BLOCK_NUM						= 32	,//�n�`�p�u���b�N
	CASTLE_NUM						= 4		,//����
	HP_HEAL_ITEM_NUM				= 8		,//hp�񕜃A�C�e��
	ATTACKER_NUM					= 256	,//�U���p��`
	WYVERN_FIRE_NUM					= 64	,//���C�o�[���̓f����
	ENEMY_NUM						= 64	,//�G
	THUNDER_NUM						= 1600	,//��
	IN_GAME_EFFECT_NUM				= 65536	,//�G�t�F�N�g
	IN_GAME_SIMPLE_EFFECT_NUM		= 256	,//�ȈՃG�t�F�N�g
	FOG_GENERATOR_NUM				= 1		,//��
	//�f�o�b�O�p��
	TEST_NUM						= 1,		//�f�o�b�O�p�}�E�X�ʒu�\���}��
}CHARACTER_NUM;

//CHARACTER�^�̔z���p��


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
