
//todo:�G�l�~�[�z�[���A�j���[�V����

#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include "amgame.h"

#include "game_state.h"
#include "chara_enemy.h"
#include "chara__common.h"

#include "image_manager.h"
#include "chara_player1.h"
#include "chara_player2.h"
#include "chara_castle.h"
#include "chara_attacker.h"
#include "chara__hit_rect.h"
#include "chara_enemy_goblin.h"
#include "chara_enemy_wyvern.h"
#include "chara_enemy_snake.h"
#include "chara_boss_stage1.h"
#include "chara_boss_stage2.h"
#include "chara__manager.h"

#include "common.h"
#include "camera.h"

#include <cassert>
#include "forDebug.h"


//�t�@�C���ǂݍ��ݗp
#define FILE_NAME_MAX	FILENAME_MAX
#define FILE_LINE_MAX	99
#define FILE_NAME_GOBLIN_DATA	"_data\\enemy_data\\_goblin_data.txt"
#define FILE_NAME_WYVERN_DATA	"_data\\enemy_data\\_wyvern_data.txt"
#define FILE_NAME_SNAKE_DATA	"_data\\enemy_data\\_snake_data.txt"
#define FILE_NAME_BOSS1_DATA	"_data\\enemy_data\\_boss1_data.txt"
#define FILE_NAME_BOSS2_DATA	"_data\\enemy_data\\_boss2_data.txt"
//�t�@�C���ǂݍ��ݗp�����܂�
//extern���ꂽ�z��
GoblinAtt goblinAttBase = {
	ENEMY_GOBLIN,	//���
	0
};
WyvernAtt wyvernAttBase = {
	ENEMY_WYVERN,	//���
	0
};
SnakeAtt snakeAttBase = {
	ENEMY_SNAKE,	//���
	0
};
TitanAtt titanAttBase = {
	BOSS_TITAN,		//���
	0
};
SeaSnakeAtt seaSnakeAttBase = {
	BOSS_SEASNAKE,	//���
	0
};
//extern�����܂�




extern int loadEnemyDataFromFile();


static int setEnemyHole(EnemyType enemy, double x, double y);
static int enemyHoleUpdate(pCharacter hole);



//(x,y)�̈ʒu�ɓG���o��
int setEnemy(EnemyType enemyType, double x, double y){
	if(enemyType == ENEMY_NONE){
		return -1;
	}
	if(enemyType >= BOSS_TITAN){
		switch(enemyType){
			case BOSS_TITAN:
				boss1Set(x, y);
				break;
			case BOSS_SEASNAKE:
				boss2Set(x, y);
				break;
			default:
				break;
		}
		return 1;
	}
	setEnemyHole(enemyType, x, y);
	return 0;
}

int checkActiveEnemy(){
	int count = 0;
	//�G�̐�
	for(int i = 0; i < ENEMY_NUM; i++){
		if(enemies[i].isActive){
			count++;
		}
	}
	//�G�o���z�[���̐�
	for(int i = 0; i < ENEMY_NUM; i++){
		if(enemyHoles[i].isActive){
			count++;
		}
	}
	//��̍��v��Ԃ�
	return count;
}

//�G�𐁂���΂�
int enemyBlow(pCharacter enemy, double vx, double vy){
	switch(enemy->att.gob.enemyType){
		case ENEMY_NONE:
			break;
		case ENEMY_GOBLIN:
			goblinBlow(enemy, vx, vy);
			break;
		case ENEMY_WYVERN:
			wyvernBlow(enemy, vx, vy / 2);
			break;
		case ENEMY_SNAKE:
			snakeBlow(enemy, vx, vy);
			break;
		case BOSS_TITAN:
			boss1Blow(enemy, vx, vy);
			enemy->att.titan.quakeCount = 8;
			break;
		case BOSS_SEASNAKE:
			boss2Blow(enemy, vx, vy);
			enemy->att.seaSnk.quakeCount = 8;
			break;
		default:
			break;
	}
	return 0;
}

//�G�Ƀ_���[�W��^����
int enemyHitDamage(pCharacter enemy, int damage){
	int kill = false;
	switch(enemy->att.gob.enemyType){
		case ENEMY_NONE:
			break;
		case ENEMY_GOBLIN:
			kill = goblinHitDamage(enemy, damage);
			break;
		case ENEMY_WYVERN:
			kill = wyvernHitDamage(enemy, damage);
			break;
		case ENEMY_SNAKE:
			kill = snakeHitDamage(enemy, damage);
			break;
		case BOSS_TITAN:
			kill = boss1HitDamage(enemy, damage);
			enemy->att.titan.quakeCount = 8;
			break;
		case BOSS_SEASNAKE:
			kill = boss2HitDamage(enemy, damage);
			enemy->att.seaSnk.quakeCount = 8;
			break;
		default:
			break;
	}
	if(kill){
		countUpEnemyKill();
	}
	return 0;
}

