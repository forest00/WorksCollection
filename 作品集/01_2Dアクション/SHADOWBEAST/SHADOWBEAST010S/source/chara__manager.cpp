#pragma once


#include "chara__manager.h"
#include "game_state.h"
#include "chara_player1.h"
#include "chara_player2.h"
#include "chara_item.h"
#include "chara_block.h"
#include "chara_castle.h"
#include "chara_enemy.h"
#include "chara_enemy_death.h"
#include "chara_point.h"
#include "stage_data_reader.h"
#include "stage_data_reader_assistant.h"
#include "tree_sign.h"
#include "image_manager.h"
#include "camera.h"
#include "amgame.h"
#include "ui_mgr.h"
#include "stage_time.h"
#include "new_effect_mgr.h"
#include <stdio.h>
#include <Windows.h>

#include "forDebug.h"

//extern���ꂽ�҂�����
pCharacter player1 = nullptr;
pCharacter player1Sub = nullptr;
pCharacter player2 = nullptr;
pCharacter player2Bullet = nullptr;
pCharacter player2Sub = nullptr;
pCharacter block = nullptr;
pCharacter castle = nullptr;
pCharacter hpHealItem = nullptr;
pCharacter attackers = nullptr;
pCharacter enemies = nullptr;
pCharacter enemyHoles = nullptr;
pCharacter thunder = nullptr;
pCharacter wyvernFires = nullptr;
pCharacter backEffects = nullptr;
pCharacter simpleEffects = nullptr;
pCharacter frontEffects = nullptr;
pCharacter fogGenerator = nullptr;
//extern���ꂽ�҂�����

#ifdef _DBGOPT_KEYBOARD_
pCharacter tes = nullptr;
#endif



typedef struct{
	pCharacter *_ptr;
	int num;
}_Character;



_Character allCharacters[] = {
	{&castle		, CASTLE_NUM},
	{&hpHealItem	, HP_HEAL_ITEM_NUM},
	{&enemyHoles	, ENEMY_NUM},
	{&block			, BLOCK_NUM},
	{&backEffects	, IN_GAME_EFFECT_NUM},
	{&thunder		, THUNDER_NUM},
	{&player1		, PLAYER1_NUM},
	{&player1Sub	, PLAYER1_SUB_NUM},
	{&player2Bullet	, PLAYER2_MAIN_NUM},
	{&player2		, PLAYER2_NUM},
	{&player2Sub	, PLAYER2_SUB_NUM},
	{&enemies		, ENEMY_NUM},
	{&wyvernFires	, WYVERN_FIRE_NUM},
	{&attackers		, ATTACKER_NUM},
	{&simpleEffects	, IN_GAME_SIMPLE_EFFECT_NUM},
	{&frontEffects	, IN_GAME_EFFECT_NUM},
	{&fogGenerator	, FOG_GENERATOR_NUM},

#ifdef _DBGOPT_KEYBOARD_
	{&tes,1},
#endif
	{nullptr,0}
};





//�N�����Ɉ�񂾂����s�����
int loadCharacterDataFromFile(){
	if(loadPlayer1DataFromFile()){
		return -1;
	}
	if(loadPlayer2DataFromFile()){
		return -2;
	}
	if(loadEnemyDataFromFile()){
		return -3;
	}
	if(loadItemDataFromFile()){
		return -4;
	}
	return 0;
}
int initCharacters(){

	InitMouse(0);//

	for(int i = 0; allCharacters[i]._ptr != nullptr; i++){
		*allCharacters[i]._ptr = (Character*)malloc(
			sizeof(Character) * allCharacters[i].num
		);
		if(*allCharacters[i]._ptr == nullptr){
			exit(0);
		}
	}

	enemyDeadEffectInit();

	pointInit();

	return 0;
}

