//�e�w�b�_�[
#include "option.h"
//���̑��̃w�b�_�[
#include "option_ui.h"
#include "..\input\input.h"

//�O���[�o���ϐ�
struct global_t
{
	bool _do_aim_assist;
	float _mouse_sensitivity;
};

static global_t g;

//�Q�[���J�n���ɌĂ�(��mylib�̏������̌�)
extern void option_init()
{
	g._do_aim_assist = true;
	g._mouse_sensitivity = 0.5f;
	option_ui_init();
}

//�Q�[���I�����ɌĂ�
extern void option_end()
{
	option_ui_end();
}

//�X�V�F���t���[���Ă�
extern void option_update()
{
	bool change_aim_assist = input_mouse_trigger(imc_right) || input_keyboard_trigger(DIK_M);
	float stages = (float)(15);
	float add_mouse_sensitivity = (float)(input_mouse_wheel()) / 120.0f / stages;
	add_mouse_sensitivity += (float)(input_keyboard_repeat(DIK_N)) / stages;
	add_mouse_sensitivity -= (float)(input_keyboard_repeat(DIK_B)) / stages;
	if (change_aim_assist)
	{
		g._do_aim_assist = !g._do_aim_assist;
		option_ui_aim_assist(g._do_aim_assist);
	}
	if (add_mouse_sensitivity)
	{
		g._mouse_sensitivity += add_mouse_sensitivity;
		g._mouse_sensitivity = min(max(g._mouse_sensitivity, 0.0f), 1.0f);
		option_ui_mouse_sensitivity(g._mouse_sensitivity);
	}
	option_ui_update();
}

//�`��F���t���[���Ă�
extern void option_draw()
{
	option_ui_draw();
}

//�G�C���A�V�X�g������
extern bool option_get_do_aim_assist()
{
	return g._do_aim_assist;
}

//�}�E�X���x(0~1)
extern float option_get_mouse_sensitivity()
{
	return g._mouse_sensitivity;
}