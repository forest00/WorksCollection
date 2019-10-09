// ---------------------------------------------------------------------------
//
// �t�@�C����: AB_DispEnv.cpp
//
// �@�\: �`�l�f�`�l�d�̉�ʊ��ݒ���u���b�W�B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif

#pragma warning( disable : 4819 )						// �R�[�h�y�[�W�̌x���}�~


// ---------------------------------------------------------------------------
// �C���N���[�h��

#include <windows.h>
#include <string.h>

#include "amgame.h"												// AMGAME�w�b�_

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// ��ʏ������@�@��ˑ����C���[
#include "AB_DispEnv.h"											// ��ʏ������@�u���b�W
#include "AM_2DGraph.h"											// 2D�`��@�@��ˑ����C���[
#include "AB_2DGraph.h"											// 2D�`��@�u���b�W
#include "AM_3DGraph.h"											// 3D�`��@�@��ˑ����C���[
#include "AM_FigureDraw.h"										// 2D�}�`�`��@�@��ˑ����C���[
#include "AM_Movie.h"											// ���[�r�[�Đ��@�@��ˑ����C���[
#include "AB_Movie.h"											// ���[�r�[�Đ��@�u���b�W
#include "AM_Input.h"											// �L�[���́@�@��ˑ����C���[
#include "AB_Debug.h"											// �f�o�b�O�@�u���b�W

#endif


// ---------------------------------------------------------------------------
// �t�@�C�����O���[�o���ϐ���

int								fg_nFrameCount = 0;				// �t���[���J�E���^


//-----------------------------------------------------------------------------
// ��ʐ���֐���

// �O���t�B�b�N�X�̏�����
extern HWND InitGraph( HINSTANCE hInstance )
{
	int					nResult;								// ���U���g�R�[�h
	int					nPresDebugMode;							// ���݂̃f�o�b�O���[�h

	AM_SetAppliInstance( hInstance );							// �A�v���C���X�^���X�̐ݒ�

	// �f�B�X�v���C������
	nResult = AM_InitDisplay();
	if ( nResult != AM_OK ) {									// ���������s�̏ꍇ
		AM_EndDisplay();
		return NULL;
	}

	AM_Init3DGraph();											// 3D�O���t�B�b�N������

	AM_InitBlendState();										// �u�����h�X�e�[�g������

	AM_PrepareFontObject();										// ������t�H���g�̏���

	// �f�o�b�O�@�\�}��
	nPresDebugMode = GetAMDebugMode();							// ���݂̃f�o�b�O���[�h��ۑ�
	SetAMDebugMode( AMDEBUG_ALL_OFF );							// �f�o�b�O�@�\�n�e�e

	int							hTitle;							// �^�C�g��

	hTitle = LoadTexture( AM_TITLE_NAME );
	if ( hTitle < 0 ) {
		if ( gg_bFullMode ) {
			DrawString( 0, 0, "Amgamew.sys������܂���I�I", GetColor( 255, 0, 128 ) );
			Flip();
			GetCh();
		}
		else {
			MessageBox( gg_hWindow, "Amgamew.sys������܂���B", "Error", MB_OK );
		}
		EndGraph();
		return NULL;
	}

	int							i, j;
	int							nTitleWidth, nTitleHeight;
	int							nTitleX, nTitleY;
	float						fYLoc;

	GetPictureSize( hTitle, &nTitleWidth, &nTitleHeight );
	nTitleX = ( gg_siDispSize.cx - nTitleWidth  ) / 2;
	nTitleY = ( gg_siDispSize.cy - nTitleHeight ) / 2;

	AmVertex		avVertices[480 * 2];			// ���_

	// ���_���̏������i���X�^�X�N���[���p�j
	for ( i = 0; i < 480; i++ ) {
		SetColorVertex( &avVertices[i * 2], 0.0f, ( gg_siDispSize.cy - 480.0f ) / 2.0f + i, 0.0f,
											0.5f, i * nTitleHeight / 482.0f + 0.5f,
											0xff, 0xff, 0xff, 0xff );
		SetColorVertex( &avVertices[i * 2 + 1], 640.0f, ( gg_siDispSize.cy - 480.0f ) / 2.0f + i, 0.0f,
												( float )nTitleWidth - 1.0f, i * nTitleHeight / 482.0f + 0.5f,
												0xff, 0xff, 0xff, 0xff );
	}
	SetDrawMode( AMDRAW_BILINEAR, 0 );

	for ( i = 0; i <= 1000; i += 20 ) {
		AM_ClearBackBuffer( 255, 255, 255 );
		fYLoc = ( 800.0f - i ) * ( 800.0f - i ) * nTitleHeight * 0.2f / ( 200.0f * 200.0f ) - nTitleHeight * 0.2f;
		for ( j = 0; j < 480; j++ ) {
			avVertices[j * 2    ].x = nTitleX               - ( j - 240.0f ) * ( j - 240.0f ) / ( 240.0f * 240.0f ) * 1000.0f;
			avVertices[j * 2 + 1].x = nTitleX + nTitleWidth + ( j - 240.0f ) * ( j - 240.0f ) / ( 240.0f * 240.0f ) * 1000.0f;
			avVertices[j * 2    ].v = -fYLoc + j * nTitleHeight / 482.0f + 0.5f;
			if ( avVertices[j * 2    ].v > nTitleHeight - 1.0f ) avVertices[j * 2    ].v = nTitleHeight - 1.0f;
			avVertices[j * 2 + 1].v = avVertices[j * 2    ].v;
		}
		Draw3DLines( avVertices, 480 * 2, hTitle );	// ���`��
		Flip();
	}

	WaitByTimer( 500 );

	for ( i = 0; i <= 1000; i += 30 ) {
		AM_ClearBackBuffer( 255, 255, 255 );
		for ( j = 0; j < 480; j++ ) {
			avVertices[j * 2    ].x = nTitleX               - ( j - 240.0f ) * ( j - 240.0f ) / ( 240.0f * 240.0f )
									* ( 1000 - i ) * 700.0f / 1000.0f;
			avVertices[j * 2 + 1].x = nTitleX + nTitleWidth + ( j - 240.0f ) * ( j - 240.0f ) / ( 240.0f * 240.0f )
									* ( 1000 - i ) * 700.0f / 1000.0f;
			avVertices[j * 2    ].v = ( j - 240 ) * ( nTitleHeight / 482.0f * ( 1000.0f - i ) + i ) / 1000.0f + nTitleHeight / 2 + 0.5f;
			if ( avVertices[j * 2    ].v > nTitleHeight - 1.0f ) avVertices[j * 2    ].v = nTitleHeight - 1.0f;
			avVertices[j * 2 + 1].v = avVertices[j * 2    ].v;
		}
		Draw3DLines( avVertices, 480 * 2, hTitle );	// ���`��
		Flip();
	}

	AM_ClearBackBuffer( 255, 255, 255 );
	SetDrawMode( AMDRAW_NEAREST, 0 );
	DrawMem( nTitleX, nTitleY, hTitle );
	Flip();
	if ( !KBHit() ) WaitByTimer( 2000 );
			   else GetCh();

	for ( i = 1000; i >= 0; i -= 20 ) {
		AM_ClearBackBuffer( i * 255 / 1000, i * 255 / 1000, i * 255 / 1000 );
		SetDrawBright( i, i, i );
		DrawMem( nTitleX, nTitleY, hTitle );
		Flip();
	}

	SetDrawBright( 1000, 1000, 1000 );
	SetDrawMode( AMDRAW_NEAREST, 0 );

	AB_Release2DGraphs();										// �摜�������J��
	nResult = AM_ClearBackBuffer( 0, 0, 0 );					// ��ʍX�V����

	AM_InitJoyPad( AM_MAX_PAD_NUM );							// �p�b�h������

	if ( KBHit() ) GetCh();										// �L�[�o�b�t�@�N���A

	RestoreAMDebugMode( nPresDebugMode );						// �f�o�b�O���[�h���A
	DeleteFile( AB_DEBUG_FILE_NAME );							// �f�o�b�O�t�@�C������

	SetFrameCounter( 1 );										// �t���[���J�E���^�Z�b�g

	return gg_hWindow;											// �E�B���h�E�n���h����Ԃ�
}


// �O���t�B�b�N�X�g�p�I��
extern int EndGraph( void )
{
	int					nResult;								// ���U���g�R�[�h

	AM_EndJoyPad();												// �p�b�h�g�p�I��
	AM_EndMouse();												// �}�E�X�g�p�I��

//	AM_StopMovie();												// ���[�r�[�Đ��X�g�b�v
	InitMovieMem();												// ���[�r�[�������J��

	AB_Release2DGraphs();										// 2D�O���t�B�b�N�J��

	AM_ReleaseFontObject();										// ������t�H���g�̊J��

	AM_End3DGraph();											// 3D�O���t�B�b�N�g�p�I��

	nResult = AM_EndDisplay();									// �f�B�X�v���C�g�p�I��

	return nResult;
}


// ���ˑ��O���[�o�����[�h�ݒ�
extern int SetGlobalMode( int nModes )
{
	int							nResult = AM_OK;				// ����

	switch ( nModes ) {
	case AMMODE_WINDOWED:										// �E�B���h�E���[�h�ɐݒ�
		if ( !gg_bDispInitialized ) {							// ��ʏ��������܂�
			gg_bFullMode = false;
		}
		nResult = AM_OK;
		break;
	}

	return nResult;
}


// �C�x���g����
extern int ProcessEvent( void )
{
	int					nResult;								// ���U���g�R�[�h

	nResult = AM_ProcessEvent();								// �C�x���g����

	return nResult;
}


// �O���t�B�b�N���[�h�ݒ�
extern int SetGraphMode( int nWidth, int nHeight, int nBitDepth )
{
	int					nResult;								// ���U���g�R�[�h

	nResult = AM_SetDisplayMode( nWidth, nHeight, nBitDepth );	// �𑜓x�ݒ�

	return nResult;
}


// �y�[�W�ݒ�i�_�~�[�j
extern void SetPage( int nActivePage, int nDisplayPage )
{

	return;
}


// ��ʍX�V����
extern int Flip( void )
{
	int					nResult;								// ���U���g�R�[�h

	// �f�o�b�O���̕\��
	if ( gg_bDisplayDebug ) {
		AB_DisplayDebugInfo();									// �f�o�b�O���\��
	}

	nResult = AM_Flip();										// ��ʍX�V����

	// �f�o�b�O�L�[�E�|�[�Y�R���g���[��
	if ( gg_bDebugKey ) {
		AB_ControlDebugPause();									// �f�o�b�O�L�[�E�|�[�Y�R���g���[��
	}

	fg_nFrameCount++;

	return nResult;
}


// �t���[���J�E���^�Z�b�g
extern int SetFrameCounter( int nCount )
{
	int							nResult;						// ���U���g�R�[�h

	nResult = fg_nFrameCount;
	fg_nFrameCount = nCount;

	return nResult;
}


// �t���[���J�E���^�擾
extern int GetFrameCounter( void )
{
	return fg_nFrameCount;
}


//	VRAM�N���A
extern void ClearVram( void )
{
	int					nResult;								// ���U���g�R�[�h

	nResult = AM_ClearBackBuffer( 0, 0, 0 );					// ��ʍX�V����

	return;
}


// ��ʃN���A
extern int HCls( void )
{
	int					nResult;								// ���U���g�R�[�h

	nResult = AM_ClearBackBuffer( 0, 0, 0 );					// ��ʍX�V����

	return nResult;
}


// �f�B�X�v���C�̐��������M���҂�
extern void WaitVSync( int nWaitNum )
{
	int					i;
	int					nResult;								// ���U���g�R�[�h

	if ( nWaitNum <= 0 ) return;								// �񐔃`�F�b�N

	for ( i = 0; i < nWaitNum; i++ ) {
		nResult = AM_WaitVBlankTrigger();						// VBlank�g���K�҂�
	}

	return;
}


// �`��G���A�̐ݒ�
extern void SetDrawArea( int x1, int y1, int x2, int y2 )
{
	int							nTemp;

	// �召�֌W�␳
	if ( x1 > x2 ) { nTemp = x1;  x1 = x2;  x2 = nTemp; }
	if ( y1 > y2 ) { nTemp = y1;  y1 = y2;  y2 = nTemp; }

	// �T�`���[�V����
	if ( x1 < 0 ) x1 = 0;
	if ( y1 < 0 ) y1 = 0;
	if ( x2 > gg_siDispSize.cx ) x2 = gg_siDispSize.cx;
	if ( y2 > gg_siDispSize.cy ) y2 = gg_siDispSize.cy;

	// �r���[�|�[�g�ݒ�
	AM_SetViewport( x1, y1, x2, y2 );

	return;
}


// �E�B���h�E�^�C�g���̐ݒ�i����J�֐��j
extern int AB_SetWindowTitle( char *szTitle )
{
	strncpy( gg_szWinTitle, szTitle, AM_MAX_WIN_TITLE );		// �^�C�g���R�s�[
	gg_szWinTitle[AM_MAX_WIN_TITLE - 1] = 0;					// �K���I�[�����悤

	return AM_OK;
}


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AB_2DGraph.cpp
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̂Q�c��ʕ`��@�\���u���b�W�B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif

#pragma warning( disable : 4819 )						// �R�[�h�y�[�W�̌x���}�~


// ---------------------------------------------------------------------------
// �C���N���[�h��

#include <math.h>

#include "amgame.h"												// AMGAME�w�b�_

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnv�w�b�_
#include "AB_Movie.h"											// AMMovie�w�b�_
#include "AB_Debug.h"											// ABDebug�w�b�_
#include "AM_2DGraph.h"											// AM2DGraph�w�b�_
#include "AB_2DGraph.h"											// AB2DGraph�w�b�_

#endif


// ---------------------------------------------------------------------------
// �t�@�C�����萔��

#define MAX_UNIT_PICTURES		2000							// �ő�P�ʃs�N�`����
#define MAX_USER_PICTURES		5000							// �ő僆�[�U�s�N�`����

#define MAX_DIRECTPIC_NAME		300								// �ő咼�ڃ��[�h�摜��


// ---------------------------------------------------------------------------
// �t�@�C�����O���[�o���ϐ���

// �摜�o�b�t�@�֌W
AM_UNIT_PICTURE					fg_UnitPictureBuf[MAX_UNIT_PICTURES];	// �P�ʃs�N�`���o�b�t�@
int								fg_nUnitPicNum = 0;						// �P�ʃs�N�`����
AM_USER_PICTURE					fg_UserPictureBuf[MAX_USER_PICTURES];	// ���[�U�s�N�`���o�b�t�@
int								fg_nUserPicNum = 0;						// ���[�U�s�N�`����

// �摜���ڃ��[�h�֌W
int								hDirectPic = -1;						// ���ډ摜�n���h��
char							szDirectName[MAX_DIRECTPIC_NAME];		// ���ډ摜�t�@�C����


// ---------------------------------------------------------------------------
// �t�@�C�����֐��v���g�^�C�v�錾��

static int AB_LoadBlockedTexture( char *szFileName, int nXSize, int nYSize,
								  int nXNum, int nYNum, int nAllNum, int *phHandleBuf, int bFlags );
																// �u���b�N��2D�O���t�B�b�N���[�h(������)
static AM_UNIT_PICTURE *AB_MakeUnitPicture( AM_LP2DPICTUREBUFFER ppbPicture, AM_2DPIC_INFO *ppiPicInfo, char *szFileName );
																// �V�K�P�ʃs�N�`��
static int AB_Make2DHandle( AM_UNIT_PICTURE *pUnitPic, int nLeft, int nTop, int nWidth, int nHeight, int bFlags );
																// �V�K�G�n���h��
static int AB_DirectLoad2DGraph( int x, int y, char *szFileName, int nConds );	// 2D�O���t�B�b�N�[�����ڃ��[�h

