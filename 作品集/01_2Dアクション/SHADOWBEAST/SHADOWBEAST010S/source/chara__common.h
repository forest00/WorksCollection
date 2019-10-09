#pragma once


#include "chara__render.h"
#include "chara__common.h"
#include "chara__hit_rect.h"
#include "anima.h"
#include "type_parameter.h"
#include "common.h"
#include <Windows.h>

//define��


#define HIT_DIR_UP		(1<<0)
#define HIT_DIR_DOWN	(1<<1)
#define HIT_DIR_LEFT	(1<<2)
#define HIT_DIR_RIGHT	(1<<3)

//define��



typedef enum{
	ENEMY_NONE,
	ENEMY_GOBLIN,
	ENEMY_WYVERN,
	ENEMY_SNAKE,
	BOSS_TITAN,
	BOSS_SEASNAKE,
	ENEMY_TYPE_MAX,
}EnemyType;


typedef struct{
	bool locked;
}AttackerAtt;

typedef struct{
	double gravity;					///�d��
	double walkSpeedBase;			///��{�ړ����x
	int maxHp;						///�ő�hp
	double jumpPowBase;				///��{�W�����v��
	int attackBase;					///��{�U����
	double avoidPowBase;			///��{����
	int stopLimit;					///�_���[�W���󂯂Ē�~���鎞��
	int stiffnessTimeLimit;			///�U����d������
	//������艺�̓[���ŏ����������
	XYd velocity;					///���ݑ��x
	int hp;							///����hp
	double walkSpeed;				///�ړ����x
	double jumpPow;					///�W�����v��
	int attack;						///�U����
	double avoidPow;				///����
	int state;						///���݂̏��
	int beforeState;				///�ЂƂO�̏��
	unsigned int stateChanged;		///��Ԃ��؂�ւ�����u��
	bool nextAttackFlag;			///���̍U���Ɉڍs����t���O
	unsigned int nextAttackCounter;	///�t���O��؂�ւ��邽�߂̃J�E���^�[
	Parameter avoidPosX;			///����ʒu
	Direction avoidDir;				///����̕���
	double knockBackX;				///�m�b�N�o�b�N
	double knockBackY;
	XYd stopCoord;					///��~�p�@�ʒu��ۑ����Ƃ�
	int stopCount;					///��~�p�@���Ԃ�ۑ����Ƃ�
	bool invincibleFlag;			///���G�t���O
	int invincibleCount;			///���G�J�E���^�[
	int invincibleLimit;			///���G�J�E���^�[�����~�b�g�𒴂����疳�G����
	int attackerMainAtk1;
	int attackerMainAtk2;
	int attackerMainAtk3;
	int stiffnessCount;				///�U����d���J�E���g
	int subAtkIndex;				///�T�u�A�^�b�N
	bool reading;					///�Ŕǂ�ł�
	bool alreadySubAttack;			///���łɃT�u�A�^�b�N
	bool alreadyResurrect;			///���łɕ���
	bool alreadyExpandShield;		///���łɏ�
	int shieldHandle;				///���̊Ǘ��p�n���h��
	int confusionStarHandle;		///�������Ǘ��p�n���h��
	int inGuardTotalHitCount;		///�K�[�h���Ƀ_���[�W���󂯂���
	int inGuardTotalHitDamage;		///�K�[�h���Ɏ󂯂��_���[�W�̑���
	int inGuardGuardCount;			///�K�[�h����p���G����p�J�E���^
	int quakeCount;					///���̐��̎��A�h���
}Player1Att;

typedef struct{
	bool isMiss;				///�A�j���[�V�����������Đ������Ύ��s�ƂȂ�
	bool isSuccess;				///�G�ɓ�����ΐ����ł���
	int aheadIndex;				///��[�̏��
	XYd aheadPos;				///��[�̍��W
	double aheadSpeed;			///��[�̈ړ����x
	int aheadStopStartFrame;	///��~����R�}
	int aheadStopEndFrame;		///��~��������R�}
	bool aheadInProgress;		///�i�s��
	bool aheadRegression;		///�ލs��
	void *aheadTarget;
}Player1SubAtt;

