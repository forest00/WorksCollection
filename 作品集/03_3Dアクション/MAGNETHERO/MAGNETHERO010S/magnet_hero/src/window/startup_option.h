#pragma once
//�Q�[���N�����̃E�B���h�E�T�C�Y��ʒu�Ȃǂ̃I�v�V�����ł�
//�e�L�X�g�t�@�C������ǂݍ��݂܂�
//�쐬�ҁ@�X�@�m�u
//�X�V����
//2019/01/22...�t�@�C���쐬
//����...�֐���ǉ�

//�N�����ɃX�N���[�����[�h���ǂ����邩�̎w��ł�
//0�Ȃ�_�C�A���O���o���Ċm�F���Ȃ��Ă͂����܂���
//1�Ȃ�t���X�N���[�����[�h�ŋN�����Ȃ��Ă͂����܂���
//2�Ȃ�E�B���h�E���[�h�ŋN�����Ȃ��Ă͂����܂���
//����ȊO�̎��͋N�����Ă͂����܂���
extern int startup_option_get_screen_mode();

//�E�B���h�E�̍������ʒu�ł�
extern int startup_option_get_left();

//�E�B���h�E�̏㏉���ʒu�ł�
extern int startup_option_get_top();

//�E�B���h�E�̏��������ł�
extern int startup_option_get_width();

//�E�B���h�E�̏����c���ł�
extern int startup_option_get_height();

//�}�E�X���B�����ǂ������w�肷��I�v�V�����ł�
//true�Ȃ�}�E�X���B���Ȃ��Ă͂����܂���
extern bool startup_option_get_hide_mouse();

//�E�B���h�E�^�C�g�����ł�
extern const char *startup_option_get_window_title();
