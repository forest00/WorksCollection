#pragma once

#include <time.h>
#include <Windows.h>
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#include "input_mouse_code.h"
#include "input_controller_code.h"

/*------------------------------------------------------------------------------*/
//���͂̏�����
//������Ăяo���Ɠ��͎���̊֐����L���Ȓl��Ԃ��悤�ɂȂ�܂�
//�v���O�������ň�񂾂��Ăяo���悤�ɂ��Ă�������
extern int input_init(HWND window_handle);

//���͂̍X�V
//���t���[���Ăяo���K�v������܂�
//����������Ă��邱�Ƃ�O��ɓ����̂Œ���
extern void input_update();

//���͂̏I��
//�Ăяo���Ɠ��͌n�̊֐�������ɓ��삵�Ȃ��Ȃ�܂�
extern void input_end();
/*------------------------------------------------------------------------------*/






/*------------------------------------------------------------------------------*/
//�L�[��������Ă��鎞�Ԃ�Ԃ�(������Ă��Ȃ����0)
//"DIK_"����n�܂�}�N�����g��
extern int input_keyboard_count(int code);
//�L�[�������ꂽ�u�Ԃ����f����
//"DIK_"����n�܂�}�N�����g��
extern bool input_keyboard_trigger(int code);
//�L�[��������Ă��邩���f����
//"DIK_"����n�܂�}�N�����g��
extern bool input_keyboard_repeat(int code);
//�L�[�������ꂽ�u�Ԃ����f����
//"DIK_"����n�܂�}�N�����g��
extern bool input_keyboard_release(int code);
/*------------------------------------------------------------------------------*/









/*------------------------------------------------------------------------------*/
//�}�E�X�����ɓ������ʂ𓾂܂�
//���-5~5
//�f������������10�𒴂����Ƃ�
extern int input_mouse_move_x();
//�}�E�X���c�ɓ������ʂ𓾂܂�
//���-5~5
//�f������������10�𒴂����Ƃ�
extern int input_mouse_move_y();
//�}�E�X�̃z�C�[���ʂ𓾂܂�
// �߂�l�͈ȉ���3��
//(+120, �}0, -120)
extern int input_mouse_wheel();
//�L�[��������Ă��鎞�Ԃ�Ԃ�(������Ă��Ȃ����0)
extern int input_mouse_count(input_mouse_code_t code);
//�L�[�������ꂽ�u�Ԃ����f����
extern bool input_mouse_trigger(input_mouse_code_t code);
//�L�[��������Ă��邩���f����
extern bool input_mouse_repeat(input_mouse_code_t code);
//�L�[�������ꂽ�u�Ԃ����f����
extern bool input_mouse_release(input_mouse_code_t code);
/*------------------------------------------------------------------------------*/










/*------------------------------------------------------------------------------*/
//���X�e�B�b�N�̉��̌X��
//���-1.0~1.0
extern double input_controller_left_x();
//���X�e�B�b�N�̏c�̌X��
//���-1.0~1.0
extern double input_controller_left_y();
//�E�X�e�B�b�N�̉��̌X��
//���-1.0~1.0
extern double input_controller_right_x();
//�E�X�e�B�b�N�̏c�̌X��
//���-1.0~1.0
extern double input_controller_right_y();
//�{�^����������Ă��鎞�Ԃ�Ԃ�(������Ă��Ȃ����0)
extern int input_controller_count(input_controller_code_t code);
//�{�^���������ꂽ�u�Ԃ����f����
extern bool input_controller_trigger(input_controller_code_t code);
//�{�^����������Ă��邩���f����
extern bool input_controller_repeat(input_controller_code_t code);
//�{�^���������ꂽ�u�Ԃ����f����
extern bool input_controller_release(input_controller_code_t code);
//�\���{�^����������Ă��鎞�Ԃ�Ԃ�(������Ă��Ȃ����0)
extern int input_controller_cross_count(input_controller_cross_code_t code);
//�\���{�^���������ꂽ�u�Ԃ����f����
extern bool input_controller_cross_trigger(input_controller_cross_code_t code);
//�\���{�^����������Ă��邩���f����
extern bool input_controller_cross_repeat(input_controller_cross_code_t code);
//�\���{�^���������ꂽ�u�Ԃ����f����
extern bool input_controller_cross_release(input_controller_cross_code_t code);
//ZL��ZR�̓��͒l�𑫂����l��Ԃ�
//(ZL�������Ɛ��̕����AZR�ŕ��̕����A�ǂ����������Ă����0�A�͈͂�99~-100)
extern int input_controller_ZL_ZR();
/*------------------------------------------------------------------------------*/