typedef struct{
	double gravity;					///�d��
	double walkSpeedBase;			///��{�ړ����x
	int maxHp;						///�ő�hp
	double jumpPowBase;				///��{�W�����v��
	int attackBase;					///��{�U����
	double avoidPowBase;			///��{����
	int stopLimit;					///�_���[�W���󂯂Ē�~���鎞��
	int stiffnessTimeLimit;			///�U����d������
	int subAtkCoolTime;				///�T�u�A�^�b�N�̃N�[���^�C��
	//������艺�̓[���ŏ����������
	XYd velocity;					///���ݑ��x
	int hp;							///����hp
	int afterResurrection;			///�������邽�тɃ��Z�b�g�����J�E���^�[
	double walkSpeed;				///�ړ����x
	double jumpPow;					///�W�����v��
	int attack;						///�U����
	double avoidPow;				///����
	int state;						///���݂̏��
	int beforeState;				///�ЂƂO�̏��
	unsigned int stateChanged;		///��Ԃ��؂�ւ�����u��
	Parameter avoidPosX;			///����ʒu
	Direction avoidDir;				///����̕���
	double knockBackX;				///�m�b�N�o�b�N
	double knockBackY;
	XYd stopCoord;					///��~�p�@�ʒu��ۑ����Ƃ�
	int stopCount;					///��~�p�@���Ԃ�ۑ����Ƃ�
	bool invincibleFlag;			///���G�t���O
	int invincibleCount;			///���G�J�E���^�[
	int invincibleLimit;			///���G�J�E���^�[�����~�b�g�𒴂����疳�G����
	int stiffnessCount;				///�U����d���J�E���g
	bool alreadyMainAtk;			///���łɃ��C���A�^�b�N
	bool alreadySubAtk;				///���łɃT�u�A�^�b�N
	bool alreadyRecoveryAtk;		///���łɃ��J�o���A�^�b�N
	int subAtkCoolCounter;			///�T�u�A�^�b�N�̃N�[���^�C���@�J�E���^�[
	bool alreadyResurrect;			///���łɕ���
}Player2Att;

typedef struct{
	int vx, vy;
	int attack;
	int attackerIndex;
}Player2MainAtt;

typedef struct{
	int time;					///���ݎ���
	int delay;					///�x������
	int attack;					///�U����
	XYd velocity;				///���ݑ��x
	double movementAmount;		///�ړ���
}Player2SubAtt;

typedef struct{
	EnemyType enemyType;					///���ʗp(���ׂĂ̓G�Ɏ�������)
	double gravity;					///�d��
	double walkSpeed;				///�ړ����x
	int hp;							///HP
	int attack;
	int stopLimit;					///�_���[�W���󂯂Ē�~���鎞��
	int stiffnessTimeLimit;			///�U����d������
	double resistSword;				///���ϐ�
	double resistGun;				///�e�ϐ�
	//�ȍ~��0�ŏ����������
	int state;						///���݂̏��
	XYd velocity;					///���ݑ��x
	int stopCount;					///��~�p�@���Ԃ�ۑ����Ƃ�
	XYd stopCoord;					///��~�p�@�ʒu��ۑ����Ƃ�
	double knockBackX;				///�m�b�N�o�b�N
	double knockBackY;				///
	int stiffnessCount;				///�U����d���J�E���g
	//�p���`�̕ϐ���
	int atkBeforeFrame;
	int atkRectIndex;
	//�p���`�̕ϐ���
	bool isDead;					///�Q�[�����Ŏ���ł���
	int deadCounter;				///���񂾏u�Ԃ���̌o�ߎ���
}GoblinAtt;