static int AM_DrawUserPicRotCond( int nCenX1, int nCenY1, int nCenX2, int nCenY2,
								  double fAngle, double fExRate,
								  AM_USER_PICTURE *pUserPicture, int bConds );
																// 2D�����t����]�`��(���E���]�����t��)

static int AM_DrawPictureCond( int dx1, int dy1, int dx2, int dy2,
							   int sx1, int sy1, int sx2, int sy2,
							   AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo, int bConds );
																// �����t���`��
static int AM_DrawPictureCond4_2( AM_2DVECTOR *pDestVerts,
								  int sx1, int sy1, int sx2, int sy2,
								  AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo, int bConds );
																// �����t���`��i�`���S�_�A�`�挳�Q�_�w��j


//-----------------------------------------------------------------------------
// 2D�摜���[�h�֐���

// 2D�O���t�B�b�N���[�h(��)
extern int LoadBmpMem( char *szFileName )
{
	return LoadTexture( szFileName );
}


// 2D�O���t�B�b�N���[�h
extern int LoadTexture( char *szFileName )
{
	AM_LP2DPICTUREBUFFER		ppbPicture;								// Buffer to hold 2D Picture
	AM_2DPIC_INFO				piPicInfo;
	AM_UNIT_PICTURE				*pUnitPic;								// �P�ʃs�N�`��
	int							hPicture;								// �s�N�`���n���h��
	AB_LOADING_DEBUG_INFO		ldDebugInfo;							// �f�o�b�O���

	ppbPicture = AM_Load2DGraph( szFileName, &piPicInfo );				// �O���t�B�b�N���[�h
	// �G���[����
	if ( !ppbPicture ) {
		if ( gg_bDebugFile ) {
			ldDebugInfo.nObjectKind = AB_DEBUG_LOADTEXTURE;
			ldDebugInfo.szFileName  = szFileName;
			ldDebugInfo.nMadeHandle = AMERR_NOFILE;
			ldDebugInfo.bError = true;
			AB_OutLoadingDebugInfo( &ldDebugInfo );
		}
		return AMERR_NOFILE;
	}

	pUnitPic = AB_MakeUnitPicture( ppbPicture, &piPicInfo, szFileName );	// �P�ʃs�N�`���o�^
	if ( !pUnitPic ) return AMERR_NOFREESURFACE;

	hPicture = AB_Make2DHandle( pUnitPic, 0, 0, piPicInfo.nWidth, piPicInfo.nHeight, 0 );	// �n���h���쐬

	// �f�o�b�O���o��
	if ( gg_bDebugFile ) {
		ldDebugInfo.nObjectKind = AB_DEBUG_LOADTEXTURE;
		ldDebugInfo.szFileName  = szFileName;
		ldDebugInfo.nMadeHandle = hPicture;
		ldDebugInfo.bError = false;
		AB_OutLoadingDebugInfo( &ldDebugInfo );
	}

	return hPicture;
}


// �u���b�N��2D�O���t�B�b�N���[�h(��)
extern int LoadBlkBmpMem( char *szFileName, int nXSize, int nYSize,
						  int nXNum, int nYNum, int nAllNum, int *phHandleBuf )
{
	return AB_LoadBlockedTexture( szFileName, nXSize, nYSize,
								  nXNum, nYNum, nAllNum, phHandleBuf, 0 );
}


// �u���b�N��2D�O���t�B�b�N���[�h(�V)
extern int LoadBlkTexture( char *szFileName, int nXSize, int nYSize,
						   int nXNum, int nYNum, int nAllNum, int *phHandleBuf )
{
	return AB_LoadBlockedTexture( szFileName, nXSize, nYSize,
								  nXNum, nYNum, nAllNum, phHandleBuf, 0 );
}


// �u���b�N��2D�O���t�B�b�N���[�h(���A���E���])
extern int LoadBlkBmpMemLR( char *szFileName, int nXSize, int nYSize,
							int nXNum, int nYNum, int nAllNum, int *phHandleBuf )
{
	return AB_LoadBlockedTexture( szFileName, nXSize, nYSize,
								  nXNum, nYNum, nAllNum, phHandleBuf, AM_PICTURE_LR );
}

// �u���b�N��2D�O���t�B�b�N���[�h(���E���])
extern int LoadBlkTextureLR( char *szFileName, int nXSize, int nYSize,
							 int nXNum, int nYNum, int nAllNum, int *phHandleBuf )
{
	return AB_LoadBlockedTexture( szFileName, nXSize, nYSize,
								  nXNum, nYNum, nAllNum, phHandleBuf, AM_PICTURE_LR );
}


// �u���b�N��2D�O���t�B�b�N���[�h(������)
static int AB_LoadBlockedTexture( char *szFileName, int nXSize, int nYSize,
								  int nXNum, int nYNum, int nAllNum, int *phHandleBuf, int bFlags )
{
	int							i, j;
	int							x, y;									// �u���b�N�̍�����W
	int							nBlockNum;								// �u���b�N��
	AM_LP2DPICTUREBUFFER		ppbPicture;								// Buffer to hold 2D Picture
	AM_2DPIC_INFO				piPicInfo;
	AM_UNIT_PICTURE				*pUnitPic;								// �P�ʃs�N�`��
	AB_LOADING_DEBUG_INFO		ldDebugInfo;							// �f�o�b�O���

	ppbPicture = AM_Load2DGraph( szFileName, &piPicInfo );				// �O���t�B�b�N���[�h
	// �G���[����
	if ( !ppbPicture ) {
		if ( gg_bDebugFile ) {
			if ( bFlags | AM_PICTURE_LR ) ldDebugInfo.nObjectKind = AB_DEBUG_LOADBLKTEXTURELR;
									 else ldDebugInfo.nObjectKind = AB_DEBUG_LOADBLKTEXTURE;
			ldDebugInfo.szFileName  = szFileName;
			ldDebugInfo.nMadeHandle = AMERR_NOFILE;
			ldDebugInfo.bError = true;
			AB_OutLoadingDebugInfo( &ldDebugInfo );
		}
		return AMERR_NOFILE;
	}
//	if ( !ppbPicture ) return AMERR_NOFILE;

	pUnitPic = AB_MakeUnitPicture( ppbPicture, &piPicInfo, szFileName );	// �P�ʃs�N�`���o�^
	if ( !pUnitPic ) return AMERR_NOFREESURFACE;

	if ( nXSize > piPicInfo.nWidth || nYSize > piPicInfo.nHeight ) return AMERR_INVALIDSIZE;	// 1�u���b�N�����G���������ꍇ
	nBlockNum = 0;														// �����u���b�N��
	y = 0;
	for ( i = 0; i < nYNum; i++ ) {
		x = 0;
		for ( j = 0; j < nXNum; j++ ) {
			if ( 1 ) {//( x + nXSize <= piPicInfo.nWidth ) && ( y + nYSize <= piPicInfo.nHeight ) ) {	// �u���b�N���G�̊O�Ɋ|����Ȃ�
				*( phHandleBuf + nBlockNum ) = AB_Make2DHandle( pUnitPic, x, y, nXSize, nYSize, 0 );	// �n���h���쐬
				nBlockNum++;
				if ( bFlags & AM_PICTURE_LR ) {
					*( phHandleBuf + nBlockNum ) = AB_Make2DHandle( pUnitPic, x, y, nXSize, nYSize, AM_PICTURE_LR );	// �n���h���쐬
					nBlockNum++;
				}
			}
			else {														// �u���b�N���G�̊O�Ɋ|����
				if ( bFlags & AM_PICTURE_LR ) {							// ���E���]���肩
					*( phHandleBuf + nBlockNum ) = *( phHandleBuf + nBlockNum - 2 );	// �ӂ��O�R�s�[
					*( phHandleBuf + nBlockNum + 1 ) = *( phHandleBuf + nBlockNum - 1 );	// �ӂ��O�R�s�[
					nBlockNum += 2;
				}
				else {
					*( phHandleBuf + nBlockNum ) = *( phHandleBuf + nBlockNum - 1 );	// �ЂƂO�R�s�[
					nBlockNum++;
				}
			}
			// �K�萔�܂ŒB���Ă���ΏI��
			if ( ( !( bFlags & AM_PICTURE_LR ) && nBlockNum >= nAllNum     ) ||
				 (  ( bFlags & AM_PICTURE_LR ) && nBlockNum >= nAllNum * 2 ) )
			{
				break;
			}
			x += nXSize;
		}
		y += nYSize;
	}

	// �f�o�b�O���o��
	if ( gg_bDebugFile ) {
		if ( bFlags & AM_PICTURE_LR ) ldDebugInfo.nObjectKind = AB_DEBUG_LOADBLKTEXTURELR;
								 else ldDebugInfo.nObjectKind = AB_DEBUG_LOADBLKTEXTURE;
		ldDebugInfo.szFileName  = szFileName;
		ldDebugInfo.nMadeHandle = *phHandleBuf;
		ldDebugInfo.nMadeHandleEnd = *( phHandleBuf + nBlockNum - 1 );
		ldDebugInfo.bError = false;
		AB_OutLoadingDebugInfo( &ldDebugInfo );
	}

	return AM_OK;
}


// �V�K�P�ʃs�N�`��
static AM_UNIT_PICTURE *AB_MakeUnitPicture( AM_LP2DPICTUREBUFFER ppbPicture, AM_2DPIC_INFO *ppiPicInfo, char *szFileName )
{
	if ( fg_nUnitPicNum >= MAX_UNIT_PICTURES ) return NULL;				// �󂫃`�F�b�N

	fg_UnitPictureBuf[fg_nUnitPicNum].a2PicInfo = *ppiPicInfo;			// �G���
	fg_UnitPictureBuf[fg_nUnitPicNum].pPicture  = ppbPicture;			// �G�I�u�W�F�N�g
	strncpy( fg_UnitPictureBuf[fg_nUnitPicNum].szFileName, szFileName, AM_MAX_PICTURE_NAME - 1 );	// �G�t�@�C����
	fg_UnitPictureBuf[fg_nUnitPicNum].szFileName[AM_MAX_PICTURE_NAME - 1] = 0;	// �t�@�C�����̏I�[���m���ɒu��
	fg_nUnitPicNum++;

	return &fg_UnitPictureBuf[fg_nUnitPicNum - 1];
}


// �V�K�G�n���h��
static int AB_Make2DHandle( AM_UNIT_PICTURE *pUnitPic, int nLeft, int nTop, int nWidth, int nHeight, int bFlags )
{
	int							hPicture = AM_PIC2D_HANDLE_HEAD;

	if ( fg_nUserPicNum >= MAX_USER_PICTURES ) return AMERR_NOFREEHANDLE;	// �n���h���󂫃`�F�b�N

	fg_UserPictureBuf[fg_nUserPicNum].pUnitPic = pUnitPic;
	fg_UserPictureBuf[fg_nUserPicNum].nLeft = nLeft;
	fg_UserPictureBuf[fg_nUserPicNum].nTop = nTop;
	fg_UserPictureBuf[fg_nUserPicNum].nWidth = nWidth;
	fg_UserPictureBuf[fg_nUserPicNum].nHeight = nHeight;
	fg_UserPictureBuf[fg_nUserPicNum].bFlags = bFlags;
	fg_nUserPicNum++;

	hPicture |= fg_nUserPicNum - 1;										// �n���h������

	return hPicture;
}


// 2D�O���t�B�b�N�o�b�t�@�쐬
extern int MakeGraphMem( int nWidth, int nHeight )
{
	return MakeTextureMem( nWidth, nHeight, 0 );
}


// 2D�O���t�B�b�N�o�b�t�@�쐬
extern int MakeTextureMem( int nWidth, int nHeight, int nBitDepth )
{
	AM_LP2DPICTUREBUFFER		ppbPicture;								// Buffer to hold 2D Picture
	AM_2DPIC_INFO				piPicInfo;
	AM_UNIT_PICTURE				*pUnitPic;								// �P�ʃs�N�`��
	int							hPicture;								// �s�N�`���n���h��

	// �G�\���̃f�[�^�Z�b�g
	piPicInfo.nWidth = nWidth;  piPicInfo.nHeight = nHeight;			// ���E����
	if ( nBitDepth == 0 ) nBitDepth = gg_nBitDepth;						// �r�b�g�f�v�X
	piPicInfo.nBitDepth = nBitDepth;

	ppbPicture = AM_Make2DGraphBuf( &piPicInfo );						// �O���t�B�b�N���[�h
	if ( !ppbPicture ) return AMERR_NOMEMORY;

	pUnitPic = AB_MakeUnitPicture( ppbPicture, &piPicInfo, "User-Made(MakeTextureMem)" );	// �P�ʃs�N�`���o�^
	if ( !pUnitPic ) return AMERR_NOFREESURFACE;

	hPicture = AB_Make2DHandle( pUnitPic, 0, 0, piPicInfo.nWidth, piPicInfo.nHeight, 0 );	// �n���h���쐬

	return hPicture;
}


// 2D�O���t�B�b�N�ă��[�h
extern int ReloadGraph( char *szFileName, int hPicture )
{
	int							nResult;								// ���U���g�R�[�h
	int							nUserPicLoc;							// ���[�U�[�G�ʒu
	AM_USER_PICTURE				*pupUserPic;							// ���[�U�[�G
//	AM_LP2DPICTUREBUFFER		ppbPicture;								// Buffer to hold 2D Picture
//	AM_2DPIC_INFO				piPicInfo;
//	AM_UNIT_PICTURE				*pUnitPic;								// �P�ʃs�N�`��
//	int							hPicture;								// �s�N�`���n���h��
	AB_LOADING_DEBUG_INFO		ldDebugInfo;							// �f�o�b�O���

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;						// �ʒu
	// �Ó����`�F�b�N
	if ( nUserPicLoc >= fg_nUserPicNum ) {
		if ( gg_bDebugFile ) {
			ldDebugInfo.nObjectKind = AB_DEBUG_RELOADGRAPH;
			ldDebugInfo.szFileName  = szFileName;
			ldDebugInfo.nMadeHandle = hPicture;
			ldDebugInfo.bError = true;
			AB_OutLoadingDebugInfo( &ldDebugInfo );
		}
		return AMERR_INVALIDHANDLE;
	}

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];

	nResult = AM_Reload2DGraph( szFileName, pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ) );

	// �f�o�b�O���o��
	if ( gg_bDebugFile ) {
		ldDebugInfo.nObjectKind = AB_DEBUG_RELOADGRAPH;
		ldDebugInfo.szFileName  = szFileName;
		ldDebugInfo.nMadeHandle = hPicture;
		ldDebugInfo.bError = nResult < 0 ? true : false;
		AB_OutLoadingDebugInfo( &ldDebugInfo );
	}

	return nResult;
}


// 2D�O���t�B�b�N�[�����ڃ��[�h
extern int LoadBmp( int x, int y, char *szFileName )
{
	int							nResult;

	nResult = AB_DirectLoad2DGraph( x, y, szFileName, 0 );

	return nResult;
}
extern int LoadBmpNP( int x, int y, char *szFileName )
{
	return LoadBmp( x, y, szFileName );
}


// 2D�O���t�B�b�N�[�����ڃ��[�h(�J���[�L�[�t��)
extern int LoadBmpTh( int x, int y, char *szFileName )
{
	int							nResult;

	nResult = AB_DirectLoad2DGraph( x, y, szFileName, AM_DRAWPIC_PICALPHA );

	return nResult;
}
extern int LoadBmpThNP( int x, int y, char *szFileName )
{
	return LoadBmpTh( x, y, szFileName );
}


