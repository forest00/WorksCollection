#pragma once

#include "stage_index.h"

//������
extern void stage_clear_data_init();

//���݂̃X�e�[�W�𓾂�
extern stage_index_t stage_get_now();

//���݂̃X�e�[�W���N���A�����Ƃ��ɌĂяo���֐�
//���̊֐����ĂԂƌ��݃X�e�[�W�ɑ΂��ăN���A�t���O������
extern void stage_clear_now_stage();

//�X�e�[�W��i�߂�
//���̊֐����ĂԂƌ��݃X�e�[�W���C���N�������g�����
//�ŏI�X�e�[�W���N���A���A���̃X�e�[�W���Ȃ��ꍇ�Atrue��Ԃ�(�S�N��)
extern bool stage_advance();

//���̃X�e�[�W���N���A���Ă��邩
extern bool stage_cleared_stage(stage_index_t index);