/*
�G�o���z�[��
*/
static int setEnemyHole(EnemyType enemy, double x, double y){
	int found = charaFindNonActive(enemyHoles, ENEMY_NUM);
	if(found < 0){
		return -1;
	}
	pCharacter hole = &enemyHoles[found];
	EnemyHoleAtt *att = &hole->att.hole;
	ImageFileId fileId = IMAGE_ENEMY_HOLE;
	const int hHole = imageMgrGetHandle(fileId, 0);
	charaInit(hole);
	hole->isActive = true;
	charaSetDispSize(hole, fileId);
	int xSize = hole->draw.size.x;
	int ySize = hole->draw.size.y;
	att->enemy = enemy;
	charaOffSet(hole, x - xSize / 2, y - ySize / 2);
	charaColSet(hole, 0, 0, xSize, ySize);
	charaSetImageHandle(hole, hHole);
	charaHome(hole, xSize / 2, ySize / 2);
	{
		AnimaData animData = {0};
		createAnimaData(&animData, fileId);
		animaSetup(&(hole->anim), getGameFrame(), &animData);
		att->currentTime = 0;
		att->lifeTime = animData.duration / 2;
		att->enemy = enemy;
		att->toOpen = true;
	}
	hole->imageFileId = fileId;
	charaFuncSet(hole, enemyHoleUpdate);
	enemyHoleUpdate(hole);
	return 0;
}

static int enemyHoleUpdate(pCharacter target){
	EnemyHoleAtt *att = &target->att.hole;
	Anima *anim = &target->anim;
	anim->time = getGameFrame();
	{
		int index = animaGetValueStep(anim);
		int handle = imageMgrGetHandle(target->imageFileId, index);
		charaSetImageHandle(target, handle);
	}
	if(att->currentTime >= att->lifeTime){
		int holeWidth = target->draw.size.x;
		int holeHeight = target->draw.size.y;
		if(att->toOpen){
			double x, y;
			getCharaOffSet(target, &x, &y);
			int ix = (int)(x + holeWidth / 2);
			int iy = (int)(y + holeHeight / 2);
			switch(att->enemy){
				case ENEMY_GOBLIN:
					goblinSet(ix, iy);
					break;
				case ENEMY_WYVERN:
					wyvernSet(ix, iy);
					break;
				case ENEMY_SNAKE:
					snakeSet(ix, iy);
					break;
				case ENEMY_NONE:
					break;
				default:
					break;
			}
			att->currentTime = 0;
			att->enemy = ENEMY_NONE;
			att->toOpen = false;
		} else{
			target->isActive = false;
		}
	}
	att->currentTime++;
	return 0;
}

//1p�̏��𓾂�E���삷��

int player1BlowEnemyBridge(double vx, double vy){
	return player1Blow(vx, vy);
}

int player1HitDamageEnemyBridge(int damage){
	return player1HitDamage(damage);
}

int getPlayer1XEnemyBridge(){
	return getPlayer1X();
}

bool isPlayer1DeadEnemyBridge(){
	return isPlayer1Dead();
}

//2p�̏��𓾂�E���삷��

int player2BlowEnemyBridge(double vx, double vy){
	return player2Blow(vx, vy);
}

int player2HitDamageEnemyBridge(int damage){
	return player2HitDamage(damage);
}

int getPlayer2XEnemyBridge(){
	return getPlayer2X();
}

bool isPlayer2DeadEnemyBridge(){
	return isPlayer2Dead();
}

//�^���[�̏��𓾂�E���삷��

int getTower1XEnemyBridge(){
	return getCastleX();
}

int towerHitDamageEnemyBridge(pCharacter target, int damage){
	return castleHitDamage(target, damage);
}