typedef struct{
	EnemyType enemyType;			///���ʗp(���ׂĂ̓G�Ɏ�������)
	double gravity;					///�d��
	double walkSpeed;				///�ړ����x
	int hp;							///HP
	int attack;
	int stopLimit;					///�_���[�W���󂯂Ē�~���鎞��
	int stiffnessTimeLimit;			///�U����d������
	double resistSword;				///���ϐ�
	double resistGun;				///�e�ϐ�
	//�ȍ~��0�ŏ����������
	int state;						///���݂̏��
	XYd velocity;					///���ݑ��x
	XYd stopCoord;					///��~�p�@�ʒu��ۑ����Ƃ�
	int stopCount;					///��~�p�@���Ԃ�ۑ����Ƃ�
	double knockBackX;				///�m�b�N�o�b�N
	double knockBackY;				///�m�b�N�o�b�N
	Direction atkDir;
	int stiffnessCount;				///�U����d���J�E���g
	bool isHittingDamage;			///�_���[�W�󂯒�
	bool isDead;					///�Q�[�����Ŏ���ł���
	int deadCounter;				///���񂾏u�Ԃ���̌o�ߎ���
}WyvernAtt;

typedef struct {
	bool used;			///�g�p���t���O
	int time;			///�o�ߎ���
	int delay;			///�x������
	Direction dir;		///����
	XYd velocity;		///���x
	int attack;			///�U����
	void *parrentPtr;
}WyvernFireAtt;

typedef struct{
	EnemyType enemyType;			///���ʗp(���ׂĂ̓G�Ɏ�������)
	double gravity;					///�d��
	double walkSpeed;				///�ړ����x
	int hp;							///HP
	int attack;
	int stopLimit;					///�_���[�W���󂯂Ē�~���鎞��
	int stiffnessTimeLimit;			///�U����d������
	double resistSword;				///���ϐ�
	double resistGun;				///�e�ϐ�
	//�ȍ~��0�ŏ����������
	int state;						///���݂̏��
	XYd velocity;					///���ݑ��x
	XYd stopCoord;					///��~�p�@�ʒu��ۑ����Ƃ�
	int stopCount;					///��~�p�@���Ԃ�ۑ����Ƃ�
	double knockBackX;				///�m�b�N�o�b�N
	double knockBackY;				///�m�b�N�o�b�N
	int stiffnessCount;				///�U����d���J�E���g
	int atkRectIndex;				///�U������Ǘ��p�F��`�C���f�b�N�X
	int atkBeforeFrame;				///�U������Ǘ��p�F1F�O�̃A�j���[�V�����R�}��
	bool isDead;					///�Q�[�����Ŏ���ł���
	int deadCounter;				///���񂾏u�Ԃ���̌o�ߎ���
}SnakeAtt;

typedef struct{
	int currentTime;	//���ݎ���
	int lifeTime;		//����
	bool toOpen;		//�J����ԃt���O
	EnemyType enemy;	//�G�l�~�[�^�C�v
}EnemyHoleAtt;

