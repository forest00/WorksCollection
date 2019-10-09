// ---------------------------------------------------------------------------
//
// �t�@�C����: AM_Connect.h
//
// �@�\: ���C�u�����w�b�_���������ꂽ���̃T�[�r�X��񋟁B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------


#ifndef __AM_CONNECT_INCLUDED__
#define __AM_CONNECT_INCLUDED__

// �����N���C�u����
#pragma comment( lib, "d3d9"   )								// D3D9���C�u����
#pragma comment( lib, "d3dx9"   )								// D3DX9���C�u����
#pragma comment( lib, "dsound"  )								// DirectSound���C�u����
#pragma comment( lib, "dxguid"  )								// DirectX GUID���C�u����
#pragma comment( lib, "winmm"   )								// �}���`���f�B�A���C�u����
#pragma comment( lib, "vfw32"   )								// �f�W�^���r�f�I���C�u����
#pragma comment( lib, "msacm32" )								// �������k�𓀃��C�u����

// VisualStudio2005�΍�
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE								// ���`���̊֐��g�p
#endif

#pragma warning( disable : 4819 )								// �R�[�h�y�[�W�̌x���}�~

// �w�b�_
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#endif

// ---------------------------------------------------------------------------
//
// �t�@�C����: AM_DispEnv.h
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̉�ʊ��ݒ�������B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AM_DISPENV_INCLUDED__
#define __AM_DISPENV_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif

#ifndef __AM_CONNECT_INCLUDED__

#include <d3d9.h>

#endif


// ---------------------------------------------------------------------------
// �c�����������R�c�I�u�W�F�N�g

#define AM_DIRECT3D				IDirect3D9
#define AM_LPDIRECT3D			LPDIRECT3D9
#define AM_DIRECT3DDEVICE		IDirect3DDevice9
#define AM_LPDIRECT3DDEVICE		LPDIRECT3DDEVICE9
#define AM_VERTEXBUFFER			IDirect3DVertexBuffer9
#define AM_LPVERTEXBUFFER		LPDIRECT3DVERTEXBUFFER9
#define AM_2DPICTUREBUFFER		IDirect3DTexture9 //IDirect3DSurface9
#define AM_LP2DPICTUREBUFFER	LPDIRECT3DTEXTURE9 //LPDIRECT3DSURFACE9
#define AM_3DTEXTUREBUFFER		IDirect3DTexture9
#define AM_LP3DTEXTUREBUFFER	LPDIRECT3DTEXTURE9
#define AM_DISPLAYBUFFER		IDirect3DSurface9
#define AM_LPDISPLAYBUFFER		LPDIRECT3DSURFACE9


// ---------------------------------------------------------------------------
// ���ʃ��U���g�R�[�h

#define AM_OK					0								// ����I��
#define AMERR_NOFILE			-1								// �t�@�C����������Ȃ�
#define AMERR_INVALIDFILE		-2								// �t�@�C���`�����s��
#define AMERR_NOMEMORY			-3								// �������s��
#define AMERR_INVALIDVALUE		-4								// �s���Ȓl
#define AMERR_INITIALIZE		-5								// ���ɏ������ς�
#define AMERR_NODRIVER			-6								// �h���C�o�����݂��Ȃ�
#define AMERR_INVALIDMODE		-7								// �s���ȃ��[�h
#define AMERR_NOINITIALIZE		-8								// ������������Ă��Ȃ�
#define AMERR_DISPLAY			-9								// ��ʎ���̃G���[
#define AMERR_NOFREEHANDLE		-10								// �n���h���̋󂫂��Ȃ�
#define AMERR_NOFREESURFACE		-11								// �T�[�t�F�X�̋󂫂��Ȃ�
#define AMERR_CANNOTCREATE		-12								// �����s��
#define AMERR_SOUNDOUT			-13								// �����o�͂��ł��Ȃ�
#define AMERR_INVALIDFORM		-14								// �s���Ȍ`��
#define AMERR_INVALIDHANDLE		-15								// �n���h�����s��
#define AMERR_CANNOT			-16								// ���߂����s�s�\
#define AMERR_OUTOFRANGE		-17								// �l���͈͊O
#define AMERR_OVERBUFFER		-18								// �o�b�t�@�I�[�o�[
#define AMERR_FILEIO			-19								// �t�@�C�����o�̓G���[
#define AMERR_INVALIDSIZE		-20								// �T�C�Y���s��
#define AMERR_INVALIDPOINTER	-21								// �|�C���^���s��
#define AMERR_NOHARDWARE		-22								// �n�[�h�E�F�A�Ȃ�
#define AMERR_NOSUITABLEFORMAT	-23								// �K�؂ȃt�H�[�}�b�g�Ȃ�
#define AMERR_NOTSUPPORTED		-24								// �T�|�[�g�Ȃ�
#define AM_ENDOFSTREAM			-25								// �X�g���[���̏I���
#define AMERR_ENDED				-26								// �A�v���͊��ɏI�����ׂ�


// ---------------------------------------------------------------------------
// �@�\�R�[�h

#define AMMODE_WINDOWED			0x0001							// �E�B���h�E���[�h�w��
#define AMMODE_NOTUSEVRAM		0x0002							// �u�q�`�l�s�g�p�w��
#define AMMODE_USEVRAM			0x0004							// �u�q�`�l�g�p�w��i�f�t�H���g�j
#define AMMODE_NORESOURCE		0x0008							// ���\�[�X�s�g�p�w��
#define AMMODE_FINDRESOURCE		0x0010							// ���\�[�X�g�p�w��i�f�t�H���g�j
#define AMMODE_NOCOLORMATCH		0x0020							// �J���[�}�b�`���O�����w��
#define AMMODE_DOCOLORMATCH		0x0040							// �J���[�}�b�`���O�w��i�f�t�H���g�j
#define AMMODE_CHECKSAMEFILE	0x0080							// ���t�@�C�����`�F�b�N
#define AMMODE_LOADSAMEFILE		0x0100							// ���t�@�C���������[�h�i�f�t�H���g�j
#define AMMODE_USE3D			0x0200							// �R�c�g�p�w��i�f�t�H���g�j
#define AMMODE_NOTUSE3D			0x0400							// �R�c�s�g�p�w��
#define AMMODE_ALWAYSUSE3D		0x0800							// ��ɂR�c�g�p�w��


// ---------------------------------------------------------------------------
// ���ʃ}�N��

// �I�u�W�F�N�g�����S�ɊJ��
#define AM_SAFE_RELEASE( Obj )		if ( Obj ) { Obj->Release();  Obj = NULL; }


// ---------------------------------------------------------------------------
// �n���h���萔

#define AM_PIC2D_HANDLE_HEAD	0x10000000						// 2D�s�N�`���n���h���w�b�_
#define AM_MOVIE_HANDLE_HEAD	0x20000000						// 2D�s�N�`���n���h���w�b�_
#define AM_HANDLE_HEAD_MASK		0xff000000						// �n���h���w�b�_�}�X�N�l


// ---------------------------------------------------------------------------
// �ŗL�萔

#define AM_MAX_WIN_TITLE		80								// �ő�E�B���h�E�^�C�g����


// ---------------------------------------------------------------------------
// �O�����J�֐��v���g�^�C�v�錾��

extern int AM_InitDisplay( void );										// �f�B�X�v���C������
extern int AM_EndDisplay( void );										// �f�B�X�v���C�g�p�I��
extern int AM_SetAppliInstance( HINSTANCE hInstance );					// �A�v���P�[�V�����C���X�^���X�̐ݒ�
extern int AM_SetDisplayMode( int nWidth, int nHeight, int nBitDepth );	// �𑜓x�̐ݒ�
extern int AM_ProcessEvent( void );										// Windows�C�x���g����
extern int AM_Flip( void );												// ��ʂ̍X�V
extern int AM_WaitVBlankTrigger( void );								// VBlank�M���̃g���K��҂�
extern int AM_ClearBackBuffer( int r, int g, int b );					// �o�b�N�o�b�t�@�̃N���A
extern int AM_SetViewport( int x1, int y1, int x2, int y2 );			// �r���[�|�[�g�̐ݒ�


// ---------------------------------------------------------------------------
// �C���^�[�t�@�C���O���[�o���ϐ��v���g�^�C�v

// Window�֌W�I�u�W�F�N�g
extern HWND								gg_hWindow;						// �E�B���h�E�n���h��
extern HINSTANCE						gg_hInstance;					// �C���X�^���X�n���h��

// DirectX�֌W�I�u�W�F�N�g
extern AM_LPDIRECT3D					gg_pD3D;						// D3D�I�u�W�F�N�g
extern AM_LPDIRECT3DDEVICE				gg_pD3DDevice;					// D3DDevice�I�u�W�F�N�g
extern AM_LPDISPLAYBUFFER				gg_pBackBuffer;					// BackBuffer�I�u�W�F�N�g

// ��ʊ��ێ��ϐ�
extern SIZE								gg_siDispSize;					// �N���C�A���g�̈�̑傫��
extern SIZE								gg_siWholeDispSize;				// ��ʑS�̂̑傫��
extern int								gg_nBitDepth;					// �r�b�g�f�v�X
extern BOOL								gg_bFullMode;					// �t���X�N���[�����[�h��
extern POINT							gg_piDispLocate;				// �N���C�A���g�̈�̈ʒu
extern D3DFORMAT						gg_dfDisplayFormat;				// �f�B�X�v���C�t�H�[�}�b�g
extern BOOL								gg_bDispInitialized;			// ��ʏ������ς݂�
extern BOOL								gg_bSceneActive;				// �V�[���̓A�N�e�B�u��
extern char								gg_szWinTitle[AM_MAX_WIN_TITLE];	// �E�B���h�E�^�C�g��

// �v���Z�X����ϐ�
extern BOOL								gg_bEndFlag;					// �I���t���O


// �b��
//HRESULT InitVB();
//VOID Render();

#endif


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AB_DispEnv.h
//
// �@�\: �`�l�f�`�l�d�̉�ʊ��ݒ���u���b�W�B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AB_DISPENV_INCLUDED__
#define __AB_DISPENV_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif

#ifndef __AM_CONNECT_INCLUDED__

#include <windows.h>

#endif


// ---------------------------------------------------------------------------
// �O�����J�萔��

#define AM_TITLE_NAME			"Amgamew.sys"


// ---------------------------------------------------------------------------
// �O�����J�֐��v���g�^�C�v�錾��

extern HWND InitGraph( HINSTANCE hInstance );					// �O���t�B�b�N�X�̏�����
extern int EndGraph( void );									// �O���t�B�b�N�X�g�p�I��
extern int SetGlobalMode( int nModes );							// ���ˑ��O���[�o�����[�h�ݒ�

extern int SetGraphMode( int nWidth, int nHeight, int nBitDepth );	// �O���t�B�b�N���[�h�ݒ�
extern int ProcessEvent( void );								// �C�x���g����
extern void SetPage( int nActivePage, int nDisplayPage );		// �y�[�W�ݒ�i�_�~�[�j
extern int Flip( void );										// ��ʍX�V����
extern int SetFrameCounter( int nCount );						// �t���[���J�E���^�Z�b�g
extern int GetFrameCounter( void );								// �t���[���J�E���^�擾
extern void ClearVram( void );									//	VRAM�N���A
extern int HCls( void );										// ��ʃN���A
extern void WaitVSync( int nWaitNum );							// �f�B�X�v���C�̐��������M���҂�
extern void SetDrawArea( int x1, int y1, int x2, int y2 );		// �`��G���A�̐ݒ�

extern int AB_SetWindowTitle( char *szTitle );					// �E�B���h�E�^�C�g���̐ݒ�i����J�֐��j


#endif


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AM_2DGraph.h
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̂Q�c��ʕ`��@�\�������B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AM_2DGRAPH_INCLUDED__
#define __AM_2DGRAPH_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnv�w�b�_
//#include "AB_2DGraph.h"											// AB2DGraph�w�b�_

#endif


// ---------------------------------------------------------------------------
// �O�����J�萔��

#define AM_MAX_BLEND_FACTOR		255								// �u�����h�t�@�N�^�ő�l
#define AM_MAX_COLOR_FACTOR		255								// �F�t�@�N�^�ő�l

// ��ʎ擾���[�h
#define AM_GET_FRONT_BUFFER		0								// �t�����g�o�b�t�@�擾
#define AM_GET_BACK_BUFFER		1								// �o�b�N�o�b�t�@�擾


// ---------------------------------------------------------------------------
// �O�����J�\���̐錾��

// 2D���_�\����
typedef struct tagAM_2DVERTEX {
	float				x, y, z, rhw;							// ���_���W
	DWORD				color;									// ���_�F
	float				tu, tv;									// �e�N�X�`�����W
} AM_2DVERTEX;

#define D3DFVF_AM2DTEX			( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )	// 2D�e�N�X�`�����_�t�H�[�}�b�g

// �摜���\����
typedef struct tagAM_2DPIC_INFO {
	int					nWidth, nHeight;						// ���E����
	int					nRealWidth, nRealHeight;				// ���ۂ́i�����I�ȁj���E����
	int					nBitDepth, nRealDepth;					// ���̃r�b�g�f�v�X�Ǝ��ۂ̃r�b�g�f�v�X
} AM_2DPIC_INFO;

// �u�����h�X�e�[�g�\����
typedef struct tagAM_BLEND_STATE {
	int					nBlendMode;								// �u�����h���[�h
	int					nBlendFactor;							// �u�����h�t�@�N�^
	int					nBrRed, nBrGreen, nBrBlue;				// �΁E�ԁE�̖��邳
} AM_BLEND_STATE;

// 2D�x�N�^�\����
typedef struct tagAM_2DVECTOR {
	float				x, y;									// ����
} AM_2DVECTOR;

// ������`�\����
typedef struct tagAM_RECT {
	int					left, top;								// ���㐬��
	int					right, bottom;							// �E������
} AM_RECT;


// ---------------------------------------------------------------------------
// �C���^�[�t�@�C���O���[�o���ϐ���

extern AM_BLEND_STATE			gg_bsBlendState;				// ���݂̃u�����h�X�e�[�g
extern AM_2DVERTEX				gg_v2DrawRect[4];				// �`���`�p���_


// ---------------------------------------------------------------------------
// �O�����J�֐��v���g�^�C�v�錾��

// ���[�h�֌W
extern AM_LP2DPICTUREBUFFER AM_Load2DGraph( char *szFileName, AM_2DPIC_INFO *pPicInfo );	// 2D�O���t�B�b�N���[�h
extern int AM_Reload2DGraph( char *szFileName, AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo );
																// 2D�O���t�B�b�N�ă��[�h
extern AM_LP2DPICTUREBUFFER AM_Make2DGraphBuf( AM_2DPIC_INFO *pPicInfo );
																// 2D�O���t�B�b�N�o�b�t�@�쐬

// �`��֌W
extern int AM_Draw2DGraph( int dx1, int dy1, int dx2, int dy2,
						   int sx1, int sy1, int sx2, int sy2, 
						   AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo );
																// 2D�O���t�B�b�N�`��
extern int AM_Draw2DGraph4_2( AM_2DVECTOR *pDestVerts,
							  int sx1, int sy1, int sx2, int sy2, 
							  AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo );
																// 2D�O���t�B�b�N�`��i�`���S�_�A�`�挳�Q�_�w��j
extern int AM_Draw2DBlockMaskEmu( int x, int y,
								  int sx1, int sy1, int sx2, int sy2, 
								  int nMaskWidth, int nMaskHeight, unsigned char *pMask,
								  AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo );
																// �}�X�N�`��G�~�����[�V����

// �`��X�e�[�g�֌W
extern int AM_InitBlendState( void );							// �u�����h�X�e�[�g������
extern int AM_SetBlendState( int nBlendCode, int nBlendFactor );	// �u�����h�X�e�[�g�ݒ�
extern int AM_RestoreBlendState( AM_BLEND_STATE *pbsRestoreBlend );	// �u�����h�X�e�[�g���A
extern int AM_GetBlendState( AM_BLEND_STATE *pbsBlendFactor );		// �u�����h�X�e�[�g�擾
extern int AM_SetDrawColor( int nRed, int nGreen, int nBlue );		// �`��F�Z�b�g
extern int AM_SetTexSampleState( int nSampleCode );					// �e�N�X�`���T���v���X�e�[�g�ݒ�
extern int AM_SetTexWrapState( int nWrapCode );						// �e�N�X�`�����b�v�X�e�[�g�ݒ�

// 2D�摜���ӊ֐�
extern int AM_GetDisplayBuffer( AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo, int nGetMode );
																// �\���p�o�b�t�@�̓��e�擾
extern int AM_SaveDisplayBuffer( AM_RECT *pSaveRect, char *szFileName, int nGetMode );
// �\���p�o�b�t�@�̃t�@�C���ւ̃Z�[�u