// 2D�O���t�B�b�N�[�����ڃ��[�h(������)
static int AB_DirectLoad2DGraph( int x, int y, char *szFileName, int nConds )
{
	if ( hDirectPic < 0 ) {												// ���߂Ē��ڃ��[�h
		hDirectPic = LoadTexture( szFileName );							// �摜���[�h
		strcpy( szDirectName, szFileName );								// ���O�L�^
	}
	else {																// 2��ڈȍ~
		if ( strcmp( szDirectName, szFileName ) != 0 ) {				// �O��Ɠ����łȂ�
			ReloadGraph( szFileName, hDirectPic );						// �����[�h
			strcpy( szDirectName, szFileName );							// ���O�L�^
		}
	}

	// �`��
	if ( nConds == 0 ) {												// �m�[�}��
		DrawMem( x, y, hDirectPic );
	}
	else {																// �J���[�L�[�t��
		DrawMemTh( x, y, hDirectPic );
	}

	return hDirectPic;
}


// �S�r�b�g�}�b�v�J��
extern int InitGraphMem( void )
{
	return AB_Release2DGraphs();
}


// �S�e�N�X�`���J��
extern int InitTexture( void )
{
	return AB_Release2DGraphs();
}


// 2D�O���t�B�b�N�J��
extern int AB_Release2DGraphs( void )
{
	int							i;

	// �O���t�B�b�N�J��
	for ( i = 0; i < fg_nUnitPicNum; i++ ) {
		AM_SAFE_RELEASE( fg_UnitPictureBuf[i].pPicture );
	}

	// �萔���Z�b�g
	fg_nUnitPicNum = 0;													// �P�ʃs�N�`�����Z�b�g
	fg_nUserPicNum = 0;													// ���[�U�[�s�N�`�����Z�b�g
	hDirectPic = -1;													// ���ډ摜�n���h�����Z�b�g

	return AM_OK;
}


// �Ō�ɍ��ꂽ�O���t�B�b�N�J���i�P�ʃs�N�`���ƃ��[�U�[�s�N�`����1:1�Ή����Ă���K�v����j
extern int AB_ReleaseLastGraph( int hPicture )
{
	if ( fg_nUnitPicNum <= 0 || fg_nUserPicNum <= 0 ) return AMERR_CANNOT;
	if ( ( hPicture & ~AM_HANDLE_HEAD_MASK ) != fg_nUserPicNum - 1 ) return AMERR_CANNOT;	// �ʒu

	AM_SAFE_RELEASE( fg_UnitPictureBuf[fg_nUnitPicNum - 1].pPicture );

	fg_nUnitPicNum--;
	fg_nUserPicNum--;

	return AM_OK;
}


//-----------------------------------------------------------------------------
// 2D�摜�`��֐���

// 2D�`��
extern int DrawMem( int x, int y, int hPicture )
{
	int					nResult;								// ���U���g�R�[�h
	int					nUserPicLoc;							// ���[�U�[�G�ʒu
	AM_USER_PICTURE		*pupUserPic;							// ���[�U�[�G
	int					bDrawConds = 0;							// �`�����
	AB_DRAWING_DEBUG_INFO		adDebugInfo;					// �f�o�b�O���

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// �ʒu
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// �Ó����`�F�b�N
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEM;			// DrawMem����
			adDebugInfo.nUsedHandle = hPicture;					// �n���h��
			adDebugInfo.nBlendKind = 0;							// �u�����h
			adDebugInfo.bError = true;							// �G���[����
			AB_SetDrawingDebugInfo( &adDebugInfo );				// �f�o�b�O���Z�b�g
		}
		return AMERR_INVALIDHANDLE;
	}

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	if ( pupUserPic->bFlags & AM_PICTURE_LR ) {					// ���E���]����
		bDrawConds |= AM_DRAWPIC_LR;
	}

	nResult = AM_DrawPictureCond( x, y, x + pupUserPic->nWidth, y + pupUserPic->nHeight,
								  pupUserPic->nLeft, pupUserPic->nTop,
								  pupUserPic->nLeft + pupUserPic->nWidth, pupUserPic->nTop  + pupUserPic->nHeight,
								  pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ), bDrawConds );

	// �f�o�b�O�����Z�b�g
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEM;				// DrawMem����
		adDebugInfo.nUsedHandle = hPicture;						// �n���h��
		adDebugInfo.pUserPicture = pupUserPic;					// ���[�U�[�s�N�`��
		adDebugInfo.x1 = x;										// �`��x���W
		adDebugInfo.y1 = y;										// �`��y���W
		adDebugInfo.nBlendKind = 0;								// �u�����h
		adDebugInfo.bError = false;								// �G���[�Ȃ�
		AB_SetDrawingDebugInfo( &adDebugInfo );					// �f�o�b�O���Z�b�g
	}

	return nResult;
}

// �A���t�@�u�����h�i�J���[�L�[�j�t��2D�`��
extern int DrawMemTh( int x, int y, int hPicture )
{
	int					nResult;								// ���U���g�R�[�h
	int					nUserPicLoc;							// ���[�U�[�G�ʒu
	AM_USER_PICTURE		*pupUserPic;							// ���[�U�[�G
	int					bDrawConds = AM_DRAWPIC_PICALPHA;		// �`�����
	AB_DRAWING_DEBUG_INFO		adDebugInfo;					// �f�o�b�O���

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// �ʒu
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// �Ó����`�F�b�N
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEM;			// DrawMem����
			adDebugInfo.nUsedHandle = hPicture;					// �n���h��
			adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;		// �u�����h
			adDebugInfo.bError = true;							// �G���[����
			AB_SetDrawingDebugInfo( &adDebugInfo );				// �f�o�b�O���Z�b�g
		}
		return AMERR_INVALIDHANDLE;
	}

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	if ( pupUserPic->bFlags & AM_PICTURE_LR ) {					// ���E���]����
		bDrawConds |= AM_DRAWPIC_LR;
	}

	nResult = AM_DrawPictureCond( x, y, x + pupUserPic->nWidth, y + pupUserPic->nHeight,
								  pupUserPic->nLeft, pupUserPic->nTop,
								  pupUserPic->nLeft + pupUserPic->nWidth, pupUserPic->nTop  + pupUserPic->nHeight,
								  pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ), bDrawConds );

	// �f�o�b�O�����Z�b�g
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEM;				// DrawMem����
		adDebugInfo.nUsedHandle = hPicture;						// �n���h��
		adDebugInfo.pUserPicture = pupUserPic;					// ���[�U�[�s�N�`��
		adDebugInfo.x1 = x;										// �`��x���W
		adDebugInfo.y1 = y;										// �`��y���W
		adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;			// �u�����h
		adDebugInfo.bError = false;								// �G���[�Ȃ�
		AB_SetDrawingDebugInfo( &adDebugInfo );					// �f�o�b�O���Z�b�g
	}

	return nResult;
}


// 2D���E���]�`��
extern int DrawMemLR( int x, int y, int hPicture )
{
	int					nResult;								// ���U���g�R�[�h
	int					nUserPicLoc;							// ���[�U�[�G�ʒu
	AM_USER_PICTURE		*pupUserPic;							// ���[�U�[�G
	int					bDrawConds = 0;							// �`�����
	AB_DRAWING_DEBUG_INFO		adDebugInfo;					// �f�o�b�O���

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// �ʒu
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// �Ó����`�F�b�N
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMLR;		// DrawMem����
			adDebugInfo.nUsedHandle = hPicture;					// �n���h��
			adDebugInfo.nBlendKind = 0;							// �u�����h
			adDebugInfo.bError = true;							// �G���[����
			AB_SetDrawingDebugInfo( &adDebugInfo );				// �f�o�b�O���Z�b�g
		}
		return AMERR_INVALIDHANDLE;
	}

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	if ( !( pupUserPic->bFlags & AM_PICTURE_LR ) ) {			// ���E���]�Ȃ�
		bDrawConds |= AM_DRAWPIC_LR;
	}

	nResult = AM_DrawPictureCond( x, y, x + pupUserPic->nWidth, y + pupUserPic->nHeight,
								  pupUserPic->nLeft, pupUserPic->nTop,
								  pupUserPic->nLeft + pupUserPic->nWidth, pupUserPic->nTop  + pupUserPic->nHeight,
								  pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ), bDrawConds );

	// �f�o�b�O�����Z�b�g
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMLR;			// DrawMemLR����
		adDebugInfo.nUsedHandle = hPicture;						// �n���h��
		adDebugInfo.pUserPicture = pupUserPic;					// ���[�U�[�s�N�`��
		adDebugInfo.x1 = x;										// �`��x���W
		adDebugInfo.y1 = y;										// �`��y���W
		adDebugInfo.nBlendKind = 0;								// �u�����h
		adDebugInfo.bError = false;								// �G���[�Ȃ�
		AB_SetDrawingDebugInfo( &adDebugInfo );					// �f�o�b�O���Z�b�g
	}

	return nResult;
}


// 2D���E���]�`��i�J���[�L�[�t���j
extern int DrawMemThLR( int x, int y, int hPicture )
{
	int					nResult;								// ���U���g�R�[�h
	int					nUserPicLoc;							// ���[�U�[�G�ʒu
	AM_USER_PICTURE		*pupUserPic;							// ���[�U�[�G
	int					bDrawConds = AM_DRAWPIC_PICALPHA;		// �`�����
	AB_DRAWING_DEBUG_INFO		adDebugInfo;					// �f�o�b�O���

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// �ʒu
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// �Ó����`�F�b�N
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMLR;		// DrawMem����
			adDebugInfo.nUsedHandle = hPicture;					// �n���h��
			adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;		// �u�����h
			adDebugInfo.bError = true;							// �G���[����
			AB_SetDrawingDebugInfo( &adDebugInfo );				// �f�o�b�O���Z�b�g
		}
		return AMERR_INVALIDHANDLE;
	}

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	if ( !( pupUserPic->bFlags & AM_PICTURE_LR ) ) {			// ���E���]�Ȃ�
		bDrawConds |= AM_DRAWPIC_LR;
	}

	nResult = AM_DrawPictureCond( x, y, x + pupUserPic->nWidth, y + pupUserPic->nHeight,
								  pupUserPic->nLeft, pupUserPic->nTop,
								  pupUserPic->nLeft + pupUserPic->nWidth, pupUserPic->nTop  + pupUserPic->nHeight,
								  pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ), bDrawConds );

	// �f�o�b�O�����Z�b�g
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMLR;			// DrawMem����
		adDebugInfo.nUsedHandle = hPicture;						// �n���h��
		adDebugInfo.pUserPicture = pupUserPic;					// ���[�U�[�s�N�`��
		adDebugInfo.x1 = x;										// �`��x���W
		adDebugInfo.y1 = y;										// �`��y���W
		adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;			// �u�����h
		adDebugInfo.bError = false;								// �G���[�Ȃ�
		AB_SetDrawingDebugInfo( &adDebugInfo );					// �f�o�b�O���Z�b�g
	}

	return nResult;
}


// 2D�g��k���`��
extern int DrawMemExp( int x1, int y1, int x2, int y2, int hPicture )
{
	int					nResult;								// ���U���g�R�[�h
	int					nUserPicLoc;							// ���[�U�[�G�ʒu
	AM_USER_PICTURE		*pupUserPic;							// ���[�U�[�G
	int					bDrawConds = 0;							// �`�����
	AB_DRAWING_DEBUG_INFO		adDebugInfo;					// �f�o�b�O���

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// �ʒu
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// �Ó����`�F�b�N
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMEXP;		// DrawMem����
			adDebugInfo.nUsedHandle = hPicture;					// �n���h��
			adDebugInfo.nBlendKind = 0;							// �u�����h
			adDebugInfo.bError = true;							// �G���[����
			AB_SetDrawingDebugInfo( &adDebugInfo );				// �f�o�b�O���Z�b�g
		}
		return AMERR_INVALIDHANDLE;
	}

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	if ( pupUserPic->bFlags & AM_PICTURE_LR ) {					// ���E���]����
		bDrawConds |= AM_DRAWPIC_LR;
	}

	nResult = AM_DrawPictureCond( x1, y1, x2, y2,
								  pupUserPic->nLeft, pupUserPic->nTop,
								  pupUserPic->nLeft + pupUserPic->nWidth, pupUserPic->nTop  + pupUserPic->nHeight,
								  pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ), bDrawConds );

	// �f�o�b�O�����Z�b�g
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMEXP;			// DrawMem����
		adDebugInfo.nUsedHandle = hPicture;						// �n���h��
		adDebugInfo.pUserPicture = pupUserPic;					// ���[�U�[�s�N�`��
		adDebugInfo.x1 = x1;									// �`��x���W
		adDebugInfo.y1 = y1;									// �`��y���W
		adDebugInfo.x2 = x2;									// �`��x���W2
		adDebugInfo.y2 = y2;									// �`��y���W2
		adDebugInfo.nBlendKind = 0;								// �u�����h
		adDebugInfo.bError = false;								// �G���[�Ȃ�
		AB_SetDrawingDebugInfo( &adDebugInfo );					// �f�o�b�O���Z�b�g
	}

	return nResult;
}


// �A���t�@�u�����h�i�J���[�L�[�j�t��2D�g��k���`��
extern int DrawMemThExp( int x1, int y1, int x2, int y2, int hPicture )
{
	int					nResult;								// ���U���g�R�[�h
	int					nUserPicLoc;							// ���[�U�[�G�ʒu
	AM_USER_PICTURE		*pupUserPic;							// ���[�U�[�G
	int					bDrawConds = AM_DRAWPIC_PICALPHA;		// �`�����
	AB_DRAWING_DEBUG_INFO		adDebugInfo;					// �f�o�b�O���

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// �ʒu
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// �Ó����`�F�b�N
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMEXP;		// DrawMem����
			adDebugInfo.nUsedHandle = hPicture;					// �n���h��
			adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;		// �u�����h
			adDebugInfo.bError = true;							// �G���[����
			AB_SetDrawingDebugInfo( &adDebugInfo );				// �f�o�b�O���Z�b�g
		}
		return AMERR_INVALIDHANDLE;
	}

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	if ( pupUserPic->bFlags & AM_PICTURE_LR ) {					// ���E���]����
		bDrawConds |= AM_DRAWPIC_LR;
	}

	nResult = AM_DrawPictureCond( x1, y1, x2, y2,
								  pupUserPic->nLeft, pupUserPic->nTop,
								  pupUserPic->nLeft + pupUserPic->nWidth, pupUserPic->nTop  + pupUserPic->nHeight,
								  pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ), bDrawConds );

	// �f�o�b�O�����Z�b�g
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMEXP;			// DrawMem����
		adDebugInfo.nUsedHandle = hPicture;						// �n���h��
		adDebugInfo.pUserPicture = pupUserPic;					// ���[�U�[�s�N�`��
		adDebugInfo.x1 = x1;									// �`��x���W
		adDebugInfo.y1 = y1;									// �`��y���W
		adDebugInfo.x2 = x2;									// �`��x���W2
		adDebugInfo.y2 = y2;									// �`��y���W2
		adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;			// �u�����h
		adDebugInfo.bError = false;								// �G���[�Ȃ�
		AB_SetDrawingDebugInfo( &adDebugInfo );					// �f�o�b�O���Z�b�g
	}

	return nResult;
}