typedef struct{
	EnemyType enemyType;			///���ʗp(���ׂĂ̓G�Ɏ�������)
	double gravity;					///�d��
	double walkSpeed;				///�ړ����x
	int hp;							///HP
	int attack;
	int stopLimit;					///�_���[�W���󂯂Ē�~���鎞��
	int attackWaitLimit;			///�U���O�҂�
	int stiffnessTimeLimit1;		///�U����d������(�U�艺�낵)
	int stiffnessTimeLimit2;		///�U����d������(�ガ����)
	int stiffnessTimeLimit3;		///�U����d������(�ːi)
	double resistSword;				///���ϐ�
	double resistGun;				///�e�ϐ�
	//�ȍ~��0�ŏ����������
	int state;						///���݂̏��
	XYd velocity;					///���ݑ��x
	XYd stopCoord;					///��~�p�@�ʒu��ۑ����Ƃ�
	int stopCount;					///��~�p�@���Ԃ�ۑ����Ƃ�
	double knockBackX;				///�m�b�N�o�b�N
	double knockBackY;				///�m�b�N�o�b�N
	int atkControlller;				///�ǂ̍U�������邩�t���O
	int atk1RectIndex;				///�U����`�o���Ǘ��p�ϐ�(�U�艺�낵)
	Parameter atk1X, atk1Y;			///�U���p��`���W�Ǘ��p�ϐ�
	int atk2RectIndexR;				///�U����`�o���Ǘ��p�ϐ�(�ガ������)
	int atk2RectIndexL;				///�U����`�o���Ǘ��p�ϐ�(�ガ�����E)
	int atk3RectIndex;				///�U����`�o���Ǘ��p�ϐ�(�ːi)
	int damegeCount;				///�_���[�W�󂯂���̃J�E���^�[
	int attackWaitCount;			///�U���O�J�E���g
	int stiffnessCount;				///�U����d���J�E���g
	int quakeCount;					///���̐��̎��A�h���
	bool isDead;					///�Q�[�����Ŏ���ł���
	int deadCounter;				///���񂾏u�Ԃ���̌o�ߎ���
}TitanAtt;

typedef struct{
	EnemyType enemyType;			///���ʗp(���ׂĂ̓G�Ɏ�������)
	double garavity;				///�d��
	double walkSpeed;				///�ړ����x
	int hp;							///HP
	int attackBite;					///���݂��̃_���[�W
	int attackThunder;				///���̃_���[�W
	int attackWater;				///���̃_���[�W
	int damageStopTime;				///�_���[�W���󂯂Ē�~���鎞��
	int bitePreStiffnessTime;		///���݂��A�j���[�V�����O��������
	int bitePostStiffnessTime;		///���݂��A�j���[�V������d������
	int biteAtkStartFrame;			///���݂��U���J�n�R�}(�����蔻��J�n)
	int biteAtkEndFrame;			///���݂��U���I���R�}(�����蔻��I��)
	int biteAnimDuration;			///���݂��U���A�j���[�V��������
	int thunderPreStiffnessTime;	///���A�j���[�V�����O��������
	int thunderPostStiffnessTime;	///���A�j���[�V������d������
	int thunderAtkStartFrame;		///���U���J�n�R�}(�����蔻��J�n)
	int thunderAtkEndFrame;			///���U���I���R�}(�����蔻��I��)
	int thunderAnimDuration;		///���U���A�j���[�V��������
	int waterPreStiffnessTime;		///���A�j���[�V�����O��������
	int waterPostStiffnessTime;		///���A�j���[�V������d������
	int waterAtkStartFrame;			///���U���J�n�R�}(�����蔻��J�n)
	int waterAtkEndFrame;			///���U���I���R�}(�����蔻��I��)
	int waterAnimDuration;			///���U���A�j���[�V��������
	double resistSword;				///���ϐ�
	double resistGun;				///�e�ϐ�
	//�ȍ~��0�ŏ����������
	int state;						///���݂̏��
	XYd velocity;					///���ݑ��x
	int damageStopCounter;			///�_���[�W�d���p�J�E���^�[
	XYd stopCoord;					///�_���[�W���󂯂����_�ł̈ʒu
	int stiffnessCounter;			///�d���J�E���^�[
	int biteAtkIndex;				///�U����`�o���Ǘ��p�ϐ�(���݂�)
	double knockBackX;				///�m�b�N�o�b�N
	double knockBackY;				///�m�b�N�o�b�N
	int atkControlller;				///�ǂ̍U�������邩�t���O
	int animStopCounter;			///�A�j����~�J�E���^�[
	bool alreadyThunder;			///���łɗ�
	int thunderOffsetX[3];			///�ǂ̈ʒu�ɗ��𗎂Ƃ���(X���W�I�t�Z�b�g)�@���ӁF�}�W�b�N�i���o�[
	bool alreadyPlaySound;			///���łɉ��Đ�
	int quakeCount;					///���̐��̎��A�h���
	bool isDead;					///�Q�[�����Ŏ���ł���
	int deadCounter;				///���񂾏u�Ԃ���̌o�ߎ���
}SeaSnakeAtt;