#endif


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AB_2DGraph.h
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̂Q�c��ʕ`��@�\���u���b�W�B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AB_2DGRAPH_INCLUDED__
#define __AB_2DGRAPH_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnv�w�b�_
#include "AM_2DGraph.h"											// AM2DGraph�w�b�_

#endif


// ---------------------------------------------------------------------------
// �O�����J�萔��

// �ėp�萔
#define AM_MAX_PICTURE_NAME		200								// �ő�G�t�@�C������

// �T���v�����[�h
#define AMDRAW_NEAREST			0x0001							// �j�A���X�g�l�C�o�[
#define AMDRAW_BILINEAR			0x0002							// �o�C���j�A�t�B���^�����O

// �u�����h���[�h
#define AMDRAW_NOBLEND			0x0004							// �u�����h�Ȃ�
#define AMDRAW_ALPHABLEND		0x0008							// �A���t�@�u�����f�B���O
#define AMDRAW_ADDBLEND			0x0010							// ���Z�u�����f�B���O

// ���b�v���[�h
#define AMDRAW_TEX_NOREPEAT		0x0020							// �e�N�X�`�����s�[�g�Ȃ�
#define AMDRAW_TEX_REPEAT		0x0040							// �e�N�X�`�����s�[�g����

// DrawMemTrans�֐��p�u�����h���[�h
#define TRANS_AVERAGE		0									// ���ςɂ�锼����
#define TRANS_AVERAGE_TH	1									// ���ςɂ�锼�����i�����F�t���j
#define TRANS_ADD			2									// ���Z�ɂ�锼����

// �G�̑���
#define AM_PICTURE_LR			0x0001							// ���E���]�t���O

// �`�摮��
#define AM_DRAWPIC_LR			0x0001							// �`�捶�E���]�t���O
#define AM_DRAWPIC_PICALPHA		0x0002							// �G�̃A���t�@�l�ŃA���t�@�u�����h
#define AM_DRAWPIC_AVEALPHA		0x0004							// ���σA���t�@�u�����h
#define AM_DRAWPIC_ADDALPHA		0x0008							// ���Z�u�����h


// ---------------------------------------------------------------------------
// �O�����J�\���̐錾��

// �P�ʃs�N�`���\����
typedef struct tagAM_UNIT_PICTURE {
	AM_2DPIC_INFO				a2PicInfo;						// �s�N�`�����
	AM_LP2DPICTUREBUFFER		pPicture;						// �s�N�`��
	char						szFileName[AM_MAX_PICTURE_NAME];	// �s�N�`���t�@�C����
} AM_UNIT_PICTURE;

// ���[�U�s�N�`���\����
typedef struct tagAM_USER_PICTURE {
	AM_UNIT_PICTURE				*pUnitPic;						// �P�ʃs�N�`��
	int							nLeft, nTop;					// �P�ʃs�N�`�����ł̈ʒu
	int							nWidth, nHeight;				// ���ƍ���
	int							bFlags;							// �����t���O
} AM_USER_PICTURE;

/*// 2D�x�N�^�\����
typedef struct tagAM_2DVECTOR {
	float						x, y;							// ����
} AM_2DVECTOR;*/


// ---------------------------------------------------------------------------
// �O�����J�֐��v���g�^�C�v�錾��

// 2D�摜���[�h�֐�
extern int LoadBmpMem( char *szFileName );						// 2D�O���t�B�b�N���[�h(��)
extern int LoadTexture( char *szFileName );						// 2D�O���t�B�b�N���[�h
extern int LoadBlkBmpMem( char *szFileName, int nXSize, int nYSize,
						  int nXNum, int nYNum, int nAllNum, int *phHandleBuf );
																// �u���b�N��2D�O���t�B�b�N���[�h(��)
extern int LoadBlkTexture( char *szFileName, int nXSize, int nYSize,
						   int nXNum, int nYNum, int nAllNum, int *phHandleBuf );
																// �u���b�N��2D�O���t�B�b�N���[�h
extern int LoadBlkBmpMemLR( char *szFileName, int nXSize, int nYSize,
							int nXNum, int nYNum, int nAllNum, int *phHandleBuf );
																// �u���b�N��2D�O���t�B�b�N���[�h(���A���E���])
extern int LoadBlkTextureLR( char *szFileName, int nXSize, int nYSize,
							 int nXNum, int nYNum, int nAllNum, int *phHandleBuf );
																// �u���b�N��2D�O���t�B�b�N���[�h(���E���])
extern int MakeGraphMem( int nWidth, int nHeight );				// 2D�O���t�B�b�N�o�b�t�@�쐬
extern int MakeTextureMem( int nWidth, int nHeight, int nBitDepth );	// 2D�O���t�B�b�N�o�b�t�@�쐬
extern int ReloadGraph( char *szFileName, int hPicture );		// 2D�O���t�B�b�N�ă��[�h
extern int LoadBmp( int x, int y, char *szFileName );			// 2D�O���t�B�b�N�[�����ڃ��[�h
extern int LoadBmpNP( int x, int y, char *szFileName );
extern int LoadBmpTh( int x, int y, char *szFileName );			// 2D�O���t�B�b�N�[�����ڃ��[�h(�J���[�L�[�t��)
extern int LoadBmpThNP( int x, int y, char *szFileName );

// �J���֐�
extern int InitGraphMem( void );								// �S�r�b�g�}�b�v�J��
extern int InitTexture( void );									// �S�e�N�X�`���J��
extern int AB_Release2DGraphs( void );							// 2D�O���t�B�b�N�J��
extern int AB_ReleaseLastGraph( int hPicture );					// �Ō�ɍ��ꂽ�O���t�B�b�N�J��

// 2D�摜�`��֐�
extern int DrawMem( int x, int y, int hPicture );				// 2D�`��
extern int DrawMemTh( int x, int y, int hPicture );				// �A���t�@�u�����h�i�J���[�L�[�j�t��2D�`��
extern int DrawMemLR( int x, int y, int hPicture );				// 2D���E���]�`��
extern int DrawMemThLR( int x, int y, int hPicture );			// 2D���E���]�`��i�J���[�L�[�t���j
extern int DrawMemExp( int x1, int y1, int x2, int y2, int hPicture );	// 2D�g��k���`��
extern int DrawMemThExp( int x1, int y1, int x2, int y2, int hPicture );	// �A���t�@�u�����h�i�J���[�L�[�j�t��2D�g��k���`��
extern int DrawMemRot( int nCenX1, int nCenY1, int nCenX2, int nCenY2,
					   double fAngle, double fExRate,
					   int hAmTexture );
																// 2D�g��k���`��
extern int DrawMemThRot( int nCenX1, int nCenY1, int nCenX2, int nCenY2,
						 double fAngle, double fExRate,
						 int hPicture );
																// 2D��]�`��i�J���[�L�[�t���j
extern int DrawMemFlex( int x1, int y1, int x2, int y2,
						int x3, int y3, int x4, int y4,
						int hPicture );
																// 2D�ό`�`��
extern int DrawMemThFlex( int x1, int y1, int x2, int y2,
						  int x3, int y3, int x4, int y4,
						  int hPicture );
																// 2D�ό`�`��i�J���[�L�[�t���j
extern int DrawMemMask( int x, int y, int hPicture,
						int nWidth, int nHeight, unsigned char *pMask );
																// 2D�}�X�N�t���`��(�[��)
extern int DrawMemThMask( int x, int y, int hPicture,
						  int nWidth, int nHeight, unsigned char *pMask );
																// 2D�}�X�N�A�J���[�L�[�t���`��(�[��)
extern int  DrawMemTrans( int x, int y, int hPicture, int nMode );		// 2D�������`��


// 2D�摜���ӊ֐�
extern int GetPictureSize( int hPicture, int *pnWidth, int *pnHeight );
																// �摜�T�C�Y�擾
extern int GetDispGraph( int hPicture );						// ���ݕ\������Ă���O���t�B�b�N�擾
extern int SaveBmp( int x1, int y1, int x2, int y2, char *szFileName );
																// �r�b�g�}�b�v�̃Z�[�u
extern int SetBmpOffset( int nOffset );							// ���[�h�摜�f�[�^�I�t�Z�b�g�ݒ�(�_�~�[)
extern int SetDrawMode( int nDrawMode, int nParam );			// �`�惂�[�h�̐ݒ�
extern int AB_GetUserPicture( int hPicture, AM_USER_PICTURE **ppupUserPicture );
																// �n���h�����烆�[�U�[�s�N�`���擾


#endif


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AB_Palette.cpp
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̂Q�c��ʕ`��@�\�������B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AB_PALETTE_INCLUDED__
#define __AB_PALETTE_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_2DGraph.h"											// AM2DGraph�w�b�_

#endif

// ---------------------------------------------------------------------------
// �O�����J�萔��

#define AM_MAX_DRAW_BRIGHT		1000							// �ő�`��P�x


//-----------------------------------------------------------------------------
// �_�~�[�֐���

extern void SetPalette( int pnum, int gbl, int rbl, int bbl );	// �p���b�g�ݒ�i�_�~�[�j
extern int GetPalette( int pnum, int *gbl, int *rbl, int *bbl );
																// �擾�i�_�~�[�j
extern int SetBmpPal( char *szBitmap );							// �a�l�o�p���b�g�ݒ�i�_�~�[�j
extern int SetBmpPalPart( char *szBitmap, int FirstEnt, int EntNum );
																// �a�l�o�p���b�g�����ݒ�i�_�~�[�j
extern int LoadPalMem( char *szFileName );						// �p���b�g���[�h�i�_�~�[�j
extern int MakePalMem( void );									// �p���b�g�p�������쐬�i�_�~�[�j
extern int SetPalToMem( int hPalette, int nEntry, int gbr, int rbr, int bbr );
																// �������p���b�g�փp���b�g�Z�b�g�i�_�~�[�j
extern int GetPalFromMem( int hPalette, int nEntry, int *gbr, int *rbr, int *bbr );
																// �������p���b�g����p���b�g�擾�i�_�~�[�j
extern int SetPalMem( int hPalette );							// �������p���b�g�Z�b�g�i�_�~�[�j
extern int SetPalMemPart( int hPalette, int nFirstEnt, int nEntNum );
																// �������p���b�g�����Z�b�g�i�_�~�[�j
extern int FreePalMem( int hPalette );							// �p���b�g�������̊J���i�_�~�[�j
extern int SetPalArea( int under, int over );					// �p���b�g�ݒ�͈͎w��i�_�~�[�j


//-----------------------------------------------------------------------------
// �g�[���R���g���[���֐���

extern int SetDrawBright( int nRedBright, int nGreenBright, int nBlueBright );
																// �`�斾�x�̐ݒ�
extern int ControlPal( int nBrGreen, int nBrRed, int nBrBlue );	// �p���b�g�F���R���g���[��
extern int MovePalette( int nSGreen, int nSRed, int nSBlue,
						int nDGreen, int nDRed, int nDBlue, int nSpeed );
																// ��ʖ��x�̕ω�


#endif


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AB_3DGraph.h
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̂R�c��ʕ`��@�\���u���b�W�B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------


#ifndef __AB_3DGRAPH_INCLUDED__
#define __AB_3DGRAPH_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif


// ---------------------------------------------------------------------------
// �C���N���[�h��

#ifndef __AM_CONNECT_INCLUDED__

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#endif


// ---------------------------------------------------------------------------
// �O�����J�萔��

#define AM3D_VALUE				float							// �R�c�l

// �|���S���`��萔
#define AMPOLY_NOLIGHT		( 1 << 0 )							// ���C�e�B���O�Ȃ�
#define AMPOLY_LIGHTING		( 1 << 1 )							// ���C�e�B���O����
#define AMPOLY_QUADRANGLE	( 1 << 2 )							// �l�p�|���S���`��
#define AMPOLY_TRIANGLE		( 1 << 3 )							// �O�p�|���S���`��

// �s��萔
#define AM_MATRIX_LW			0								// ���[�J�������[���h
#define AM_MATRIX_WV			1								// ���[���h���r���[


// ---------------------------------------------------------------------------
// �O�����J�\���̐錾��

// 3D���_�\����
typedef struct tagAmVertex {
	AM3D_VALUE					x, y, z;						// ���W
	AM3D_VALUE					u, v;							// �e�N�X�`�����W
	AM3D_VALUE					nx, ny, nz;						// �@���x�N�g��
	unsigned char				r, g, b, a;						// �F�E�A���t�@�l
} AmVertex;

// �s��\����
typedef struct tagAmMatrix {
	AM3D_VALUE				m[4][4];							// �s��v�f
} AmMatrix;

// �x�N�g���\����
typedef struct tagAmVector {
	AM3D_VALUE				x, y, z;							// �x�N�g���v�f
} AmVector;


// ---------------------------------------------------------------------------
// �O�����J�֐��v���g�^�C�v�錾��

// �`��֐�
extern int DrawPolygons( AmVertex *pVertices, int nVertexNum, int hTexture, int nFlags );
																// �|���S���`��
extern int Draw3DLines( AmVertex *pVertices, int nVertexNum, int hTexture );
																// ����`��
// �c�[���֐�
extern int SetLightVertex( AmVertex *pVertex, AM3D_VALUE x, AM3D_VALUE y, AM3D_VALUE z,
						   AM3D_VALUE u, AM3D_VALUE v,
						   AM3D_VALUE nx, AM3D_VALUE ny, AM3D_VALUE nz );
																// ���C�e�B���O���蒸�_�f�[�^�Z�b�g
extern int SetColorVertex( AmVertex *pVertex, AM3D_VALUE x, AM3D_VALUE y, AM3D_VALUE z,
						   AM3D_VALUE u, AM3D_VALUE v,
						   unsigned char r, unsigned char g, unsigned char b, unsigned char a );
																// ���C�e�B���O�Ȃ����_�f�[�^�Z�b�g
extern int SetLWMatrix( AmMatrix *pMatrix );					// ���[�J�������[���h�s��Z�b�g
extern int SetWVMatrix( AmMatrix *pMatrix );					// ���[���h���r���[�s��Z�b�g
extern int GetLWMatrix( AmMatrix *pMatrix );					// ���[�J�������[���h�s��擾
extern int GetWVMatrix( AmMatrix *pMatrix );					// ���[���h���r���[�s��擾
extern int MultMatrix( AmMatrix *pDest, AmMatrix *pSrc1, AmMatrix *pSrc2 );
																// �s��̐όv�Z
extern int MultCross( AmVector *pDest, AmVector *pSrc1, AmVector *pSrc2 );
																// �O�όv�Z
extern int SetIdentityMatrix( AmMatrix *pDest );				// �P�ʍs��Z�b�g
extern int SetRTMatrix( AmMatrix *pDest, AM3D_VALUE tx, AM3D_VALUE ty, AM3D_VALUE tz,
										 AM3D_VALUE rx, AM3D_VALUE ry, AM3D_VALUE rz );
																// ��]�{���s�ړ��s��Z�b�g


#endif


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AM_3DGraph.h
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̂R�c��ʕ`��@�\�������B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------


#ifndef __AM_3DGRAPH_INCLUDED__
#define __AM_3DGRAPH_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif


// ---------------------------------------------------------------------------
// �C���N���[�h��

#ifndef __AM_CONNECT_INCLUDED__

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "AM_2DGraph.h"
#include "AB_3DGraph.h"

#endif


// ---------------------------------------------------------------------------
// �O�����J�\���̐錾��

// 2D���_�\����
typedef struct tagAM_3DVERTEX {
    D3DXVECTOR3					vPos;							// �ʒu
    D3DXVECTOR3					vNorm;							// �@���x�N�g��
	DWORD						color;							// ���_�F
    float						tu, tv;							// �e�N�X�`�����W
} AM_3DVERTEX;

