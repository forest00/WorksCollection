#pragma once
//
//�X�e�[�W��UI
//
//�쐬�ҁ@�X�m�u
//����
//2019/02/05...�t�@�C�����쐬
//2019/02/05...�������A�`��A�I���֐���ǉ�

//DirectX
#include <d3dx9.h>

//������
extern void stage_ui_init();

//�I��
extern void stage_ui_end();

//�X�V
extern void stage_ui_update();

//�`��
extern void stage_ui_draw(time_t goal_frame, time_t goal_limit_frame);

