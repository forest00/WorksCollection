


#include <Windows.h>
#include "game\public\game.h"

//�G���g���[�|�C���g
int WINAPI WinMain(HINSTANCE instance_handle, HINSTANCE, LPSTR, int)
{

	//�Q�[���S�̂̏�����
	//���s�����牽�������֐��𔲂���
	if (game_init() != 0)
	{
		MessageBox(0, "���������s�ł�", "", MB_OK);
		return -1;
	}


	//�Q�[�����[�v
	while (game_shoud_continue())
	{
		game_update();
		game_draw();
	}

	//�Q�[���S�̂̏I��
	game_end();

	return 0;
}




