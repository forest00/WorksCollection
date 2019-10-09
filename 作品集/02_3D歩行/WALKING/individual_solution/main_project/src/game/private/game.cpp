
//�����̐e�w�b�_�[
#include "..\public\game.h"
//���̑��̃w�b�_�[
#include "..\..\math_with_defines\math_with_defines.h"
#include "game_system.h"
#include "game_base.h"
#include "..\..\loop\public\loop_timer.h"








/*---------------------------------------------------------------------------------------------*/
extern int game_init()
{
	//�V�X�e�������̏�����
	if (game_system_init() != 0)
	{
		return -1;
	}

	//�Q�[�������̏�����
	game_base_init();

	//���Ԃ̌v���p
	loop_timer_init();

	return 0;
}



extern bool game_shoud_continue()
{
	//���b�Z�[�W����������
	return process_message();
}


extern void game_update()
{
	//�V�X�e�������̍X�V
	game_system_update();

	//�Q�[�������̍X�V
	game_base_update();

	//���Ԃ̌v���p
	loop_timer_record();
	
}



extern void game_draw()
{
	//game_system_draw_end();
	

	//�`��̏���(���s�����牽�������I��)
	if (game_system_draw_begin() != 0)
	{
		return;
	}

	//�Q�[�������̕`��
	game_base_draw();

	//�`��I���
	game_system_draw_end();

}


extern void game_end()
{

	//�Q�[�������̏I��
	game_base_end();


	//�V�X�e�������̏I��
	game_system_end();

}
/*---------------------------------------------------------------------------------------------*/


