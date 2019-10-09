
//�e�w�b�_�[
#include "..\public\loop_timer.h"
//���̑��̃w�b�_�[
#include <Windows.h>

static DWORD g_loop_timer_last;

static double g_loop_timer_time;



//������
//���[�v���n�܂�O�Ɉ�񂾂��Ă�
extern void loop_timer_init()
{
	g_loop_timer_last = timeGetTime();
	g_loop_timer_time = 0.0;
}

//�L�^
//���[�v���ɖ���Ă�
extern void loop_timer_record()
{
	DWORD now = timeGetTime();
	g_loop_timer_time = (double)(now - g_loop_timer_last) / 1000.0f;
	g_loop_timer_last = now;
}


//���[�v�ɂ����������Ԃ𓾂�
extern double loop_timer_get()
{
	return g_loop_timer_time;
}