//��ԑJ�ڎ��Ɉ�񂾂����s�����
int preCharacters(){
	for(int i = 0; allCharacters[i]._ptr != nullptr; i++){
		charaBulkInit(*allCharacters[i]._ptr,
			allCharacters[i].num);
	}

#ifdef _DBGOPT_KEYBOARD_
	//�f�o�b�O�p�}�E�X����
	charaSet(tes, 1);
	charaSetDispSize(tes, 64, 64);
	charaColSet(tes, 0, 0, 64, 64);
	charaSetImageHandle(tes, imageMgrGetHandle(IMAGE_ENEMY_HOLE, 0));
	charaColor(tes, AMDRAW_ALPHABLEND, 255, 255, 255, 255);
#endif

	//���݃X�e�[�W���擾
	int stage = getSelectedStage();


	//�t�@�C���ǂݍ��ݗp�ɏ�����������
	initTreeSign();
	initStageDataReaderAssistant();
	initStageData();

	//�ʏ�X�e�[�W��
	if(stage >= 0){
		//�t�@�C������̓ǂݍ���
		//�u���b�N�����̒��ŏo�Ă���
		loadFromFileStageData(stage + 1);
	} else{
		//�`���[�g���A��
		//���낢��ݒ肷��
		initPosPlayer1 = {300,400};
		initPosPlayer2 = {500,400};
		WORLD_WIDTH = 2560;
		WORLD_HEIGHT = 720;
		WORLD_CENTER_X = WORLD_WIDTH / 2;
		WORLD_CENTER_Y = WORLD_HEIGHT / 2;
		GROUND_Y = 640;
		initPosCastle = {WORLD_CENTER_X,GROUND_Y};
		setBlock(0, GROUND_Y, WORLD_WIDTH, WORLD_HEIGHT - GROUND_Y);
	}

	//�v���C���[���o��
	{
		pXYi  pos = nullptr;
		//1p
		pos = &initPosPlayer1;
		setPlayer1(pos->x, pos->y);
		//2p
		pos = &initPosPlayer2;
		setPlayer2(pos->x, pos->y);
	}
	//������o��
	setProtectTarget(initPosCastle.x, initPosCastle.y);
	//�J�����̏�����
	initCamera();
	//ui
	uiMgrInit();
	uiMgrTeachBossStage(stage == 2 || stage == 5);
	
	//2018/10/04�ǉ��G�t�F�N�g������
	newEffectInitAll();

	return 0;
}

//�X�V
int CharactersUpdate(){

	for(int i = 0; allCharacters[i]._ptr != nullptr; i++){
		charaBulkUpdate(*allCharacters[i]._ptr,
			allCharacters[i].num);
	}


#ifdef _DBGOPT_KEYBOARD_
	{
		int mx = 0, my = 0;
		GetMouseLoc(&mx, &my);
		static int count = 0;
		double x = (double)mx;
		double y = (double)my;
		correctByCameraR(&x, &y);
		int uux = (int)(x + tes->draw.relative.x - tes->draw.size.x / 2);
		int uuy = (int)(y + tes->draw.relative.y - tes->draw.size.y / 2);
		charaOffSet(tes, uux, uuy);
		if(ForDebugFreshKey('1') || CheckKey(AMINPUT_MULT) & MOUSEKEY_LEFT){//
			count++;
			if((count == 1) || (((count) % 12) == 0)){
				setEnemy(ENEMY_GOBLIN, x, y);
			}
		} else if(ForDebugFreshKey('2') || CheckKey(AMINPUT_MULT) & MOUSEKEY_MIDDLE){
			count++;
			if((count == 1) || (((count) % 12) == 0)){
				setEnemy(ENEMY_WYVERN, x, y);
			}
		} else if(ForDebugFreshKey('3') || CheckKey(AMINPUT_MULT) & MOUSEKEY_RIGHT){
			count++;
			if((count == 1) || (((count) % 12) == 0)){
				setEnemy(ENEMY_SNAKE, x, y);
			}
		} else if(ForDebugFreshKey('4')){
			count++;
			if((count == 1) || (((count) % 12) == 0)){
				setEnemy(BOSS_TITAN, x, y);
			}
		} else if(ForDebugFreshKey('5')){
			count++;
			if((count == 1) || (((count) % 12) == 0)){
				setEnemy(BOSS_SEASNAKE, x, y);
			}
		} else{
			count = 0;
		}
		if(ForDebugDownKey('R')){
			loadCharacterDataFromFile();
		}
		if(ForDebugDownKey('F')){
			enemyDeadEffectSet(ENEMY_GOBLIN, x, y, LEFT);
		}
		if(ForDebugDownKey('V')){
			enemyDeadEffectSet(ENEMY_WYVERN, x, y, LEFT);
		}
		if(ForDebugDownKey('T')){
			enemyDeadEffectSet(ENEMY_SNAKE, x, y, LEFT);
		}
		if(ForDebugDownKey('G')){
			enemyDeadEffectSet(BOSS_TITAN, x, y, LEFT);
		}
		if(ForDebugDownKey('Y')){
			enemyDeadEffectSet(BOSS_SEASNAKE, x, y, LEFT);
		}
	}
#endif

	//�t�@�C������ǂݍ��񂾂��̂𔽉f
	updateTreeSign();
	updateStageDataReaderAssistant();
	reflectStageData();

	//�G���S���G�t�F�N�g�X�V
	enemyDeadEffectUpdate();

	//�J�����𓮂���
	moveCamera();

	//ui
	uiMgrUpdate();

	//2018/10/04�ǉ��G�t�F�N�g�X�V
	newEffectUpdateAll();

	return 0;
}

