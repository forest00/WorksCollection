#pragma once
//�Q�[���̃��C�������Ŏg�����ׂẴX�v���C�g�𓝍�����t�@�C���ł�
//�쐬�ҁ@�X�m�u
//����
//2019/01/31...�t�@�C�����쐬
//����...�������A�X�V�A�`��A�I���֐����L�q
//����...���b�Z�[�W���s�֐����L�q

//DirectX�̃w�b�_�[
#include <d3dx9.h>


//������
extern void sprite_integration_init();

//�I��
extern void sprite_integration_end();

//�X�V
extern void sprite_integration_update();

//�`��
extern void sprite_integration_draw();

//�т�т�
extern void sprite_integration_bibibi_fire(int base, int random_range);

//QTE�J�n
extern void sprite_integration_qte_start();

//QTE��������
extern bool sprite_integration_qte_judge();

//QTE�^�C���A�E�g�`�F�b�N
extern bool sprite_integration_qte_timeout();
