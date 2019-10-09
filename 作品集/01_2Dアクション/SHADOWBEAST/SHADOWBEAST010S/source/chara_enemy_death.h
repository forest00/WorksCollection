#pragma once
//�G�����S�����Ƃ��̐o�G�t�F�N�g���o���܂�
#include "chara__common.h"
#include "chara_point.h"



/*
�G�����񂾂Ƃ��̃G�t�F�N�g���o���֐�
@param	type	�G�̎���ID
@param	x		�G��x���W
@param	y		�G��y���W
@return			0...���� -1...���s
*/
int enemyDeadEffectSet(EnemyType type, double x, double y, Direction dir);

/*
�G���S���̃G�t�F�N�g���o�����߂̏���������֐�
���̊֐����Ăяo���Ȃ��Ƒ��̊֐��͖��Ӗ��ɂȂ�܂�
@return			0...���� -1...���s
*/
int enemyDeadEffectInit();

/*
�G���S���̃G�t�F�N�g���X�V����֐�
�Ăяo�����тɃG�t�F�N�g��1�t���[�������������܂�
@return			0...���� -1...���s
*/
int enemyDeadEffectUpdate();

/*
�G���S���̃G�t�F�N�g��`�悷��֐�
@return			0...���� -1...���s
*/
int enemyDeadEffectDraw();

/*
�G���S���̃G�t�F�N�g�̎g�p����߂�֐�
@return			0...���� -1...���s
*/
int enemyDeadEffectEnd();









