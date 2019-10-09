#pragma once
//	
//	�~������uMagnet Hero�v
//	
//	�Q�[���̏�Ԃ��Ǘ�����Ƃ���ł�
//	
//	�����ł́A��Ԃ��ƂɈقȂ�֐����Ăяo����Ƃ����܂�
//	
//	���Direct3D�̏����������Ă����K�v������܂�
//	
//	�쐬��	�X	�m�u
//	
//	�ŏI�X�V		2019/01/16
//	



#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


//���ꂼ��̏��
enum magnet_hero_state_t
{
	mhs_none,				//�X�e�[�g�Ȃ�
	mhs_title,				//�^�C�g��
	mhs_game_main,			//���C���Q�[������
	mhs_ending,				//�G���f�B���O
	mhs_ended,				//�I��������ׂ�
	mhs_max,				//�X�e�[�g�̍ő吔
};

/*
*@fn magnet_hero_state_set_first_state
*@brief ��ԍŏ��̏�Ԃ�ݒ�
*@param state �ŏ��̏�ԂƂ��Đݒ肳���l
*/
extern void magnet_hero_state_set_first_state(magnet_hero_state_t state);


/*
*@fnmagnet_hero_state_update
*@brief ��Ԃ��ƂɍX�V����֐����Ăяo��
*@param now_state ���݂̏��
*@param next_state ���̏�Ԃ��i�[����ꏊ
*@detail now_state��magnet_hero_state_t_none�̏ꍇ�Amagnet_hero_state_set_first_state�Őݒ肳�ꂽ��Ԃɂ���
*/
extern void magnet_hero_state_update(magnet_hero_state_t now_state, magnet_hero_state_t *next_state);

/*
*@fnmagnet_hero_state_update
*@brief ��Ԃ��Ƃɕ`��֐����Ăяo��
*@param state ���
*/
extern void magnet_hero_state_draw(magnet_hero_state_t state);


