#pragma once
//	
//	�~������uMagnet Hero�v
//	
//	�`�[�����S���o���܂�
//	
//	���Direct3D�̏����������Ă����K�v������܂�
//	
//	�쐬��	�X	�m�u
//	
//	�X�V����
//	2018/12/10...�t�@�C�����쐬
//	

#include "..\..\magnet_hero_state.h"

extern void game_main_init();
extern void game_main_end();
extern void game_main_update(magnet_hero_state_t *next_state);
extern void game_main_draw();
