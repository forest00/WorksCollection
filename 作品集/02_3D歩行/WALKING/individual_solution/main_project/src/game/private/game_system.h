#pragma once


//�Q�[���̃V�X�e�������̏�������X�V�Ȃǂ�����֐��Q�ł�



//�V�X�e�������̏�����
extern int game_system_init();

//���b�Z�[�W����
extern bool process_message();

//�V�X�e�������̍X�V
extern void game_system_update();

//�`��̊J�n
extern int game_system_draw_begin();

//�`��̏I��(���̊֐��́A�΂ɂȂ�begin�֐����������Ă����Ԃ���Ȃ��ƁA�Ă񂶂�_��)
extern void game_system_draw_end();

//�V�X�e�������̏I��
extern void game_system_end();

