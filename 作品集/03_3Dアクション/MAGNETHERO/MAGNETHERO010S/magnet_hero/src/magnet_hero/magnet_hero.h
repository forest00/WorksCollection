#pragma once
//	
//	�~������uMagnet Hero�v
//	
//	�Q�[���̃��C���������L�q���ꂽ�����ł�
//	
//	���Direct3D�̏����������Ă����K�v������܂�
//	
//	�쐬��	�X	�m�u
//	
//	�ŏI�X�V		2018/12/07
//	


#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


//�Q�[���̏�����
extern int magnet_hero_init();

//�Q�[���̏I��
extern void magnet_hero_end();

//�Q�[���̍X�V
extern void magnet_hero_update();

//�Q�[���̕`��
extern void magnet_hero_draw();

//�Q�[�����I��������ׂ�
extern bool magnet_hero_ended();

