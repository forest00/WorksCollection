#pragma once
/*
*@file glitter.h
*@brief ���炫��G�t�F�N�g�P�̂𑀍삷��
*@auther �X�@�m�u
*/

//DirectX���C�u����
#include <d3d9.h>
#include <d3dx9.h>
//�C�[�W���O�^�C�v
#include "..\..\..\..\easing\easing_type.h"
//�e�N�X�`��ID
#include "..\..\..\..\texture_manager\texuture_id.h"

//���炫��\����
struct glitter_t
{
	float _spawn_sec;						//���܂ꂽ�u��(�b)
	float _life_sec;						//����(�b)
	float _scale_base;						//�g�嗦
	easing_type_t _easing_type;				//�C�[�W���O�^�C�v
	texture_id_t _texture_id;				//�e�N�X�`��ID
	D3DXVECTOR3 _start_position;			//�J�n�ʒu
	D3DXVECTOR3 _end_position;				//�I���ʒu
	float _red;								//�Ԑ���(0~1)
	float _green;							//�ΐ���(0~1)
	float _blue;							//����(0~1)
};

//���炫��\���̏������p�\����
struct glitter_initializer_t
{
	float _life_sec;						//����(�b)
	float _scale_base;						//�g�嗦
	easing_type_t _easing_type;				//�C�[�W���O�^�C�v
	texture_id_t _texture_id;				//�e�N�X�`��ID
	D3DXVECTOR3 _start_position;			//�J�n�ʒu
	D3DXVECTOR3 _end_position;				//�I���ʒu
	float _red;								//�Ԑ���(0~1)
	float _green;							//�ΐ���(0~1)
	float _blue;							//����(0~1)
};

/*
*@fn glitter_t_init_with_alloc
*@brief ���炫��G�t�F�N�g���������m�ۂ�����ŏ��������鏉��������
*@detail �����Ń������m�ۂ��Ă���̂ŊO����glitter_t_end_with_free()���g���ďI�����������Ȃ���΂Ȃ�Ȃ�
*/
extern void glitter_t_init_with_alloc(glitter_t **obj, const glitter_initializer_t *initializer);

/*
*@fn glitter_t_init
*@brief ���炫��G�t�F�N�g������������
*/
extern void glitter_t_init(glitter_t *obj, const glitter_initializer_t *initializer);

/*
*@fn glitter_t_end_with_free
*@brief�@���炫��G�t�F�N�g�I��������������Ń������������
*@detail ���̊֐��ŏI��������������A����ȍ~�͂ǂ̑�������Ă͂����Ȃ�
*/
extern void glitter_t_end_with_free(glitter_t **obj);

/*
*@fn glitter_t_end
*@brief�@���炫��G�t�F�N�g�I������������
*/
extern void glitter_t_end(glitter_t *obj);

/*
*@fn glitter_t_update
*@brief�@���炫��G�t�F�N�g���X�V����
*/
extern void glitter_t_update(glitter_t *obj, bool *should_end);

/*
*@fn glitter_t_draw
*@brief�@���炫��G�t�F�N�g��`�悷��
*/
extern void glitter_t_draw(const glitter_t *obj);

/*
*@fn glitter_t_elapsed_sec
*@brief�@���炫��G�t�F�N�g�̌o�ߎ��Ԃ��擾����
*/
extern void glitter_t_elapsed_sec(const glitter_t *obj, float *elapsed_sec);

/*
*@fn glitter_t_now_position
*@brief�@���炫��G�t�F�N�g�̌��݈ʒu���擾����
*/
extern void glitter_t_now_position(const glitter_t *obj, D3DXVECTOR3 *now_position);

/*
*@fn glitter_t_alpha
*@brief ���炫��G�t�F�N�g�̓����x�𓾂�(0~255)
*/
extern void glitter_t_alpha(const glitter_t *obj, int *alpha);