// 2D��]�`��
extern int DrawMemRot( int nCenX1, int nCenY1, int nCenX2, int nCenY2,
					   double fAngle, double fExRate,
					   int hPicture )
{
	int					nResult;								// ���U���g�R�[�h
	int					nUserPicLoc;							// ���[�U�[�G�ʒu
	AB_DRAWING_DEBUG_INFO		adDebugInfo;					// �f�o�b�O���

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// �ʒu
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// �Ó����`�F�b�N
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMROT;		// DrawMem����
			adDebugInfo.nUsedHandle = hPicture;					// �n���h��
			adDebugInfo.nBlendKind = 0;							// �u�����h
			adDebugInfo.bError = true;							// �G���[����
			AB_SetDrawingDebugInfo( &adDebugInfo );				// �f�o�b�O���Z�b�g
		}
		return AMERR_INVALIDHANDLE;
	}

	// �`��
	nResult = AM_DrawUserPicRotCond( nCenX1, nCenY1, nCenX2, nCenY2, fAngle, fExRate,
									 &fg_UserPictureBuf[nUserPicLoc], 0 );

	// �f�o�b�O�����Z�b�g
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMROT;			// DrawMem����
		adDebugInfo.nUsedHandle = hPicture;						// �n���h��
		adDebugInfo.pUserPicture = &fg_UserPictureBuf[nUserPicLoc];	// ���[�U�[�s�N�`��
		adDebugInfo.x1 = nCenX1;								// �`��x���W
		adDebugInfo.y1 = nCenY1;								// �`��y���W
		adDebugInfo.x2 = nCenX2;								// �`��x���W2
		adDebugInfo.y2 = nCenY2;								// �`��y���W2
		adDebugInfo.fAngle = ( float )fAngle;					// �`��p�x
		adDebugInfo.fExRate = ( float )fExRate;					// �`��g�嗦
		adDebugInfo.nBlendKind = 0;								// �u�����h
		adDebugInfo.bError = false;								// �G���[�Ȃ�
		AB_SetDrawingDebugInfo( &adDebugInfo );					// �f�o�b�O���Z�b�g
	}

	return nResult;
}


// 2D��]�`��i�J���[�L�[�t���j
extern int DrawMemThRot( int nCenX1, int nCenY1, int nCenX2, int nCenY2,
						 double fAngle, double fExRate,
						 int hPicture )
{
	int					nResult;								// ���U���g�R�[�h
	int					nUserPicLoc;							// ���[�U�[�G�ʒu
	AB_DRAWING_DEBUG_INFO		adDebugInfo;					// �f�o�b�O���

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// �ʒu
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// �Ó����`�F�b�N
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMROT;		// DrawMem����
			adDebugInfo.nUsedHandle = hPicture;					// �n���h��
			adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;		// �u�����h
			adDebugInfo.bError = true;							// �G���[����
			AB_SetDrawingDebugInfo( &adDebugInfo );				// �f�o�b�O���Z�b�g
		}
		return AMERR_INVALIDHANDLE;
	}

	// �`��
	nResult = AM_DrawUserPicRotCond( nCenX1, nCenY1, nCenX2, nCenY2, fAngle, fExRate,
									 &fg_UserPictureBuf[nUserPicLoc], AM_DRAWPIC_PICALPHA );

	// �f�o�b�O�����Z�b�g
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMROT;			// DrawMem����
		adDebugInfo.nUsedHandle = hPicture;						// �n���h��
		adDebugInfo.pUserPicture = &fg_UserPictureBuf[nUserPicLoc];		// ���[�U�[�s�N�`��
		adDebugInfo.x1 = nCenX1;								// �`��x���W
		adDebugInfo.y1 = nCenY1;								// �`��y���W
		adDebugInfo.x2 = nCenX2;								// �`��x���W2
		adDebugInfo.y2 = nCenY2;								// �`��y���W2
		adDebugInfo.fAngle = ( float )fAngle;					// �`��p�x
		adDebugInfo.fExRate = ( float )fExRate;					// �`��g�嗦
		adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;			// �u�����h
		adDebugInfo.bError = false;								// �G���[�Ȃ�
		AB_SetDrawingDebugInfo( &adDebugInfo );					// �f�o�b�O���Z�b�g
	}

	return nResult;
}


// 2D�����t����]�`��(���E���]�����t��)
static int AM_DrawUserPicRotCond( int nCenX1, int nCenY1, int nCenX2, int nCenY2,
								  double fAngle, double fExRate,
								  AM_USER_PICTURE *pUserPicture, int bConds )
{
	int					i;
	int					nResult;								// ���U���g�R�[�h
	AM_2DVECTOR			v2Vertices[4];							// 4���_
	float				fCost, fSint;							// �T�C���R�T�C��
	float				tx, ty;									// �ꎞ���W

	if ( pUserPicture->bFlags & AM_PICTURE_LR ) {					// ���E���]����
		bConds |= AM_DRAWPIC_LR;
	}

	// �T�C���R�T�C��
	fCost = (float)cos( fAngle );  fSint = (float)sin( fAngle );

	// ��{���W
	v2Vertices[0].x = ( float )( 0.0f                 - nCenX1 );  v2Vertices[0].y = ( float )( 0.0f                  - nCenY1 );
	v2Vertices[1].x = ( float )( pUserPicture->nWidth - nCenX1 );  v2Vertices[1].y = ( float )( 0.0f                  - nCenY1 );
	v2Vertices[2].x = ( float )( 0.0f                 - nCenX1 );  v2Vertices[2].y = ( float )( pUserPicture->nHeight - nCenY1 );
	v2Vertices[3].x = ( float )( pUserPicture->nWidth - nCenX1 );  v2Vertices[3].y = ( float )( pUserPicture->nHeight - nCenY1 );

	for ( i = 0; i < 4; i++ ) {								// �l�p�`���W����
		// ��]
		tx = ( v2Vertices[i].x * fCost - v2Vertices[i].y * fSint );
		ty = ( v2Vertices[i].x * fSint + v2Vertices[i].y * fCost );
		// �g�k�E���s�ړ�
		v2Vertices[i].x = tx * ( float )fExRate + nCenX2;
		v2Vertices[i].y = ty * ( float )fExRate + nCenY2;
	}

	// �`��
	nResult = AM_DrawPictureCond4_2( v2Vertices,
									 pUserPicture->nLeft, pUserPicture->nTop,
									 pUserPicture->nLeft + pUserPicture->nWidth, pUserPicture->nTop  + pUserPicture->nHeight,
									 pUserPicture->pUnitPic->pPicture, &( pUserPicture->pUnitPic->a2PicInfo ), bConds );

	return nResult;
}


// 2D�ό`�`��
extern int DrawMemFlex( int x1, int y1, int x2, int y2,
						int x3, int y3, int x4, int y4,
						int hPicture )
{
	int					nResult;								// ���U���g�R�[�h
	int					nUserPicLoc;							// ���[�U�[�G�ʒu
	AM_USER_PICTURE		*pupUserPic;							// ���[�U�[�G
	AM_2DVECTOR			v2Vertices[4];							// 4���_
	int					bDrawConds = 0;							// �`�����
	AB_DRAWING_DEBUG_INFO		adDebugInfo;					// �f�o�b�O���

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// �ʒu
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// �Ó����`�F�b�N
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMFLEX;		// DrawMemFlex����
			adDebugInfo.nUsedHandle = hPicture;					// �n���h��
			adDebugInfo.nBlendKind = 0;							// �u�����h
			adDebugInfo.bError = true;							// �G���[����
			AB_SetDrawingDebugInfo( &adDebugInfo );				// �f�o�b�O���Z�b�g
		}
		return AMERR_INVALIDHANDLE;
	}

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	if ( pupUserPic->bFlags & AM_PICTURE_LR ) {					// ���E���]����
		bDrawConds |= AM_DRAWPIC_LR;
	}

	// ���W
	v2Vertices[0].x = ( float )x1;  v2Vertices[0].y = ( float )y1;
	v2Vertices[1].x = ( float )x2;  v2Vertices[1].y = ( float )y2;
	v2Vertices[2].x = ( float )x3;  v2Vertices[2].y = ( float )y3;
	v2Vertices[3].x = ( float )x4;  v2Vertices[3].y = ( float )y4;

	// �`��
	nResult = AM_DrawPictureCond4_2( v2Vertices,
									 pupUserPic->nLeft, pupUserPic->nTop,
									 pupUserPic->nLeft + pupUserPic->nWidth, pupUserPic->nTop  + pupUserPic->nHeight,
									 pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ), bDrawConds );

	// �f�o�b�O�����Z�b�g
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMFLEX;			// DrawMem����
		adDebugInfo.nUsedHandle = hPicture;						// �n���h��
		adDebugInfo.pUserPicture = pupUserPic;					// ���[�U�[�s�N�`��
		adDebugInfo.x1 = x1;									// �`��x���W
		adDebugInfo.y1 = y1;									// �`��y���W
		adDebugInfo.x2 = x2;									// �`��x���W2
		adDebugInfo.y2 = y2;									// �`��y���W2
		adDebugInfo.x3 = x3;									// �`��x���W3
		adDebugInfo.y3 = y3;									// �`��y���W3
		adDebugInfo.x4 = x4;									// �`��x���W4
		adDebugInfo.y4 = y4;									// �`��y���W4
		adDebugInfo.nBlendKind = 0;								// �u�����h
		adDebugInfo.bError = false;								// �G���[�Ȃ�
		AB_SetDrawingDebugInfo( &adDebugInfo );					// �f�o�b�O���Z�b�g
	}

	return nResult;
}


// 2D�ό`�`��i�J���[�L�[�t���j
extern int DrawMemThFlex( int x1, int y1, int x2, int y2,
						  int x3, int y3, int x4, int y4,
						  int hPicture )
{
	int					nResult;								// ���U���g�R�[�h
	int					nUserPicLoc;							// ���[�U�[�G�ʒu
	AM_USER_PICTURE		*pupUserPic;							// ���[�U�[�G
	AM_2DVECTOR			v2Vertices[4];							// 4���_
	int					bDrawConds = AM_DRAWPIC_PICALPHA;		// �`�����
	AB_DRAWING_DEBUG_INFO		adDebugInfo;				// �f�o�b�O���

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// �ʒu
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// �Ó����`�F�b�N
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMFLEX;		// DrawMemFlex����
			adDebugInfo.nUsedHandle = hPicture;					// �n���h��
			adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;		// �u�����h
			adDebugInfo.bError = true;							// �G���[����
			AB_SetDrawingDebugInfo( &adDebugInfo );				// �f�o�b�O���Z�b�g
		}
		return AMERR_INVALIDHANDLE;
	}

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	if ( pupUserPic->bFlags & AM_PICTURE_LR ) {					// ���E���]����
		bDrawConds |= AM_DRAWPIC_LR;
	}

	// ���W
	v2Vertices[0].x = ( float )x1;  v2Vertices[0].y = ( float )y1;
	v2Vertices[1].x = ( float )x2;  v2Vertices[1].y = ( float )y2;
	v2Vertices[2].x = ( float )x3;  v2Vertices[2].y = ( float )y3;
	v2Vertices[3].x = ( float )x4;  v2Vertices[3].y = ( float )y4;

	// �`��
	nResult = AM_DrawPictureCond4_2( v2Vertices,
									 pupUserPic->nLeft, pupUserPic->nTop,
									 pupUserPic->nLeft + pupUserPic->nWidth, pupUserPic->nTop  + pupUserPic->nHeight,
									 pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ), bDrawConds );

	// �f�o�b�O�����Z�b�g
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMFLEX;			// DrawMem����
		adDebugInfo.nUsedHandle = hPicture;						// �n���h��
		adDebugInfo.pUserPicture = pupUserPic;					// ���[�U�[�s�N�`��
		adDebugInfo.x1 = x1;									// �`��x���W
		adDebugInfo.y1 = y1;									// �`��y���W
		adDebugInfo.x2 = x2;									// �`��x���W2
		adDebugInfo.y2 = y2;									// �`��y���W2
		adDebugInfo.x3 = x3;									// �`��x���W3
		adDebugInfo.y3 = y3;									// �`��y���W3
		adDebugInfo.x4 = x4;									// �`��x���W4
		adDebugInfo.y4 = y4;									// �`��y���W4
		adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;			// �u�����h
		adDebugInfo.bError = false;								// �G���[�Ȃ�
		AB_SetDrawingDebugInfo( &adDebugInfo );					// �f�o�b�O���Z�b�g
	}

	return nResult;
}


// 2D�}�X�N�t���`��(�[��)
extern int DrawMemMask( int x, int y, int hPicture,
						int nWidth, int nHeight, unsigned char *pMask )
{
	int					nResult;								// ���U���g�R�[�h
	int					nUserPicLoc;							// ���[�U�[�G�ʒu
	AM_USER_PICTURE		*pupUserPic;							// ���[�U�[�G

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// �ʒu
	if ( nUserPicLoc >= fg_nUserPicNum ) return AMERR_INVALIDHANDLE;	// �Ó����`�F�b�N]

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];

	nResult = AM_Draw2DBlockMaskEmu( x, y, 
									 pupUserPic->nLeft, pupUserPic->nTop,
									 pupUserPic->nLeft + pupUserPic->nWidth, pupUserPic->nTop  + pupUserPic->nHeight,
									 nWidth, nHeight, pMask,
									 pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ) );

	return nResult;
}


// 2D�}�X�N�A�J���[�L�[�t���`��(�[��)
extern int DrawMemThMask( int x, int y, int hPicture,
						  int nWidth, int nHeight, unsigned char *pMask )
{
	int					nResult;								// ���U���g�R�[�h

	nResult = DrawMemMask( x, y, hPicture, nWidth, nHeight, pMask );

	return nResult;
}


// 2D�������`��
extern int DrawMemTrans( int x, int y, int hPicture, int nMode )
{
	int					nResult;								// ���U���g�R�[�h
	int					nUserPicLoc;							// ���[�U�[�G�ʒu
	AM_USER_PICTURE		*pupUserPic;							// ���[�U�[�G
	int					bDrawConds = 0;							// �`�����

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// �ʒu
	if ( nUserPicLoc >= fg_nUserPicNum ) return AMERR_INVALIDHANDLE;	// �Ó����`�F�b�N

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	if ( pupUserPic->bFlags & AM_PICTURE_LR ) {					// ���E���]����
		bDrawConds |= AM_DRAWPIC_LR;
	}

	// �`�惂�[�h����
	switch ( nMode ) {
	// ���ϔ�����
	case TRANS_AVERAGE:
	case TRANS_AVERAGE_TH:
		bDrawConds |= AM_DRAWPIC_AVEALPHA;
		break;

	// ���Z������
	case TRANS_ADD:
		bDrawConds |= AM_DRAWPIC_ADDALPHA;
		break;
	}

	nResult = AM_DrawPictureCond( x, y, x + pupUserPic->nWidth, y + pupUserPic->nHeight,
								  pupUserPic->nLeft, pupUserPic->nTop,
								  pupUserPic->nLeft + pupUserPic->nWidth, pupUserPic->nTop  + pupUserPic->nHeight,
								  pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ), bDrawConds );

	return nResult;
}


// �����t���`��
static int AM_DrawPictureCond( int dx1, int dy1, int dx2, int dy2,
							   int sx1, int sy1, int sx2, int sy2,
							   AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo, int bConds )
{
	int					nResult;								// ���U���g�R�[�h
	int					nTemp;									// �ꎞ�ϐ�
	AM_BLEND_STATE		bsBlendBuf;								// �u�����h�X�e�[�g�ޔ��ꏊ

	if ( bConds & AM_DRAWPIC_LR ) {								// ���E���]���肩
		nTemp = sx1;  sx1 = sx2;  sx2 = nTemp;
	}

	if ( bConds & ( AM_DRAWPIC_PICALPHA | AM_DRAWPIC_AVEALPHA | AM_DRAWPIC_ADDALPHA ) ) {	// �G�̃A���t�@�l�l����
		AM_GetBlendState( &bsBlendBuf );							// �u�����h�X�e�[�g�擾
		if ( bConds & AM_DRAWPIC_ADDALPHA ) {
			if ( bConds & AM_DRAWPIC_AVEALPHA ) {
				AM_SetBlendState( AMDRAW_ADDBLEND, AM_MAX_BLEND_FACTOR / 2 );		// ���Z�u�����h
			}
			else {
				AM_SetBlendState( AMDRAW_ADDBLEND, AM_MAX_BLEND_FACTOR );			// ���Z�u�����h
			}
		}
		else {
			if ( bConds & AM_DRAWPIC_AVEALPHA ) {
				AM_SetBlendState( AMDRAW_ALPHABLEND, AM_MAX_BLEND_FACTOR / 2 );		// �A���t�@�u�����h
			}
			else {
				AM_SetBlendState( AMDRAW_ALPHABLEND, bsBlendBuf.nBlendFactor );		// �A���t�@�u�����h
			}
		}
	}

	// ���`��
	nResult = AM_Draw2DGraph( dx1, dy1, dx2, dy2,
							  sx1, sy1, sx2, sy2,
							  pPicture, pPicInfo );

	if ( bConds & ( AM_DRAWPIC_PICALPHA | AM_DRAWPIC_AVEALPHA | AM_DRAWPIC_ADDALPHA ) ) {	// �G�̃A���t�@�l�l����
		AM_RestoreBlendState( &bsBlendBuf );					// �u�����h�X�e�[�g���A
	}

	return nResult;
}


