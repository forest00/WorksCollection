//	
//	�~������uMagnet Hero�v
//	
//	�쐬��	�X	�m�u
//	
//	�ŏI�X�V		2018/12/13
//	

//�e�w�b�_�[
#include "magnet_hero_state.h"
//���̑��̃w�b�_�[
#include "magnet_hero_state\title\title.h"
#include "magnet_hero_state\game_main\game_main.h"
#include "magnet_hero_state\ending\ending.h"


static magnet_hero_state_t g_first_state = mhs_none;


//��Ԃ��Ƃɏ���������֐����Ăяo��
static void magnet_hero_state_init(magnet_hero_state_t state)
{
	switch (state)
	{
	case mhs_none:
		break;
	case mhs_title:
		title_init();
		break;
	case mhs_game_main:
		game_main_init();
		break;
	case mhs_ending:
		ending_init();
		break;
	case mhs_max:
		break;
	default:
		break;
	}
}


//��Ԃ��ƂɏI������֐����Ăяo��
static void magnet_hero_state_end(magnet_hero_state_t state)
{
	switch (state)
	{
	case mhs_none:
		break;
	case mhs_title:
		title_end();
		break;
	case mhs_game_main:
		game_main_end();
		break;
	case mhs_ending:
		ending_end();
		break;
	case mhs_max:
		break;
	default:
		break;
	}
}


//��ԍŏ��̏�Ԃ�ݒ�
extern void magnet_hero_state_set_first_state(magnet_hero_state_t state)
{
	g_first_state = state;
}



//��Ԃ��ƂɍX�V����֐����Ăяo��
extern void magnet_hero_state_update(magnet_hero_state_t now_state, magnet_hero_state_t *next_state)
{
	magnet_hero_state_t prev_state = now_state;
	switch (now_state)
	{
	case mhs_none:
		now_state = g_first_state;
		break;
	case mhs_title:
		title_update(&now_state);
		break;
	case mhs_game_main:
		game_main_update(&now_state);
		break;
	case mhs_ending:
		ending_update(&now_state);
		break;
	case mhs_max:
		break;
	default:
		break;
	}
	if (now_state != prev_state)
	{///��Ԃ��؂�ւ����
		//�O�̏�Ԃ��I��������
		magnet_hero_state_end(prev_state);
		//���݂̏�Ԃ�������
		magnet_hero_state_init(now_state);
		if (next_state)
		{
			*next_state = now_state;
		}
	}
}


//��Ԃ��Ƃɕ`��֐����Ăяo��
extern void magnet_hero_state_draw(magnet_hero_state_t state)
{
	switch (state)
	{
	case mhs_none:
		break;
	case mhs_title:
		title_draw();
		break;
	case mhs_game_main:
		game_main_draw();
		break;
	case mhs_ending:
		ending_draw();
		break;
	case mhs_max:
		break;
	default:
		break;
	}
}
