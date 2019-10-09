#pragma once

#include "type_parameter.h"

/**
 *  �������Ԍ�̋���
 */
typedef enum {
	REMOVE, ///< �O��
	LOOP,   ///< �J��Ԃ�
	KEEP    ///< �ۂ�
} BehaviorAfter;
/**
 *  �G�t�F�N�g
 */
typedef struct {
	bool active;	      ///< ������
	unsigned int delay;     ///< �x��
	unsigned int lifetime;  ///< ��������
	BehaviorAfter behavior_after; ///< �������Ԍ�̋���

	Parameter x, y;   ///< ���W
	Parameter alpha;  ///< �A���t�@
	Parameter angle;  ///< ��]�p
	Parameter scale;  ///< �g�嗦

	unsigned int blend;   ///< �������@
	int texture_w;///< �摜�̕�
	int texture_h;///< �摜�̍���

	int handle;     ///< �摜�n���h��
	unsigned int start_time;///< �J�n����

} Effect;

/**
 *  �G�t�F�N�g�Ǘ��̏�����
 */
void EffectMngInit();
/**
 *  �G�t�F�N�g�Ǘ��̍X�V
 */
void EffectMngUpdate();
/**
 *  �G�t�F�N�g�Ǘ��̕`��
 */
void EffectMngRender();
/**
 *  �G�t�F�N�g�̎擾
 *  @return ��Effect�ւ̃|�C���^
 */
Effect* GetEffect();
/**
 *  �p�����[�^�̐ݒ�
 *  @param prm    �ݒ��p�����[�^�ւ̃|�C���^
 *  @param easing �C�[�W���O���
 *  @param value  ���݂̒l
 *  @param start  �J�n�̒l
 *  @param stop	  �I���̒l
 */
void ParameterSet(Parameter* prm, EasingType easing, double value, double start, double stop);

