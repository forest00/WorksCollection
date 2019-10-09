#pragma once
//
//�X�e�[�W�v���C���ɋN����l�X�ȃC�x���g���Ǘ�����ꏊ�ł�
//�����ōs���v�Z�̌��ʂ͂��ł��擾�ł��܂�
//
//�쐬�ҁ@�X�m�u
//����
//2019/02/05...�t�@�C�����쐬
//2019/02/05...�������A�X�V�A�I���֐���ǉ�
//2019/02/05...�v���[���[����S�[���܂ł̋������擾����֐���ǉ�
//2019/02/05...�v���[���[���S�[���ɓ��B�������Ƃ𔻒肷��֐���ǉ�
//2019/02/05...�X�e�[�W�J�n���̎��Ԃ��L�^����֐���ǉ�
//2019/02/05...�S�[���̔��肪�ł���֐���ǉ�
//2019/02/05...�X�V�����񐔂��擾����֐���ǉ�
//2019/02/05...�S�[�����Ă���o�߂������Ԃ��擾�ł���֐���ǉ�

//DirectX
#include <d3dx9.h>

//������
extern void stage_event_init();

//�I��
extern void stage_event_end();

//�X�V
extern void stage_event_update();

//�v���[���[����S�[���܂ł̋���
extern float stage_event_get_distance_from_player_to_goal();

//�v���[���[���S�[���n�_�̋߂��ɂ���Ƃ���true��Ԃ�
extern bool stage_event_get_player_near_goal(float threshold);

//�X�e�[�W���J�n����u�ԂɌĂ΂��֐�
extern void stage_event_stage_start();

//�v���[���[���S�[���n�_�ɒB�����Ƃ��ɌĂ΂��֐�
extern void stage_event_stage_goal();

//���łɃS�[�����Ă���
extern bool stage_event_get_already_goal();

//�J�n�����u��
extern time_t stage_event_get_start_moment();

//�S�[�������u��
extern time_t stage_event_get_goal_moment();

//�S�[�����Ă���o�߂������Ԃ��擾
extern time_t stage_event_get_goal_elapsed();
