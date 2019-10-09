#pragma once
#include "rect.h"

//�X�N���[��������
//����...0
//���s...-1
//���������s�����Ă���Ƃ��A�܂��́A�A���ŌĂяo�����Ƃ��A���s����
extern int InitScreen();

//�X�N���[���I��
//����...0
//���s...-1
//����������Ă��Ȃ��Ƃ��A���s����
extern int EndScreen();

//�X�N���[���S����
extern void ClearScreen();

//�X�N���[�����擾
extern int ScreenWidth();

//�X�N���[�������擾
extern int ScreenHeight();

//�X�N���[����������(�ꕶ��)
//����...0
//���s...-1
//�͈͊O�ɏ����������Ƃ����Ƃ��A�܂��́A�k�������������������Ƃ����Ƃ��A���s����
extern int WriteScreen(int x, int y, char c);

//�X�N���[����������(������)
//����...0
//���s...-1
//�����񂪒������ăX�N���[���Ɏ��܂�Ȃ��Ƃ��A���s����
extern int WriteScreen(int x, int y, const char *s);

//�X�N���[���`��
extern void DrawScreen();

//���W����C���f�b�N�X�ɕϊ�
//�C���f�b�N�X��Ԃ�
extern int ToScreenIndex(int x, int y);

//�C���f�b�N�X������W�ɕϊ�
//���W��X�l��Ԃ��AY�l���|�C���^�o�R�ŕԂ�
extern int ToScreenCoord(int index, int *y);

//�X�N���[���͈͊O�`�F�b�N
//�͈͊O�̂Ƃ��^
extern bool OutOfScreen(int x, int y);

//�X�N���[���͈͊O�`�F�b�N
//�͈͊O�̂Ƃ��^
extern bool OutOfScreen(Rect rect);

//���ԑ҂�
//�~���b�Ŏw�肷��
extern void Wait(int millisec);