// �����t���`��i�`���S�_�A�`�挳�Q�_�w��j
static int AM_DrawPictureCond4_2( AM_2DVECTOR *pDestVerts,
								  int sx1, int sy1, int sx2, int sy2,
								  AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo, int bConds )
{
	int					nResult;								// ���U���g�R�[�h
	int					nTemp;									// �ꎞ�ϐ�
	AM_BLEND_STATE		bsBlendBuf;								// �u�����h�X�e�[�g�ޔ��ꏊ

	if ( bConds & AM_DRAWPIC_LR ) {								// ���E���]���肩
		nTemp = sx1;  sx1 = sx2;  sx2 = nTemp;
	}

	if ( bConds & AM_DRAWPIC_PICALPHA ) {						// �G�̃A���t�@�l�l����
		AM_GetBlendState( &bsBlendBuf );							// �u�����h�X�e�[�g�擾
		AM_SetBlendState( AMDRAW_ALPHABLEND, bsBlendBuf.nBlendFactor );		// �A���t�@�u�����h
	}

	// ���`��
	nResult = AM_Draw2DGraph4_2( pDestVerts,
								 sx1, sy1, sx2, sy2,
								 pPicture, pPicInfo );

	if ( bConds & AM_DRAWPIC_PICALPHA ) {						// �G�̃A���t�@�l�l����
		AM_RestoreBlendState( &bsBlendBuf );					// �u�����h�X�e�[�g���A
	}

	return nResult;
}


//-----------------------------------------------------------------------------
// 2D�摜���ӊ֐���

// �摜�T�C�Y�擾
extern int GetPictureSize( int hPicture, int *pnWidth, int *pnHeight )
{
	int					nResult = AM_OK;						// ���U���g�R�[�h
	int					nUserPicLoc;							// ���[�U�[�G�ʒu
	AM_USER_PICTURE		*pupUserPic;							// ���[�U�[�G
//	int					nHandleKind;							// �n���h�����

	switch ( hPicture & AM_HANDLE_HEAD_MASK ) {
	// 2D�G�̏ꍇ
	case AM_PIC2D_HANDLE_HEAD:
		nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// �ʒu
		if ( nUserPicLoc >= fg_nUserPicNum ) return AMERR_INVALIDHANDLE;	// �Ó����`�F�b�N

		pupUserPic = &fg_UserPictureBuf[nUserPicLoc];

		*pnWidth = pupUserPic->nWidth;  *pnHeight = pupUserPic->nHeight;
		break;

	// ���[�r�[�̏ꍇ
	case AM_MOVIE_HANDLE_HEAD:
		return GetMovieSize( hPicture, pnWidth, pnHeight );
		break;

	default:
		nResult = AMERR_INVALIDHANDLE;
		break;
	}

	return nResult;
}


// ���ݕ\������Ă���O���t�B�b�N�擾
extern int GetDispGraph( int hPicture )
{
	int					nResult;								// ���U���g�R�[�h
	int					nUserPicLoc;							// ���[�U�[�G�ʒu
	AM_USER_PICTURE		*pupUserPic;							// ���[�U�[�G

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// �ʒu
	if ( nUserPicLoc >= fg_nUserPicNum ) return AMERR_INVALIDHANDLE;	// �Ó����`�F�b�N

	// �\����ʎ擾
	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	nResult = AM_GetDisplayBuffer( pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ),
								   AM_GET_FRONT_BUFFER );

	return nResult;
}


// �r�b�g�}�b�v�̃Z�[�u
extern int SaveBmp( int x1, int y1, int x2, int y2, char *szFileName )
{
	int							nResult;
	AM_RECT						rcSaveRect;
	int							nTemp;

	// �召�֌W����
	if ( x1 > x2 ) { nTemp = x1;  x1 = x2;  x2 = nTemp; }		
	if ( y1 > y2 ) { nTemp = y1;  y1 = y2;  y2 = nTemp; }

	// �T�`���[�V����
	if ( x1 < 0 ) x1 = 0;
	if ( y1 < 0 ) y1 = 0;
	if ( x2 > gg_siDispSize.cx ) x2 = gg_siDispSize.cx;
	if ( y2 > gg_siDispSize.cy ) y2 = gg_siDispSize.cy;

	// �傫���`�F�b�N
	if ( x1 == x2 || y1 == y2 ) return AMERR_INVALIDSIZE;

	// �R�s�[
	rcSaveRect.left  = x1;  rcSaveRect.top    = y1;
	rcSaveRect.right = x2;  rcSaveRect.bottom = y2;

	// �Z�[�u
	nResult = AM_SaveDisplayBuffer( &rcSaveRect, szFileName, AM_GET_FRONT_BUFFER );

	return nResult;
}


// ���[�h�摜�f�[�^�I�t�Z�b�g�ݒ�(�_�~�[)
extern int SetBmpOffset( int nOffset )
{
	return AM_OK;
}


// �`�惂�[�h�̐ݒ�
extern int SetDrawMode( int nDrawMode, int nParam )
{
	if ( !gg_bDispInitialized ) return AMERR_NOINITIALIZE;		// �������`�F�b�N

	// �e�N�X�`���T���v�����[�h
	if ( nDrawMode & AMDRAW_NEAREST ) {							// �j�A���X�g�l�C�o�[
			AM_SetTexSampleState( AMDRAW_NEAREST );
	}
	if ( nDrawMode & AMDRAW_BILINEAR ) {						// �o�C���j�A�t�B���^�����O
			AM_SetTexSampleState( AMDRAW_BILINEAR );
	}

	// �u�����h���[�h
	if ( nDrawMode & AMDRAW_NOBLEND ) {							// �u�����h�Ȃ�
			AM_SetBlendState( AMDRAW_NOBLEND, 0 );
	}
	if ( nDrawMode & AMDRAW_ALPHABLEND ) {						// �A���t�@�u�����f�B���O
			AM_SetBlendState( AMDRAW_ALPHABLEND, nParam );
	}
	if ( nDrawMode & AMDRAW_ADDBLEND ) {						// ���Z�u�����f�B���O
			AM_SetBlendState( AMDRAW_ADDBLEND, nParam );
	}

	// �e�N�X�`�����b�v���[�h
	if ( nDrawMode & AMDRAW_TEX_NOREPEAT ) {					// �e�N�X�`�����s�[�g�Ȃ�
		AM_SetTexWrapState( AMDRAW_TEX_NOREPEAT );				// �e�N�X�`���N�����v
	}
	if ( nDrawMode & AMDRAW_TEX_REPEAT ) {						// �e�N�X�`�����s�[�g����
		AM_SetTexWrapState( AMDRAW_TEX_REPEAT );				// �e�N�X�`���N�����v
	}

	return AM_OK;
}


// �n���h�����烆�[�U�[�s�N�`���擾
extern int AB_GetUserPicture( int hPicture, AM_USER_PICTURE **ppupUserPicture )
{
	int					nUserPicLoc;							// ���[�U�[�G�ʒu

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// �ʒu
	if ( nUserPicLoc >= fg_nUserPicNum ) return AMERR_INVALIDHANDLE;	// �Ó����`�F�b�N

	// �G�Ə��擾
	*ppupUserPicture = &fg_UserPictureBuf[nUserPicLoc];

	return AM_OK;
}


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AB_3DGraph.cpp
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̂R�c��ʕ`��@�\���u���b�W�B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#pragma warning( disable : 4819 )						// �R�[�h�y�[�W�̌x���}�~


// ---------------------------------------------------------------------------
// �C���N���[�h��

#include "amgame.h"												// AMGAME�w�b�_

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnv�w�b�_
#include "AB_2DGraph.h"
#include "AM_3DGraph.h"
#include "AB_3DGraph.h"

#endif


//-----------------------------------------------------------------------------
// 3D�摜�`��֐���

// �|���S���`��
extern int DrawPolygons( AmVertex *pVertices, int nVertexNum, int hTexture, int nFlags )	
{
	int							nResult = AM_OK;
	AM_USER_PICTURE				*pUserPicture;

	if ( hTexture >= 0 && ( nResult = AB_GetUserPicture( hTexture, &pUserPicture ) ) == AM_OK ) {	// �e�N�X�`������̏ꍇ
		AM_Draw3DPolygons( pVertices, nVertexNum,
						   pUserPicture->nLeft, pUserPicture->nTop,
						   pUserPicture->pUnitPic->pPicture, &( pUserPicture->pUnitPic->a2PicInfo ), nFlags );
	}
	else {														// �e�N�X�`�������̏ꍇ
		AM_Draw3DPolygons( pVertices, nVertexNum,
						   0, 0,
						   NULL, NULL, nFlags );
	}

	return nResult;
}


// ����`��
extern int Draw3DLines( AmVertex *pVertices, int nVertexNum, int hTexture )	
{
	int							nResult = AM_OK;
	AM_USER_PICTURE				*pUserPicture;

	if ( hTexture >= 0 && ( nResult = AB_GetUserPicture( hTexture, &pUserPicture ) ) == AM_OK ) {	// �e�N�X�`������̏ꍇ
		AM_Draw3DLines( pVertices, nVertexNum,
						pUserPicture->nLeft, pUserPicture->nTop,
						pUserPicture->pUnitPic->pPicture, &( pUserPicture->pUnitPic->a2PicInfo ) );
	}
	else {														// �e�N�X�`�������̏ꍇ
		AM_Draw3DLines( pVertices, nVertexNum,
						0, 0,
						NULL, NULL );
	}

	return nResult;
}


//-----------------------------------------------------------------------------
// 3D�c�[���֐���

// ���C�e�B���O���蒸�_�f�[�^�Z�b�g
extern int SetLightVertex( AmVertex *pVertex, AM3D_VALUE x, AM3D_VALUE y, AM3D_VALUE z,
						   AM3D_VALUE u, AM3D_VALUE v,
						   AM3D_VALUE nx, AM3D_VALUE ny, AM3D_VALUE nz )
{
	pVertex->x = x;  pVertex->y = y;  pVertex->z = z;  
	pVertex->u = u;  pVertex->v = v;
	pVertex->nx = nx;  pVertex->ny = ny;  pVertex->nz = nz;

	return AM_OK;
}


// ���C�e�B���O�Ȃ����_�f�[�^�Z�b�g
extern int SetColorVertex( AmVertex *pVertex, AM3D_VALUE x, AM3D_VALUE y, AM3D_VALUE z,
						   AM3D_VALUE u, AM3D_VALUE v,
						   unsigned char r, unsigned char g, unsigned char b, unsigned char a )
{
	pVertex->x = x;  pVertex->y = y;  pVertex->z = z;  
	pVertex->u = u;  pVertex->v = v;
	pVertex->r = r;  pVertex->g = g;  pVertex->b = b;  pVertex->a = a;

	return AM_OK;
}


// ���[�J�������[���h�s��Z�b�g
extern int SetLWMatrix( AmMatrix *pMatrix )
{
	int							nResult;

	nResult = AM_SetTransformMatrix( AM_MATRIX_LW, pMatrix );

	return nResult;
}


// ���[���h���r���[�s��Z�b�g
extern int SetWVMatrix( AmMatrix *pMatrix )
{
	int							nResult;

	nResult = AM_SetTransformMatrix( AM_MATRIX_WV, pMatrix );

	return nResult;
}


// ���[�J�������[���h�s��擾
extern int GetLWMatrix( AmMatrix *pMatrix )
{
	int							nResult;

	nResult = AM_GetTransformMatrix( AM_MATRIX_LW, pMatrix );

	return nResult;
}


// ���[���h���r���[�s��擾
extern int GetWVMatrix( AmMatrix *pMatrix )
{
	int							nResult;

	nResult = AM_GetTransformMatrix( AM_MATRIX_WV, pMatrix );

	return nResult;
}


// �s��̐όv�Z
extern int MultMatrix( AmMatrix *pDest, AmMatrix *pSrc1, AmMatrix *pSrc2 )
{
	int				i, j;
	AmMatrix		amTemp;

	for ( i = 0; i < 4; i++ ) {
		for ( j = 0; j < 4; j++ ) {
			amTemp.m[i][j] = pSrc1->m[i][0] * pSrc2->m[0][j] +
							 pSrc1->m[i][1] * pSrc2->m[1][j] + 
							 pSrc1->m[i][2] * pSrc2->m[2][j] + 
							 pSrc1->m[i][3] * pSrc2->m[3][j];
		}
	}
	*pDest = amTemp;

	return AM_OK;
}


// �O�όv�Z
extern int MultCross( AmVector *pDest, AmVector *pSrc1, AmVector *pSrc2 )
{
	AmVector			avTemp;

	avTemp.x = pSrc1->y * pSrc2->z - pSrc1->z * pSrc2->y;
	avTemp.y = pSrc1->z * pSrc2->x - pSrc1->x * pSrc2->z;
	avTemp.z = pSrc1->x * pSrc2->y - pSrc1->y * pSrc2->x;

	*pDest = avTemp;

	return AM_OK;
}


// ---------------------------------------------------------------------------
// �s��Z�b�g�֐���

// �P�ʍs��Z�b�g
extern int SetIdentityMatrix( AmMatrix *pDest )
{
	int				i, j;

	for ( i = 0; i < 4; i++ ) {
		for ( j = 0; j < 4; j++ ) {
			if ( i == j ) {
				pDest->m[i][j] = 1.0f;
			}
			else {
				pDest->m[i][j] = 0.0f;
			}
		}
	}

	return AM_OK;
}


// ��]�{���s�ړ��s��Z�b�g
extern int SetRTMatrix( AmMatrix *pDest, AM3D_VALUE tx, AM3D_VALUE ty, AM3D_VALUE tz,
										 AM3D_VALUE rx, AM3D_VALUE ry, AM3D_VALUE rz )
{
//	int				i, j;
	AmMatrix		mtRes;							// ���ʍs��
	AmMatrix		mtPres;							// ���ݍs��

	// x����]
	SetIdentityMatrix( &mtRes );					// �P�ʍs���
	mtRes.m[1][1] =  cosf( rx );  mtRes.m[1][2] =  sinf( rx );
	mtRes.m[2][1] = -sinf( rx );  mtRes.m[2][2] =  cosf( rx );

	// y����]
	SetIdentityMatrix( &mtPres );					// �P�ʍs���
	mtPres.m[0][0] =  cosf( ry );  mtPres.m[0][2] = -sinf( ry );
	mtPres.m[2][0] =  sinf( ry );  mtPres.m[2][2] =  cosf( ry );
	MultMatrix( &mtRes, &mtRes, &mtPres );			// �ϊ���������

	// z����]
	SetIdentityMatrix( &mtPres );					// �P�ʍs���
	mtPres.m[0][0] =  cosf( rz );  mtPres.m[0][1] =  sinf( rz );
	mtPres.m[1][0] = -sinf( rz );  mtPres.m[1][1] =  cosf( rz );
	MultMatrix( &mtRes, &mtRes, &mtPres );			// �ϊ���������

	mtRes.m[3][0] = tx;  mtRes.m[3][1] = ty;  mtRes.m[3][2] = tz;

	*pDest = mtRes;

	return AM_OK;
}


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AB_Palette.cpp
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̂Q�c��ʕ`��@�\���u���b�W�B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#pragma warning( disable : 4819 )						// �R�[�h�y�[�W�̌x���}�~


// ---------------------------------------------------------------------------
// �C���N���[�h��

#include <windows.h>
#include <d3d9.h>

#include "amgame.h"												// AMGAME�w�b�_

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnv�w�b�_
#include "AB_DispEnv.h"											// AMDispEnv�w�b�_
#include "AM_2DGraph.h"											// AM2DGraph�w�b�_
#include "AB_2DGraph.h"											// AB2DGraph�w�b�_
#include "AB_Palette.h"											// ABPalette�w�b�_

#endif


// ---------------------------------------------------------------------------
// �����萔��`��



