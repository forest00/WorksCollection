#pragma once

//�L�[�{�[�h�̓��͂��擾���邽�߂̍\����
struct Keyboard
{
	bool push[256];
};

//�L�[�{�[�h�̓��͂��擾����
extern void CheckKey(Keyboard *key);

//���L�[�R�[�h��
extern int ArrowUp();
extern int ArrowDown();
extern int ArrowRight();
extern int ArrowLeft();
//���L�[�R�[�h��

//�G�X�P�[�v�L�[�R�[�h
extern int EscapeKey();
