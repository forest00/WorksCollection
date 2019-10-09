/**
 * @file  anima.cpp
 * �A�j���[�V��������
 *
 * @date 2016-06-16
 * @version 3.0
 *
 * Copyright (C) Amusement Media Academy All rights Resved.
 */

 // �C���N���[�h
#include "anima.h"
#include "image_manager.h"



/**
�A�j���f�[�^�̍쐬
*/
int createAnimaData(AnimaData *outPut, ImageFileId file_id){
	const BlockImage *f = getImageFile(file_id);
	*outPut = {(unsigned int)f->duration,
				(unsigned int)f->a_num};
	return 0;
}

/**
 *  �A�j���\���̂̐ݒ�
 */
void animaSetup(Anima *anim, unsigned int started, const AnimaData* anima_data) {
	anim->started = started;
	anim->duration = anima_data->duration;
	anim->limit = anima_data->limit;
}
/**
*  �A�j���[�V�����R�}���̎擾
*/
unsigned int animaGetFrame(const Anima *anim){
	double d = anim->duration / anim->limit;
	unsigned int frame = (unsigned int)((anim->time - anim->started) / d);
	return frame;
}
/**
 *  �A�j���[�V�����񐔂̎擾
 */
unsigned int animaGetCount(const Anima *anim) {
	unsigned int count = (anim->time - anim->started) / anim->duration;
	return count;
}
/**
 *  �����ɑΉ������l�̎擾
 */
unsigned int animaGetValueStep(const Anima *anim) {
	// ���݂̒l�����߂�
	unsigned int index = (anim->time - anim->started) * anim->limit;

	index /= anim->duration;//int�^���m�̊���Z�@�����_�ȉ����؂�̂Ă��邱�Ƃ�z�肵�Ă���
	// �l�����[�v������
	index %= anim->limit;

	return index;
}
