//	
//	�~������uMagnet Hero�v
//	
//	�v���O�����̃G���g���[�|�C���g����`����Ă���t�@�C���ł�
//	
//	�쐬��	�X	�m�u
//	

#include "framework\framework.h"

//�G���g���[�|�C���g�ł�
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//�Q�Ƃ��Ȃ��p�����[�^�[
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);
	//�Q�[���̏���������
	if (frame_init(hInstance) == 0)
	{
		//�Q�[���𑱂���ׂ���
		while (frame_continue())
		{
			//�Q�[���̍X�V����
			frame_update();
			//�Q�[���̕`�揈��
			frame_draw();
		}
	}
	//�Q�[���̏I������
	frame_end();
	//���U���g��Ԃ�
	return S_OK;
}