//�`��
int drawCharacters(){



	for(int i = 0; allCharacters[i]._ptr != nullptr; i++){
		//�l�[���v���[�g�`�恫
		if(i == 5){
			SetDrawMode(AMDRAW_ALPHABLEND, 200);
			SetDrawBright(1000, 1000, 1000);
			double x = 0, y = 0;
			x = player1->pos.x;
			y = player1->pos.y;
			x += player1->hit.relative.x;
			y += player1->hit.relative.y;
			x += player1->hit.size.x / 2;
			correctByCamera(&x, &y);
			x -= 80;
			y -= 110;
			DrawMemTh((int)(x), (int)(y), imageMgrGetHandle(IMAGE_NAME_PRATE_1P, 0));
			x = player2->pos.x;
			y = player2->pos.y;
			x += player2->hit.relative.x;
			y += player2->hit.relative.y;
			x += player2->hit.size.x / 2;
			correctByCamera(&x, &y);
			x -= 80;
			y -= 110;
			DrawMemTh((int)(x), (int)(y), imageMgrGetHandle(IMAGE_NAME_PRATE_2P, 0));
			SetDrawMode(AMDRAW_NOBLEND, 0);
		}
		//�l�[���v���[�g�`�恪
		//�Ŕ`�恫
		if(i == 5){
			drawTreeSign();
		}
		//�Ŕ`�恪
		charaBulkDraw(*allCharacters[i]._ptr,
			allCharacters[i].num);
	}

	//�G���S���G�t�F�N�g�`��
	enemyDeadEffectDraw();
	//���hp��������
	{
		int hp = 0;
		int maxHp = 0;
		getCastleHp(&hp, &maxHp);
		uiMgrTeachCastleHp(hp, maxHp);
	}
	//ui
	uiMgrDraw();
	//�`���[�g���A���摜�̕`��
	drawStageDataReaderAssistant();

	//2018/10/04�ǉ��G�t�F�N�g�`��
	newEffectDrawAll();

	return 0;
}

//�I�����Ɉ�񂾂��Ă΂��
int endCharacters(){

	endTreeSign();

	for(int i = 0; allCharacters[i]._ptr != nullptr; i++){
		free(*allCharacters[i]._ptr);
		*allCharacters[i]._ptr = nullptr;
		allCharacters[i]._ptr = nullptr;
	}

	enemyDeadEffectEnd();

	uiMgrEnd();

	newEffectEndAll();

	return 0;
}