#define D3DFVF_AM3DVERTEX		( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )	// 3D���_�t�H�[�}�b�g


// ---------------------------------------------------------------------------
// �O�����J�֐��v���g�^�C�v�錾��

extern int AM_Init3DGraph( void );								// 3D�ݒ菉����
extern int AM_End3DGraph( void );								// 3D�O���t�B�b�N�X�g�p�I��
extern int AM_SetTransformMatrix( int nKind, AmMatrix *pmaMatrix );
																// �s�񏉊���
extern int AM_GetTransformMatrix( int nKind, AmMatrix *pmaMatrix );
																// �s��擾

extern int AM_Draw3DPolygons( AmVertex *pVertices, int nVertexNum,
							  int sx, int sy,
							  AM_LP3DTEXTUREBUFFER pTexture, AM_2DPIC_INFO *pPicInfo, int nFlags );
																// 3D�|���S���`��
extern int AM_Draw3DLines( AmVertex *pVertexes, int nVertexNum,
						   int sx, int sy,
						   AM_LP3DTEXTUREBUFFER pTexture, AM_2DPIC_INFO *pPicInfo );
																// 3D����`��

#endif


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AM_FigureDraw.h
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̂Q�c��ʕ`��@�\�������B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AM_FIGUREDRAW_INCLUDED__
#define __AM_FIGUREDRAW_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif

// ---------------------------------------------------------------------------
// �C���N���[�h��

//#include <d3d9.h>
//#include <d3dx9.h>

//#include "AM_DispEnv.h"											// AMDispEnv�w�b�_
//#include "AM_2DGraph.h"											// AMDispEnv�w�b�_


// ---------------------------------------------------------------------------
// �O�����J�萔

#define AM_MAX_POINT_NUM		640								// �`��ő�_��
#define AM_MAX_RECT_NUM			( AM_MAX_POINT_NUM / 2 )		// �`��ő咷���`��

#define AM_DEFAULT_FONT_SIZE	16								// �f�t�H���g�t�H���g�T�C�Y


// ---------------------------------------------------------------------------
// �O�����J�\����

// �_�f�[�^
typedef struct tagAM_POINT_DATA {
	int							x, y, z;						// ���W
	unsigned int				nColor;							// �F
} AM_POINT_DATA;


//-----------------------------------------------------------------------------
// �}�`�`��֐�

extern int AM_DrawPointArray( AM_POINT_DATA *pPoints, int nPointNum );
																// �_��̕`��
extern int AM_GetColorCode( int r, int g, int b );				// �F�R�[�h�̎擾
extern int AM_GetBackbufferPointColor( int x, int y );			// �o�b�N�o�b�t�@�̎w��_�̐F�擾
extern int AM_DrawRectangleArray( AM_POINT_DATA *papPoints, int nRectNum );
																// �����`�̕`��
// ������֌W
extern int AM_PrepareFontObject( void );						// ������t�H���g�̏���
extern int AM_ReleaseFontObject( void );						// ������t�H���g�̊J��
extern int AM_SetStringFontSize( int nFontSize );				// ������t�H���g�T�C�Y�̐ݒ�

extern int AM_GetStringFontSize( void );						// ������t�H���g�T�C�Y�̎擾
extern int AM_DrawString( int x, int y, char *szString, int nColor );
																// ������̕`��


#endif


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AB_FigureDraw.h
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̂Q�c��ʕ`��@�\���u���b�W�B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AB_FIGUREDRAW_INCLUDED__
#define __AB_FIGUREDRAW_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif

// ---------------------------------------------------------------------------
// �C���N���[�h��


// ---------------------------------------------------------------------------
// �O�����J�萔��

#define AM_MIN_FONT_SIZE			4							// �ŏ��t�H���g�T�C�Y
#define AM_MAX_FONT_SIZE			8192						// �ő�t�H���g�T�C�Y


//-----------------------------------------------------------------------------
// �}�`�`��֐�

extern void DrawPixel( int x, int y, int color );				// 1�_�̕`��
extern int GetColor( int nRed, int nGreen, int nBlue );			// �F�R�[�h�̎擾
extern int GetPixel( int x, int y );							// �w��_�̐F�擾
extern void DrawFBox( int x1, int y1, int x2, int y2, int color );
																// �����`�̕`��
extern void DrawTBox( int x1, int y1, int x2, int y2, int color );
																// �����������`�̕`��
// ������֌W
extern int DrawString( int x, int y, char *szString, int nColor );
																// ������̕`��
extern int SetFontSize( int nFontSize );						// ̫�Ļ��ނ̕ύX


