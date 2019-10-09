#pragma once
/**
 * @file  anima.h
 * @brief �A�j���[�V��������
 *
 * @date 2016-06-16
 * @version 3.0
 *
 * Copyright (C) Amusement Media Academy All rights Resved.
 */
#include "image_manager.h"

 ///< �A�j���[�V�����p�\����
typedef struct {
	unsigned int time;			///< ���ݎ���
	unsigned int started;		///< �J�n����
	unsigned int duration;		///< ��������
	unsigned int limit;			///< �R�}������
} Anima;

typedef struct {
	unsigned int duration;		///< ��������
	unsigned int limit;			///< �R�}������
} AnimaData;


/**
�A�j���f�[�^�̍쐬
*/
int createAnimaData(AnimaData *outPut, ImageFileId file_id);
/**
 *  �A�j���\���̂̐ݒ�
 *
 *  @param anima �A�j���\���̂ւ̃|�C���^
 *  @param started �J�n����
 *  @param anima_data �A�j���f�[�^�\���̂ւ̃|�C���^
 */
void animaSetup(Anima *anim, unsigned int started, const AnimaData* anima_data);
/**
*  �A�j���[�V�����R�}���̎擾
*
*  @param anima �A�j���\���̂ւ̃|�C���^
*  @retval frame ���݂̃R�}��
*/
unsigned int animaGetFrame(const Anima *anim);
/**
 *  �A�j���[�V�����񐔂̎擾
 *
 *  @param anima �A�j���\���̂ւ̃|�C���^
 *  @retval count �A�j���[�V�������J��Ԃ�����
 */
unsigned int animaGetCount(const Anima *anim);
/**
 *  �����ɑΉ������l�̎擾
 *  @param anima �A�j���[�V�����\���̂ւ̃|�C���^
 *  @retval value �����ɑΉ������l
 */
unsigned int animaGetValueStep(const Anima *anim);
