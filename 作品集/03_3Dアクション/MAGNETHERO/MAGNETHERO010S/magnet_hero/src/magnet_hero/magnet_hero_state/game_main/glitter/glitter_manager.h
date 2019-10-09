#pragma once
/*
*@file glitter_manager.h
*@brief ���炫��G�t�F�N�g���Ǘ�����
*@auther �X�@�m�u
*/

//DirectX���C�u����
#include <d3d9.h>
#include <d3dx9.h>
//�C�[�W���O�^�C�v
#include "..\..\..\..\easing\easing_type.h"

//���炫��G�t�F�N�g�̎��
enum glitter_kind_t
{
	gk_none,
	gk_orange,
	gk_purple,
	gk_ring,
	gk_green,
	gk_blue,
	gk_white,
	gk_red,
	gk_max,
};

//���炫��G�t�F�N�g�ǉ��p�\����
struct glitter_add_t
{
	glitter_kind_t _kind;					//���
	float _life_sec;						//����(�b)
	float _scale_base;						//�g�嗦
	easing_type_t _easing_type;				//�C�[�W���O�^�C�v
	D3DXVECTOR3 _start_position;			//�J�n�ʒu
	D3DXVECTOR3 _end_position;				//�I���ʒu
	float _red;								//�Ԑ���(0~1)
	float _green;							//�ΐ���(0~1)
	float _blue;							//����(0~1)
};

/*
*@fn glitter_manager_init
*@brief ���炫��G�t�F�N�g������������
*/
extern void glitter_manager_init();

/*
*@fn glitter_manager_end
*@brief�@���炫��G�t�F�N�g���I������
*/
extern void glitter_manager_end();

/*
*@fn
*@brief�@���炫��G�t�F�N�g���X�V����
*/
extern void glitter_manager_update();

/*
*@fn glitter_manager_draw
*@brief�@���炫��G�t�F�N�g��`�悷��
*/
extern void glitter_manager_draw();

/*
*@fn glitter_manager_add
*@brief�@���炫��G�t�F�N�g��ǉ�����
*@detail ���炫��G�t�F�N�g�͎��Ԍo�߂ŏ���ɏ����܂�
*/
extern void glitter_manager_add(const glitter_add_t *info);

/*
*@fn glitter_manager_random
*@brief �����_���Ȓl���擾
*/
extern glitter_kind_t gk_random();