#endif
// ---------------------------------------------------------------------------
//
// �t�@�C����: AM_Input.h
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̃��[�U���͋@�\�������B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------


#ifndef __AM_INPUT_INCLUDED__
#define __AM_INPUT_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif

// ---------------------------------------------------------------------------
// �O�����J�萔��

#define AM_MAX_PAD_NUM			2										// �ő�p�b�h��


// ---------------------------------------------------------------------------
// �C���^�[�t�@�C���O���[�o���ϐ���

extern int						gg_nJoyPadNum;					// �W���C�p�b�h��
extern int						gg_bMouseCursor;				// �}�E�X�J�[�\��
extern int						gg_bMouseActive;				// �}�E�X�L����


// ---------------------------------------------------------------------------
// �O�����J�֐��v���g�^�C�v�錾��

// �L�[�{�[�h�֌W
extern int AM_GetSingleChar( void );									// �L�[���͑҂��E���̓L�[�擾�֐�
extern int AM_IsKeyHit( void );											// �L�[�{�[�h�����`�F�b�N�֐�
extern int AM_SetKeyPushed( int nKeyCode );								// �L�[���͒ʒm�֐�
extern int AM_AddKeyInput( int nKeyState );								// �L�[���͕t��

// �p�b�h�֌W
extern int AM_InitJoyPad( int nMaxPadNum );								// �p�b�h������
extern int AM_EndJoyPad( void );										// �p�b�h�g�p�I��
extern int AM_AddJoyInput( int nPadIndex, int nKeyState );				// �P�̃p�b�h���͕t��

// �}�E�X�֌W
extern int AM_InitMouse( int bMouseState );								// �}�E�X������
extern int AM_EndMouse( void );											// �}�E�X�g�p�I��
extern int AM_SetMouseState( int bMouseState );							// �}�E�X��Ԑݒ�
extern int AM_GetMousePosition( int *nXLoc, int *nYLoc );				// �}�E�X�ʒu�擾
extern int AM_SetMouseLoc( int nXLoc, int nYLoc );						// �}�E�X�ʒu�ݒ�


#endif


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̃��[�U���͋@�\���u���b�W�B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------


#ifndef __AB_INPUT_INCLUDED__
#define __AB_INPUT_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif

// ---------------------------------------------------------------------------
// �O�����J�萔��

// �L�[�萔
#define KEYIN_2					0x0001					// �Q�L�[�`�F�b�N�}�X�N
#define KEYIN_DOWN				0x0001					// ���L�[�`�F�b�N�}�X�N
#define KEYIN_4					0x0002					// �S�L�[�`�F�b�N�}�X�N
#define KEYIN_LEFT				0x0002					// ���L�[�`�F�b�N�}�X�N
#define KEYIN_6					0x0004					// �U�L�[�`�F�b�N�}�X�N
#define KEYIN_RIGHT				0x0004					// ���L�[�`�F�b�N�}�X�N
#define KEYIN_8					0x0008					// �W�L�[�`�F�b�N�}�X�N
#define KEYIN_UP				0x0008					// ���L�[�`�F�b�N�}�X�N
#define KEYIN_SPC				0x0010					// �X�y�[�X�`�F�b�N�}�X�N
#define KEYIN_ESC				0x0020					// �����߃`�F�b�N�}�X�N
#define KEYIN_START				0x0020					// �X�^�[�g�L�[�`�F�b�N�}�X�N
#define KEYIN_SFT				0x0040					// �V�t�g�`�F�b�N�}�X�N
#define KEYIN_RET				0x0080					// ���^�[���`�F�b�N�}�X�N
#define KEYIN_Z					0x0100					// �y�`�F�b�N�}�X�N
#define KEYIN_X					0x0200					// �w�`�F�b�N�}�X�N
#define KEYIN_C					0x0400					// �b�`�F�b�N�}�X�N
#define KEYIN_A					0x0800					// �`�`�F�b�N�}�X�N
#define KEYIN_S					0x1000					// �r�`�F�b�N�}�X�N
#define KEYIN_D					0x2000					// �c�`�F�b�N�}�X�N
#define KEYIN_Q					0x4000					// �p�`�F�b�N�}�X�N
#define KEYIN_W					0x8000					// �v�`�F�b�N�}�X�N

// �f�o�b�L���O�L�[�萔
#define KEYIN_DEBUG_PAUSE		0x01000000				// �|�[�Y�`�F�b�N�}�X�N
#define KEYIN_DEBUG_DISPLAY		0x02000000				// �f�o�b�O�\���`�F�b�N�}�X�N
#define KEYIN_DEBUG_COLOR		0x04000000				// �f�o�b�O�F�ύX�`�F�b�N�}�X�N

// �}�E�X�{�^���萔
#define MOUSEKEY_LEFT			0x00010000				// �}�E�X���{�^��
#define MOUSEKEY_RIGHT			0x00020000				// �}�E�X�E�{�^��
#define MOUSEKEY_MIDDLE			0x00040000				// �}�E�X�����{�^��

// �}�E�X�{�^���@�L�[�R�[�h
#define MOUSECODE_LEFT			0x101					// �}�E�X���{�^���R�[�h
#define MOUSECODE_RIGHT			0x102					// �}�E�X�E�{�^���R�[�h
#define MOUSECODE_MIDDLE		0x103					// �}�E�X�����{�^���R�[�h

// �L�[���̓��[�h
#define AMINPUT_MULT			0								// �}���`�v���b�N�X����
#define AMINPUT_KEYBOARD		1								// �L�[�{�[�h����
#define AMINPUT_PAD1			2								// �p�b�h�P����
#define AMINPUT_PAD2			3								// �p�b�h�Q����


// ---------------------------------------------------------------------------
// �O�����J�֐��v���g�^�C�v�錾��

extern int GetCh( void );												// �ꕶ������
extern int KBHit( void );												// �L�[���̓`�F�b�N
extern int CheckKey( int nInputMode );									// �L�[��������Ă��邩�`�F�b�N

// �}�E�X�֌W
extern int InitMouse( int bMouseState );								// �}�E�X������
extern int SetMouseState( int bMouseState );							// �}�E�X��Ԑݒ�
extern int GetMouseLoc( int *nXLoc, int *nYLoc );						// �}�E�X�ʒu�擾
extern int SetMouseLoc( int nXLoc, int nYLoc );							// �}�E�X�ʒu�ݒ�


#endif
// ---------------------------------------------------------------------------
//
// �t�@�C����: AM_Movie.h
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̃��[�r�[�Đ��@�\�������B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AM_MOVIE_INCLUDED__
#define __AM_MOVIE_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif

#include <windows.h>
#include <digitalv.h>
#include <vfw.h>
#include <msacm.h>
#include <mmsystem.h>
#include <dsound.h>

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_2DGraph.h"											// AM2DGraph�w�b�_

#endif


#define MOVIE_FULL_LOC	-1000000					// ���[�r�[�t���Đ��ʒu


typedef struct tagAMAVIAUDIO {						// ���k��������\����
	PAVISTREAM			pAudioStream;				// �I�[�f�B�I�X�g���[��
	HACMSTREAM			hACMStream;					// �`�b�l�X�g���[���n���h��
	ACMSTREAMHEADER		acmHeader;					// �`�b�l�X�g���[���w�b�_
//	char				*pOutBuffer1, *pOutBuffer2;	// �o�̓f�[�^�o�b�t�@1,2
	LPDIRECTSOUNDBUFFER	lpDSBPlay;					// DirectSound�Đ��o�b�t�@
	BYTE				*pAudioSrcBuf;				// �]���������o�b�t�@
	DWORD				nAudioSrcBufSize;			// �����]�����T�C�Y
	BYTE				*pAudioDestBuf;				// �]���批���o�b�t�@
	DWORD				nAudioDestBufSize;			// �����]����T�C�Y
	LONG				nBlockNum;					// �o�b�t�@�u���b�N��
	LONG				nCurrentBlock;				// ���݂̍Đ��u���b�N

} AMAVIAUDIO;


typedef struct tagAMMOVIE {							// ���[�r�[�\����
	PAVIFILE			pAVIFile;					// �`�u�h�t�@�C��
	AVIFILEINFO			AVIInfo;					// �`�u�h�t�@�C�����
	PAVISTREAM			pVideoStream;				// �r�f�I�X�g���[��
	AVISTREAMINFO		VideoInfo;					// �r�f�I�X�g���[�����
	PGETFRAME			pVideoFrame;				// �r�f�I�t���[��
	PAVISTREAM			pAudioStream;				// �I�[�f�B�I�X�g���[��
	AVISTREAMINFO		AudioInfo;					// �I�[�f�B�I�X�g���[�����
//	int					nWidth, nHeight;			// ���ƍ���
	double				fVidSampDelay;				// �r�f�I�T���v���Ԋu
	double				fFirstTime;					// �ŏ��̎���
	int					nPresFrame;					// ���݂̃t���[��
} AMMOVIE;


extern MCIDEVICEID				gg_wAVIDevice;					// �`�u�h�f�o�C�X�h�c

extern int AM_PlayMovie( char *szFileName, int x, int y, int nWidth, int nHeight );
																// �ʏ타�[�r�[�Đ�
extern int AM_StopMovie( void );								// �ʏ타�[�r�[�Đ��X�g�b�v

extern int AM_OpenMovieFile( char *szFileName, AMMOVIE *pMovieData );
																// ���[�r�[�t�@�C���I�[�v��
extern int AM_CloseMovieFile( AMMOVIE *pMovieData );			// ���[�r�[�t�@�C���N���[�Y
extern int AM_PlayMovieFile( int x, int y, AMMOVIE *pMovieData );	// ���[�r�[�t�@�C���Đ�
extern int AM_GetMovieFrame( AMMOVIE *pMovieData, int nFrameLoc,
							 AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo );
																// ���[�r�[�̓���t���[���擾


#endif
// ---------------------------------------------------------------------------
//
// �t�@�C����: AB_Movie.h
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̃��[�r�[�Đ��@�\���u���b�W�B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AB_MOVIE_INCLUDED__
#define __AB_MOVIE_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif


// ---------------------------------------------------------------------------
// �O�����J�萔��

#define AM_MAX_MOVIE_NUM		10								// �ő僀�[�r�[��



// ---------------------------------------------------------------------------
// �O�����J�֐��v���g�^�C�v�錾��

extern int PlayMovie( int x, int y, char *szFileName );			// ���[�r�[�Đ�
extern int PlayMovieBack( int x, int y, char *szFileName );		// �o�b�N���[�r�[�Đ�
extern int PlayMovieFull( char *szFileName );					// �ٽ�ذ݃��[�r�[�Đ�
extern int PlayMovieFullBack( char *szFileName );				// �ٽ�ذ��ޯ�Ѱ�ް�Đ�(�_�~�[)
extern int SetMovieTime( int nFirst, int nLast );				// ���[�r�[�Đ����Ԑݒ�

extern int LoadMovieMem( char *szFileName );					// ���[�r�[�̃������ւ̃��[�h
extern int PlayMovieMem( int x, int y, int hMovie );			// ���������[�r�[�̍Đ�
extern int PlayMovieMemFull( int hMovie );						// ���������[�r�[�̃t���Đ�
extern int GetMovieFrame( int hPicture, int hMovie, int nFrameNum );
																// ���[�r�[�̓���t���[���擾
extern int GetMovieSize( int hMovie, int *nWidth, int *nHeight );	// ���[�r�[�T�C�Y�̎擾
extern int GetMovieCount( int hMovie );							// ���[�r�[�t���[���J�E���g���̎擾
extern int InitMovieMem( void );								// ���[�r�[�p�������̊J��



#endif
// ---------------------------------------------------------------------------
//
// �t�@�C����: AB_Time.h
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̎��ԋ@�\���u���b�W�B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AB_TIME_INCLUDED__
#define __AB_TIME_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif


// ---------------------------------------------------------------------------
// �O�����J�\���̐錾��

typedef struct tagAMDATETIME {									// �����\����
	DWORD						nPackedTime;					// �p�b�N�g����
	unsigned int				nYear;							// �X�V�N�i����j
	unsigned char				nMonth;							// �X�V��
	unsigned char				nDay;							// �X�V��
	unsigned char				nHour;							// �X�V��
	unsigned char				nMinute;						// �X�V��
	unsigned char				nSecond;						// �X�V�b
	unsigned char				dPadding;						// �p�f�B���O
	WORD						dPadding2;						// �p�f�B���O
} AMDATETIME;


//-----------------------------------------------------------------------------
// ���Ԋ֐���

extern int GetNowTime( int nDummy );							// �L�[���͑҂��E���̓L�[�擾�֐�
extern int WaitByTimer( int nWaitTime );						// �^�C�}�ő҂i�P�^�P�O�O�O�b�P�ʁj
extern int GetDateTime( AMDATETIME *pDateTime );				// �����̎擾
extern int UnpackDateTime( AMDATETIME *pDateTime );				// �����̃A���p�b�N


#endif


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AM_Time.h
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̎��ԋ@�\�������B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AM_TIME_INCLUDED__
#define __AM_TIME_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif

//#include <windows.h>
//#include <mmsystem.h>

#ifndef __AM_CONNECT_INCLUDED__

#include "AB_Time.h"											// ���ԋ@�\�@�u���b�W���C���[

#endif



//-----------------------------------------------------------------------------
// �L�[�{�[�h���͐���֐���

extern int AM_GetPresentTime( void );							// ���ݎ��Ԏ擾�֐�
extern int AM_GetDateTime( AMDATETIME *pDateTime );				// �����̎擾


#endif


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

//             �A�~���[�Y�����g���f�B�A�����w�@�@�Q�[�����C�u����

//                          �h�`�l�f�`�l�d�h

//                                Ver. 3.00

//         Programmed 1996-2004 by K.Katoh, Academy of Amusementmedia

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// �`�l�f�`�l�d�@�s�c�@�c�����������r�����������W���[��


#ifndef __AMDSOUND_INCLUDED__						// �ݸٰ�ޯ�ރ`�F�b�N
#define __AMDSOUND_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif

//#define DIRECTSOUND_VERSION			0x0500
#include <mmsystem.h>
#include <dsound.h>

// �����N���C�u����
#pragma comment( lib, "dsound"  )					// DirectSound���C�u����
#pragma comment( lib, "dxguid"  )					// DirectX GUID���C�u����
#pragma comment( lib, "winmm"   )					// �}���`���f�B�A���C�u����

// ---------------------------------------------------------------------------
// ���C�u�������ʕϐ�
extern BOOL		bMidiPlaying;


// ---------------------------------------------------------------------------
// �o�b�l���C�u�����֐�

extern int InitSound( int nSoundMode );				// �T�E���h�̏�����
extern int EndSound( void );						// �T�E���h�g�p�I��
extern int EndDSound( void );						// �����T�E���h�g�p�I��
extern int InitSoundMem( void );					// �T�E���h�������̏�����
extern int PlayWav( char *szFileName );				// Wav���ڍĐ�
extern int PlayWavBack( char *szFileName );			// Wav���ڃo�b�N�Đ�
extern int PlayWavStreamBack( char *szFileName );	// Wav�X�g���[���o�b�N�Đ�
extern int CheckPcm( void );						// �o�b�l���t���`�F�b�N
extern int StopPcm( void );							// �o�b�l���t��~
extern int MakeDSBuffer( LPWAVEFORMATEX lpWaveForm,		// DirectSound�o�b�t�@�쐬
						 char *pWavBuffer, DWORD nBufferSize,
						 LPDIRECTSOUNDBUFFER *lplpDSBDest );
extern int MakeDSStreamBuf( LPWAVEFORMATEX lpWaveForm,	// DirectSound�X�g���[�~���O�o�b�t�@�쐬
							int nBufferSize, LPDIRECTSOUNDBUFFER *lplpDSBDest );
extern int LoadWavMem( char *szFileName );			// �������ւ̂v�`�u���[�h
extern int PlayMem( int hSound );					// �������Đ�
extern int PlayMemBack( int hSound );				// �������o�b�N�Đ�
extern int PlayMemLoop( int hSound );				// ���������[�v�Đ�
extern int CheckPlayMem( int hSound );				// ���������t���`�F�b�N
extern int StopPlayMem( int hSound );				// ���������t�X�g�b�v
extern int SetPcmVolume( int nVolume );				// �o�b�l�{���E���̃Z�b�g
extern int SetMemVolume( int nVolume, int hSound );	// �������o�b�l�{���E���̃Z�b�g


// ---------------------------------------------------------------------------
// �~���[�W�b�N���C�u�����֐�

extern int PlayMidi( char *szFileName );			// MIDI�t�@�C���̉��t
extern int PlayMidiBack( char *szFileName );		// MIDI�t�@�C���̃o�b�N���t
extern int PlayMidiLoop( char *szFileName );		// MIDI�t�@�C���̃��[�v���t
extern int CheckMusic( void );						// �l�h�c�h���t���`�F�b�N
extern int StopMusic( void );						// �l�h�c�h���t�X�g�b�v
extern int SetMusicVolume( DWORD nVolume );			// �l�h�c�h���ʃZ�b�g
extern int MakeFullPath( char *szDest, char *szSrc );	// �t���p�X�̐���
extern void ProcessMciNortify( void );				// �l�b�h�ʒm����


// ---------------------------------------------------------------------------
// �O�����J�ϐ�

extern LPDIRECTSOUND			lpDS;				// DirectSound�I�u�W�F

#endif
// ---------------------------------------------------------------------------
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̃f�o�b�O�@�\���u���b�W�B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------


#ifndef __AB_DEBUG_INCLUDED__
#define __AB_DEBUG_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// ���`���̊֐��g�p
#endif

// ---------------------------------------------------------------------------
// �O�����J�萔��

// �f�o�b�O���t�@�C����
#define AB_DEBUG_FILE_NAME			"AM_DEBUG.txt"				// �f�o�b�O�t�@�C����
#define AB_DEBUG_FILE_NAME_L		L"AM_DEBUG.txt"				// �f�o�b�O�t�@�C����

// �f�o�b�O���[�h�Z�b�g�萔
#define AMDEBUG_ALL_OFF				0							// �S�Ẵf�o�b�O�@�\���I�t
#define AMDEBUG_ALL_ON				1							// �S�Ẵf�o�b�O�@�\���I��
#define AMDEBUG_REGISTER_OFF		2							// �f�o�b�O���o�^���I�t
#define AMDEBUG_REGISTER_ON			3							// �f�o�b�O���o�^���I��
#define AMDEBUG_DISPLAY_OFF			4							// �f�o�b�O���\�����I�t
#define AMDEBUG_DISPLAY_ON			5							// �f�o�b�O���\�����I��
#define AMDEBUG_KEY_OFF				6							// �f�o�b�O�L�[������I�t
#define AMDEBUG_KEY_ON				7							// �f�o�b�O�L�[������I��
#define AMDEBUG_FILE_OFF			8							// �f�o�b�O�L�[������I�t
#define AMDEBUG_FILE_ON				9							// �f�o�b�O�L�[������I��

// �f�o�b�O���[�h�萔�}�X�N
#define AMDEBUG_REGISTER			1							// �f�o�b�O���o�^
#define AMDEBUG_DISPLAY				2							// �f�o�b�O���\��
#define AMDEBUG_KEY					4							// �f�o�b�O�L�[����
#define AMDEBUG_FILE				8							// �f�o�b�O�t�@�C���o��

// �`��f�o�b�O�I�u�W�F�N�g���
#define AB_DEBUG_DRAWMEM			0							// DrawMem�n(DrawMem,DrawMemTh)����
#define AB_DEBUG_DRAWMEMLR			1							// DrawMemLR�n����
#define AB_DEBUG_DRAWMEMEXP			2							// DrawMemExp�n����
#define AB_DEBUG_DRAWMEMROT			3							// DrawMemRot�n����
#define AB_DEBUG_DRAWMEMFLEX		4							// DrawMemFlex�n����
#define AB_DEBUG_DRAWFBOX			5							// DrawFBox�n����
#define AB_DEBUG_OUT_STRING			0x1000						// ������o��

// ���[�h�f�o�b�O�I�u�W�F�N�g���
#define AB_DEBUG_LOADTEXTURE		0x100						// LoadTexture����
#define AB_DEBUG_LOADBLKTEXTURE		0x101						// LoadBlkTexture����
#define AB_DEBUG_LOADBLKTEXTURELR	0x102						// LoadBlkTextureLR����
#define AB_DEBUG_RELOADGRAPH		0x103						// ReloadGraph����

// ---------------------------------------------------------------------------
// �O�����J�\���̐錾��

// �`��f�o�b�O���\����
typedef struct tagAB_DRAWING_DEBUG_INFO {
	int							nObjectKind;					// �I�u�W�F�N�g���
	int							nUsedHandle;					// �g�p�n���h��
	AM_USER_PICTURE				*pUserPicture;					// �s�N�`�����
	int							x1, y1, x2, y2, x3, y3, x4, y4;	// �`��ʒu
	float						fAngle, fExRate;				// ��]�p�E�g�嗦
	int							nBlendKind;						// �u�����h���
	int							nColor;							// �`��F
	int							bError;							// �G���[�̗L��
} AB_DRAWING_DEBUG_INFO;

// ���[�h�f�o�b�O���\����
typedef struct tagAB_LOADING_DEBUG_INFO {
	int							nObjectKind;					// �I�u�W�F�N�g���
	char						*szFileName;					// �t�@�C����
	int							nMadeHandle;					// �쐬���ꂽ�n���h��
	int							nMadeHandleEnd;					// �쐬���ꂽ�Ō�̃n���h��
	int							bError;							// �G���[�̗L��
} AB_LOADING_DEBUG_INFO;


// ---------------------------------------------------------------------------
// �C���^�[�t�@�C���O���[�o���ϐ���

extern int						gg_bRegisterDebug;				// �f�o�b�O����o�^���邩
extern int						gg_bDisplayDebug;				// �f�o�b�O����\�����邩
extern int						gg_bDebugKey;					// �f�o�b�O�p�L�[��L���ɂ��邩
extern int						gg_bDebugFile;					// �f�o�b�O�t�@�C���o�͂�L���ɂ��邩


// ---------------------------------------------------------------------------
// �O�����J�֐��v���g�^�C�v�錾��

extern int SetAMDebugMode( int nDebugMode );					// �f�o�b�O���[�h�̃Z�b�g
extern int GetAMDebugMode( void );								// �f�o�b�O���[�h�̎擾
extern int RestoreAMDebugMode( int nDebugFlags );				// �f�o�b�O���[�h�̕��A
extern int OutputAMDebugString( char *szOutString );			// �f�o�b�O������̏o��

extern int AB_SetDrawingDebugInfo( AB_DRAWING_DEBUG_INFO *pDebugInfo );
																// �`��n�f�o�b�O���Z�b�g�֐�
extern int AB_DisplayDebugInfo( void );							// �f�o�b�O���\���֐�
extern int AB_ControlDebugPause( void );						// �f�o�b�O�@�\�E�|�[�Y�R���g���[���֐�

extern int AB_OutLoadingDebugInfo( AB_LOADING_DEBUG_INFO *pLoadDebugInfo );	// ���[�h�n�f�o�b�O���o�͊֐�



#endif