typedef struct{
	int duration;			//�p������
	int elapsed;			//�o�ߎ���
	int delay;				//�x��
	int atkStartTime;		//�U���J�n
	int atkEndTime;			//�U���I���
	int attack;				//�U����
}ThunderAtt;

typedef struct{
	XYd velocity;
	int attack;
}WaterAtt;

typedef struct{
	int maxHp;						///�ő�hp
	int hp;							///hp
	bool aliveFlag;						///�����t���O
	bool invincibleFlag;			///���G�t���O
	int invincibleCount;			///���G�J�E���^�[
	int invincibleLimit;			///���G�J�E���^�[�����~�b�g�𒴂����疳�G����
}CastleAtt;

typedef struct{
	XYd velocity;					///���ݑ��x
}ItemAtt;

typedef struct{
	double offsetX;			///���W�I�t�Z�b�gx
	double offsetY;			///���W�I�t�Z�b�gy
	Parameter x, y;			///���W
	Parameter scale;		///�g��{��
	Parameter angle;		///�p�x
	Parameter alpha;		///�����x
	int red, green, blue;	///�ԗΐ���
	int currentTime;		///���ݎ���
	int lifeTime;			///����
	int blend;				///�u�����h���@
	bool shouldAnim;		///�A�j���[�V�������ׂ���
	bool flipFlag;			///���]�t���O
}EasingEffectAtt;

typedef struct{
	XYd velocity;			///���ݑ��x
	Parameter alpha;		///�����x
	int red, green, blue;	///�ԗΐ���
	int currentTime;		///���ݎ���
	int lifeTime;			///����
	int blend;				///�u�����h���@
}FireEffectAtt;

typedef union{
	AttackerAtt atk;
	Player1Att pl1;
	Player1SubAtt pl1Sub;
	Player2Att pl2;
	Player2MainAtt pl2Main;
	Player2SubAtt pl2Sub;
	GoblinAtt gob;
	WyvernAtt wyv;
	WyvernFireAtt wyvFire;
	SnakeAtt snk;
	EnemyHoleAtt hole;
	TitanAtt titan;
	SeaSnakeAtt seaSnk;
	ThunderAtt thunder;
	WaterAtt water;
	CastleAtt cstl;
	EasingEffectAtt easingEfc;
	FireEffectAtt fireEfc;
	ItemAtt item;
	int blkHitDir;
}CharaAtt;




//Character�^�̍\���̂�p��


typedef struct tagCharacter {
	//�A�N�e�B�u�t���O(�A�N�e�B�u��:1)
	bool isActive;
	//�ړ��p
	XYd beforePos;
	XYd pos;
	Direction dir;
	//�����蔻��p
	Box hit;
	//�摜����p
	ImageFileId imageFileId;
	//�A�j���[�V�����p
	Anima anim;
	//�`��p
	DrawInfo draw;
	//���ۂ̓���p
	int(*update)(tagCharacter*);
	//���̑��A����Ȃ����
	CharaAtt att;
}Character, *pCharacter;






//�������p


/*
�L�����P�̂�����������
�����o�[�S�Ăɑ΂��āA������Ԃɂӂ��킵���l��������
@param target ����������Ώۂ̃L����
@return
0...����
1...���s
*/
int  charaInit(pCharacter target);

/*
�L�����̔z�������������
�z��̑S�v�f�ɑ΂��āA������Ԃɂӂ��킵���l��������
@param characters �������������z��̐擪�A�h���X 
@param num �z��̗v�f��
@return
0...����
1...���s
*/
int  charaBulkInit(Character characters[], int num);






