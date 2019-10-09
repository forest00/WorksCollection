#ifndef __INIT_GRAPH_NO_SYS_H__
#define __INIT_GRAPH_NO_SYS_H__
#pragma once
#include "amgame.h"
// �O���t�B�b�N�X�̏�����(sys�t�@�C���Ȃ��œ���)
extern HWND InitGraphNoSys(HINSTANCE hInstance) {
	int					nResult;								// ���U���g�R�[�h
	int					nPresDebugMode;							// ���݂̃f�o�b�O���[�h

	AM_SetAppliInstance(hInstance);							// �A�v���C���X�^���X�̐ݒ�

															// �f�B�X�v���C������
	nResult = AM_InitDisplay();
	if (nResult != AM_OK) {									// ���������s�̏ꍇ
		AM_EndDisplay();
		return NULL;
	}

	AM_Init3DGraph();											// 3D�O���t�B�b�N������

	AM_InitBlendState();										// �u�����h�X�e�[�g������

	AM_PrepareFontObject();										// ������t�H���g�̏���

																// �f�o�b�O�@�\�}��
	nPresDebugMode = GetAMDebugMode();							// ���݂̃f�o�b�O���[�h��ۑ�
	SetAMDebugMode(AMDEBUG_ALL_OFF);							// �f�o�b�O�@�\�n�e�e
#if 0
	int							hTitle;							// �^�C�g��

	hTitle = LoadTexture(AM_TITLE_NAME);
	if (hTitle < 0) {
		if (gg_bFullMode) {
			DrawString(0, 0, "Amgamew.sys������܂���I�I", GetColor(255, 0, 128));
			Flip();
			GetCh();
		} else {
			MessageBox(gg_hWindow, "Amgamew.sys������܂���B", "Error", MB_OK);
		}
		EndGraph();
		return NULL;
	}

	int							i, j;
	int							nTitleWidth, nTitleHeight;
	int							nTitleX, nTitleY;
	float						fYLoc;

	GetPictureSize(hTitle, &nTitleWidth, &nTitleHeight);
	nTitleX = (gg_siDispSize.cx - nTitleWidth) / 2;
	nTitleY = (gg_siDispSize.cy - nTitleHeight) / 2;

	AmVertex		avVertices[480 * 2];			// ���_

													// ���_���̏������i���X�^�X�N���[���p�j
	for (i = 0; i < 480; i++) {
		SetColorVertex(&avVertices[i * 2], 0.0f, (gg_siDispSize.cy - 480.0f) / 2.0f + i, 0.0f,
			0.5f, i * nTitleHeight / 482.0f + 0.5f,
			0xff, 0xff, 0xff, 0xff);
		SetColorVertex(&avVertices[i * 2 + 1], 640.0f, (gg_siDispSize.cy - 480.0f) / 2.0f + i, 0.0f,
			(float)nTitleWidth - 1.0f, i * nTitleHeight / 482.0f + 0.5f,
			0xff, 0xff, 0xff, 0xff);
	}
	SetDrawMode(AMDRAW_BILINEAR, 0);

	for (i = 0; i <= 1000; i += 20) {
		AM_ClearBackBuffer(255, 255, 255);
		fYLoc = (800.0f - i) * (800.0f - i) * nTitleHeight * 0.2f / (200.0f * 200.0f) - nTitleHeight * 0.2f;
		for (j = 0; j < 480; j++) {
			avVertices[j * 2].x = nTitleX - (j - 240.0f) * (j - 240.0f) / (240.0f * 240.0f) * 1000.0f;
			avVertices[j * 2 + 1].x = nTitleX + nTitleWidth + (j - 240.0f) * (j - 240.0f) / (240.0f * 240.0f) * 1000.0f;
			avVertices[j * 2].v = -fYLoc + j * nTitleHeight / 482.0f + 0.5f;
			if (avVertices[j * 2].v > nTitleHeight - 1.0f) avVertices[j * 2].v = nTitleHeight - 1.0f;
			avVertices[j * 2 + 1].v = avVertices[j * 2].v;
		}
		Draw3DLines(avVertices, 480 * 2, hTitle);	// ���`��
		Flip();
	}

	WaitByTimer(500);

	for (i = 0; i <= 1000; i += 30) {
		AM_ClearBackBuffer(255, 255, 255);
		for (j = 0; j < 480; j++) {
			avVertices[j * 2].x = nTitleX - (j - 240.0f) * (j - 240.0f) / (240.0f * 240.0f)
				* (1000 - i) * 700.0f / 1000.0f;
			avVertices[j * 2 + 1].x = nTitleX + nTitleWidth + (j - 240.0f) * (j - 240.0f) / (240.0f * 240.0f)
				* (1000 - i) * 700.0f / 1000.0f;
			avVertices[j * 2].v = (j - 240) * (nTitleHeight / 482.0f * (1000.0f - i) + i) / 1000.0f + nTitleHeight / 2 + 0.5f;
			if (avVertices[j * 2].v > nTitleHeight - 1.0f) avVertices[j * 2].v = nTitleHeight - 1.0f;
			avVertices[j * 2 + 1].v = avVertices[j * 2].v;
		}
		Draw3DLines(avVertices, 480 * 2, hTitle);	// ���`��
		Flip();
	}

	AM_ClearBackBuffer(255, 255, 255);
	SetDrawMode(AMDRAW_NEAREST, 0);
	DrawMem(nTitleX, nTitleY, hTitle);
	Flip();
	if (!KBHit()) WaitByTimer(2000);
	else GetCh();

	for (i = 1000; i >= 0; i -= 20) {
		AM_ClearBackBuffer(i * 255 / 1000, i * 255 / 1000, i * 255 / 1000);
		SetDrawBright(i, i, i);
		DrawMem(nTitleX, nTitleY, hTitle);
		Flip();
	}
#endif

	SetDrawBright(1000, 1000, 1000);
	SetDrawMode(AMDRAW_NEAREST, 0);

	AB_Release2DGraphs();										// �摜�������J��
	nResult = AM_ClearBackBuffer(0, 0, 0);					// ��ʍX�V����

	AM_InitJoyPad(AM_MAX_PAD_NUM);							// �p�b�h������

	if (KBHit()) GetCh();										// �L�[�o�b�t�@�N���A

	RestoreAMDebugMode(nPresDebugMode);						// �f�o�b�O���[�h���A
	DeleteFile(AB_DEBUG_FILE_NAME);							// �f�o�b�O�t�@�C������

	SetFrameCounter(1);										// �t���[���J�E���^�Z�b�g

	return gg_hWindow;											// �E�B���h�E�n���h����Ԃ�
}
#endif