//-----------------------------------------------------------------------------
// �p���b�g����i�p�~�ς݁A�_�~�[�j�֐���

// �p���b�g�ݒ�i�_�~�[�j
extern void SetPalette( int nEntry, int gbr, int rbr, int bbr )
{
	return;
}


// �p���b�g�擾�i�_�~�[�j
extern int GetPalette( int nEntry, int *gbr, int *rbr, int *bbr )
{
	return AM_OK;
}


// �a�l�o�p���b�g�ݒ�i�_�~�[�j
extern int SetBmpPal( char *filename )
{
	return AM_OK;
}


// �a�l�o�p���b�g�����ݒ�i�_�~�[�j
extern int SetBmpPalPart( char *filename, int FirstEnt, int EntNum )
{
	return AM_OK;
}


// �������ւ̃p���b�g���[�h�i�_�~�[�j
extern int LoadPalMem( char *szFileName )
{
	return AM_OK;
}


// �������p���b�g�Z�b�g�i�_�~�[�j
extern int SetPalMem( int hPalette )
{
	return AM_OK;
}


// �������p���b�g�����Z�b�g�i�_�~�[�j
extern int SetPalMemPart( int hPalette, int nFirstEnt, int nEntNum )
{
	return AM_OK;
}


// �p���b�g�p�������擾�i�_�~�[�j
extern int MakePalMem( void )
{
	return AM_OK;
}


// �������p���b�g�փp���b�g�Z�b�g�i�_�~�[�j
extern int SetPalToMem( int hPalette, int nEntry, int gbr, int rbr, int bbr )
{
	return AM_OK;
}


// �������p���b�g����p���b�g�擾�i�_�~�[�j
extern int GetPalFromMem( int hPalette, int nEntry,
						  int *gbr, int *rbr, int *bbr )
{
	return AM_OK;
}


// �p���b�g�������̊J���i�_�~�[�j
extern int FreePalMem( int hPalette )
{
	return AM_OK;
}


// �p���b�g���[�W�����̐ݒ�i�_�~�[�j
extern int SetPalArea( int under, int over )
{
	return AM_OK;
}


//-----------------------------------------------------------------------------
// �g�[���R���g���[���֐���

// �`�斾�x�̐ݒ�
extern int SetDrawBright( int nRedBright, int nGreenBright, int nBlueBright )
{
	int							nResult;

	nResult = AM_SetDrawColor( nRedBright   * AM_MAX_COLOR_FACTOR / AM_MAX_DRAW_BRIGHT,
							   nGreenBright * AM_MAX_COLOR_FACTOR / AM_MAX_DRAW_BRIGHT,
							   nBlueBright  * AM_MAX_COLOR_FACTOR / AM_MAX_DRAW_BRIGHT );

	return nResult;
}


// �p���b�g�F���R���g���[��
extern int ControlPal( int nBrGreen, int nBrRed, int nBrBlue )
{
	int							nResult;

	nResult = SetDrawBright( nBrRed, nBrGreen, nBrBlue );		// ���ꂩ��`���G�̐F���̂�

	return nResult;
}


// ��ʖ��x�̕ω�
extern int MovePalette( int nSGreen, int nSRed, int nSBlue,
						int nDGreen, int nDRed, int nDBlue, int nSpeed )
{
	int				i;
	int				hDispBuffer;
	float			nPresRed, nPresGreen, nPresBlue;
	float			nRedDel, nGreenDel, nBlueDel;
	AM_BLEND_STATE		bsBlendBuf;								// �u�����h�X�e�[�g�ޔ��ꏊ

	if ( !gg_pD3DDevice ) return AMERR_CANNOT;					// �C�j�V�����C�Y�`�F�b�N
	if ( nSpeed <= 0 ) return AMERR_INVALIDVALUE;				// ���x�`�F�b�N

	// �P�x�ۑ�
	AM_GetBlendState( &bsBlendBuf );							// �u�����h�X�e�[�g�擾

	// ���݂̐F�E�F���x�v�Z
	nPresRed   = (float)( nSRed   * 10 );
	nPresGreen = (float)( nSGreen * 10 );
	nPresBlue  = (float)( nSBlue  * 10 );
	nRedDel   = ( nDRed   - nSRed   ) * 10.0f / nSpeed;
	nGreenDel = ( nDGreen - nSGreen ) * 10.0f / nSpeed;
	nBlueDel  = ( nDBlue  - nSBlue  ) * 10.0f / nSpeed;

	hDispBuffer = MakeTextureMem( gg_siDispSize.cx, gg_siDispSize.cy, 0 );	// �摜�o�b�t�@�쐬
	if ( hDispBuffer < 0 ) return hDispBuffer;
	GetDispGraph( hDispBuffer );								// ���݂̉摜�擾

	for ( i = 0; i < nSpeed; i++ ) {
		nPresRed   += nRedDel;
		nPresGreen += nGreenDel;
		nPresBlue  += nBlueDel;

		SetDrawBright( ( int )nPresRed, ( int )nPresGreen, ( int )nPresBlue );
		DrawMem( 0, 0, hDispBuffer );
		Flip();
		if ( !ProcessEvent() ) break;							// �I���Ȃ甲����
	}

	AB_ReleaseLastGraph( hDispBuffer );							// �ꎞ�摜�o�b�t�@�J��
	AM_RestoreBlendState( &bsBlendBuf );						// �u�����h�X�e�[�g���A

	return AM_OK;
}


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AB_FigureDraw.cpp
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̂Q�c��ʕ`��@�\���u���b�W�B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#pragma warning( disable : 4819 )						// �R�[�h�y�[�W�̌x���}�~


// ---------------------------------------------------------------------------
// �C���N���[�h��

#include <d3d9.h>
//#include <d3dx9.h>

#include "amgame.h"												// AMGAME�w�b�_

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnv�w�b�_
#include "AM_2DGraph.h"											// AM2DGraph�w�b�_
#include "AB_2DGraph.h"											// AB2DGraph�w�b�_
#include "AB_Debug.h"											// ABDebug�w�b�_
#include "AM_FigureDraw.h"										// AMFigureDraw�w�b�_
#include "AB_FigureDraw.h"										// AMFigureDraw�w�b�_

#endif


//-----------------------------------------------------------------------------
// �}�`�`��֐���

// 1�_�̕`��
extern void DrawPixel( int x, int y, int color )
{
	int							nResult;
	AM_POINT_DATA				apDrawPoint;					// �`��|�C���g

	apDrawPoint.x = x;  apDrawPoint.y = y;  apDrawPoint.z = 0;	// ���W
	apDrawPoint.nColor = color;

	nResult = AM_DrawPointArray( &apDrawPoint, 1 );				// �_�`��

	return;
}


// �F�R�[�h�̎擾
extern int GetColor( int nRed, int nGreen, int nBlue )
{
	int							nResult;

	// �F�T�`���[�V����
	if ( nRed   < 0 ) nRed   = 0;  if ( nRed   > 255 ) nRed   = 255;
	if ( nGreen < 0 ) nGreen = 0;  if ( nGreen > 255 ) nGreen = 255;
	if ( nBlue  < 0 ) nBlue  = 0;  if ( nBlue  > 255 ) nBlue  = 255;

	nResult = AM_GetColorCode( nRed, nGreen, nBlue );			// �F�Q�b�g

	return nResult;
}


// �w��_�̐F�擾
extern int GetPixel( int x, int y )
{
	int							nResult;

	// �G���[�`�F�b�N
	if ( x < 0 || x > gg_siDispSize.cx || y < 0 || y > gg_siDispSize.cy ) return AMERR_OUTOFRANGE;

	nResult = AM_GetBackbufferPointColor( x, y );

	return nResult;
}


// �����`�̕`��
extern void DrawFBox( int x1, int y1, int x2, int y2, int color )
{
	AM_POINT_DATA				apRectPoint[2];					// ����E�E���̓_

	// ����
	apRectPoint[0].x = x1;  apRectPoint[0].y = y1;  apRectPoint[0].z = 0;
	apRectPoint[0].nColor = color;

	// �E��
	apRectPoint[1].x = x2;  apRectPoint[1].y = y2;  apRectPoint[1].z = 0;
	apRectPoint[1].nColor = color;

	AM_DrawRectangleArray( apRectPoint, 1 );					// �`��

	// �f�o�b�O�����Z�b�g
	if ( gg_bRegisterDebug ) {
		AB_DRAWING_DEBUG_INFO		adDebugInfo;				// �f�o�b�O���
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWFBOX;			// DrawFBox����
		adDebugInfo.x1 = x1;									// �`��x���W
		adDebugInfo.y1 = y1;									// �`��y���W
		adDebugInfo.x2 = x2;									// �`��x���W2
		adDebugInfo.y2 = y2;									// �`��y���W2
		adDebugInfo.nColor = color;								// �`��F
		adDebugInfo.nBlendKind = 0;								// �u�����h
		AB_SetDrawingDebugInfo( &adDebugInfo );					// �f�o�b�O���Z�b�g
	}

	return;
}


// �����������`�̕`��
extern void DrawTBox( int x1, int y1, int x2, int y2, int color )
{
	AM_POINT_DATA				apRectPoint[2];					// ����E�E���̓_
	AM_BLEND_STATE				bsBlendBuf;						// �u�����h�X�e�[�g�ޔ��ꏊ

	// ����
	apRectPoint[0].x = x1;  apRectPoint[0].y = y1;  apRectPoint[0].z = 0;
	apRectPoint[0].nColor = ( color & 0xffffff ) | 0x80000000;

	// �E��
	apRectPoint[1].x = x2;  apRectPoint[1].y = y2;  apRectPoint[1].z = 0;
	apRectPoint[1].nColor = ( color & 0xffffff ) | 0x80000000;

	AM_GetBlendState( &bsBlendBuf );							// �u�����h�X�e�[�g�擾
	AM_SetBlendState( AMDRAW_ALPHABLEND, AM_MAX_BLEND_FACTOR / 2 );		// �A���t�@�u�����h

	AM_DrawRectangleArray( apRectPoint, 1 );					// �`��

	AM_RestoreBlendState( &bsBlendBuf );						// �u�����h�X�e�[�g���A

	// �f�o�b�O�����Z�b�g
	if ( gg_bRegisterDebug ) {
		AB_DRAWING_DEBUG_INFO		adDebugInfo;				// �f�o�b�O���
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWFBOX;			// DrawFBox����
		adDebugInfo.x1 = x1;									// �`��x���W
		adDebugInfo.y1 = y1;									// �`��y���W
		adDebugInfo.x2 = x2;									// �`��x���W2
		adDebugInfo.y2 = y2;									// �`��y���W2
		adDebugInfo.nColor = color;								// �`��F
		adDebugInfo.nBlendKind = AMDRAW_ALPHABLEND;				// �u�����h
		AB_SetDrawingDebugInfo( &adDebugInfo );					// �f�o�b�O���Z�b�g
	}

	return;
}


// ������̕`��
extern int DrawString( int x, int y, char *szString, int nColor )
{
	int							nResult;

	nResult = AM_DrawString( x, y, szString, nColor );

	return nResult;
}


// ̫�Ļ��ނ̕ύX
extern int SetFontSize( int nFontSize )
{
	int							nResult;

	// �T�`���[�V����
	if ( nFontSize < AM_MIN_FONT_SIZE ) {
		nFontSize = AM_MIN_FONT_SIZE;
	}
	if ( nFontSize > AM_MAX_FONT_SIZE ) {
		nFontSize = AM_MAX_FONT_SIZE;
	}

	nResult = AM_SetStringFontSize( nFontSize );				// �t�H���g�T�C�Y�ݒ�

	return AM_OK;
}