//�o�����ŊǗ��p

/*
�L�����̔z��̑S�v�f�̒�����A�A�N�e�B�u�ȗv�f��T��(�擪����)
@param characters �Ώۂ̔z��
@param num �z��̗v�f��
@return �A�N�e�B�u�ȗv�f�̗v�f�ԍ�
*/
int charaFindActive(Character characters[], int num);

/*
�L�����̔z��̑S�v�f�̒�����A�A�N�e�B�u�łȂ��v�f��T��(�擪����)
@param characters �Ώۂ̔z��
@param num �z��̗v�f��
@return �A�N�e�B�u�łȂ��v�f�̗v�f�ԍ�
*/
int charaFindNonActive(Character characters[], int num);

/*
�L�����z��̒�����A�A�N�e�B�u�łȂ��v�f��T���A��������������A�v�f�ԍ���Ԃ�
@param characters �Ώۂ̔z��
@param num �z��̗v�f��
@return
�֐������������ꍇ�A������s�����v�f�̃C���f�b�N�X
���s�����ꍇ�A-1
*/
int charaSet(Character characters[], int num);

/*
�w�肵���L�������A�N�e�B�u�łȂ���
@param target ����Ώۂ̃L����
@return
0...����
1...���s
*/
int charaClear(pCharacter target);

/*
�w�肵���L�����z��̑S�v�f���A�N�e�B�u�łȂ���
@param characters �Ώۂ̔z��
@param num �z��̗v�f��
@return
0...����
1...���s
*/
int charaBulkClear(Character characters[], int num);




//�e����i�[�p

/*
�L�����̕\���p�̃T�C�Y���w�肷��
@param target �Ώۂ̃L����
@param sizeX x�����̃T�C�Y
@param sizeY y�����̃T�C�Y
@return
0...����
1...���s
*/
int charaSetDispSize(pCharacter target, int sizeX, int sizeY);
/*
�L�����̕\���p�̃T�C�Y���w�肷��
@param target �Ώۂ̃L����
@param ImageFileId �摜�f�[�^�x�[�X��̈ʒu
@return
0...����
1...���s
*/
int charaSetDispSize(pCharacter target, ImageFileId);
/*
�L�����̕\���p�̃T�C�Y���w�肷��
@param target �Ώۂ̃L����
@param CharaHitAreaId ��`�����蔻��p�f�[�^�x�[�X��̈ʒu
@return
0...����
1...���s
*/
int charaSetDispSize(pCharacter target, CharaHitAreaId);

/*
�L�����̃��[���h���W���w�肷��
�ύX�O�̍��W��ۑ����Ă���̂ŁA�ړ��Ȃǂ��������ꍇ�͂��̊֐����g������
@param target �Ώۂ̃L����
@param x ���[���h���Wx
@param y ���[���h���Wy
@return
0...����
1...���s
*/
int charaOffSet(pCharacter target, double x, double y);

/*
�L�����̓����蔻��̈���f�[�^�x�[�X����w�肷��
@param target �Ώۂ̃L����
@param id �f�[�^�x�[�X��̈ʒu
@return
0...����
1...���s
*/
int charaColSet(pCharacter target, CharaHitAreaId id);

/*
�L�����̓����蔻��̈�𒼐ڎw�肷��
@param target �Ώۂ̃L����
@param x �����蔻��I�t�Z�b�gx
@param y �����蔻��I�t�Z�b�gy
@param w �����蔻�艡��
@param h �����蔻��c��
@return
0...����
1...���s
*/
int charaColSet(pCharacter target, int x, int y, int w, int h);

/*
�L�����̈ړ��E��]�E�g��k���̌��_���w�肷��
@param target �Ώۂ̃L����
@param cenX ���S�ʒux
@param cenY ���S�ʒuy
@return
0...����
1...���s
*/
int charaHome(pCharacter target, int cenX, int cenY);

