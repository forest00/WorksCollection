#pragma once
//	
//	�~������uMagnet Hero�v
//	
//	�J�����̊Ǘ����s���܂�
//	
//	���Direct3D�̏����������Ă����K�v������܂�
//	
//	�쐬��	�X	�m�u
//	


#include "camera_definition.h"

#include "..\fbx_object\object_manager.h"

//������
extern void camera_manager_init();
//�I��
extern void camera_manager_end();
//�X�V
extern void camera_manager_update(game_main_fbx_object_array_t *object_array);
//�r���[�s��ƃv���W�F�N�V�����s������߂ă��C�u�����̂ق��ɃZ�b�g
extern void camera_manager_set_view_and_projection(float near_clip, float far_clip);
//�S���擾
extern const game_main_camera_t *camera_manager_get();
//�������̊p�x�擾
extern float camera_manager_get_phi();
//�c�����̊p�x�擾
extern float camera_manager_get_theta();
//�������̊p�x�w��
extern void camera_manager_set_phi(float phi);
//�c�����̊p�x�w��
extern void camera_manager_set_theta(float theta);