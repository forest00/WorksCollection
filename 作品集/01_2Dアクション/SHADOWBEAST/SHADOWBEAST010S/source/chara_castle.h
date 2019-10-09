#pragma once

#include "chara__common.h"

/*
���ׂ����_���Z�b�g����
@param x ���[���hx���W
@param y ���[���hy���W
@return 0...���� -1...���s
*/
int setProtectTarget(double x, double y);

/*
�����x���W�𓾂�֐�
@return ����̃��[���hx���W
*/
int getCastleX();

/*
����Ƀ_���[�W��^����֐�
@param target ����ւ̃|�C���^
@param damage �_���[�W��
@return 0...���� -1...��͖��G��ԁA�_���[�W�͒ʂ�Ȃ�
*/
int castleHitDamage(pCharacter target, int damage);

/*
�A�C�e�����o��
@param vx		�A�C�e���𐁂���΂�x�����ւ̗�
@param vy		�A�C�e���𐁂���΂�y�����ւ̗�
@return 0...���� -1...���s
*/
int setItemOnCastle(double vx, double vy);

/*
���HP�𓾂�
@param hp		��̌���HP���i�[����ϐ��ւ̃|�C���^
@param maxHp	��̍ő�HP���i�[����ϐ��ւ̃|�C���^
*/
void getCastleHp(int *hp, int *maxHp);
