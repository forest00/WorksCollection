#pragma once
//�Q�[�����̃I�v�V������UI���Ǘ�����Ƃ���ł�
//�I�v�V������
//�E�G�C���A�V�X�gON/OFF�؂�ւ�
//�E�}�E�X���x�̕ύX
//�ȏ�̓�ł�
//����̓Q�[�������ł��؂�ւ�����悤�ɍ���܂�
//����ɑΉ�����UI�ɂ��邱��
//�쐬�ҁF�X�@�m�u

//������
extern void option_ui_init();

//�I��
extern void option_ui_end();

//�X�V
extern void option_ui_update();

//�`��
extern void option_ui_draw();


//�G�C���A�V�X�g��ON/OFF���؂芷����ꂽ��Ă�
extern void option_ui_aim_assist(bool on_off);


//�}�E�X���x���ύX���ꂽ��Ă�
extern void option_ui_mouse_sensitivity(float t);