/*
�L�������g��E�k��������
@param target �Ώۂ̃L����
@param scaleX �g��{��x
@param scaleY �g��{��y
@return
0...����
1...���s
*/
int charaScale(pCharacter target, double scaleX, double scaleY);

/*
�L��������]������
@param target �Ώۂ̃L����
@param angle ��]�p�x(���W�A��)
@return
0...����
1...���s
*/
int charaRot(pCharacter target, double angle);

/*
�L�����̃u�����h���@����ѕ\���F���w�肷��
@param target �Ώۂ̃L����
@param blend �u�����h���@(amgane�ɂ���`����Ă���)
@param alpha �����x
@param red �Ԑ���(0~255)
@param green �ΐ���(0~255)
@param blue ����(0~255)
@return
0...����
1...���s
*/
int charaColor(pCharacter target, int blend, int alpha,
			   int red, int green, int blue);

/*
�L�����̕\���p�摜�n���h�����w�肷��
@param target �Ώۂ̃L����
@param handle �摜�n���h��
@return
0...����
1...���s
*/
int charaSetImageHandle(pCharacter target, int handle);

/*
�L�����������]�����邩�ǂ����t���O��ݒ�
@param target �Ώۂ̃L����
@param flagX �t���O(true�Ȃ牡���])
@return
0...����
1...���s
*/
int charaFlipX(pCharacter target, bool flagX);

/*
�L�������X�V����֐����w��
@param target �Ώۂ̃L����
@param update �L�����X�V���ɌĂ΂��R�[���o�b�N�֐�
@return
0...����
1...���s
*/
int charaFuncSet(pCharacter target, int(*update)(pCharacter));

//�e����擾�p

bool getCharaExist(const pCharacter target);

int getCharaOffSet(const pCharacter target, double *x, double *y);

int getCharaHitArea(const pCharacter target, double *left, double *top, double *right, double *bottom);

int getCharaHitAreaCenter(const pCharacter target, double *centerX, double *centerY);

bool getCharaFlipFlag(const pCharacter target);







//�����蔻��p

int createBoxForHit(const pCharacter chara, pRect out);
int createBoxForHit(const pCharacter chara, CharaHitAreaId id, pRect out);

bool charaHit(pCharacter target, pCharacter opponent);

bool charaHitBox(pCharacter target, int left, int top, int right, int bottom);
bool charaHitBox(pCharacter target, int x, int y, const pBox in);
bool charaHitBox(pCharacter target, int x, int y, const Box *in);
bool charaHitBox(pCharacter target, int x, int y, CharaHitAreaId id);
bool charaHitBox(pCharacter target, const pRect in);
bool charaHitBox(pCharacter target, const Rect *in);

int charaBulkHit(Character characters[], int num, pCharacter target);
int charaBulkHit(Character characters[], int num, pCharacter target, CharaHitAreaId id);
int charaBulkHit(Character characters[], int num, int x, int y, CharaHitAreaId id);

int charaCorrect(pCharacter chara, pCharacter subject, int bFlag);

bool charaLanding(int hitInfo);


//���ۂ̓���p

/*
�L�����z�񒆂̃A�N�e�B�u�ȗv�f�ɑ΂��āA�X�V�������s��
@param characters �Ώۂ̔z��
@param num �z��̗v�f��
@return
0...����
1...���s
*/
int charaBulkUpdate(Character characters[], int num);




//�`��p

/*
�L�����P�̂�`�悷��
@param target �Ώۂ̃L����
@return
0...����
1...���s
*/
int charaDraw(pCharacter target);

/*
�L�����z�񒆂̃A�N�e�B�u�ȗv�f��S�ĕ`�悷��
@param characters �Ώۂ̔z��
@param num �z��̗v�f��
@return
0...����
1...���s
*/
int charaBulkDraw(Character characters[], int num);


