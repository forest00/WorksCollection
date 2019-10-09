#pragma once
//�쐬�ҁ@�X�m�u
//����
//2019/02/05...�t�@�C�����쐬
//2019/02/05...�������֐���ǉ�
//2019/02/05...���܂��܂Ȏ擾�֐���ǉ�


//DirectX
#include <d3dx9.h>
//�C���f�b�N�X
#include "stage_index.h"

//������
extern void stage_data_init();

//�X�e�[�WN�̃X�^�[�g�n�_
extern void stage_data_get_start_point(stage_index_t n, D3DXVECTOR3 *position);

//�X�e�[�WN�̃X�^�[�g���̃v���[���[�̌���
extern void stage_data_get_start_player_direction(stage_index_t n, float *direction);

//�X�e�[�WN�X�^�[�g���̃J�����̌���
extern void stage_data_get_start_camera_angle(stage_index_t n, float *phi, float *theta);

//�X�e�[�WN�łǂꂾ���S�[���ɋ߂Â�����S�[���ɂ��邩
extern float stage_data_get_goal_goal_threshold(stage_index_t n);

//�X�e�[�WN�̃S�[���n�_
extern void stage_data_get_goal_point(stage_index_t n, D3DXVECTOR3 *position);
