#pragma once
//�X�e�[�W�̃Z�b�g�A�b�v������
//�v���[���[�Ƃ��̏������������ōs���܂�
//�쐬�ҁ@�X�m�u

//�X�e�[�W�̃C���f�b�N�X
#include "stage_index.h"
//fbx�I�u�W�F�N�g�Ǘ��p�w�b�_�[
#include "..\fbx_object\object_manager.h"

//N�Ԗڂ̃X�e�[�W�̃Z�b�g�A�b�v���s���֐��ł�
//�������ς݂̃J�����ƁA�������ς݂�FBX�I�u�W�F�N�g�z��Ǘ��p�\���̂��K�v�ł�
extern void stage_setup(stage_index_t n, game_main_fbx_object_array_t *obj_array);

//N�Ԗڂ̃X�e�[�W�����Z�b�g����֐��ł�
extern void stage_reset(stage_index_t n);

//N�Ԗڂ̃X�e�[�W��j������֐��ł�
extern void stage_destroy(stage_index_t n, game_main_fbx_object_array_t *obj_array);