//�G�̏����t�@�C������ǂݍ���
int loadEnemyDataFromFile(){
	FILE *in = nullptr;
	char fileName[FILE_NAME_MAX] = "\0";
	char comment[FILE_LINE_MAX] = "\0";
	int opened = -1;
	strncpy_s(fileName, FILE_NAME_GOBLIN_DATA, FILE_NAME_MAX);
	opened = fopen_s(&in, fileName, "r");
	if(opened){
		assert(!opened && "�S�u�����p�̃t�@�C�����J���܂���");
		exit(0);
	} else{
		fscanf_s(in, "%lf", &goblinAttBase.gravity);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%lf", &goblinAttBase.walkSpeed);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &goblinAttBase.hp);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &goblinAttBase.attack);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &goblinAttBase.stopLimit);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &goblinAttBase.stiffnessTimeLimit);
		fclose(in);
	}
	goblinAttBase.resistGun = 1.0;
	goblinAttBase.resistSword = 1.0;
	goblinAttBase.walkSpeed /= 100.0;

	strncpy_s(fileName, FILE_NAME_WYVERN_DATA, FILE_NAME_MAX);
	opened = fopen_s(&in, fileName, "r");
	if(opened){
		assert(!opened && "���C�o�[���p�̃t�@�C�����J���܂���");
		exit(0);
	} else{
		fscanf_s(in, "%lf", &wyvernAttBase.gravity);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%lf", &wyvernAttBase.walkSpeed);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &wyvernAttBase.hp);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &wyvernAttBase.attack);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &wyvernAttBase.stopLimit);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &wyvernAttBase.stiffnessTimeLimit);
		fclose(in);
	}
	wyvernAttBase.resistGun = 1.0;
	wyvernAttBase.resistSword = 1.0;
	wyvernAttBase.walkSpeed /= 100.0;

	strncpy_s(fileName, FILE_NAME_SNAKE_DATA, FILE_NAME_MAX);
	opened = fopen_s(&in, fileName, "r");
	if(opened){
		assert(!opened && "�֗p�̃t�@�C�����J���܂���");
		exit(0);
	} else{
		fscanf_s(in, "%lf", &snakeAttBase.gravity);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%lf", &snakeAttBase.walkSpeed);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &snakeAttBase.hp);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &snakeAttBase.attack);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &snakeAttBase.stopLimit);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &snakeAttBase.stiffnessTimeLimit);
		fclose(in);
	}
	snakeAttBase.resistGun = 1.0;
	snakeAttBase.resistSword = 1.0;
	snakeAttBase.walkSpeed /= 100.0;

	strncpy_s(fileName, FILE_NAME_BOSS1_DATA, FILE_NAME_MAX);
	opened = fopen_s(&in, fileName, "r");
	if(opened){
		assert(!opened && "�{�X�F���l�p�̃t�@�C�����J���܂���");
		exit(0);
	} else{
		fscanf_s(in, "%lf", &titanAttBase.gravity);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%lf", &titanAttBase.walkSpeed);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &titanAttBase.hp);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &titanAttBase.attack);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &titanAttBase.stopLimit);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &titanAttBase.attackWaitLimit);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &titanAttBase.stiffnessTimeLimit1);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &titanAttBase.stiffnessTimeLimit2);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &titanAttBase.stiffnessTimeLimit3);
		fclose(in);
	}
	titanAttBase.resistGun = 1.0;
	titanAttBase.resistSword = 1.0;
	titanAttBase.walkSpeed /= 100.0;

	strncpy_s(fileName, FILE_NAME_BOSS2_DATA, FILE_NAME_MAX);
	opened = fopen_s(&in, fileName, "r");
	if(opened){
		assert(!opened && "�{�X�F�E�~�w�r�p�̃t�@�C�����J���܂���");
		exit(0);
	} else{
		fscanf_s(in, "%lf", &seaSnakeAttBase.garavity);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%lf", &seaSnakeAttBase.walkSpeed);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.hp);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.attackBite);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.attackThunder);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.attackWater);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.damageStopTime);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.bitePreStiffnessTime);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.bitePostStiffnessTime);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.biteAtkStartFrame);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.biteAtkEndFrame);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.biteAnimDuration);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.thunderPreStiffnessTime);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.thunderPostStiffnessTime);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.thunderAtkStartFrame);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.thunderAtkEndFrame);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.thunderAnimDuration);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.waterPreStiffnessTime);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.waterPostStiffnessTime);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.waterAtkStartFrame);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.waterAtkEndFrame);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &seaSnakeAttBase.waterAnimDuration);
		fclose(in);
	}
	seaSnakeAttBase.resistGun = 1.0;
	seaSnakeAttBase.resistSword = 1.0;
	seaSnakeAttBase.walkSpeed /= 100.0;

	return 0;
}