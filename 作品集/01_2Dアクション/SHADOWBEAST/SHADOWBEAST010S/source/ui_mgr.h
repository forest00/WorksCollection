#pragma once


/*
1p�̂�����������֐�
���t���[���Ăяo����1p�̌��݂����ƍő傈���������Ă��܂�
*/
extern int uiMgrTeach1pHp(int currentHp, int maxHp);

/*
2p�̂�����������֐�
*/
extern int uiMgrTeach2pHp(int currentHp, int maxHp);

/*
���_�̂�����������֐�
*/
extern int uiMgrTeachCastleHp(int currentHp, int maxHp);

/*
�{�X�̂�����������֐�
*/
extern int uiMgrTeachBossHp(int currentHp, int maxHp);

/*
�{�X�X�e�[�W�ł��邱�Ƃ�������֐�
*/
extern int uiMgrTeachBossStage(bool isBossStage);


/*
ui�����ׂď���������֐�
*/
extern int uiMgrInit();

/*
ui�����ׂčX�V����֐�
*/
extern int uiMgrUpdate();

/*
ui�����ׂĕ`�悷��֐�
*/
extern int uiMgrDraw();

/*
ui�����ׂď�������֐�
*/
extern int uiMgrEnd();

