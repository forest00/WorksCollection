#pragma once
//�Q�[�����̃I�v�V�������Ǘ�����Ƃ���ł�
//�I�v�V������
//�E�G�C���A�V�X�gON/OFF�؂�ւ�
//�E�}�E�X���x�̕ύX
//�ȏ�̓�ł�
//����̓Q�[�������ł��؂�ւ�����悤�ɍ��Ȃ��Ă͂����܂���
//�쐬�ҁF�X�@�m�u



//�Q�[���J�n���ɌĂ�
extern void option_init();

//�Q�[���I�����ɌĂ�
extern void option_end();

//�X�V�F���t���[���Ă�
extern void option_update();

//�`��F���t���[���Ă�
extern void option_draw();

//�G�C���A�V�X�g������
extern bool option_get_do_aim_assist();

//�}�E�X���x(0~1)
extern float option_get_mouse_sensitivity();