//-----------------------------------------------------------------------------
// �ȏ�
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̃��[�U���͋@�\���u���b�W�B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#pragma warning( disable : 4819 )						// �R�[�h�y�[�W�̌x���}�~

#include <windows.h>

#include "amgame.h"												// AMGAME�w�b�_

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnv�w�b�_
#include "AM_Input.h"											// �L�[���́@�@��ˑ����C���[
#include "AB_Input.h"											// �L�[���́@�u���b�W

#endif


// ---------------------------------------------------------------------------
// �L�[�{�[�h����֐���

// �ꕶ������
extern int GetCh( void )
{
	return AM_GetSingleChar();
}


// �L�[���̓`�F�b�N
extern int KBHit( void )
{
	return AM_IsKeyHit();
}


// �L�[��������Ă��邩�`�F�b�N
extern int CheckKey( int nInputMode )
{
	int							i;
	int							nResult = 0;

	switch ( nInputMode ) {
	// �}���`�v������
	case AMINPUT_MULT:
		
		nResult = AM_AddKeyInput( 0 );							// �L�[�{�[�h����

		for ( i = 0; i < gg_nJoyPadNum; i++ ) {					// �p�b�h����
			nResult = AM_AddJoyInput( i, nResult );
		}
		break;

	// �L�[�{�[�h����
	case AMINPUT_KEYBOARD:										
		nResult = AM_AddKeyInput( 0 );
		break;

	// �p�b�h1����
	case AMINPUT_PAD1:
		nResult = AM_AddJoyInput( 0, 0 );
		break;

	// �p�b�h2����
	case AMINPUT_PAD2:										
		nResult = AM_AddJoyInput( 1, 0 );
		break;

	// �s������
	default:
		nResult = AMERR_INVALIDVALUE;
		break;
	}


	return nResult;
}


// ---------------------------------------------------------------------------
// �}�E�X����֐���

// �}�E�X������
extern int InitMouse( int bMouseState )
{
	int							nResult;
	
	nResult = AM_InitMouse( bMouseState );

	return nResult;
}


// �}�E�X��Ԑݒ�
extern int SetMouseState( int bMouseState )
{
	int							nResult;
	
	nResult = AM_SetMouseState( bMouseState );

	return nResult;
}


// �}�E�X�ʒu�擾
extern int GetMouseLoc( int *nXLoc, int *nYLoc )
{
	int							nResult;
	
	nResult = AM_GetMousePosition( nXLoc, nYLoc );

	return nResult;
}


// �}�E�X�ʒu�ݒ�
extern int SetMouseLoc( int nXLoc, int nYLoc )
{
	int							nResult;
	
	nResult = AM_SetMouseLoc( nXLoc, nYLoc );

	return nResult;
}


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AB_Movie.cpp
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̃��[�r�[�Đ��@�\���u���b�W�B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#pragma warning( disable : 4819 )						// �R�[�h�y�[�W�̌x���}�~


// ---------------------------------------------------------------------------
// �C���N���[�h��

#include <windows.h>
#include <d3d9.h>

#include "amgame.h"												// AMGAME�w�b�_

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnv�w�b�_
#include "AM_2DGraph.h"											// AM2DGraph�w�b�_
#include "AB_2DGraph.h"											// AB2DGraph�w�b�_
#include "AB_Time.h"											// AMMovie�w�b�_
#include "AM_Movie.h"											// AMMovie�w�b�_
#include "AB_Movie.h"											// AMMovie�w�b�_

#endif


// ---------------------------------------------------------------------------
// �t�@�C�����O���[�o���ϐ���

AMMOVIE					MovieBuf[AM_MAX_MOVIE_NUM];				// ���[�r�[�o�b�t�@
int						fg_nMovieNum = 0;						// ���[�r�[��


//-----------------------------------------------------------------------------
// ���[�r�[�Đ��֐���

// ���[�r�[�Đ�
extern int PlayMovie( int x, int y, char *szFileName )
{
	AMMOVIE				SpotMovie;
	int					nResult;

	nResult = AM_OpenMovieFile( szFileName, &SpotMovie );
	if ( nResult != AM_OK ) return nResult;
	nResult = AM_PlayMovieFile( x, y, &SpotMovie );
	AM_CloseMovieFile( &SpotMovie );

	return nResult;
}


// �o�b�N���[�r�[�Đ�(�_�~�[)
extern int PlayMovieBack( int x, int y, char *szFileName )
{
	int		nResult;

	nResult = PlayMovie( x, y, szFileName );
//	nResult = AM_PlayMovie( szFileName, x, y, 0, 0 );

	return nResult;
}


// �ٽ�ذ݃��[�r�[�Đ�
extern int PlayMovieFull( char *szFileName )
{
	AMMOVIE				SpotMovie;
	int					nResult;

	nResult = AM_OpenMovieFile( szFileName, &SpotMovie );
	if ( nResult != AM_OK ) return nResult;
	nResult = AM_PlayMovieFile( MOVIE_FULL_LOC, MOVIE_FULL_LOC, &SpotMovie );
	AM_CloseMovieFile( &SpotMovie );

	return nResult;
}


// �ٽ�ذ��ޯ�Ѱ�ް�Đ�(�_�~�[)
extern int PlayMovieFullBack( char *szFileName )
{
	int		nResult;

	nResult = PlayMovieFull( szFileName );

	return nResult;
}


/*extern int SetMovieTime( int nFirst, int nLast )	// ���[�r�[�Đ����Ԑݒ�
{
	nMovieFirst = nFirst;
	nMovieLast = nLast;

	return AM_OK;
}*/


extern int LoadMovieMem( char *szFileName )			// ���[�r�[�̃������ւ̃��[�h
{
	int					nResult;

	if ( fg_nMovieNum >= AM_MAX_MOVIE_NUM ) return AMERR_NOFREEHANDLE;

	nResult = AM_OpenMovieFile( szFileName, &MovieBuf[fg_nMovieNum] );
	if ( nResult != AM_OK ) return nResult;
	fg_nMovieNum++;

	return AM_MOVIE_HANDLE_HEAD + fg_nMovieNum - 1;
}


extern int PlayMovieMem( int x, int y, int hMovie )		// ���������[�r�[�̍Đ�
{
	int					nResult;

	if ( ( hMovie & AM_HANDLE_HEAD_MASK ) != AM_MOVIE_HANDLE_HEAD ) return AMERR_INVALIDHANDLE;
	hMovie &= ~AM_HANDLE_HEAD_MASK;
	if ( hMovie < 0 || hMovie >= fg_nMovieNum ) return AMERR_INVALIDHANDLE;

	nResult = AM_PlayMovieFile( x, y, &MovieBuf[hMovie] );

	return nResult;
}


extern int PlayMovieMemFull( int hMovie )			// ���������[�r�[�̃t���Đ�
{
	int					nResult;

	if ( ( hMovie & AM_HANDLE_HEAD_MASK ) != AM_MOVIE_HANDLE_HEAD ) return AMERR_INVALIDHANDLE;
	hMovie &= ~AM_HANDLE_HEAD_MASK;
	if ( hMovie < 0 || hMovie >= fg_nMovieNum ) return AMERR_INVALIDHANDLE;

	nResult = AM_PlayMovieFile( MOVIE_FULL_LOC, MOVIE_FULL_LOC, &MovieBuf[hMovie] );

	return nResult;
}


extern int GetMovieFrame( int hPicture, int hMovie, int nFrameNum )	// ���[�r�[�̓���t���[���擾
{
	int							nResult;
	AM_USER_PICTURE				*pupUserPic;					// ���[�U�[�G

	// ���[�r�[�n���h������
	if ( ( hMovie & AM_HANDLE_HEAD_MASK ) != AM_MOVIE_HANDLE_HEAD ) return AMERR_INVALIDHANDLE;
	hMovie &= ~AM_HANDLE_HEAD_MASK;
	if ( hMovie < 0 || hMovie >= fg_nMovieNum ) return AMERR_INVALIDHANDLE;

	// �G�n���h������
	nResult = AB_GetUserPicture( hPicture, &pupUserPic );		// ���[�U�[�G�擾
	if ( nResult < 0 ) return nResult;

	nResult = AM_GetMovieFrame( &MovieBuf[hMovie], nFrameNum,
								pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ) );

	return nResult;
}


extern int GetMovieSize( int hMovie, int *nWidth, int *nHeight )	// ���[�r�[�T�C�Y�̎擾
{
	if ( ( hMovie & AM_HANDLE_HEAD_MASK ) != AM_MOVIE_HANDLE_HEAD ) return AMERR_INVALIDHANDLE;
	hMovie &= ~AM_HANDLE_HEAD_MASK;
	if ( hMovie < 0 || hMovie >= fg_nMovieNum ) return AMERR_INVALIDHANDLE;
	
	*nWidth  = MovieBuf[hMovie].AVIInfo.dwWidth;
	*nHeight = MovieBuf[hMovie].AVIInfo.dwHeight;

	return AM_OK;
}


extern int GetMovieCount( int hMovie )				// ���[�r�[�t���[���J�E���g���̎擾
{
	int				i;
	double			fNowTime;

	if ( ( hMovie & AM_HANDLE_HEAD_MASK ) != AM_MOVIE_HANDLE_HEAD ) return AMERR_INVALIDHANDLE;
	hMovie &= ~AM_HANDLE_HEAD_MASK;
	if ( hMovie < 0 || hMovie >= fg_nMovieNum ) return AMERR_INVALIDHANDLE;

	if ( MovieBuf[hMovie].fFirstTime < 0 ) {		// �t���[���������Ȃ��̏ꍇ
		MovieBuf[hMovie].fFirstTime = GetNowTime( 0 ) / 1000.0;
		MovieBuf[hMovie].nPresFrame = 0;
	}
	else {
		fNowTime = GetNowTime( 0 ) / 1000.0 - MovieBuf[hMovie].fFirstTime;
		for ( i = 1; i <= 2; i++ ) { 
			if ( fNowTime >= ( MovieBuf[hMovie].nPresFrame + 1 ) * MovieBuf[hMovie].fVidSampDelay )	// �t���[���i�ނ�
			{
				MovieBuf[hMovie].nPresFrame++;
			}
		}
	}
	if ( MovieBuf[hMovie].nPresFrame >= ( int )MovieBuf[hMovie].VideoInfo.dwLength ) {	// �I���Ȃ烊�Z�b�g
		MovieBuf[hMovie].fFirstTime = -1;
		MovieBuf[hMovie].nPresFrame = -1;
	}

	if ( MovieBuf[hMovie].nPresFrame < 0 ) return AM_ENDOFSTREAM;
	return MovieBuf[hMovie].nPresFrame;
}


extern int InitMovieMem( void )						// ���[�r�[�p�������̊J��
{
	int					i;

	for ( i = 0; i < fg_nMovieNum; i++ ) {
		AM_CloseMovieFile( &MovieBuf[i] );
	}
	fg_nMovieNum = 0;

	return AM_OK;
}


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AB_Time.cpp
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̎��ԋ@�\���u���b�W�B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#pragma warning( disable : 4819 )						// �R�[�h�y�[�W�̌x���}�~


#include "amgame.h"												// AMGAME�w�b�_

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// �L�[���́@�u���b�W���C���[
#include "AB_Input.h"											// �L�[���́@�u���b�W���C���[
#include "AM_Time.h"											// ���ԋ@�\�@�u���b�W���C���[

#endif


// ---------------------------------------------------------------------------
// �t�@�C�����O���[�o���ϐ���



//-----------------------------------------------------------------------------
// ���Ԋ֐���

// �L�[���͑҂��E���̓L�[�擾�֐�
extern int GetNowTime( int nDummy )
{
	int							nResult;

	nResult = AM_GetPresentTime();

	return nResult;
}


// �^�C�}�ő҂i�P�^�P�O�O�O�b�P�ʁj
extern int WaitByTimer( int nWaitTime )
{
	int			nFirstTime;

	nFirstTime = AM_GetPresentTime();
	while( AM_ProcessEvent() && AM_GetPresentTime() - nFirstTime < nWaitTime ) {
		if ( CheckKey( AMINPUT_MULT ) ) break;
	}

	return AM_OK;
}


// �����̎擾
extern int GetDateTime( AMDATETIME *pDateTime )
{
	int							nResult;

	nResult = AM_GetDateTime( pDateTime );

	return nResult;
}


// �����̃A���p�b�N
extern int UnpackDateTime( AMDATETIME *pDateTime )
{
	int		nDate, nTime;										// ����

	nDate = pDateTime->nPackedTime >> 16;						// �N����
	nTime = pDateTime->nPackedTime & 0xffff;					// �����b

	pDateTime->nYear   = (unsigned int )( ( nDate >> 9  ) & 0x7f );	// �N
	pDateTime->nYear   += 1980;									// �N�I�t�Z�b�g
	pDateTime->nMonth  = (unsigned char)( ( nDate >> 5  ) & 0xf 	);	// ��
	pDateTime->nDay    = (unsigned char)(   nDate         & 0x1f );	// ��
	pDateTime->nHour   = (unsigned char)( ( nTime >> 11 ) & 0x1f );	// ��
	pDateTime->nMinute = (unsigned char)( ( nTime >> 5  ) & 0x3f );	// ��
	pDateTime->nSecond = (unsigned char)( ( nTime         & 0x1f ) << 1 );	// �b

	return AM_OK;
}


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AB_Debug.cpp
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̃f�o�b�O�@�\���u���b�W�B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2005 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif

#pragma warning( disable : 4819 )						// �R�[�h�y�[�W�̌x���}�~


#include <stdio.h>
#include "amgame.h"												// AMGAME�w�b�_

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// �f�B�X�v���C���@�u���b�W���C���[
#include "AB_Input.h"											// �L�[���́@�u���b�W���C���[
#include "AM_2DGraph.h"											// AM2DGraph�w�b�_
#include "AM_FigureDraw.h"										// AMFigureDraw�w�b�_
#include "AB_Debug.h"											// �����w�b�_

#endif

#define MAX_DEBUG_STRING_LEN		100							// �ő�f�o�b�O������
#define MAX_DEBUG_STRING_NUM		( 8 * 60 )					// �ő�f�o�b�O�I�u�W�F�N�g��
#define MAX_DEBUG_DRAWING_OBJECTS	( 8 * 60 )					// �ő�`��f�o�b�O�I�u�W�F�N�g��
#define DEBUG_COLOR_VALS			5							// �f�o�b�O�F�o���G�[�V������


// ---------------------------------------------------------------------------
// �C���^�[�t�@�C���O���[�o���ϐ���

int								gg_bRegisterDebug = false;		// �f�o�b�O����o�^���邩
int								gg_bDisplayDebug = false;		// �f�o�b�O����\�����邩
int								gg_bDebugKey = true;			// �f�o�b�O�p�L�[���͂�L���ɂ��邩
int								gg_bDebugFile = true;			// �f�o�b�O�t�@�C���o�͂�L���ɂ��邩


// ---------------------------------------------------------------------------
// �t�@�C�����O���[�o���ϐ���

// �f�o�b�O������֌W
char							fg_szMyDebugStrings[MAX_DEBUG_STRING_NUM][MAX_DEBUG_STRING_LEN];	// �\���f�o�b�O������
int								fg_nMyDebugStringNum = 0;			// �f�o�b�O������

// �`��n�f�o�b�O�I�u�W�F�N�g�֌W
AB_DRAWING_DEBUG_INFO			fg_adDrawingDebugObjects[MAX_DEBUG_DRAWING_OBJECTS];	// �\������`��f�o�b�O�I�u�W�F�N�g
int								fg_nMyDrawingDebugObjects = 0;		// �`��f�o�b�O�I�u�W�F�N�g��

// �f�o�b�O�\������
int								fg_nDebugColor = 0xffffffff;	// �f�o�b�O�\���̐F
int								fg_nDebugColorVals[DEBUG_COLOR_VALS] = { 0xffffffff, 0xffff0000, 0xff00ff00, 0xff0000ff, 0xff000000 };
int								fg_nPresentDebugColor = 0;		// ���݂̃f�o�b�O�F


// ---------------------------------------------------------------------------
// �����֐��v���g�^�C�v�錾��

static int ToggleDisplayDebug( void );							// �f�o�b�O�\���̂n�m�^�n�e�e�؂�ւ�
static int ChangeDebugColor( void );							// �f�o�b�O�\���F�̐؂�ւ�



//-----------------------------------------------------------------------------
// ���[�U�[���J�֐���

// �f�o�b�O���[�h�̃Z�b�g
extern int SetAMDebugMode( int nDebugMode )
{
	switch( nDebugMode ) {
	// �S�Ẵf�o�b�O�@�\���I�t
	case AMDEBUG_ALL_OFF:
		gg_bRegisterDebug = false;								// �o�^
		gg_bDisplayDebug  = false;								// �\��
		gg_bDebugKey      = false;								// �L�[
		gg_bDebugFile     = false;								// �t�@�C��
		break;

	// �S�Ẵf�o�b�O�@�\���I��
	case AMDEBUG_ALL_ON:
		gg_bRegisterDebug = true;								// �o�^
		gg_bDisplayDebug  = true;								// �\��
		gg_bDebugKey      = true;								// �L�[
		gg_bDebugFile     = true;								// �t�@�C��
		break;

	// �f�o�b�O���o�^���I�t
	case AMDEBUG_REGISTER_OFF:
		gg_bRegisterDebug = false;								// �o�^
		break;

	// �f�o�b�O���o�^���I��
	case AMDEBUG_REGISTER_ON:
		gg_bRegisterDebug = true;								// �o�^
		break;

	// �f�o�b�O���\�����I�t
	case AMDEBUG_DISPLAY_OFF:
		gg_bDisplayDebug = false;								// �\��
		break;

	// �f�o�b�O���\�����I��
	case AMDEBUG_DISPLAY_ON:
		gg_bDisplayDebug = true;								// �\��
		break;

	// �f�o�b�O�L�[���I�t
	case AMDEBUG_KEY_OFF:
		gg_bDebugKey = false;									// �L�[
		break;

	// �f�o�b�O�L�[���I��
	case AMDEBUG_KEY_ON:
		gg_bDebugKey = true;									// �L�[
		break;

	// �t�@�C�����I�t
	case AMDEBUG_FILE_OFF:
		gg_bDebugFile = false;									// �t�@�C��
		break;

	// �t�@�C�����I��
	case AMDEBUG_FILE_ON:
		gg_bDebugFile = true;									// �t�@�C��
		break;

	}

	return AM_OK;
}


// �f�o�b�O���[�h�̎擾
extern int GetAMDebugMode( void )
{
	int							nResult;

	nResult = 0;
	if ( gg_bRegisterDebug ) nResult += AMDEBUG_REGISTER;		// �o�^
	if ( gg_bDisplayDebug  ) nResult += AMDEBUG_DISPLAY;		// �\��
	if ( gg_bDebugKey      ) nResult += AMDEBUG_KEY;			// �L�[
	if ( gg_bDebugFile     ) nResult += AMDEBUG_FILE;			// �t�@�C��

	return nResult;
}


// �f�o�b�O���[�h�̕��A
extern int RestoreAMDebugMode( int nDebugFlags )
{
	// �o�^
	if ( nDebugFlags & AMDEBUG_REGISTER ) gg_bRegisterDebug = true;
									 else gg_bRegisterDebug = false;
	// �\��
	if ( nDebugFlags & AMDEBUG_DISPLAY  ) gg_bDisplayDebug = true;
									 else gg_bDisplayDebug = false;
	// �L�[
	if ( nDebugFlags & AMDEBUG_KEY      ) gg_bDebugKey = true;
									 else gg_bDebugKey = false;
	// �t�@�C��
	if ( nDebugFlags & AMDEBUG_FILE     ) gg_bDebugFile = true;
									 else gg_bDebugFile = false;

	return AM_OK;
}


// �f�o�b�O������̏o��
extern int OutputAMDebugString( char *szOutString )
{
	// �o�^�\��
	if ( fg_nMyDebugStringNum >= MAX_DEBUG_STRING_NUM ||
		 fg_nMyDrawingDebugObjects >= MAX_DEBUG_DRAWING_OBJECTS )
	{
		return AMERR_OVERBUFFER;
	}

	// �f�o�b�O�I�u�W�F�N�g�o�^
	fg_adDrawingDebugObjects[fg_nMyDrawingDebugObjects].nObjectKind = AB_DEBUG_OUT_STRING;
	fg_adDrawingDebugObjects[fg_nMyDrawingDebugObjects].nUsedHandle = fg_nMyDebugStringNum;

	// �f�o�b�O������o�^
	strncpy( fg_szMyDebugStrings[fg_nMyDebugStringNum], szOutString, MAX_DEBUG_STRING_LEN );
	fg_szMyDebugStrings[fg_nMyDebugStringNum][MAX_DEBUG_STRING_LEN - 1] = 0;	// �m���ɏI�[��u��

	fg_nMyDrawingDebugObjects++;
	fg_nMyDebugStringNum++;

	// �f�o�b�O�\���������n�m��
	gg_bDisplayDebug = true;									// �\��
//	gg_bRegisterDebug = true;									// �o�^

	return AM_OK;
}



//-----------------------------------------------------------------------------
// �����֐���

// �`��n�f�o�b�O���Z�b�g�֐�
extern int AB_SetDrawingDebugInfo( AB_DRAWING_DEBUG_INFO *pDebugInfo )
{
	// �o�b�t�@�̋󂫃`�F�b�N
	if ( fg_nMyDrawingDebugObjects >= MAX_DEBUG_DRAWING_OBJECTS ) return AMERR_OVERBUFFER;

	// �����o�b�t�@�ɒ~��
	fg_adDrawingDebugObjects[fg_nMyDrawingDebugObjects] = *pDebugInfo;
	fg_nMyDrawingDebugObjects++;

	return AM_OK;
}


// �f�o�b�O���\���֐�
extern int AB_DisplayDebugInfo( void )
{
	int							i;
	int							x, y;
	char						szCommandName[100];
	char						szOut[MAX_DEBUG_STRING_LEN];
	int							nFontSize;						// �t�H���g�T�C�Y

	// �萔�Z�b�g
	nFontSize = AM_GetStringFontSize();							// �t�H���g�T�C�Y�擾
	x = 0;  y = 0;

	// �L���v�V�����\��
	sprintf( szOut, "Frame:%d", GetFrameCounter() );
	AM_DrawString( x, y, szOut, fg_nDebugColor );
	y += nFontSize;

	// �`��n�f�o�b�O�I�u�W�F�N�g�\��
	for ( i = 0; i < fg_nMyDrawingDebugObjects; i++ ) {
		strcpy( szOut, "Undefined Error!" );					// �o�͏�����
		switch( fg_adDrawingDebugObjects[i].nObjectKind ) {
		case AB_DEBUG_DRAWMEM:									// DrawMem�̏ꍇ
			if ( fg_adDrawingDebugObjects[i].nBlendKind == 0 ) {
				strcpy( szCommandName, "DrawMem     " );
			}
			else {
				strcpy( szCommandName, "DrawMemTh   " );
			}
			if ( !fg_adDrawingDebugObjects[i].bError ) {
				sprintf( szOut, "%s( %4d, %4d ) %s", szCommandName,
													fg_adDrawingDebugObjects[i].x1,
													fg_adDrawingDebugObjects[i].y1,
													fg_adDrawingDebugObjects[i].pUserPicture->pUnitPic->szFileName );
			}
			break;

		case AB_DEBUG_DRAWMEMLR:								// DrawMemLR�̏ꍇ
			if ( fg_adDrawingDebugObjects[i].nBlendKind == 0 ) {
				strcpy( szCommandName, "DrawMemLR   " );
			}
			else {
				strcpy( szCommandName, "DrawMemThLR " );
			}
			if ( !fg_adDrawingDebugObjects[i].bError ) {
				sprintf( szOut, "%s( %4d, %4d ) %s", szCommandName,
													fg_adDrawingDebugObjects[i].x1,
													fg_adDrawingDebugObjects[i].y1,
													fg_adDrawingDebugObjects[i].pUserPicture->pUnitPic->szFileName );
			}
			break;

		case AB_DEBUG_DRAWMEMEXP:								// DrawMemExp�̏ꍇ
			if ( fg_adDrawingDebugObjects[i].nBlendKind == 0 ) {
				strcpy( szCommandName, "DrawMemExp  " );
			}
			else {
				strcpy( szCommandName, "DrawMemThExp" );
			}
			if ( !fg_adDrawingDebugObjects[i].bError ) {
				sprintf( szOut, "%s( %4d, %4d )-( %4d, %4d ) %s", szCommandName,
																fg_adDrawingDebugObjects[i].x1,
																fg_adDrawingDebugObjects[i].y1,
																fg_adDrawingDebugObjects[i].x2,
																fg_adDrawingDebugObjects[i].y2,
																fg_adDrawingDebugObjects[i].pUserPicture->pUnitPic->szFileName );
			}
			break;

		case AB_DEBUG_DRAWMEMROT:								// DrawMemRot�̏ꍇ
			if ( fg_adDrawingDebugObjects[i].nBlendKind == 0 ) {
				strcpy( szCommandName, "DrawMemRot" );
			}
			else {
				strcpy( szCommandName, "DrawMemThRot" );
			}
			if ( !fg_adDrawingDebugObjects[i].bError ) {
				sprintf( szOut, "%s C1(%d,%d)C2(%d,%d) Ang=%5.2f Exp=%5.2f %s",
								szCommandName,
								fg_adDrawingDebugObjects[i].x1,
								fg_adDrawingDebugObjects[i].y1,
								fg_adDrawingDebugObjects[i].x2,
								fg_adDrawingDebugObjects[i].y2,
								fg_adDrawingDebugObjects[i].fAngle,
								fg_adDrawingDebugObjects[i].fExRate,
								fg_adDrawingDebugObjects[i].pUserPicture->pUnitPic->szFileName );
			}
			break;

		case AB_DEBUG_DRAWMEMFLEX:								// DrawMemFlex�̏ꍇ
			if ( fg_adDrawingDebugObjects[i].nBlendKind == 0 ) {
				strcpy( szCommandName, "DrawMemFlex  " );
			}
			else {
				strcpy( szCommandName, "DrawMemThFlex" );
			}
			if ( !fg_adDrawingDebugObjects[i].bError ) {
				sprintf( szOut, "%s(%d,%d)-(%d,%d)-(%d,%d)-(%d,%d) %s",
								szCommandName,
								fg_adDrawingDebugObjects[i].x1,
								fg_adDrawingDebugObjects[i].y1,
								fg_adDrawingDebugObjects[i].x2,
								fg_adDrawingDebugObjects[i].y2,
								fg_adDrawingDebugObjects[i].x3,
								fg_adDrawingDebugObjects[i].y3,
								fg_adDrawingDebugObjects[i].x4,
								fg_adDrawingDebugObjects[i].y4,
								fg_adDrawingDebugObjects[i].pUserPicture->pUnitPic->szFileName );
			}
			break;

		case AB_DEBUG_DRAWFBOX:									// DrawFBox�̏ꍇ
			if ( fg_adDrawingDebugObjects[i].nBlendKind == 0 ) {
				strcpy( szCommandName, "DrawFBox     " );
			}
			else {
				strcpy( szCommandName, "DrawTBox     " );
			}
			if ( !fg_adDrawingDebugObjects[i].bError ) {
				sprintf( szOut, "%s( %4d, %4d )-( %4d, %4d ) Color=0x%x", szCommandName,
																		fg_adDrawingDebugObjects[i].x1,
																		fg_adDrawingDebugObjects[i].y1,
																		fg_adDrawingDebugObjects[i].x2,
																		fg_adDrawingDebugObjects[i].y2,
																		fg_adDrawingDebugObjects[i].nColor );
			}
			break;

		case AB_DEBUG_OUT_STRING:								// �f�o�b�O������o�͂̏ꍇ
			strcpy( szOut, fg_szMyDebugStrings[fg_adDrawingDebugObjects[i].nUsedHandle] );
			break;
		}
		// �G���[�̏ꍇ�̕�����
		if ( fg_adDrawingDebugObjects[i].bError ) {
			sprintf( szOut, "%s( ERROR ) Handle = %d", szCommandName, fg_adDrawingDebugObjects[i].nUsedHandle );
		}
		AM_DrawString( x, y, szOut, fg_nDebugColor );
		y += nFontSize;
	}

	// �I�u�W�F�N�g������
	fg_nMyDrawingDebugObjects = 0;
	fg_nMyDebugStringNum = 0;

	return AM_OK;
}


// �f�o�b�O�@�\�E�|�[�Y�R���g���[���֐�
extern int AB_ControlDebugPause( void )
{
	static int					nLastInKey = 0;					// �ЂƂO�̃L�[����
	int							nInKey;							// ���݂̃L�[����
	int							nPushedKey;						// �������ꂽ�L�[
	int							nGetKey;						// �Q�b�g�����L�[
	static int					bPauseDemand = false;			// �|�[�Y�v��

	// �����L�[�`�F�b�N
	nInKey = CheckKey( AMINPUT_MULT );
	nPushedKey = nInKey & ~nLastInKey;
	nLastInKey = nInKey;

	// �f�o�b�O�\���L�[�`�F�b�N
	if ( nPushedKey & KEYIN_DEBUG_DISPLAY ) ToggleDisplayDebug();

	// �f�o�b�O�F�L�[�`�F�b�N
	if ( nPushedKey & KEYIN_DEBUG_COLOR ) ChangeDebugColor();

	// �|�[�Y�L�[�����`�F�b�N
	if ( !bPauseDemand ) {
		if ( !( nPushedKey & KEYIN_DEBUG_PAUSE ) ) return AM_OK;
	}

	// �L�[�҂�
	if ( KBHit() ) GetCh();
	nGetKey = GetCh();

	nLastInKey = 0;

	// ����|�[�Y���邩�`�F�b�N
	if ( nGetKey == 'P' || nGetKey == 'p' ) bPauseDemand = true;
									   else bPauseDemand = false;

	// �f�o�b�O�\���L�[�`�F�b�N(�|�[�Y��)
	if ( nGetKey == 'O' || nGetKey == 'o' ) {
		ToggleDisplayDebug();
		nLastInKey |= KEYIN_DEBUG_DISPLAY;
		bPauseDemand = true;									// �Ăу|�[�Y��
	}

	// �f�o�b�O�F�L�[�`�F�b�N(�|�[�Y��)
	if ( nGetKey == 'I' || nGetKey == 'i' ) {
		ChangeDebugColor();
		nLastInKey |= KEYIN_DEBUG_COLOR;
		bPauseDemand = true;									// �Ăу|�[�Y��
	}

	return AM_OK;
}


// �f�o�b�O�\���̂n�m�^�n�e�e�؂�ւ�
static int ToggleDisplayDebug( void )
{
	if ( gg_bDisplayDebug ) {
		gg_bDisplayDebug  = false;							// �\��
		if ( gg_bRegisterDebug ) gg_bRegisterDebug = false;	// �o�^
							else gg_bRegisterDebug = true;
//		gg_bRegisterDebug = false;							// �o�^
	}
	else {
		gg_bDisplayDebug  = true;							// �\��
		gg_bRegisterDebug = true;							// �o�^
	}

	// �I�u�W�F�N�g������
	fg_nMyDrawingDebugObjects = 0;
	fg_nMyDebugStringNum = 0;

	return AM_OK;
}


// �f�o�b�O�\���F�̐؂�ւ�
static int ChangeDebugColor( void )
{
	// �F��i�߂�
	fg_nPresentDebugColor++;
	if ( fg_nPresentDebugColor >= DEBUG_COLOR_VALS ) fg_nPresentDebugColor = 0;

	// �F���Z�b�g
	fg_nDebugColor = fg_nDebugColorVals[fg_nPresentDebugColor];

	return AM_OK;
}


// ���[�h�n�f�o�b�O���o�͊֐�
extern int AB_OutLoadingDebugInfo( AB_LOADING_DEBUG_INFO *pLoadDebugInfo )
{
	FILE						*fp;
	
	if ( ( fp = fopen( AB_DEBUG_FILE_NAME, "a" ) ) == NULL ) return AMERR_FILEIO;

	// �R�}���h���E�t�@�C�����\��
	switch( pLoadDebugInfo->nObjectKind ) {
	case AB_DEBUG_LOADTEXTURE:									// LoadTexture�֐�
		fprintf( fp, "LoadTexture �t�@�C�����F\"%s\" ", pLoadDebugInfo->szFileName );
		if ( pLoadDebugInfo->bError ) {
			fprintf( fp, "���s " );
			fprintf( fp, "Handle = %d", pLoadDebugInfo->nMadeHandle );
		}
		else {
			fprintf( fp, "���� " );
			fprintf( fp, "Handle = 0x%x", pLoadDebugInfo->nMadeHandle );
		}
		break;

	case AB_DEBUG_LOADBLKTEXTURE:								// LoadBlkTexture�֐�
	case AB_DEBUG_LOADBLKTEXTURELR:								// LoadBlkTextureLR�֐�
		if ( pLoadDebugInfo->nObjectKind == AB_DEBUG_LOADBLKTEXTURE ) fprintf( fp, "LoadBlkTexture " );
																 else fprintf( fp, "LoadBlkTextureLR " );
		fprintf( fp, "�t�@�C�����F\"%s\" ", pLoadDebugInfo->szFileName );
		if ( pLoadDebugInfo->bError ) {
			fprintf( fp, "���s " );
			fprintf( fp, "�߂�l = %d", pLoadDebugInfo->nMadeHandle );
		}
		else {
			fprintf( fp, "���� " );
			fprintf( fp, "Handle = 0x%x�`0x%x", pLoadDebugInfo->nMadeHandle, pLoadDebugInfo->nMadeHandleEnd );
		}
		break;

	case AB_DEBUG_RELOADGRAPH:									// ReloadGraph�֐�
		fprintf( fp, "ReloadGraph �t�@�C�����F\"%s\" ", pLoadDebugInfo->szFileName );
		if ( pLoadDebugInfo->bError ) {
			fprintf( fp, "���s " );
		}
		else {
			fprintf( fp, "���� " );
		}
		fprintf( fp, "Handle = 0x%x", pLoadDebugInfo->nMadeHandle );
		break;
	}

	fprintf( fp, "\n" );
	fclose( fp );

	return AM_OK;
}


//-----------------------------------------------------------------------------
// �ȏ�
