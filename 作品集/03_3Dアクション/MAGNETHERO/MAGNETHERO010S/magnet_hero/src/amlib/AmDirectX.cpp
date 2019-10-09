// ---------------------------------------------------------------------------
//
// �t�@�C����: AM_DispEnv.cpp
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̉�ʊ��ݒ�������B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#pragma warning( disable : 4819 )						// �R�[�h�y�[�W�̌x���}�~
#define _CRT_SECURE_NO_WARNINGS							// �Z�L�����e�B�x���}�~(_vsnprintf�Asscanf�A_snprintf�Asprintf�g�p�̂���)

// ---------------------------------------------------------------------------
// �C���N���[�h��

#include <stdio.h>
#include <stdarg.h>

#include <d3d9.h>												// D3D9�I�u�W�F�N�g
#include <d3dx9.h>												// D3D9X�I�u�W�F�N�g

#include "amgame.h"												// AMGAME�w�b�_

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnv�w�b�_
#include "AM_Input.h"											// ���[�U���́@�@��ˑ����C���[
#include "AB_Input.h"											// ���[�U���́@�u���b�W
#include "AM_Sound.h"											// �T�E���h�@�@��ˑ����C���[
#include "AM_Movie.h"											// ���[�r�[�@�@��ˑ����C���[

#endif

// DirectX�p���֐��G�C���A�X��`
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;
int (WINAPIV * _sscanf)(const char*, const char*, ...) = sscanf;
int (WINAPIV * __snprintf)(char *, size_t, const char*, ...) = _snprintf;
int (WINAPIV * _sprintf)(char*, const char*, ...) = sprintf;

// ---------------------------------------------------------------------------
// �����g�p�萔��`��

#define WIN_CLASS_NAME			"AMWindow"						// �E�B���h�E�N���X��
#define WIN_TITLE				"AMGAME Library version 3.20f"	// �E�B���h�E�^�C�g��

//#define DISPENV_DEBUG_MODE										// �f�o�b�O���[�h��

#define AM_MIN_DISPLAY_WIDTH	320								// �ŏ��f�B�X�v���C��
#define AM_MIN_DISPLAY_HEIGHT	240								// �ŏ��f�B�X�v���C����
#define AM_MAX_DISPLAY_WIDTH	1600							// �ő�f�B�X�v���C��
#define AM_MAX_DISPLAY_HEIGHT	1200							// �ő�f�B�X�v���C����

#define AM_MAX_EVENTS			10								// �ő�ꊇ�����C�x���g��


// ---------------------------------------------------------------------------
// �C���^�[�t�@�C���O���[�o���ϐ���

// Window�֌W�I�u�W�F�N�g
HWND							gg_hWindow		= NULL;			// �E�B���h�E�n���h��
HINSTANCE						gg_hInstance	= NULL;			// �C���X�^���X�n���h��

// DirectX�֌W�I�u�W�F�N�g
AM_LPDIRECT3D					gg_pD3D			= NULL;			// D3D�I�u�W�F�N�g
AM_LPDIRECT3DDEVICE				gg_pD3DDevice	= NULL;			// D3DDevice�I�u�W�F�N�g
AM_LPDISPLAYBUFFER				gg_pBackBuffer = NULL;				// BackBuffer�I�u�W�F�N�g

// ��ʊ��ێ��ϐ�
SIZE							gg_siDispSize	= { 640, 480 };	// �N���C�A���g�̈�̑傫��
SIZE							gg_siWholeDispSize	= { 640, 480 };	// ��ʑS�̂̑傫��
int								gg_nBitDepth	= 32;			// �r�b�g�f�v�X
BOOL							gg_bFullMode	= true;			// �t���X�N���[�����[�h��
POINT							gg_piDispLocate = { 0, 0 };		// �N���C�A���g�̈�̈ʒu
D3DFORMAT						gg_dfDisplayFormat = D3DFMT_X8R8G8B8;	// �f�B�X�v���C�t�H�[�}�b�g
BOOL							gg_bDispInitialized = false;	// ��ʏ������ς݂�
BOOL							gg_bSceneActive = false;		// �V�[���̓A�N�e�B�u��
char							gg_szWinTitle[AM_MAX_WIN_TITLE] = WIN_TITLE;	// �E�B���h�E�^�C�g��

// �v���Z�X����ϐ�
BOOL							gg_bEndFlag = false;			// �I���t���O


// ---------------------------------------------------------------------------
// �t�@�C�����O���[�o���ϐ���

//AM_LPVERTEXBUFFER				gg_pVB			= NULL;			// Buffer to hold vertices
//AM_LP2DPICTUREBUFFER			fg_ppbPicture	= NULL;			// Buffer to hold 2D Picture

BOOL							fg_bWinClass_Reg	= false;	// �E�B���h�E�N���X���W�X�^�t���O
BOOL							fg_bCloseFlag	= false;		// �N���[�Y�t���O


// ---------------------------------------------------------------------------
// ���O�ϐ���

#ifdef DISPENV_DEBUG_MODE
FILE			*fpLog = NULL;
#endif


// A structure for our custom vertex type
struct CUSTOMVERTEX
{
    FLOAT x, y, z, rhw; // The transformed position for the vertex
    DWORD color;        // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE )


// ---------------------------------------------------------------------------
// �O�����J�֐��v���g�^�C�v�錾��
extern int AM_InitDisplay( void );								// �f�B�X�v���C������
extern int AM_EndDisplay( void );								// �f�B�X�v���C�g�p�I��


// ---------------------------------------------------------------------------
// �����֐��v���g�^�C�v�錾��

// �E�B���h�E����
static HWND AM_InitWindow( HINSTANCE hInstance );				// �ʏ�E�B���h�E������
static int AM_EndWindow( void );								// �E�B���h�E�g�p�I��

// Direct3D����
static int AM_InitD3D( HWND hWnd );								// Direct3D������
static int AM_DecideDisplayForm( void );						// ��ʃs�N�Z���t�H�[�}�b�g����
static VOID AM_EndD3D( void );									// Direct3D�g�p�I��

LRESULT WINAPI AM_WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );


//-----------------------------------------------------------------------------
// ������ʐ���֐���

// �f�B�X�v���C������
extern int AM_InitDisplay( void )
{
#ifdef DISPENV_DEBUG_MODE
	fpLog = fopen( "DispEnv_log.txt", "w" );
	fprintf( fpLog, "AMGAME Display environment settings\n" );
	fprintf( fpLog, "Display size = ( %d, %d )   Bit depth = %d\n", gg_siDispSize.cx, gg_siDispSize.cy, gg_nBitDepth );
	if ( gg_bFullMode ) fprintf( fpLog, "Full-screen mode\n" );
				   else fprintf( fpLog, "Windowed mode\n" );
#endif

	if ( !gg_hInstance ) return AMERR_DISPLAY;
	if ( !( gg_hWindow = AM_InitWindow( gg_hInstance ) ) ) {
		MessageBox( NULL, "Can't Initialize the Window object!", "Windows Initialize Error", MB_OK );
		return AMERR_DISPLAY;
	}

	if( !SUCCEEDED( AM_InitD3D( gg_hWindow ) ) ) {
		MessageBox( gg_hWindow, "Can't Initialize the Direct3D object!", "DirectX Initialize Error", MB_OK );
		return AMERR_DISPLAY;
	}

	gg_bDispInitialized = true;									// �������ς�

	return AM_OK;
}


// �f�B�X�v���C�g�p�I��
extern int AM_EndDisplay( void )
{
    AM_EndD3D();												// Direct3D�g�p�I��
	AM_EndWindow();												// Window�g�p�I��
#ifdef DISPENV_DEBUG_MODE
	if ( fpLog ) {
		fclose( fpLog );
		fpLog = NULL;
	}
#endif
	return AM_OK;
}


// �A�v���P�[�V�����C���X�^���X�̐ݒ�
// hInstance : �A�v���C���X�^���X
extern int AM_SetAppliInstance( HINSTANCE hInstance )
{
	gg_hInstance = hInstance;

	return AM_OK;
}


// �𑜓x�̐ݒ�
// nWidth : �f�B�X�v���C���@nHeight : �f�B�X�v���C�����@nBitDepth : �r�b�g�f�v�X
extern int AM_SetDisplayMode( int nWidth, int nHeight, int nBitDepth )
{
	int					nResultCode = AM_OK;

	if ( gg_pD3DDevice ) return AMERR_CANNOT;					// ����D3D�f�o�C�X������ΐݒ�s��

	// �𑜓x�ݒ�
	if ( ( nWidth  >= AM_MIN_DISPLAY_WIDTH  && nWidth  <= AM_MAX_DISPLAY_WIDTH  ) &&
		 ( nHeight >= AM_MIN_DISPLAY_HEIGHT && nHeight <= AM_MAX_DISPLAY_HEIGHT ) )
	{
		gg_siDispSize.cx = nWidth;  gg_siDispSize.cy = nHeight;
	}
	else {
		nResultCode = AMERR_CANNOT;
	}

	// �r�b�g�f�v�X�ݒ�
	if ( nBitDepth == 16 || nBitDepth == 32 ) {
		if ( gg_bFullMode ) gg_nBitDepth = nBitDepth;			// �t���X�N���[���Ȃ�r�b�g�f�v�X�Z�b�g
	}
	else {
		nResultCode = AMERR_CANNOT;
	}

	return nResultCode;
}


//-----------------------------------------------------------------------------
// �E�B���h�E����֐���

// �ʏ�E�B���h�E�\��
// Windows�̒ʏ�̃E�B���h�E��\������B
// hInstance : �A�v���P�[�V�����C���X�^���X
static HWND AM_InitWindow( HINSTANCE hInstance )
{
//	int					i;
    WNDCLASSEX			wc;
	HWND                hwnd;
	RECT				reClient;
	RECT				reWinLoc;						// �E�B���h�E�ʒu

	// �E�B���h�E�N���X�\���̂̐ݒ�
    wc.style = CS_HREDRAW | CS_VREDRAW;  
    wc.lpfnWndProc = AM_WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL; //LoadIcon( hInstance, "PIXY_ICON" );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	if ( gg_bFullMode )
		wc.hbrBackground = NULL; //(HBRUSH)( COLOR_ACTIVECAPTION + 1 );
	else
		wc.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
    wc.lpszMenuName = NULL; //NAME;
    wc.lpszClassName = WIN_CLASS_NAME;
    wc.cbSize = sizeof( WNDCLASSEX );
	wc.hIconSm = NULL; //LoadIcon( hInstance, "PIXY_S_ICON" );

    RegisterClassEx( &wc );                // �E�B���h�E�N���X�̓o�^
	fg_bWinClass_Reg = true;

    if ( gg_bFullMode ) 
		hwnd = CreateWindowEx(               // �E�B���h�E�̐���
			WS_EX_TOPMOST,
			WIN_CLASS_NAME, gg_szWinTitle,
			WS_POPUP, //WS_OVERLAPPEDWINDOW,
			0, 0,
			gg_siDispSize.cx, //GetSystemMetrics( SM_CXSCREEN ),
			gg_siDispSize.cy, //GetSystemMetrics( SM_CYSCREEN ),
			NULL, NULL,
			hInstance,
			NULL );
	else {
		reClient.left   = ( GetSystemMetrics( SM_CXSCREEN )
						   - gg_siDispSize.cx ) / 2;
		reClient.top    = ( GetSystemMetrics( SM_CYSCREEN )
						   - gg_siDispSize.cy ) / 2;
		gg_piDispLocate.x = reClient.left;
		gg_piDispLocate.y = reClient.top;
		reClient.right  = reClient.left + gg_siDispSize.cx;
		reClient.bottom = reClient.top  + gg_siDispSize.cy;
		AdjustWindowRectEx( &reClient, WS_OVERLAPPED | WS_SYSMENU, TRUE,
							WS_EX_OVERLAPPEDWINDOW );
		reWinLoc.left   = reClient.left   - 3;
		reWinLoc.top    = reClient.top    - 3;
		reWinLoc.right  = reClient.right  + 3;
		reWinLoc.bottom = reClient.bottom + 3;
		hwnd = CreateWindowEx(						// �E�B���h�E�̐���
			WS_EX_OVERLAPPEDWINDOW,
			WIN_CLASS_NAME, gg_szWinTitle,
			WS_OVERLAPPED | WS_SYSMENU,
			reWinLoc.left, reWinLoc.top,
			reWinLoc.right - reWinLoc.left, reWinLoc.bottom - reWinLoc.top,
			NULL, NULL,
			hInstance,
			NULL );
		RECT		rcMadeClient;
		int			nMadeWidth, nMadeHeight;
		GetClientRect( hwnd, &rcMadeClient );
		nMadeWidth = rcMadeClient.right - rcMadeClient.left;
		nMadeHeight = rcMadeClient.bottom - rcMadeClient.top;
		if ( ( nMadeWidth != gg_siDispSize.cx ) || ( nMadeHeight != gg_siDispSize.cy ) ) {
			reWinLoc.right += gg_siDispSize.cx - nMadeWidth;
			reWinLoc.bottom += gg_siDispSize.cy - nMadeHeight;
			MoveWindow( hwnd, reWinLoc.left, reWinLoc.top,
							  reWinLoc.right - reWinLoc.left, reWinLoc.bottom - reWinLoc.top, 
							  TRUE );
		}
//		GetClientRect( hwnd, &rcMadeClient );
	}
    if( !hwnd ) return( hwnd );
 
    ShowWindow( hwnd, SW_SHOW );
    UpdateWindow( hwnd );							// �E�B���h�E�̕\��
    SetFocus( hwnd );								// �t�H�[�J�X�̃Z�b�g
//	hMainWnd = hwnd;
//	hInst = hInstance;
/*	fg_bMoviePlaying = FALSE;							// �`�u�h������
	nMovieFirst = -1;  nMovieLast = -1;
	for ( i = 0; i < AM_MAXMOVIENUM; i++ )			// �`�u�h�o�b�t�@������
		ZeroMemory( &MovieBuf[i], sizeof( MovieBuf[i] ) );

	AmInitJoypad( JOYSTICKID1, &Pad1 );				// �p�b�h�P������
	AmInitJoypad( JOYSTICKID2, &Pad2 );				// �p�b�h�Q������*/

	return( hwnd );

}

// �E�B���h�E�g�p�I��
static int AM_EndWindow( void )
{
	if ( fg_bWinClass_Reg ) {
		UnregisterClass( WIN_CLASS_NAME, gg_hInstance );
		fg_bWinClass_Reg = false;
	}

	return AM_OK;
}


// Windows�C�x���g����
extern int AM_ProcessEvent( void )
{
	int					nEventCnt;
    MSG					msg;

//	if ( bLockFlag ) return( FALSE );
	if ( gg_bEndFlag ) return false;							// ���łɏI����

	nEventCnt = 0;
	while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) {
		if ( !GetMessage( &msg, NULL, 0, 0 ) ) {
			gg_bEndFlag = true;
//			StopPcm();											// ���y��~
			return false;
		}
		TranslateMessage( &msg );
        DispatchMessage( &msg );
		nEventCnt++;
		if ( nEventCnt >= AM_MAX_EVENTS ) break;
	}
	return true;
}


// Windows�E�B���h�E�v���V�[�W��
LRESULT WINAPI AM_WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
	// ��������
	case WM_ACTIVATEAPP:
		if ( !gg_bMouseCursor ) SetCursor( NULL );
		break;

	// �}�E�X�J�[�\���Z�b�g
    case WM_SETCURSOR:
		if ( gg_bFullMode && !gg_bMouseCursor ) {
			SetCursor( NULL );
			return TRUE;
		}
		break;

	// �}�E�X����
	case WM_LBUTTONDOWN:							// �}�E�X���{�^��
		if ( gg_bMouseActive ) {
			AM_SetKeyPushed( MOUSECODE_LEFT );
//			bPushFlag = TRUE;
//			wKeyCode = MOUSECODE_LEFT;
		}
		break;

	case WM_RBUTTONDOWN:							// �}�E�X�E�{�^��
		if ( gg_bMouseActive ) {
			AM_SetKeyPushed( MOUSECODE_RIGHT );
//			bPushFlag = TRUE;
//			wKeyCode = MOUSECODE_RIGHT;
		}
		break;

	case WM_MBUTTONDOWN:							// �}�E�X�����{�^��
		if ( gg_bMouseActive ) {
			AM_SetKeyPushed( MOUSECODE_MIDDLE );
//			bPushFlag = TRUE;
//			wKeyCode = MOUSECODE_MIDDLE;
		}
		break;

	// �L�[�{�[�h����
	case WM_CHAR:
		AM_SetKeyPushed( ( int )wParam );
		break;

/*	// �����I���`�F�b�N
    case WM_KEYDOWN:
        if( wParam == VK_F12 && !fg_bCloseFlag ) {
		    fg_bCloseFlag = TRUE;
            PostMessage( hWnd, WM_CLOSE, 0, 0 );
//			wKeyCode = 0;
	    }
		break;*/

	case MM_MCINOTIFY:
		if ( lParam == (long)gg_wAVIDevice ) {					// �`�u�h
//			AM_StopMovie();
		}
		else {													// �l�h�c�h
			ProcessMciNortify();
		}
		break;

    case WM_DESTROY:
        PostQuitMessage( 0 );
        return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}


//-----------------------------------------------------------------------------
// Direct3D����֐���

// DirectGraph������
static int AM_InitD3D( HWND hWnd )
{
	D3DPRESENT_PARAMETERS		d3dpp;

	// D3D�I�u�W�F�N�g�쐬
	if( !( gg_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return AMERR_DISPLAY;

	// �s�N�Z���t�H�[�}�b�g����
	AM_DecideDisplayForm();

    // D3DDevice����邽�߂̍\���̃Z�b�g
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.BackBufferWidth  = gg_siDispSize.cx;					// ��ʂ̕�
	d3dpp.BackBufferHeight = gg_siDispSize.cy;					// ��ʂ̍���
	d3dpp.BackBufferFormat = gg_dfDisplayFormat;				// �s�N�Z���t�H�[�}�b�g
	d3dpp.BackBufferCount  = 1;									// �o�b�N�o�b�t�@���i�t�����g�܂܂��j
	d3dpp.MultiSampleType  = D3DMULTISAMPLE_NONE;				// �}���`�T���v���i�A���`�G���A�V���O�H�j
	d3dpp.SwapEffect       = D3DSWAPEFFECT_DISCARD; //D3DSWAPEFFECT_FLIP;	// ��ʐ؂�ւ����[�h
	d3dpp.hDeviceWindow    = NULL;								// �^�[�Q�b�g�E�B���h�E
	d3dpp.Windowed         = gg_bFullMode ? FALSE : TRUE;		// �E�B���h�E���[�h��
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�t�H�[�}�b�g
	d3dpp.Flags            = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER; 	// �t���O
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
	d3dpp.PresentationInterval = gg_bFullMode ? D3DPRESENT_INTERVAL_DEFAULT
											  : D3DPRESENT_INTERVAL_ONE;	// �\������

    // D3DDevice�̍쐬
    if ( FAILED( gg_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&d3dpp, &gg_pD3DDevice ) ) )
    {
		return AMERR_DISPLAY;
    }

    // �y�o�b�t�@�̗L����
    gg_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

    // �J�����O��OFF
    gg_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

/*	// �X�v���C�g�̍쐬
	if ( FAILED( D3DXCreateSprite( g_pd3dDevice, &spSprite ) ) )
	{
		return LBERR_INITIALIZE;
	}*/

	// �o�b�N�o�b�t�@�擾
	gg_pD3DDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &gg_pBackBuffer );

	// �o�b�N�o�b�t�@�N���A
//    gg_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
	AM_ClearBackBuffer( 0, 0, 0 );

	// ��ʑS�̂̑傫���ݒ�
	if ( gg_bFullMode ) {
		gg_siWholeDispSize = gg_siDispSize;						// �t���X�N���[���Ȃ炻�̂܂�
	}
	else {
		gg_siWholeDispSize.cx = GetSystemMetrics( SM_CXSCREEN );
		gg_siWholeDispSize.cy = GetSystemMetrics( SM_CYSCREEN );
	}

    // Begin the scene
    gg_pD3DDevice->BeginScene();
	gg_bSceneActive = true;

	AM_Flip();
	AM_ClearBackBuffer( 0, 0, 0 );

	return AM_OK;
}


// ��ʃs�N�Z���t�H�[�}�b�g����
static int AM_DecideDisplayForm( void )
{
//	unsigned int		i;
	unsigned int		nDispModeNum;
	D3DDISPLAYMODE		D3DispMode;

	if( !gg_pD3D ) return AMERR_NOINITIALIZE;					// �������ς݂�

	// �s�N�Z���t�H�[�}�b�g����
	if ( gg_bFullMode ) {										// �t���X�N���[���̏ꍇ
		switch( gg_nBitDepth ) {
		case 32:												// �R�Q�r�b�g���[�h
			nDispModeNum = gg_pD3D->GetAdapterModeCount( D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8 );
			if ( nDispModeNum ) {								// X8R8G8B8����
				#ifdef DISPENV_DEBUG_MODE
					fprintf( fpLog, "Pixel format = D3DFMT_X8R8G8B8\n" );
				#endif
				gg_dfDisplayFormat = D3DFMT_X8R8G8B8;
			}
			else {												// X8R8G8B8�Ȃ�
				#ifdef DISPENV_DEBUG_MODE
					fprintf( fpLog, "32-bit pixel format decision failed!\n" );
				#endif
			}
			break;

		case 16:												// �P�U�r�b�g���[�h
			nDispModeNum = gg_pD3D->GetAdapterModeCount( D3DADAPTER_DEFAULT, D3DFMT_R5G6B5 );
			if ( nDispModeNum ) {								// R5G6B5����
				#ifdef DISPENV_DEBUG_MODE
					fprintf( fpLog, "Pixel format = D3DFMT_R5G6B5\n" );
				#endif
				gg_dfDisplayFormat = D3DFMT_R5G6B5;
			}
			else {												// R5G6B5�Ȃ�
				nDispModeNum = gg_pD3D->GetAdapterModeCount( D3DADAPTER_DEFAULT, D3DFMT_X1R5G5B5 );
				if ( nDispModeNum ) {							// X1R5G5B5����
				#ifdef DISPENV_DEBUG_MODE
					fprintf( fpLog, "Pixel format = D3DFMT_X1R5G5B5\n" );
				#endif
					gg_dfDisplayFormat = D3DFMT_X1R5G5B5;
				}
			}
			break;
		}
	}
	else {														// �E�B���h�E���[�h�̏ꍇ
	    if ( FAILED( gg_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &D3DispMode ) ) )
			return AMERR_CANNOT;
		gg_dfDisplayFormat = D3DispMode.Format;					// �t�H�[�}�b�g�i�[

		// �r�b�g�f�v�X�Z�b�g
		if ( gg_dfDisplayFormat == D3DFMT_R5G6B5 || gg_dfDisplayFormat == D3DFMT_X1R5G5B5 ){
            gg_nBitDepth = 16;
		}
		else {
            gg_nBitDepth = 32;
		}
	}

	#ifdef DISPENV_DEBUG_MODE
		fprintf( fpLog, "Display mode successfully decided.\n" );
	#endif
	return AM_OK;
}


// Direct3D�g�p�I��
VOID AM_EndD3D( void )
{
	// �V�[���I������
	if ( gg_bSceneActive ) {
		gg_pD3DDevice->EndScene();								// �V�[���I��
		gg_bSceneActive = false;
	}

	AM_SAFE_RELEASE( gg_pBackBuffer );							// �o�b�N�o�b�t�@
	AM_SAFE_RELEASE( gg_pD3DDevice );							// D3D�f�o�C�X

	AM_SAFE_RELEASE( gg_pD3D );									// D3D
}


// ��ʂ̍X�V
extern int AM_Flip( void )
{
	D3DRASTER_STATUS			rsRasterData;

	if ( gg_bEndFlag ) return AMERR_ENDED;						// �I����

    // End the scene
    gg_pD3DDevice->EndScene();
	gg_bSceneActive = false;

	// �E�B���h�E���[�h�Ȃ�VBlank�҂�
	if ( !gg_bFullMode ) {
		do {													// VBlank�ɂȂ�܂ő҂�
			if ( !AM_ProcessEvent() ) break;					// �Ƃ肠�����C�x���g����
			gg_pD3DDevice->GetRasterStatus( 0, &rsRasterData );
		} while ( !rsRasterData.InVBlank );
	}

	// �I����ĂȂ���Ε\��
	if ( !gg_bEndFlag ) {
		gg_pD3DDevice->Present( NULL, NULL, NULL, NULL );		// �o�b�N�o�b�t�@�̓��e��\��
		gg_pD3DDevice->BeginScene();							// �V�[���J�n
		gg_bSceneActive = true;
	}
	else return AMERR_ENDED;

	return AM_OK;
}


// VBlank�M���̃g���K��҂�
extern int AM_WaitVBlankTrigger( void )
{
	D3DRASTER_STATUS			rsRasterData;

	if ( gg_bEndFlag ) return AMERR_ENDED;						// �I����

	// VBlank�łȂ��Ȃ�܂ő҂�
	do {
		if ( !AM_ProcessEvent() ) break;						// �Ƃ肠�����C�x���g����
		gg_pD3DDevice->GetRasterStatus( 0, &rsRasterData );
	} while ( rsRasterData.InVBlank );

	// VBlank�ɂȂ�܂ő҂�
	do {
		if ( !AM_ProcessEvent() ) break;						// �Ƃ肠�����C�x���g����
		gg_pD3DDevice->GetRasterStatus( 0, &rsRasterData );
	} while ( !rsRasterData.InVBlank );

	return AM_OK;
}


// �o�b�N�o�b�t�@�̃N���A
extern int AM_ClearBackBuffer( int r, int g, int b )
{
	// ��ʁE�f�v�X�o�b�t�@�N���A
//    gg_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
	gg_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						  D3DCOLOR_XRGB( r, g, b ), 1.0f, 0 );

	return AM_OK;
}


// �r���[�|�[�g�̐ݒ�
extern int AM_SetViewport( int x1, int y1, int x2, int y2 )
{
	D3DVIEWPORT9				dvViewport;

	if( !gg_pD3DDevice ) return AMERR_NOINITIALIZE;				// �������ς݂�

	// �p�����[�^�ݒ�
	dvViewport.X = x1;  dvViewport.Y = y1;						// ����
	dvViewport.Width = x2 - x1;  dvViewport.Height = y2 - y1;	// ���E����
	dvViewport.MinZ = 0.0f;  dvViewport.MaxZ = 1.0f;			// Z�͈̔�

	// �r���[�|�[�g�ݒ�
	gg_pD3DDevice->SetViewport( &dvViewport );

	return AM_OK;
}


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AM_2DGraph.cpp
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̂Q�c��ʕ`��@�\�������B
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
#include <d3dx9.h>

#include "amgame.h"												// AMGAME�w�b�_

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnv�w�b�_
#include "AM_2DGraph.h"											// AM2DGraph�w�b�_
#include "AB_2DGraph.h"											// AB2DGraph�w�b�_

#endif


// ---------------------------------------------------------------------------
// �����g�p�萔��`��
//int								gg_nBlendMode = AMDRAW_NOBLEND;			// ���݂̃u�����h���[�h
//int								gg_nBlendFactor = AM_MAX_BLEND_FACTOR;	// ���݂̃u�����h�t�@�N�^


// ---------------------------------------------------------------------------
// �C���^�[�t�@�C���O���[�o���ϐ���

AM_BLEND_STATE					gg_bsBlendState;				// ���݂̃u�����h�X�e�[�g

// �`���`�p���_
extern AM_2DVERTEX				gg_v2DrawRect[4] = { {   0.0f,   0.0f, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f },
													 { 640.0f,   0.0f, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f },
													 {   0.0f, 480.0f, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f },
													 { 640.0f, 480.0f, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f } };


// ---------------------------------------------------------------------------
// �t�@�C�����O���[�o���ϐ���

//AM_LP2DPICTUREBUFFER			fg_ppbPicture	= NULL;			// Buffer to hold 2D Picture


// ---------------------------------------------------------------------------
// �����֐��v���g�^�C�v�錾��


static int AM_Get2ExpNumber( int nSource );						// ���̐��l�ȏ�̂Q�̂���̐��l�ɂ���
static int AM_Calc2DUVCoord( float *du1, float *dv1, float *du2, float *dv2,
							 int sx1, int sy1, int sx2, int sy2, 
							 AM_2DPIC_INFO *pPicInfo );
																// 2D�O���t�B�b�N�@�e�N�X�`�����W�v�Z
static int AM_SetBlendFactor( int nBlendFactor );				// �u�����h�t�@�N�^�Z�b�g


//-----------------------------------------------------------------------------
// 2D�摜���[�h�֐���

// 2D�O���t�B�b�N���[�h
extern AM_LP2DPICTUREBUFFER AM_Load2DGraph( char *szFileName, AM_2DPIC_INFO *pPicInfo )
{
	AM_LP2DPICTUREBUFFER		p2dGraph = NULL;				// ���[�h�e�N�X�`��
	HRESULT						hRes;
	D3DXIMAGE_INFO				iiImageInfo;
	LPDIRECT3DSURFACE9			lpTopSurf;
	D3DSURFACE_DESC				ddSurfInfo;
	D3DFORMAT					dfTexFormat;					// �e�N�X�`���t�H�[�}�b�g
	D3DCOLOR					nColorKey;						// �J���[�L�[
//	D3DSURFACE_DESC				ddSurfInfo;

	// �s�N�Z���t�H�[�}�b�g
	dfTexFormat = ( gg_nBitDepth == 32 ) ? D3DFMT_A8R8G8B8 : D3DFMT_A1R5G5B5;

	// �J���[�L�[����
	if ( strstr( szFileName, ".bmp" ) || strstr( szFileName, ".BMP" ) ) {	// �r�b�g�}�b�v�̏ꍇ
		nColorKey = 0xff000000;												// �r�b�g�}�b�v�Ȃ�J���[�L�[����
	}
	else {
		nColorKey = 0;														// �r�b�g�}�b�v�ȊO�Ȃ�J���[�L�[�Ȃ�
	}

	// �e�N�X�`�����[�h
	hRes = D3DXCreateTextureFromFileEx( gg_pD3DDevice, ( LPCSTR )szFileName, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0,
										dfTexFormat, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
										nColorKey, &iiImageInfo, NULL,
										&p2dGraph );
//    hRes = D3DXCreateTextureFromFile( gg_pD3DDevice, ( LPCSTR )szFileName, &p2dGraph );

	if ( FAILED( hRes ) ) return NULL;

	// �T�[�t�F�X���擾
	hRes = p2dGraph->GetSurfaceLevel( 0, &lpTopSurf );
	if ( FAILED( hRes ) ) return NULL;
	lpTopSurf->GetDesc( &ddSurfInfo );
	lpTopSurf->Release();

	pPicInfo->nWidth      = iiImageInfo.Width;
	pPicInfo->nHeight     = iiImageInfo.Height;
	pPicInfo->nRealWidth  = ddSurfInfo.Width;
	pPicInfo->nRealHeight = ddSurfInfo.Height;
//	pPicInfo->nBitDepth = 
/*	if ( FAILED( D3DXCreateTextureFromFileEx( 640, 480, gg_dfDisplayFormat, D3DPOOL_DEFAULT, &p2dGraph, NULL ) ) )
	{
		return NULL;
	}

	if ( FAILED( D3DXLoadSurfaceFromFile( p2dGraph, NULL, NULL, szFileName, NULL, D3DX_FILTER_POINT, 0, NULL ) ) )
	{
		return NULL;
	}*/

	return p2dGraph;
}


// 2D�O���t�B�b�N�o�b�t�@�쐬
extern AM_LP2DPICTUREBUFFER AM_Make2DGraphBuf( AM_2DPIC_INFO *pPicInfo )
{
	AM_LP2DPICTUREBUFFER		p2dGraph = NULL;				// ���[�h�e�N�X�`��
	HRESULT						hRes;
	D3DFORMAT					dfTexFormat;					// �e�N�X�`���t�H�[�}�b�g

	dfTexFormat = ( pPicInfo->nBitDepth == 32 ) ? D3DFMT_A8R8G8B8 : D3DFMT_A1R5G5B5;

	// �����l�Z�b�g
	pPicInfo->nRealWidth  = AM_Get2ExpNumber( pPicInfo->nWidth  );
	pPicInfo->nRealHeight = AM_Get2ExpNumber( pPicInfo->nHeight );
	pPicInfo->nRealDepth = pPicInfo->nBitDepth;

	// �e�N�X�`���쐬
	hRes = D3DXCreateTexture( gg_pD3DDevice, pPicInfo->nRealWidth, pPicInfo->nRealHeight,
							  1, 0, dfTexFormat, D3DPOOL_MANAGED, &p2dGraph );
	if ( FAILED( hRes ) ) return NULL;

	return p2dGraph;
}


// 2D�O���t�B�b�N�ă��[�h
extern int AM_Reload2DGraph( char *szFileName, AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo )
{
//	AM_LP2DPICTUREBUFFER		p2dGraph = NULL;				// ���[�h�e�N�X�`��
	HRESULT						hRes;
//	D3DXIMAGE_INFO				iiImageInfo;
	LPDIRECT3DSURFACE9			lpTopSurf;
//	D3DSURFACE_DESC				ddSurfInfo;
//	D3DFORMAT					dfTexFormat;					// �e�N�X�`���t�H�[�}�b�g
//	D3DSURFACE_DESC				ddSurfInfo;
	RECT						rcDest;							// �]�����`

	// �T�[�t�F�X�擾
	hRes = pPicture->GetSurfaceLevel( 0, &lpTopSurf );
	if ( FAILED( hRes ) ) return AMERR_DISPLAY;

	rcDest.left = 0;  rcDest.top = 0;  rcDest.right = pPicInfo->nWidth;  rcDest.bottom = pPicInfo->nHeight;

	if ( FAILED( D3DXLoadSurfaceFromFile( lpTopSurf, NULL, &rcDest, szFileName, NULL, D3DX_FILTER_LINEAR, 0xff000000, NULL ) ) )
	{
		return AMERR_NOFILE;
	}

	lpTopSurf->Release();

	return AM_OK;
}


// ���̐��l�ȏ�̂Q�̂���̐��l�ɂ���
static int AM_Get2ExpNumber( int nSource )
{
	int							i;
	unsigned int				nUnsignedSrc;					// �����Ȃ������l
//	unsigned int				nBitScale;						// �r�b�g�X�P�[��
	int							bBit1 = false;					// 1�r�b�g�����邩
	int							nResult;						// ����

	if ( nSource == 0 ) return 0;
	nUnsignedSrc = ( unsigned int )nSource;

	for ( i = 0; i < 32; i++ ) {
		if ( nUnsignedSrc != 1 ) {								// ���l���ŏI�i�P�j�ɂȂ��Ă��Ȃ�
			if ( nUnsignedSrc & 1 ) bBit1 = true;
		}
		else {													// ���l���P�ɂȂ��Ă���
			nResult = ( bBit1 ) ? ( 1 << ( i + 1 ) ) : nSource;
		}
		nUnsignedSrc = nUnsignedSrc >> 1;
	}

	return nResult;
}


//-----------------------------------------------------------------------------
// 2D�摜�`��֐���

// 2D�O���t�B�b�N�`��i�`���Q�_�A�`�挳�Q�_�w��j
extern int AM_Draw2DGraph( int dx1, int dy1, int dx2, int dy2,
						   int sx1, int sy1, int sx2, int sy2, 
						   AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo )
{
	float						tu1, tv1, tu2, tv2;

	AM_Calc2DUVCoord( &tu1, &tv1, &tu2, &tv2, sx1, sy1, sx2, sy2, pPicInfo );
//	tu1  = ( float )( sx1 + 0.5f ) / pPicInfo->nRealWidth;  tv1  = ( float )( sy1 + 0.5f ) / pPicInfo->nRealHeight;
//	tu2  = ( float )( sx2  /* - 0.5f */ ) / pPicInfo->nRealWidth;  tv2  = ( float )( sy2 /*- 0.5f*/ ) / pPicInfo->nRealHeight;
	gg_v2DrawRect[0].x = ( float )dx1;  gg_v2DrawRect[0].y = ( float )dy1;  gg_v2DrawRect[0].tu = tu1;  gg_v2DrawRect[0].tv = tv1;  
	gg_v2DrawRect[1].x = ( float )dx2;  gg_v2DrawRect[1].y = ( float )dy1;  gg_v2DrawRect[1].tu = tu2;  gg_v2DrawRect[1].tv = tv1;  
 	gg_v2DrawRect[2].x = ( float )dx1;  gg_v2DrawRect[2].y = ( float )dy2;  gg_v2DrawRect[2].tu = tu1;  gg_v2DrawRect[2].tv = tv2;
 	gg_v2DrawRect[3].x = ( float )dx2;  gg_v2DrawRect[3].y = ( float )dy2;  gg_v2DrawRect[3].tu = tu2;  gg_v2DrawRect[3].tv = tv2;
	gg_pD3DDevice->SetTexture( 0, pPicture );
    gg_pD3DDevice->SetFVF( D3DFVF_AM2DTEX );
	gg_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, gg_v2DrawRect, sizeof( AM_2DVERTEX ) );

	return AM_OK;
}


// 2D�O���t�B�b�N�`��i�`���S�_�A�`�挳�Q�_�w��j
extern int AM_Draw2DGraph4_2( AM_2DVECTOR *pDestVerts,
							  int sx1, int sy1, int sx2, int sy2, 
							  AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo )
{
	int							i;
	float						tu1, tv1, tu2, tv2;

	AM_Calc2DUVCoord( &tu1, &tv1, &tu2, &tv2, sx1, sy1, sx2, sy2, pPicInfo );
//	tu1  = ( float )( sx1 + 0.5f ) / pPicInfo->nRealWidth;  tv1  = ( float )( sy1 + 0.5f ) / pPicInfo->nRealHeight;
//	tu2  = ( float )( sx2  /* - 0.5f */ ) / pPicInfo->nRealWidth;  tv2  = ( float )( sy2 /*- 0.5f*/ ) / pPicInfo->nRealHeight;

	// ���_���W
	for ( i = 0; i < 4; i++ ) {
		gg_v2DrawRect[i].x = ( pDestVerts + i )->x;  gg_v2DrawRect[i].y = ( pDestVerts + i )->y;
	}
	// �e�N�X�`�����W
	gg_v2DrawRect[0].tu = tu1;  gg_v2DrawRect[0].tv = tv1;  
	gg_v2DrawRect[1].tu = tu2;  gg_v2DrawRect[1].tv = tv1;  
 	gg_v2DrawRect[2].tu = tu1;  gg_v2DrawRect[2].tv = tv2;
 	gg_v2DrawRect[3].tu = tu2;  gg_v2DrawRect[3].tv = tv2;

	gg_pD3DDevice->SetTexture( 0, pPicture );
    gg_pD3DDevice->SetFVF( D3DFVF_AM2DTEX );
	gg_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, gg_v2DrawRect, sizeof( AM_2DVERTEX ) );

	return AM_OK;
}


// 2D�O���t�B�b�N�@�e�N�X�`�����W�v�Z
static int AM_Calc2DUVCoord( float *du1, float *dv1, float *du2, float *dv2,
							 int sx1, int sy1, int sx2, int sy2, 
							 AM_2DPIC_INFO *pPicInfo )
{
	// u����
	if ( sx1 <= sx2 ) {											// ���E���]�Ȃ�
		*du1  = ( float )( sx1 + 0.5f ) / pPicInfo->nRealWidth;
		*du2  = ( float )( sx2 /* + 0.5f*/  ) / pPicInfo->nRealWidth;
	}
	else {														// ���E���]����
		*du1  = ( float )( sx1 - 0.5f ) / pPicInfo->nRealWidth;
		*du2  = ( float )( sx2 /*  - 0.5f*/ ) / pPicInfo->nRealWidth;
	}

	// v����
	if ( sy1 <= sy2 ) {											// ���E���]�Ȃ�
		*dv1  = ( float )( sy1 + 0.5f ) / pPicInfo->nRealHeight;
		*dv2  = ( float )( sy2 /*- 0.5f*/ ) / pPicInfo->nRealHeight;
	}
	else {
		*dv1  = ( float )( sy1 - 0.5f ) / pPicInfo->nRealHeight;
		*dv2  = ( float )( sy2 /*- 0.5f*/ ) / pPicInfo->nRealHeight;
	}

	return AM_OK;
}


// �}�X�N�`��G�~�����[�V����
int			nBitNumTable[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };	// �r�b�g���e�[�u��

extern int AM_Draw2DBlockMaskEmu( int x, int y,
								  int sx1, int sy1, int sx2, int sy2, 
								  int nMaskWidth, int nMaskHeight, unsigned char *pMask,
								  AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo )
{
	int					dx, dy;
	int					i;
	int					nLeft, nRight, nTop, nBottom;
	int					nLocLeft, nLocRight, nLocTop, nLocBottom;
	AM_2DVERTEX			*ptlvBuf, *ptlvPresent;
	int					nMask_x, nMask_y;
	unsigned char		cMaskValue;
	int					nAlphaValue, nAlphaValue2;
	int					nTLVBufSize;				// �o�b�t�@�T�C�Y
	AM_BLEND_STATE		bsBlendBuf;								// �u�����h�X�e�[�g�ޔ��ꏊ
	int					nDrawWidth, nDrawHeight;	// �`�敝�E����

	nTLVBufSize = ( ( gg_siDispSize.cx + 7 ) / 8 + 2 ) * 2;
	ptlvBuf = ( AM_2DVERTEX * )malloc( sizeof( AM_2DVERTEX ) * nTLVBufSize );
	if ( !ptlvBuf ) return AMERR_NOMEMORY;
	AM_GetBlendState( &bsBlendBuf );							// �u�����h�X�e�[�g�擾
	AM_SetBlendState( AMDRAW_ALPHABLEND, bsBlendBuf.nBlendFactor );		// �A���t�@�u�����h
	gg_pD3DDevice->SetTexture( 0, pPicture );
	gg_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );	// �\�ʕύX
	for ( i = 0; i < nTLVBufSize; i++ ) {			// ���_�o�b�t�@������
		*( ptlvBuf + i ) = gg_v2DrawRect[0];
	}
	nDrawWidth = sx2 - sx1;  nDrawHeight = sy2 - sy1;	// �傫���v�Z
	nLeft   = ( int )( x               + .0001 );
	nTop    = ( int )( y               + .0001 );
	nRight  = ( int )( x + nDrawWidth  + .0001 );
	nBottom = ( int )( y + nDrawHeight + .0001 );
	for ( dy = nTop; dy < nBottom + 8; dy += 8 ) {
		nLocTop = dy & 0xfff8;						// �㉺�u���b�N��
		if ( nLocTop < nTop ) nLocTop = nTop;
		nLocBottom = ( dy + 8 ) & 0xfff8;
		if ( nLocBottom > nBottom ) nLocBottom = nBottom;
		ptlvPresent = ptlvBuf;
		for ( dx = nLeft; dx < nRight + 8; dx += 8 ) {
			nLocLeft = dx & 0xfff8;					// ���E�u���b�N��
			if ( nLocLeft < nLeft ) nLocLeft = nLeft;
			nLocRight = ( dx + 8 ) & 0xfff8;
			if ( nLocRight > nRight ) nLocRight = nRight;
			nMask_x = ( dx >> 3 ) % nMaskWidth;
			nMask_y = dy % nMaskHeight;
			nAlphaValue = 0; nAlphaValue2 = 0;
			for ( i = 0; i < 16; i++ ) {			// �}�X�N�A���t�@�Z�o
				cMaskValue = *( pMask + nMask_y * nMaskWidth + nMask_x );
				if ( i < 8 ) 
					nAlphaValue += nBitNumTable[cMaskValue & 0x0f] +
								   nBitNumTable[cMaskValue >> 4];
				else
					nAlphaValue2 += nBitNumTable[cMaskValue & 0x0f] +
									nBitNumTable[cMaskValue >> 4];
				nMask_y++;
				if ( nMask_y >= nMaskHeight ) nMask_y = 0;
			}
			nAlphaValue  = nAlphaValue  << 2;
			if ( nAlphaValue  > 255 ) nAlphaValue  = 255;
			nAlphaValue2 = nAlphaValue2 << 2;
			if ( nAlphaValue2 > 255 ) nAlphaValue2 = 255;
			ptlvPresent->x = ( float )nLocLeft;		// ���W�Z�b�g
			ptlvPresent->y = ( float )nLocTop;
			ptlvPresent->tu = ( float )( sx1 + nLocLeft - nLeft ) / pPicInfo->nRealWidth;
			ptlvPresent->tv = ( float )( sy1 + nLocTop  - nTop  ) / pPicInfo->nRealHeight;
			ptlvPresent->color = ( nAlphaValue  << 24 ) + 0xffffff;
			( ptlvPresent + 1 )->x = ( float )nLocLeft;
			( ptlvPresent + 1 )->y = ( float )nLocBottom;
			( ptlvPresent + 1 )->tu = ( float )( sx1 + nLocLeft    - nLeft ) / pPicInfo->nRealWidth;
			( ptlvPresent + 1 )->tv = ( float )( sy1 + nLocBottom  - nTop  ) / pPicInfo->nRealHeight;
			ptlvPresent->color = ( nAlphaValue2 << 24 ) + 0xffffff;
			ptlvPresent += 2;
		}
		ptlvPresent->x = ( float )nLocRight;		// �Ō�̂Q���W�Z�b�g
		ptlvPresent->y = ( float )nLocTop;
		ptlvPresent->tu = ( float )( sx1 + nLocRight - nLeft ) / pPicInfo->nRealWidth;
		ptlvPresent->tv = ( float )( sy1 + nLocTop   - nTop  ) / pPicInfo->nRealHeight;
		ptlvPresent->color = ( nAlphaValue  << 24 ) + 0xffffff;
		( ptlvPresent + 1 )->x = ( float )nLocRight;
		( ptlvPresent + 1 )->y = ( float )nLocBottom;
		( ptlvPresent + 1 )->tu = ( float )( sx1 + nLocRight  - nLeft ) / pPicInfo->nRealWidth;
		( ptlvPresent + 1 )->tv = ( float )( sy1 + nLocBottom - nTop  ) / pPicInfo->nRealHeight;
		ptlvPresent->color = ( nAlphaValue2 << 24 ) + 0xffffff;
		ptlvPresent += 2;

		gg_pD3DDevice->SetFVF( D3DFVF_AM2DTEX );
		gg_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, ( DWORD )( ptlvPresent - ptlvBuf ),
										ptlvBuf, sizeof( AM_2DVERTEX ) );
	}

	gg_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );	// �\�ʕύX
	AM_RestoreBlendState( &bsBlendBuf );					// �u�����h�X�e�[�g���A
	free( ptlvBuf );

	return AM_OK;
}


//-----------------------------------------------------------------------------
// 2D�`����ݒ�֐���

// �u�����h�X�e�[�g������
extern int AM_InitBlendState( void )
{
	// �A���t�@�u�����h�Ȃ�
	gg_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	gg_bsBlendState.nBlendMode = AMDRAW_NOBLEND;
	AM_SetBlendFactor( AM_MAX_BLEND_FACTOR );					// �u�����h�t�@�N�^�ő�
	gg_bsBlendState.nBrRed = 255;  gg_bsBlendState.nBrGreen = 255;  gg_bsBlendState.nBrBlue = 255;	// �`��F�͔�

	// �F����
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	// �A���t�@����
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	// �e�N�Z���T���v���ݒ�
	gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );

	// �e�N�X�`���N�����v
	gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );


	return AM_OK;
}


// �u�����h�X�e�[�g�ݒ�
extern int AM_SetBlendState( int nBlendCode, int nBlendFactor )
{
	// �u�����h�Ȃ�
	if ( nBlendCode == AMDRAW_NOBLEND ) {
		if ( gg_bsBlendState.nBlendMode != AMDRAW_NOBLEND ) {
			gg_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
//			gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			gg_bsBlendState.nBlendMode = AMDRAW_NOBLEND;
		}
		AM_SetBlendFactor( 255 );								// �u�����h�t�@�N�^�ݒ�
	}

	// �A���t�@�u�����f�B���O
	if ( nBlendCode == AMDRAW_ALPHABLEND ) {
		if ( gg_bsBlendState.nBlendMode != AMDRAW_ALPHABLEND ) {
			if ( gg_bsBlendState.nBlendMode == AMDRAW_NOBLEND )
				gg_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );	// �u�����h�n�e�e�Ȃ�n�m��
//			gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
//			gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
			gg_pD3DDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			gg_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			gg_bsBlendState.nBlendMode = AMDRAW_ALPHABLEND;
		}
		AM_SetBlendFactor( nBlendFactor );						// �u�����h�t�@�N�^�ݒ�
//		gg_nBlendFactor = nParam;
	}

	// ���Z�u�����f�B���O
	if ( nBlendCode == AMDRAW_ADDBLEND ) {
		if ( gg_bsBlendState.nBlendMode != AMDRAW_ADDBLEND ) {
			if ( gg_bsBlendState.nBlendMode == AMDRAW_NOBLEND )
				gg_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );	// �u�����h�n�e�e�Ȃ�n�m��
//			gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
//			gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
			gg_pD3DDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			gg_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			gg_bsBlendState.nBlendMode = AMDRAW_ADDBLEND;
		}
		AM_SetBlendFactor( nBlendFactor );						// �u�����h�t�@�N�^�ݒ�
//		gg_nBlendFactor = nParam;
	}

	return AM_OK;
}


// �u�����h�t�@�N�^�Z�b�g
static int AM_SetBlendFactor( int nBlendFactor )
{
	int							i;

	// �T�`���[�V����
	if ( nBlendFactor < 0 ) nBlendFactor = 0;
	if ( nBlendFactor > AM_MAX_BLEND_FACTOR ) nBlendFactor = AM_MAX_BLEND_FACTOR;

	gg_bsBlendState.nBlendFactor = nBlendFactor;				// �u�����h�t�@�N�^�ۑ�

	// �`�撸�_�Z�b�g
	for ( i = 0; i < 4; i++ ) {
		gg_v2DrawRect[i].color = ( gg_v2DrawRect[0].color & 0x00ffffff ) | ( nBlendFactor << 24 );
	}

	return AM_OK;
}


// �u�����h�X�e�[�g���A
extern int AM_RestoreBlendState( AM_BLEND_STATE *pbsRestoreBlend )
{
	AM_SetBlendState( pbsRestoreBlend->nBlendMode, pbsRestoreBlend->nBlendFactor );	// �u�����h�t�@�N�^�ݒ�
	// �K�v�Ȃ�F�t�@�N�^�ݒ�
	if ( gg_bsBlendState.nBrRed   != pbsRestoreBlend->nBrRed   ||
		 gg_bsBlendState.nBrGreen != pbsRestoreBlend->nBrGreen ||
		 gg_bsBlendState.nBrBlue  != pbsRestoreBlend->nBrBlue )
	{
		AM_SetDrawColor( pbsRestoreBlend->nBrRed, pbsRestoreBlend->nBrGreen, pbsRestoreBlend->nBrBlue );
	}
	gg_bsBlendState = *pbsRestoreBlend;							// �u�����h�X�e�[�g�ۑ�

	return AM_OK;
}


// �u�����h�X�e�[�g�擾
extern int AM_GetBlendState( AM_BLEND_STATE *pbsBlendFactor )
{
	*pbsBlendFactor = gg_bsBlendState;

	return AM_OK;
}


// �e�N�X�`���T���v���X�e�[�g�ݒ�
extern int AM_SetTexSampleState( int nSampleCode )
{
	int							nResult = AM_OK;				// ���ʃR�[�h

	switch ( nSampleCode ) {

	// �j�A���X�g�l�C�o�[�@
	case AMDRAW_NEAREST:										
		gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		break;

	// �o�C���j�A�@
	case AMDRAW_BILINEAR:										
		gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		break;

	// �Y���R�[�h�Ȃ�
	default:
		nResult = AMERR_INVALIDMODE;
	}

	return nResult;
}


// �`��F�Z�b�g
extern int AM_SetDrawColor( int nRed, int nGreen, int nBlue )
{
	int							i;
	int							nCompositColor;					// �����ς݂̐F

	// �T�`���[�V����
	if ( nRed   < 0 ) nRed = 0;
	if ( nRed   > AM_MAX_COLOR_FACTOR ) nRed   = AM_MAX_COLOR_FACTOR;
	if ( nGreen < 0 ) nGreen = 0;
	if ( nGreen > AM_MAX_COLOR_FACTOR ) nGreen = AM_MAX_COLOR_FACTOR;
	if ( nBlue  < 0 ) nBlue = 0;
	if ( nBlue  > AM_MAX_COLOR_FACTOR ) nBlue  = AM_MAX_COLOR_FACTOR;

	// �F���ۑ�
	gg_bsBlendState.nBrRed   = nRed;
	gg_bsBlendState.nBrGreen = nGreen;
	gg_bsBlendState.nBrBlue  = nBlue;

	nCompositColor = ( nRed << 16 ) + ( nGreen << 8 ) + nBlue;

	// �`�撸�_�Z�b�g
	for ( i = 0; i < 4; i++ ) {
		gg_v2DrawRect[i].color = ( gg_v2DrawRect[0].color & 0xff000000 ) | nCompositColor;
	}

	return AM_OK;
}


// �e�N�X�`�����b�v�X�e�[�g�ݒ�
extern int AM_SetTexWrapState( int nWrapCode )
{
	int							nResult = AM_OK;				// ���ʃR�[�h

	switch ( nWrapCode ) {

	// �e�N�X�`���N�����v
	case AMDRAW_TEX_NOREPEAT:
		gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
		break;

	// �e�N�X�`�����s�[�g
	case AMDRAW_TEX_REPEAT:
		gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
		gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
		break;

	// �Y���R�[�h�Ȃ�
	default:
		nResult = AMERR_INVALIDMODE;
	}

	return nResult;
}


//-----------------------------------------------------------------------------
// 2D�摜���ӊ֐���

// �\���p�o�b�t�@�̓��e�擾
extern int AM_GetDisplayBuffer( AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo, int nGetMode )
{
	HRESULT						hRes;							// ����
	AM_LPDISPLAYBUFFER			pTempBuffer;					// �e���|�����o�b�t�@
	LPDIRECT3DSURFACE9			lpTopSurf;						// �e�N�X�`���g�b�v�T�[�t�F�X
	RECT						rcSrc, rcDest;					// �]�����E�]�����`

	switch ( nGetMode ) {
	case AM_GET_FRONT_BUFFER:									// �t�����g�o�b�t�@�擾
		gg_pD3DDevice->CreateOffscreenPlainSurface( gg_siWholeDispSize.cx, gg_siWholeDispSize.cy,
													D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pTempBuffer, NULL );
		gg_pD3DDevice->GetFrontBufferData( 0, pTempBuffer );

		hRes = pPicture->GetSurfaceLevel( 0, &lpTopSurf );		// �]����T�[�t�F�X�擾
		SetRect( &rcSrc, gg_piDispLocate.x, gg_piDispLocate.y,
						 gg_piDispLocate.x + gg_siDispSize.cx, gg_piDispLocate.y + gg_siDispSize.cy );	// �]������`
		SetRect( &rcDest, 0, 0, pPicInfo->nWidth, pPicInfo->nHeight );	// �]�����`
		// �]��
		D3DXLoadSurfaceFromSurface( lpTopSurf, NULL, &rcDest, pTempBuffer, NULL, &rcSrc, D3DX_FILTER_LINEAR, 0 );
//		gg_pD3DDevice->StretchRect( pTempBuffer, NULL, lpTopSurf, NULL, D3DTEXF_LINEAR );
		pTempBuffer->Release();									// �]�����T�[�t�F�X�J��
		lpTopSurf->Release();									// �]����T�[�t�F�X�J��
	}

    return AM_OK;
}


// �\���p�o�b�t�@�̃t�@�C���ւ̃Z�[�u
extern int AM_SaveDisplayBuffer( AM_RECT *pSaveRect, char *szFileName, int nGetMode )
{
	int							i, j;
	HRESULT						hRes;							// ����
	AM_LPDISPLAYBUFFER			pTempBuffer;					// �e���|�����o�b�t�@
	D3DLOCKED_RECT				lrLockedRect;					// ���b�N���ꂽ�̈���
	BITMAPFILEHEADER			bfFileHeader;					// BMP�t�@�C���w�b�_
	BITMAPINFOHEADER			biInfoHeader;					// BMP�C���t�H�w�b�_
	HANDLE						hWriteFile;						// �������݃t�@�C���n���h��
	DWORD						nWritten;						// �������ݍς݃o�C�g��
	int							nHeaderSize;					// �w�b�_�T�C�Y
	int							nFileSize;						// �t�@�C���T�C�Y
	int							nSaveWidth, nSaveHeight;		// �Z�[�u���E����
	DWORD						*pStartAddr;					// ����X�^�[�g�A�h���X
	DWORD						*pSwap1, *pSwap2;				// �X���b�v�|�C���g1,2
	DWORD						nTempBuf;						// �ꎞ�o�b�t�@
	DWORD						*pSurfRead;						// �T�[�t�F�X�ǂݏo��
	char						*pSurfWrite;					// �T�[�t�F�X��������
	int							nDWORDPitch;					// DWORD�P�ʃs�b�`

	// �t�@�C���I�[�v��
	hWriteFile = CreateFile( szFileName, GENERIC_WRITE,
							 0, NULL, CREATE_ALWAYS,
							 FILE_ATTRIBUTE_NORMAL, NULL );
	if ( hWriteFile == INVALID_HANDLE_VALUE )		// �����`�F�b�N
		return AMERR_CANNOTCREATE;

	// �ʒu����
	if ( !gg_bFullMode ) {
		pSaveRect->left   += gg_piDispLocate.x;
		pSaveRect->top    += gg_piDispLocate.y;
		pSaveRect->right  += gg_piDispLocate.x;
		pSaveRect->bottom += gg_piDispLocate.y;
	}

	// �傫���Z�b�g
	nSaveWidth  = pSaveRect->right  - pSaveRect->left;
	nSaveHeight = pSaveRect->bottom - pSaveRect->top;

	// �w�b�_�E�t�@�C���T�C�Y
	nHeaderSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER );
	nFileSize = nHeaderSize + nSaveWidth * nSaveHeight * 3;

	// �t�@�C���w�b�_
	bfFileHeader.bfType = 'MB';
	bfFileHeader.bfSize = nFileSize;
	bfFileHeader.bfOffBits = nHeaderSize;
	WriteFile( hWriteFile, &bfFileHeader, sizeof( BITMAPFILEHEADER ), &nWritten, NULL );

	// �C���t�H�w�b�_
	biInfoHeader.biSize          = sizeof( BITMAPINFOHEADER );
	biInfoHeader.biWidth         = nSaveWidth;
	biInfoHeader.biHeight        = nSaveHeight;
	biInfoHeader.biPlanes        = 1;
	biInfoHeader.biBitCount      = 24;
	biInfoHeader.biCompression   = BI_RGB;
	biInfoHeader.biSizeImage     = 0;
	biInfoHeader.biXPelsPerMeter = 0;
	biInfoHeader.biYPelsPerMeter = 0;
	biInfoHeader.biClrUsed       = 0;
	biInfoHeader.biClrImportant  = 0;
	WriteFile( hWriteFile, &biInfoHeader, sizeof( BITMAPINFOHEADER ), &nWritten, NULL );

	switch ( nGetMode ) {
	case AM_GET_FRONT_BUFFER:									// �t�����g�o�b�t�@�擾
		gg_pD3DDevice->CreateOffscreenPlainSurface( gg_siWholeDispSize.cx, gg_siWholeDispSize.cy,
													D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pTempBuffer, NULL );
		gg_pD3DDevice->GetFrontBufferData( 0, pTempBuffer );

		// �t�@�C���փZ�[�u
		hRes = pTempBuffer->LockRect( &lrLockedRect, NULL, 0 );
		if ( SUCCEEDED( hRes ) ) {
			// �㉺���]
			nDWORDPitch = lrLockedRect.Pitch >> 2;				// DWORD�ł̃s�b�`
			pStartAddr = ( DWORD * )lrLockedRect.pBits + pSaveRect->top * nDWORDPitch + pSaveRect->left;
			pSwap1 = pStartAddr;
			pSwap2 = pSwap1 + nDWORDPitch * ( nSaveHeight - 1 );
			for ( i = 0; i < nSaveHeight / 2; i++ ) {
				for ( j = 0; j < nSaveWidth; j++ ) {
					nTempBuf = *( pSwap1 + j );  *( pSwap1 + j ) = *( pSwap2 + j );  *( pSwap2 + j ) = nTempBuf;
				}
				pSwap1 += nDWORDPitch;
				pSwap2 -= nDWORDPitch;
			}
			// �t�H�[�}�b�g�ϊ�
			pSurfRead  = pStartAddr;
			pSurfWrite = ( char  * )lrLockedRect.pBits;
			for ( i = 0; i < nSaveHeight; i++ ) {
				for ( j = 0; j < nSaveWidth; j++ ) {
					*( ( DWORD * )pSurfWrite ) = *( pSurfRead + j );
					pSurfWrite += 3;
				}
				pSurfRead += nDWORDPitch;
			}
			// ��������
			WriteFile( hWriteFile, lrLockedRect.pBits, nSaveWidth * nSaveHeight * 3, &nWritten, NULL );
			pTempBuffer->UnlockRect();
		}
		pTempBuffer->Release();
	}

	CloseHandle( hWriteFile );									// �t�@�C���N���[�Y

    return AM_OK;
}


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AM_3DGraph.cpp
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̂R�c��ʕ`��@�\�������B
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
#include <d3dx9.h>

#include "amgame.h"												// AMGAME�w�b�_

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnv�w�b�_
#include "AM_3DGraph.h"											// AMDispEnv�w�b�_
#include "AM_Input.h"											// AMInput�w�b�_

#endif


// ---------------------------------------------------------------------------
// �t�@�C�����O���[�o���ϐ���

int								fg_bLighting = false;			// �����������肩


// ---------------------------------------------------------------------------
// �����֐��v���g�^�C�v�錾��

static int AM_InitMatrices( void );								// �s�񏉊���
static int AM_InitLights( void );								// ����������


//-----------------------------------------------------------------------------
// 3D���ݒ�֐���

// 3D�O���t�B�b�N�X������
extern int AM_Init3DGraph( void )
{
	AM_InitLights();											// ����
	AM_InitMatrices();											// �s��

	return AM_OK;
}


// �s�񏉊���
static int AM_InitMatrices( void )
{
	D3DXVECTOR3		v3CamLocation, v3CamLookAt, v3CamUp;		// �J�����ʒu�A�����_�A�����

	// ���[���h�s��
    D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );
	gg_pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );

    // �r���[�s��
    D3DXMATRIX matView;
	v3CamLocation = D3DXVECTOR3( gg_siDispSize.cx / 2.0f, gg_siDispSize.cy / 2.0f,
								 ( float )( 2.0f * 290.0f * ( gg_siDispSize.cy / 480.0f ) ) );	// �J�����ʒu
	v3CamLookAt = D3DXVECTOR3( gg_siDispSize.cx / 2.0f, gg_siDispSize.cy / 2.0f, 0.0f );	// �����_
	v3CamUp = D3DXVECTOR3( 0.0f, -1.0f, 0.0f );					// ������x�N�g��
    D3DXMatrixLookAtLH( &matView, &v3CamLocation, 
                                  &v3CamLookAt,
                                  &v3CamUp );
    gg_pD3DDevice->SetTransform( D3DTS_VIEW, &matView );

	// �v���W�F�N�V�����s��
    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, ( float )gg_siDispSize.cx / gg_siDispSize.cy, 1.0f, 2000.0f );
    gg_pD3DDevice->SetTransform( D3DTS_PROJECTION, &matProj );

/*	// �`�F�b�N
	AM_3DVERTEX cvRectVerts[4];
	cvRectVerts[0].vPos.x = 0.0f;  cvRectVerts[0].vPos.y =  0.0f;  cvRectVerts[0].vPos.z = 0.0f;
	cvRectVerts[0].color = 0xff0000ff;
	cvRectVerts[0].tu = 0.0f;  cvRectVerts[0].tv = 0.0f;
	cvRectVerts[1].vPos.x =  480.0f;  cvRectVerts[1].vPos.y =  0.0f;  cvRectVerts[1].vPos.z = 0.0f;
	cvRectVerts[1].color = 0xff00ff00;
	cvRectVerts[1].tu = 1.0f;  cvRectVerts[1].tv = 0.0f;
	cvRectVerts[2].vPos.x = 0.0f;  cvRectVerts[2].vPos.y = 480.0f;  cvRectVerts[2].vPos.z = 0.0f;
	cvRectVerts[2].color = 0xffff0000;
	cvRectVerts[2].tu = 0.0f;  cvRectVerts[2].tv = 1.0f;
	cvRectVerts[3].vPos.x =  480.0f;  cvRectVerts[3].vPos.y = 480.0f;  cvRectVerts[3].vPos.z = 0.0f;
	cvRectVerts[3].color = 0xff00ffff;
	cvRectVerts[3].tu = 1.0f;  cvRectVerts[3].tv = 1.0f;

	AM_ClearBackBuffer();

	// �F����
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2 );
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	// �A���t�@����
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

	gg_pD3DDevice->SetFVF( D3DFVF_AM3DVERTEX );
	gg_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, cvRectVerts, sizeof( AM_3DVERTEX ) );

	AM_Flip();
	AM_GetSingleChar();*/

	return AM_OK;
}


// �s�񏉊���
extern int AM_SetTransformMatrix( int nKind, AmMatrix *pmaMatrix )
{
//	D3DXMATRIX					mat;
	int							nResult = AM_OK;

	switch ( nKind ) {
	case AM_MATRIX_LW:											// ���[�J�������[���h
//		mat = *( ( D3DXMATRIX * )pmaMatrix );
		gg_pD3DDevice->SetTransform( D3DTS_WORLD, ( D3DMATRIX * )pmaMatrix );
		break;

	case AM_MATRIX_WV:											// ���[���h���r���[
//		mat = *( ( D3DXMATRIX * )pmaMatrix );
		gg_pD3DDevice->SetTransform( D3DTS_VIEW, ( D3DMATRIX * )pmaMatrix );
		break;

	default:													// �l�͕s�K��
		nResult = AMERR_INVALIDVALUE;
		break;
	}

	return nResult;
}


// �s��擾
extern int AM_GetTransformMatrix( int nKind, AmMatrix *pmaMatrix )
{
	int							nResult = AM_OK;

	switch ( nKind ) {
	case AM_MATRIX_LW:											// ���[�J�������[���h
		gg_pD3DDevice->GetTransform( D3DTS_WORLD, ( D3DMATRIX * )pmaMatrix );
		break;

	case AM_MATRIX_WV:											// ���[���h���r���[
		gg_pD3DDevice->GetTransform( D3DTS_VIEW, ( D3DMATRIX * )pmaMatrix );
		break;

	default:													// �l�͕s�K��
		nResult = AMERR_INVALIDVALUE;
		break;
	}

	return nResult;
}


// ����������
static int AM_InitLights( void )
{
    gg_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

    // Set up a material. The material here just has the diffuse and ambient
    // colors set to yellow. Note that only one material can be used at a time.
    D3DMATERIAL9 mtrl;
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
    mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
    mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
    mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
    gg_pD3DDevice->SetMaterial( &mtrl );

    // Set up a white, directional light, with an oscillating direction.
    // Note that many lights may be active at a time (but each one slows down
    // the rendering of our scene). However, here we are just using one. Also,
    // we need to set the D3DRS_LIGHTING renderstate to enable lighting
    D3DXVECTOR3 vecDir;
    D3DLIGHT9 light;
    ZeroMemory( &light, sizeof(D3DLIGHT9) );
    light.Type       = D3DLIGHT_DIRECTIONAL;
    light.Diffuse.r  = 1.0f;
    light.Diffuse.g  = 1.0f;
    light.Diffuse.b  = 1.0f;
    vecDir = D3DXVECTOR3( 1.0f, -1.0f, 1.0f );
    D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );
    light.Range       = 1000.0f;
    gg_pD3DDevice->SetLight( 0, &light );
    gg_pD3DDevice->LightEnable( 0, TRUE );
//    gg_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

    // Finally, turn on some ambient light.
    gg_pD3DDevice->SetRenderState( D3DRS_AMBIENT, 0x00606060 );

	return AM_OK;
}


// 3D�O���t�B�b�N�X�g�p�I��
extern int AM_End3DGraph( void )
{
	return AM_OK;
}


//-----------------------------------------------------------------------------
// 3D�`��֐���

// 3D�|���S���`��
extern int AM_Draw3DPolygons( AmVertex *pVertices, int nVertexNum,
							  int sx, int sy,
							  AM_LP3DTEXTUREBUFFER pTexture, AM_2DPIC_INFO *pPicInfo, int nFlags )	
{
	int							i;
//	HRESULT						nTexRes;
	AmVertex					*pSrcVert;

//	if ( !lpD3DDevice ) return AMERR_NOINITIALIZE;
	gg_pD3DDevice->SetTexture( 0, pTexture );
    gg_pD3DDevice->SetFVF( D3DFVF_AM3DVERTEX );
	if ( nVertexNum < 0 ) return AMERR_INVALIDVALUE;

	if ( nFlags & AMPOLY_NOLIGHT ) {				// ���C�e�B���O�����̏ꍇ
		AM_3DVERTEX					*pDestVert;
		AM_3DVERTEX					*pVertBuf = new AM_3DVERTEX[nVertexNum];

		if ( fg_bLighting ) {
			gg_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
//			lpD3DDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
			fg_bLighting = false;
		}
		pSrcVert  = pVertices;
		pDestVert = pVertBuf;
		for ( i = 0; i < nVertexNum; i++ ) {
			ZeroMemory( pDestVert, sizeof( AM_3DVERTEX ) );
			pDestVert->vPos.x  = pSrcVert->x;
			pDestVert->vPos.y  = pSrcVert->y;
			pDestVert->vPos.z  = pSrcVert->z;
			pDestVert->color = ( pSrcVert->a << 24 ) + ( pSrcVert->r << 16 ) + ( pSrcVert->g << 8 ) + pSrcVert->b;
//			pDestVert->color = RGBA_MAKE( pSrcVert->r, pSrcVert->g, pSrcVert->b, pSrcVert->a );
			if ( pTexture && pPicInfo ) {
				pDestVert->tu  = ( float )( sx + pSrcVert->u ) / pPicInfo->nRealWidth; 
				pDestVert->tv  = ( float )( sy + pSrcVert->v ) / pPicInfo->nRealHeight;
			}
			pSrcVert++;
			pDestVert++;
		}
		if ( nFlags & AMPOLY_QUADRANGLE ) {			// �l�p�|���S���̏ꍇ
			for ( i = 0; i < nVertexNum; i += 4 ) {
				gg_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &pVertBuf[i], sizeof( AM_3DVERTEX ) );
			}
		}
		else {										// �O�p�|���S���̏ꍇ
			gg_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, nVertexNum / 3, pVertBuf, sizeof( AM_3DVERTEX ) );
		}
		delete []pVertBuf;
	}
	else {											// ���C�e�B���O����̏ꍇ
		AM_3DVERTEX		*pDestVert;
		AM_3DVERTEX *pVertBuf = new AM_3DVERTEX[nVertexNum];

		if ( !fg_bLighting ) {
			gg_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
//			lpD3DDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
			fg_bLighting = true;
		}
		pSrcVert  = pVertices;
		pDestVert = pVertBuf;
		for ( i = 0; i < nVertexNum; i++ ) {
			ZeroMemory( pDestVert, sizeof( AM_3DVERTEX ) );
			pDestVert->vPos.x  = pSrcVert->x;
			pDestVert->vPos.y  = pSrcVert->y;
			pDestVert->vPos.z  = pSrcVert->z;
			pDestVert->vNorm.x = pSrcVert->nx;
			pDestVert->vNorm.y = pSrcVert->ny;
			pDestVert->vNorm.z = pSrcVert->nz;
			pDestVert->color = 0xffffffff;
			if ( pTexture && pPicInfo ) {
				pDestVert->tu  = ( float )( sx + pSrcVert->u ) / pPicInfo->nRealWidth; 
				pDestVert->tv  = ( float )( sy + pSrcVert->v ) / pPicInfo->nRealHeight;
			}
			pSrcVert++;
			pDestVert++;
		}
		if ( nFlags & AMPOLY_QUADRANGLE ) {			// �l�p�|���S���̏ꍇ
			for ( i = 0; i < nVertexNum; i += 4 ) {
				gg_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &pVertBuf[i], sizeof( AM_3DVERTEX ) );
			}
		}
		else {										// �O�p�|���S���̏ꍇ
			gg_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, nVertexNum / 3, pVertBuf, sizeof( AM_3DVERTEX ) );
		}
		delete []pVertBuf;
	}

	// ���C�e�B���OOFF
	if ( fg_bLighting ) {
		gg_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		fg_bLighting = false;
	}

//	delete []pVertBuf;

	return AM_OK;
}


// 3D����`��
extern int AM_Draw3DLines( AmVertex *pVertexes, int nVertexNum,
						   int sx, int sy,
						   AM_LP3DTEXTUREBUFFER pTexture, AM_2DPIC_INFO *pPicInfo )	
{
	int				i;
//	int				nTexRes;
//	D3DVECTOR		dvVector;
//	D3DVERTEX		*pDestVert;
	AmVertex		*pSrcVert;

	gg_pD3DDevice->SetTexture( 0, pTexture );
    gg_pD3DDevice->SetFVF( D3DFVF_AM3DVERTEX );
/*	if ( !lpD3DDevice ) return AMERR_NOINITIALIZE;
	if ( ( nTexRes = Prepare2DTex( &hTexture ) ) == AM_OK ) {
		MySetTexture( TexBuffer[hTexture].pTexture );
	}
	else {
		MySetTexture( NULL );
	}*/
	if ( nVertexNum < 0 ) return AMERR_INVALIDVALUE;

	AM_3DVERTEX		*pDestVert;
	AM_3DVERTEX		*pVertBuf = new AM_3DVERTEX[nVertexNum];

	if ( fg_bLighting ) {
		gg_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		fg_bLighting = false;
	}
	pSrcVert  = pVertexes;
	pDestVert = pVertBuf;
	for ( i = 0; i < nVertexNum; i++ ) {
		ZeroMemory( pDestVert, sizeof( AM_3DVERTEX ) );
		pDestVert->vPos.x  = pSrcVert->x;
		pDestVert->vPos.y  = pSrcVert->y;
		pDestVert->vPos.z  = pSrcVert->z;
		pDestVert->color = ( pSrcVert->a << 24 ) + ( pSrcVert->r << 16 ) + ( pSrcVert->g << 8 ) + pSrcVert->b;
		if ( pTexture && pPicInfo ) {
			pDestVert->tu  = ( float )( sx + pSrcVert->u ) / pPicInfo->nRealWidth; 
			pDestVert->tv  = ( float )( sy + pSrcVert->v ) / pPicInfo->nRealHeight;
		}
/*		pDestVert->x  = pSrcVert->x;// - 0.5f;
		pDestVert->y  = pSrcVert->y;// - 0.5f;
		pDestVert->z  = pSrcVert->z;
		pDestVert->color = RGBA_MAKE( pSrcVert->r, pSrcVert->g, pSrcVert->b, pSrcVert->a );
		if ( nTexRes == AM_OK ) {
			pDestVert->tu = ( pSrcVert->u ) / TexBuffer[hTexture].nTexWidth;
			pDestVert->tv = ( pSrcVert->v ) / TexBuffer[hTexture].nTexHeight;
		}*/
//		pDestVert->tu = ( pSrcVert->u /*+ 0.5f*/ ) / TexBuffer[hTexture].nTexWidth;
//		pDestVert->tv = ( pSrcVert->v /*+ 0.5f*/ ) / TexBuffer[hTexture].nTexHeight;
		pSrcVert++;
		pDestVert++;
	}
	gg_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, nVertexNum / 2, pVertBuf, sizeof( AM_3DVERTEX ) );
//	lpD3DDevice->DrawPrimitive( D3DPT_LINELIST, D3DFVF_LVERTEX,
//								pVertBuf, nVertexNum, NULL );
	delete []pVertBuf;

	return AM_OK;
}


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AM_FigureDraw.cpp
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̂Q�c��ʕ`��@�\�������B
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
#include <d3dx9.h>
#include <string.h>

#include "amgame.h"												// AMGAME�w�b�_

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnv�w�b�_
#include "AM_2DGraph.h"											// AM2DGraph�w�b�_
#include "AM_FigureDraw.h"										// AMFigureDraw�w�b�_

#endif


// ---------------------------------------------------------------------------
// �t�@�C�����\���̐錾��


// 2D���_�\����
typedef struct tagAM_POINTVERTEX {
	float				x, y, z, rhw;							// ���_���W
	int					color;									// ���_�F
} AM_POINTVERTEX;

#define D3DFVF_AMPOINT			( D3DFVF_XYZRHW | D3DFVF_DIFFUSE )	// 2D�e�N�X�`�����_�t�H�[�}�b�g


// ---------------------------------------------------------------------------
// �t�@�C�����O���[�o���ϐ���

// �o�b�t�@�֌W
AM_POINTVERTEX					fg_apPointBuf[AM_MAX_POINT_NUM];	// �|�C���g�o�b�t�@

// �t�H���g�֌W
HFONT							fg_hStringFont = NULL;				// ������̃t�H���g
int								fg_nStringFontSize = -1;			// ������t�H���g�T�C�Y
LPD3DXFONT						fg_pD3DXFont = NULL;				// D3DXFont�I�u�W�F�N�g


// ---------------------------------------------------------------------------
// �����֐��v���g�^�C�v�錾��

static int AM_GetModulateColor( int nColor1, int nColor2 );			// �F�̊|���Z�v�Z


//-----------------------------------------------------------------------------
// �}�`�`��֐���

// �_��̕`��
extern int AM_DrawPointArray( AM_POINT_DATA *papPoints, int nPointNum )
{
	int							i;
	int							nResult = AM_OK;

	if ( nPointNum > AM_MAX_POINT_NUM ) {
		nPointNum = AM_MAX_POINT_NUM;
		nResult = AMERR_OUTOFRANGE;
	}

	// �_���W�R�s�[
	for ( i = 0; i < nPointNum; i++ ) {
		fg_apPointBuf[i].x = ( float )( papPoints + i )->x;
		fg_apPointBuf[i].y = ( float )( papPoints + i )->y;
		fg_apPointBuf[i].z = ( float )( papPoints + i )->z;
		fg_apPointBuf[i].rhw = 1.0f;
		fg_apPointBuf[i].color = ( DWORD )( papPoints + i )->nColor;
	}

	// �`��
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2 );	// �f�B�t���[�Y�F�̂�
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2 );	// �f�B�t���[�Y�A���t�@�̂�
	gg_pD3DDevice->SetFVF( D3DFVF_AMPOINT );
	gg_pD3DDevice->DrawPrimitiveUP( D3DPT_POINTLIST, ( DWORD )nPointNum,
									fg_apPointBuf, sizeof( AM_POINTVERTEX ) );
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );	// �e�N�X�`�������A
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );	// �e�N�X�`���A���t�@�����A

	return nResult;
}


// �F�R�[�h�̎擾
extern int AM_GetColorCode( int r, int g, int b )
{
	int							nResult;

	nResult = ( int )D3DCOLOR_ARGB( AM_MAX_BLEND_FACTOR, r, g, b );

	return nResult;
}


// �o�b�N�o�b�t�@�̎w��_�̐F�擾
extern int AM_GetBackbufferPointColor( int x, int y )
{
	int							nResult = AMERR_CANNOT;			// �֐��̌���
	HRESULT						hRes;							// �Ăяo������
	RECT						rcSrc;							// �]������`
	D3DLOCKED_RECT				lrLockedRect;					// ���b�N���ꂽ�̈���

	SetRect( &rcSrc, x, y, x + 1, y + 1 );						// �]������`
	hRes = gg_pBackBuffer->LockRect( &lrLockedRect, &rcSrc, D3DLOCK_READONLY );
	if ( SUCCEEDED( hRes ) ) {
		if ( gg_nBitDepth == 32 ) {
			nResult = *( ( int * )( lrLockedRect.pBits ) );// + y * ( lrLockedRect.Pitch >> 2 ) + x );		// 32�r�b�g�F
		}
		else {
			nResult = ( int ) *( ( short * )( lrLockedRect.pBits ) );// + y * ( lrLockedRect.Pitch >> 1 ) + x );		// 16�r�b�g�F
			if ( gg_dfDisplayFormat == D3DFMT_R5G6B5 ) {		// R5G6B5�̏ꍇ
				nResult = 0xff000000 + ( ( nResult & 0xf800 ) << 8 ) + ( ( nResult & 0x07e0 ) << 5 ) + ( ( nResult & 0x001f ) << 3 );
			}
			else {												// X1R5G5B5�̏ꍇ
				nResult = 0xff000000 + ( ( nResult & 0x7c00 ) << 9 ) + ( ( nResult & 0x03e0 ) << 6 ) + ( ( nResult & 0x001f ) << 3 );
			}
		}
		gg_pBackBuffer->UnlockRect();
	}

	return nResult;
}


// �����`�̕`��
extern int AM_DrawRectangleArray( AM_POINT_DATA *papPoints, int nRectNum )
{
	int							i;
	int							nResult = AM_OK;
	AM_POINT_DATA				*pPresPoint;					// ���݂̓_

	if ( nRectNum > AM_MAX_RECT_NUM ) {
		nRectNum = AM_MAX_RECT_NUM;
		nResult = AMERR_OUTOFRANGE;
	}

	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2 );	// �f�B�t���[�Y�F�̂�
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2 );	// �f�B�t���[�Y�A���t�@�̂�
//	gg_pD3DDevice->SetTexture( 0, NULL );							// �e�N�X�`���Ȃ���

	pPresPoint = papPoints;
	for ( i = 0; i < nRectNum; i++ ) {
		// ����
		fg_apPointBuf[0].x = ( float )pPresPoint->x;
		fg_apPointBuf[0].y = ( float )pPresPoint->y;
		fg_apPointBuf[0].z = ( float )pPresPoint->z;
		fg_apPointBuf[0].rhw = 1.0f;
		fg_apPointBuf[0].color = AM_GetModulateColor( pPresPoint->nColor, gg_v2DrawRect[0].color );

		// �E��
		fg_apPointBuf[1] = fg_apPointBuf[0];
		fg_apPointBuf[1].x = ( float )( pPresPoint + 1 )->x;

		// ����
		fg_apPointBuf[2].x = ( float )pPresPoint->x;
		fg_apPointBuf[2].y = ( float )( pPresPoint + 1 )->y;
		fg_apPointBuf[2].z = ( float )( pPresPoint + 1 )->z;
		fg_apPointBuf[2].rhw = 1.0f;
		fg_apPointBuf[2].color = AM_GetModulateColor( ( pPresPoint + 1 )->nColor, gg_v2DrawRect[2].color );

		// �E��
		fg_apPointBuf[3] = fg_apPointBuf[2];
		fg_apPointBuf[3].x = ( float )( pPresPoint + 1 )->x;

		// �`��
		gg_pD3DDevice->SetFVF( D3DFVF_AMPOINT );
		gg_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, nRectNum * 2,
										fg_apPointBuf, sizeof( AM_POINTVERTEX ) );

		pPresPoint += 2;
	}

	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );	// �e�N�X�`���F�����A
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );	// �e�N�X�`���A���t�@�����A

	return nResult;
}


// �F�̊|���Z�v�Z
static int AM_GetModulateColor( int nColor1, int nColor2 )
{
	int				nAlpha, nRed, nGreen, nBlue;

	nAlpha = ( ( ( nColor1 >> 24 ) & 0xff ) * ( ( nColor2 >> 24 ) & 0xff ) ) / 0xff;
	nRed =   ( ( ( nColor1 >> 16 ) & 0xff ) * ( ( nColor2 >> 16 ) & 0xff ) ) / 0xff;
	nGreen = ( ( ( nColor1 >>  8 ) & 0xff ) * ( ( nColor2 >>  8 ) & 0xff ) ) / 0xff;
	nBlue =  ( ( ( nColor1       ) & 0xff ) * ( ( nColor2       ) & 0xff ) ) / 0xff;

	return ( nAlpha << 24 ) + ( nRed << 16 ) + ( nGreen << 8 ) + ( nBlue );
}


// ������t�H���g�̏���
extern int AM_PrepareFontObject( void )
{
	int							nResult;
//	HRESULT						hRes;

	// Windows�t�H���g�I�u�W�F�N�g�쐬
	nResult = AM_SetStringFontSize( AM_DEFAULT_FONT_SIZE );
	if ( nResult < 0 ) return nResult;

/*	// D3DX�t�H���g�I�u�W�F�N�g�쐬
	hRes = D3DXCreateFont( gg_pD3DDevice, fg_hStringFont, &fg_pD3DXFont );
	if ( FAILED( hRes ) ) return AMERR_DISPLAY;
*/
	return AM_OK;
}


// ������t�H���g�̊J��
extern int AM_ReleaseFontObject( void )
{
	AM_SAFE_RELEASE( fg_pD3DXFont );							// D3DX�t�H���g�J��

	if ( fg_hStringFont != NULL ) {
		DeleteObject( fg_hStringFont );							// Windows�t�H���g�폜
		fg_hStringFont = NULL;
	}

	return AM_OK;
}

// ������t�H���g�T�C�Y�̐ݒ�
extern int AM_SetStringFontSize( int nFontSize )
{
    HFONT						hNewFont;						// �V�K�t�H���g
	int							nNWeight;						// �����̃E�F�C�g�i�����j
	HRESULT						hRes;							// ����

	if ( nFontSize == fg_nStringFontSize ) return AM_OK;			// �T�C�Y�������Ȃ牽�����Ȃ�

	// ��������
    if ( nFontSize <= 22 ) nNWeight = FW_SEMIBOLD;
	                  else nNWeight = FW_NORMAL;

	// �t�H���g����
	hNewFont = CreateFont( nFontSize, 0,
		                   0, 0, nNWeight, FALSE, FALSE, FALSE,
		                   SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
						   CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY,
						   FIXED_PITCH, NULL );
	if ( hNewFont == NULL ) return AMERR_INVALIDVALUE;

    if ( fg_hStringFont != NULL ) DeleteObject( fg_hStringFont );	// ���ɂ���΍폜

	// ���L�ϐ��Z�b�g
    fg_hStringFont = hNewFont;
	fg_nStringFontSize = nFontSize;

	// D3DX�t�H���g�I�u�W�F�N�g�쐬
	AM_SAFE_RELEASE( fg_pD3DXFont );								// D3DX�t�H���g�J��
#if D3DX_VERSION == 0x0900
	hRes = D3DXCreateFont( gg_pD3DDevice, fg_hStringFont, &fg_pD3DXFont );	// D3DX�t�H���g�쐬
#else
	hRes = D3DXCreateFont( gg_pD3DDevice, nFontSize, nFontSize / 2, nNWeight, 1, false,
						   SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, NONANTIALIASED_QUALITY,
						   FIXED_PITCH, NULL, &fg_pD3DXFont );
#endif
	if ( FAILED( hRes ) ) return AMERR_DISPLAY;

    return AM_OK;
}


// ������t�H���g�T�C�Y�̎擾
extern int AM_GetStringFontSize( void )
{
	return fg_nStringFontSize;
}


// ������̕`��
extern int AM_DrawString( int x, int y, char *szString, int nColor )
{
	HRESULT						hRes;
	int							nStrLength;						// ������̒���
	RECT						rcDraw;

	nStrLength = ( int )strlen( szString );
	SetRect( &rcDraw, x, y, x + nStrLength * fg_nStringFontSize, y + fg_nStringFontSize );
#if D3DX_VERSION == 0x0900
	hRes = fg_pD3DXFont->DrawTextA( ( LPCSTR )szString, nStrLength, &rcDraw, DT_LEFT | DT_TOP, ( D3DCOLOR )nColor );
#else
	hRes = fg_pD3DXFont->DrawTextA( NULL, ( LPCSTR )szString, nStrLength, &rcDraw, DT_LEFT | DT_TOP, ( D3DCOLOR )nColor );
#endif

	return AM_OK;
}


//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AM_Input.cpp
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̃��[�U���͋@�\�������B
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

#include "AM_DispEnv.h"											// ��ʏ������@�@��ˑ����C���[
#include "AM_Input.h"											// �L�[���́@�@��ˑ����C���[
#include "AB_Input.h"											// �L�[���́@�u���b�W���C���[

#endif


// ---------------------------------------------------------------------------
// �\���̒�`��

typedef struct tagAM_JOYPAD {									// �W���C�p�b�h�\����
	UINT		uJoyID;											// �W���C�X�e�B�b�N�h�c
	BOOL		bValid;											// �W���C�X�e�B�b�N�L����
	DWORD		nMinX, nMaxX;									// �X�e�B�b�N�̍ő�A�ŏ��l
	DWORD		nMinY, nMaxY;
	DWORD		nCenterX, nCenterY;								// �����l
	DWORD		nLeft, nRight;									// ���E�������l
	DWORD		nTop, nBottom;									// �㉺�������l
} AM_JOYPAD;


// ---------------------------------------------------------------------------
// �C���^�[�t�@�C���O���[�o���ϐ���

int								gg_nJoyPadNum = -1;				// �W���C�p�b�h��
int								gg_bMouseCursor = false;		// �}�E�X�J�[�\��
int								gg_bMouseActive = false;		// �}�E�X�L����


// ---------------------------------------------------------------------------
// �t�@�C�����O���[�o���ϐ���

int								fg_bKeyPushed = false;			// �L�[�{�[�h�̃L�[�������ꂽ��
int								fg_nKeyCode;					// �L�[�R�[�h
AM_JOYPAD						fg_jpPads[AM_MAX_PAD_NUM];		// �W���C�p�b�h


// ---------------------------------------------------------------------------
// �����֐��v���g�^�C�v�錾��

static int AM_InitSinglePad( UINT nJoyID, AM_JOYPAD *pJoyPad );	// �W���C�p�b�h������


//-----------------------------------------------------------------------------
// �L�[�{�[�h���͐���֐���

// �L�[���͑҂��E���̓L�[�擾�֐�
extern int AM_GetSingleChar( void )
{
	while( AM_ProcessEvent() && !AM_IsKeyHit() );
	fg_bKeyPushed = false;

	return fg_nKeyCode;
}

// �L�[�{�[�h�����`�F�b�N�֐�
extern int AM_IsKeyHit( void )
{
	if ( !AM_ProcessEvent() ) return ( int )( VK_ESCAPE );		// ���ɏI���̏ꍇ

	if ( fg_bKeyPushed ) return fg_nKeyCode;
					else return false;
}

// �L�[���͒ʒm�֐�
extern int AM_SetKeyPushed( int nKeyCode )
{
	fg_nKeyCode = nKeyCode;
	fg_bKeyPushed = true;

	return AM_OK;
}

// �L�[���͕t��
extern int AM_AddKeyInput( int nKeyState )
{
	BYTE			cKeyBuf[256];

	GetKeyboardState( cKeyBuf );								// �L�[�{�[�h

	if ( ( cKeyBuf[ VK_NUMPAD2 ] & 0x80 ) ||
		 ( cKeyBuf[ VK_DOWN    ] & 0x80 ) ) nKeyState |= KEYIN_DOWN;
	if ( ( cKeyBuf[ VK_NUMPAD4 ] & 0x80 ) ||
		 ( cKeyBuf[ VK_LEFT    ] & 0x80 ) ) nKeyState |= KEYIN_LEFT;
	if ( ( cKeyBuf[ VK_NUMPAD6 ] & 0x80 ) ||
		 ( cKeyBuf[ VK_RIGHT   ] & 0x80 ) ) nKeyState |= KEYIN_RIGHT;
	if ( ( cKeyBuf[ VK_NUMPAD8 ] & 0x80 ) ||
		 ( cKeyBuf[ VK_UP      ] & 0x80 ) ) nKeyState |= KEYIN_UP;

	if ( ( cKeyBuf[ VK_SPACE   ] & 0x80 ) ) nKeyState |= KEYIN_SPC;
	if ( ( cKeyBuf[ VK_ESCAPE  ] & 0x80 ) ) nKeyState |= KEYIN_ESC;
	if ( ( cKeyBuf[ VK_SHIFT   ] & 0x80 ) ) nKeyState |= KEYIN_SFT;
	if ( ( cKeyBuf[ VK_RETURN  ] & 0x80 ) ) nKeyState |= KEYIN_RET;

	if ( ( cKeyBuf[ 'Z'        ] & 0x80 ) ) nKeyState |= KEYIN_Z;
	if ( ( cKeyBuf[ 'X'        ] & 0x80 ) ) nKeyState |= KEYIN_X;
	if ( ( cKeyBuf[ 'C'        ] & 0x80 ) ) nKeyState |= KEYIN_C;
	if ( ( cKeyBuf[ 'A'        ] & 0x80 ) ) nKeyState |= KEYIN_A;
	if ( ( cKeyBuf[ 'S'        ] & 0x80 ) ) nKeyState |= KEYIN_S;
	if ( ( cKeyBuf[ 'D'        ] & 0x80 ) ) nKeyState |= KEYIN_D;
	if ( ( cKeyBuf[ 'Q'        ] & 0x80 ) ) nKeyState |= KEYIN_Q;
	if ( ( cKeyBuf[ 'W'        ] & 0x80 ) ) nKeyState |= KEYIN_W;

	// �f�o�b�O�p����
	if ( ( cKeyBuf[ 'P'        ] & 0x80 ) ) nKeyState |= KEYIN_DEBUG_PAUSE;
	if ( ( cKeyBuf[ 'O'        ] & 0x80 ) ) nKeyState |= KEYIN_DEBUG_DISPLAY;
	if ( ( cKeyBuf[ 'I'        ] & 0x80 ) ) nKeyState |= KEYIN_DEBUG_COLOR;
	
	// �}�E�X
	if ( gg_bMouseActive ) {
		if ( ( cKeyBuf[ VK_LBUTTON ] & 0x80 ) ) nKeyState |= MOUSEKEY_LEFT;
		if ( ( cKeyBuf[ VK_RBUTTON ] & 0x80 ) ) nKeyState |= MOUSEKEY_RIGHT;
		if ( ( cKeyBuf[ VK_MBUTTON ] & 0x80 ) ) nKeyState |= MOUSEKEY_MIDDLE;
	}

	return nKeyState;
}


//-----------------------------------------------------------------------------
// �p�b�h���͐���֐���

UINT						nPadIDs[AM_MAX_PAD_NUM] = { JOYSTICKID1, JOYSTICKID2 };

// �p�b�h������
extern int AM_InitJoyPad( int nMaxPadNum )
{
	int							i;
	int							nActivePadNum = 0;				// �A�N�e�B�u�ȃp�b�h��
	int							nResult;

	if ( nMaxPadNum > AM_MAX_PAD_NUM ) nMaxPadNum = AM_MAX_PAD_NUM;
	for ( i = 0; i < nMaxPadNum; i++ ) {
		nResult = AM_InitSinglePad( nPadIDs[i], &fg_jpPads[i] );	// �p�b�h������
		if ( nResult != AM_OK ) break;							// ������ΏI��
		nActivePadNum++;
	}

	gg_nJoyPadNum = nActivePadNum;								// �p�b�h���o�^
//	if ( nActivePadNum > nMaxPadNum ) nActivePadNum = nMaxPadNum;	// �p�b�h������

	return nActivePadNum;
}


// �P�̃W���C�p�b�h������
static int AM_InitSinglePad( UINT nJoyID, AM_JOYPAD *pJoyPad )
{
	JOYCAPS						jCaps;
	JOYINFOEX					jJoyInfo;
	int							nResult = AMERR_NOHARDWARE;

	pJoyPad->bValid = false;									// �����ɂ��Ă���
	if ( joyGetDevCaps( nJoyID, &jCaps, sizeof( jCaps ) )
		 == JOYERR_NOERROR )
	{
		pJoyPad->uJoyID = nJoyID;
		pJoyPad->nMinX = jCaps.wXmin;  pJoyPad->nMaxX = jCaps.wXmax;
		pJoyPad->nMinY = jCaps.wYmin;  pJoyPad->nMaxY = jCaps.wYmax;
		pJoyPad->nCenterX = ( jCaps.wXmax - jCaps.wXmin ) / 2;
		pJoyPad->nCenterY = ( jCaps.wYmax - jCaps.wYmin ) / 2;
		pJoyPad->nLeft   =   ( pJoyPad->nMaxX - pJoyPad->nMinX ) / 3
						 + pJoyPad->nMinX;
		pJoyPad->nRight  = ( ( pJoyPad->nMaxX - pJoyPad->nMinX ) / 3 ) * 2
						 + pJoyPad->nMinX;
		pJoyPad->nTop    =   ( pJoyPad->nMaxY - pJoyPad->nMinY ) / 3
						 + pJoyPad->nMinY;
		pJoyPad->nBottom = ( ( pJoyPad->nMaxY - pJoyPad->nMinY ) / 3 ) * 2
						 + pJoyPad->nMinY;
		jJoyInfo.dwSize = sizeof( jJoyInfo );					// Joy�\���̃Z�b�g
		jJoyInfo.dwFlags = JOY_RETURNALL;
		if ( joyGetPosEx( pJoyPad->uJoyID, &jJoyInfo ) == JOYERR_NOERROR ) {
			pJoyPad->bValid = TRUE;								// �p�b�h�L����
			nResult = AM_OK;
		}
	}

	return nResult;
}


// �p�b�h�g�p�I��
extern int AM_EndJoyPad( void )
{
	return AM_OK;
}


// �P�̃p�b�h���͕t��
extern int AM_AddJoyInput( int nPadIndex, int nKeyState )
{
	AM_JOYPAD					*pJoyPad;						// �p�b�h
	JOYINFOEX					jJoyInfo;						// �p�b�h���

	// �l�`�F�b�N�ƃp�b�h�擾
	if ( nPadIndex < 0 || nPadIndex >= gg_nJoyPadNum ) return AMERR_NOHARDWARE;
	pJoyPad = &fg_jpPads[nPadIndex];

	if ( pJoyPad->bValid ) {									// �L����
		jJoyInfo.dwSize = sizeof( jJoyInfo );					// Joy�\���̃Z�b�g
		jJoyInfo.dwFlags = JOY_RETURNALL;
		jJoyInfo.dwXpos = pJoyPad->nCenterX;
		jJoyInfo.dwYpos = pJoyPad->nCenterY;
		jJoyInfo.dwButtons = 0;
		if ( joyGetPosEx( pJoyPad->uJoyID, &jJoyInfo ) == JOYERR_NOERROR ) {
			if ( jJoyInfo.dwYpos > pJoyPad->nBottom ) nKeyState |= KEYIN_2;
			if ( jJoyInfo.dwXpos < pJoyPad->nLeft   ) nKeyState |= KEYIN_4;
			if ( jJoyInfo.dwXpos > pJoyPad->nRight  ) nKeyState |= KEYIN_6;
			if ( jJoyInfo.dwYpos < pJoyPad->nTop    ) nKeyState |= KEYIN_8;
			nKeyState |= ( jJoyInfo.dwButtons & 0xff ) << 8;
			if ( jJoyInfo.dwButtons & 0x100 ) nKeyState |= KEYIN_ESC;
			if ( jJoyInfo.dwButtons & 0x200 ) nKeyState |= KEYIN_SPC;
		}
	}

	return nKeyState;
}


//-----------------------------------------------------------------------------
// �}�E�X���͐���֐���

// �}�E�X������
extern int AM_InitMouse( int bMouseState )
{
	if ( !gg_hWindow ) return AMERR_NOINITIALIZE;

	gg_bMouseActive = true;
	AM_SetMouseState( bMouseState );					// ��Ԑݒ�

	return AM_OK;
}


// �}�E�X�g�p�I��
extern int AM_EndMouse( void )
{

	return AM_OK;
}


// �}�E�X��Ԑݒ�
extern int AM_SetMouseState( int bMouseState )
{
	if ( !gg_bMouseActive ) return AMERR_NOINITIALIZE;	// �������`�F�b�N
	if ( bMouseState ) {
		gg_bMouseCursor = true;
	}
	else {
		gg_bMouseCursor = false;
	}
	PostMessage( gg_hWindow, WM_SETCURSOR, ( WPARAM )gg_hWindow, 0 );

	return AM_OK;
}


// �}�E�X�ʒu�擾
extern int AM_GetMousePosition( int *nXLoc, int *nYLoc )
{
	POINT		MousePnt;

	if ( !gg_bMouseActive ) {							// �������`�F�b�N
		*nXLoc = 0;  *nYLoc = 0;
		return AMERR_NOINITIALIZE;
	}

	if ( !GetCursorPos( &MousePnt ) ) return AMERR_CANNOT;
	if ( !gg_bFullMode ) {
		ScreenToClient( gg_hWindow, &MousePnt );
		//MousePnt.x -= gg_piDispLocate.x;
		//MousePnt.y -= gg_piDispLocate.y;
	}
	*nXLoc = MousePnt.x;  *nYLoc = MousePnt.y;

	return AM_OK;
}


// �}�E�X�ʒu�ݒ�
extern int AM_SetMouseLoc( int nXLoc, int nYLoc )
{
	POINT		MousePnt;

	if ( !gg_bMouseActive ) return AMERR_NOINITIALIZE;		// �������`�F�b�N

	if ( !gg_bFullMode ) {
		MousePnt.x = nXLoc;
		MousePnt.y = nYLoc;
		ClientToScreen( gg_hWindow, &MousePnt );
		nXLoc = MousePnt.x;
		nYLoc = MousePnt.y;
	}
	SetCursorPos( nXLoc, nYLoc );

	return AM_OK;
}



//-----------------------------------------------------------------------------
// �ȏ�
// ---------------------------------------------------------------------------
//
// �t�@�C����: AM_Movie.cpp
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̃��[�r�[�Đ��@�\�������B
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
#include <digitalv.h>
#include <vfw.h>
#include <msacm.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dsound.h>

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnv�w�b�_
#include "AB_DispEnv.h"											// ABDispEnv�w�b�_
#include "AM_2DGraph.h"											// AM2DGraph�w�b�_
#include "AM_Sound.h"											// AMSound�w�b�_
#include "AB_Time.h"											// ABTime�w�b�_
#include "AB_Input.h"											// ABInput�w�b�_
#include "AM_Movie.h"											// AMMovie�w�b�_

#endif


// ---------------------------------------------------------------------------
// �C���^�[�t�@�C���O���[�o���ϐ���

MCIDEVICEID						gg_wAVIDevice;					// �`�u�h�f�o�C�X�h�c


// ---------------------------------------------------------------------------
// �t�@�C�����O���[�o���ϐ���

int								fg_bMoviePlaying = false;		// ���[�r�[�Đ�����
int								fg_nMovieFirst = -1;			// ���[�r�[�Đ��N�_
int								fg_nMovieLast = -1;				// ���[�r�[�Đ��I�_
int								fg_bAmAVIInit = false;			// AVI���[�r�[�Đ��������ς݂�


//-----------------------------------------------------------------------------
// ���[�r�[�Đ��֐���

/*// �ʏ타�[�r�[�Đ�
extern int AM_PlayMovie( char *szFileName, int x, int y, int nWidth, int nHeight )
{
	MCI_DGV_OPEN_PARMS		mciOpen;
	MCI_DGV_RECT_PARMS		mciRect;
	MCI_DGV_WINDOW_PARMS	mciWindow;
	MCI_DGV_PUT_PARMS		mciPut;
	MCI_DGV_PLAY_PARMS		mciPlay;
	DWORD					lPlayFlag;
	MCIERROR				nResult;

//	while ( bActiveFlag == FALSE && ProcessEvent() );			// �A�N�e�B�u�҂�
	if ( fg_bMoviePlaying == TRUE ) AM_StopMovie();				// �O�̏I��

//	SetPage( 0, 0 );
	if ( x == 0 && y == 0 &&									// �S��ʂȂ�N���A
		 nWidth == gg_siDispSize.cx && nHeight == gg_siDispSize.cy )
	{
		HCls();
	}
	mciOpen.dwCallback = ( DWORD_PTR )gg_hWindow;				// �C�x���g�n���h��
	mciOpen.lpstrElementName = szFileName;						// �t�@�C���I�[�v��
	nResult = mciSendCommand( 0, MCI_OPEN,
							  (DWORD)( MCI_OPEN_ELEMENT | MCI_DGV_OPEN_NOSTATIC ),
							  (DWORD_PTR)&mciOpen );
	if ( nResult != 0 ) return AMERR_NOFILE;
	gg_wAVIDevice = mciOpen.wDeviceID;

	if ( nWidth == 0 || nHeight == 0 ) {			// �T�C�Y����
		mciWindow.hWnd = gg_hWindow;					// ���T�C�Y
		mciSendCommand( gg_wAVIDevice, MCI_WHERE,
						(DWORD)MCI_DGV_WHERE_SOURCE, (DWORD_PTR)&mciRect);
	}
	else {											// �w��T�C�Y
		mciRect.rc.right = nWidth;
		mciRect.rc.bottom = nHeight;
	}

	mciWindow.hWnd = gg_hWindow;						// �E�B���h�E�֘A�t��
	mciSendCommand( gg_wAVIDevice, MCI_WINDOW,
					(DWORD)MCI_DGV_WINDOW_HWND, (DWORD_PTR)&mciWindow );

	mciPut.dwCallback = (DWORD_PTR)gg_hWindow;		// �]����w��
	mciPut.rc.left = x;  mciPut.rc.top = y;
	mciPut.rc.right = mciRect.rc.right;  mciPut.rc.bottom = mciRect.rc.bottom;
	mciSendCommand( gg_wAVIDevice, MCI_PUT,
					MCI_DGV_PUT_DESTINATION | MCI_DGV_RECT, (DWORD_PTR)&mciPut );

	mciPlay.dwCallback = (DWORD_PTR)gg_hWindow;
	lPlayFlag = MCI_NOTIFY;
	if ( fg_nMovieFirst >= 0 && fg_nMovieLast >= 0 ) {	// �^�C�����[�W�����w�肠�邩
		mciPlay.dwFrom = fg_nMovieFirst;
		mciPlay.dwTo = fg_nMovieLast;
		lPlayFlag |= MCI_FROM | MCI_TO;
		fg_nMovieFirst = -1;  fg_nMovieLast = -1;
	}
	nResult = mciSendCommand( gg_wAVIDevice, MCI_PLAY, lPlayFlag,	// �Đ�
							  (DWORD_PTR)&mciPlay );
	if ( nResult == 0 ) fg_bMoviePlaying = TRUE;
				   else return AMERR_INVALIDVALUE;

	return AM_OK;
}


// �ʏ타�[�r�[�Đ��X�g�b�v
extern int AM_StopMovie( void )
{
	if ( fg_bMoviePlaying ) {
		fg_bMoviePlaying = FALSE;
		mciSendCommand( gg_wAVIDevice, MCI_CLOSE, 0, NULL );
//		nMovieFirst = -1;  nMovieLast = -1;
	}

	return AM_OK;
}*/


// ���[�r�[�t�@�C���I�[�v��
extern int AM_OpenMovieFile( char *szFileName, AMMOVIE *pMovieData )
{
	int					i;
//	int					bError;
	HRESULT				nRes;						// ����
	PAVIFILE			pAVIFile;
	AVIFILEINFO			AVIInfo;
	PAVISTREAM			pAVIStream;
	AVISTREAMINFO		StreamInfo;
	PAVISTREAM			pVideoStream;
	AVISTREAMINFO		VideoInfo;
	PAVISTREAM			pAudioStream;
	AVISTREAMINFO		AudioInfo;
//	double				fAudioLen;					// ���y����(�b�P��)

	if ( !fg_bAmAVIInit ) {							// �C�j�V�����C�Y�`�F�b�N
		AVIFileInit();
		fg_bAmAVIInit = TRUE;
	}
	nRes = AVIFileOpen( &pAVIFile, szFileName, OF_READ, NULL );
	if ( nRes ) return AMERR_NOFILE;
	nRes = AVIFileInfo( pAVIFile, &AVIInfo, sizeof( AVIInfo ) );
	if ( nRes ) return AMERR_INVALIDFILE;

//	bError = false;
	pVideoStream = NULL;
	pAudioStream = NULL;
	for ( i = 0; i < ( int )AVIInfo.dwStreams; i++ ) {
		nRes = AVIFileGetStream( pAVIFile, &pAVIStream, 0L, i );
		if ( nRes ) break;
		nRes = AVIStreamInfo( pAVIStream, &StreamInfo, sizeof( StreamInfo ) );
		if ( nRes ) continue;
		if ( StreamInfo.fccType == streamtypeVIDEO && !pVideoStream ) {
			pVideoStream = pAVIStream;
			AVIStreamAddRef( pVideoStream );
			VideoInfo = StreamInfo;
		}
		if ( StreamInfo.fccType == streamtypeAUDIO && !pAudioStream ) {
			pAudioStream = pAVIStream;
			AVIStreamAddRef( pAudioStream );
			AudioInfo = StreamInfo;
		}
	}

	if ( !pVideoStream ) return AMERR_INVALIDFILE;
	pMovieData->pVideoFrame = AVIStreamGetFrameOpen( pVideoStream,	// �t���[���I�[�v��
													 0 );//(LPBITMAPINFOHEADER)AVIGETFRAMEF_BESTDISPLAYFMT );
	if ( !pMovieData->pVideoFrame ) {
		AM_CloseMovieFile( pMovieData );
		return AMERR_NOTSUPPORTED;
	}

	pMovieData->pAVIFile      = pAVIFile;
	pMovieData->AVIInfo       = AVIInfo;
	pMovieData->pVideoStream  = pVideoStream;
	pMovieData->VideoInfo     = VideoInfo;
	pMovieData->pAudioStream  = pAudioStream;
	if ( pAudioStream ) pMovieData->AudioInfo = AudioInfo;
//	pMovieData->AudioInfo     = AudioInfo;
	pMovieData->fVidSampDelay = ( double )pMovieData->VideoInfo.dwScale / pMovieData->VideoInfo.dwRate;	// �T���v�����O�Ԋu
	//// �r�f�I���Z��������΃I�[�f�B�I�ɍ��킹��
	//fAudioLen = ( ( double )pMovieData->AudioInfo.dwScale / pMovieData->AudioInfo.dwRate ) * pMovieData->AudioInfo.dwLength;
	//if ( ( pMovieData->fVidSampDelay * pMovieData->VideoInfo.dwLength ) < fAudioLen ) {
	//	pMovieData->fVidSampDelay = fAudioLen / pMovieData->VideoInfo.dwLength;
	//}
	pMovieData->fFirstTime    = -1;
	pMovieData->nPresFrame    = -1;
//	pMovieData->pVideoFrame   = NULL;

	return AM_OK;
}


// ���[�r�[�t�@�C���N���[�Y
extern int AM_CloseMovieFile( AMMOVIE *pMovieData )
{
	if ( !pMovieData->pVideoStream ) return AM_OK;

	if ( pMovieData->pVideoFrame ) {				// �r�f�I�t���[���J��
		AVIStreamGetFrameClose( pMovieData->pVideoFrame );
		pMovieData->pVideoFrame = NULL;
	}
	if ( pMovieData->pVideoStream ) {				// �r�f�I�X�g���[���J��
		AVIStreamRelease( pMovieData->pVideoStream );
		pMovieData->pVideoStream = NULL;
	}

	if ( pMovieData->pAudioStream ) {				// �I�[�f�B�I�X�g���[���J��
		AVIStreamRelease( pMovieData->pAudioStream );
		pMovieData->pAudioStream = NULL;
	}

	AVIFileRelease( pMovieData->pAVIFile );			// AVI�t�@�C���J��

	return AM_OK;
}


extern int AM_PlayMovieFile( int x, int y, AMMOVIE *pMovieData )	// ���[�r�[�t�@�C���Đ�
{
	int					i;
	LPBITMAPINFOHEADER	lpBMPInfoHeader;			// �t���[���̃r�b�g�}�b�v�|�C���^
	HBITMAP				hBitmap;					// ���Ԃc�c�a�n���h��
	HDC					hDCSrc;						// �]�����c�b
	HDC					hDCDest;					// �]����c�b
	int					nDispX, nDispY;				// �\�����W
//	int					fNowTime;					// ���ݎ���
//	int					fNextTime;					// ���̕\������
//	int					fSampDelay;					// �T���v�����O�Ԋu�i���ԒP�ʂ�ms�j
	double				fNowTime = 1.0f;			// ���ݎ���
	double				fNextTime = 0.0f;			// ���̕\������
	double				fFirstTime = 0.0f;			// �ŏ��̕\������
//	double				fSampDelay;					// �T���v�����O�Ԋu
	char				*pPicture;					// �摜�f�[�^�|�C���^
	WAVEFORMATEX		*pwfSrcWave;				// ���f�[�^Wave�t�H�[�}�b�g
	WAVEFORMATEX		wfDestWave;					// ��Wave�t�H�[�}�b�g
	long				nHeaderSize;				// �����w�b�_�T�C�Y�o�b�t�@
	int					nResult = AM_OK;			// ���s����
	HACMSTREAM			hACMStream;					// �`�b�l�X�g���[���n���h��
	ACMSTREAMHEADER		acmHeader;					// �`�b�l�X�g���[���w�b�_
	BYTE				*pAudioSrcBuf, *pAudioDestBuf;	// �����o�b�t�@
	int					nRes, nRes2 = AVIERR_NODATA;
	MMRESULT			nMMRes;						// �l�l�@�`�o�h����
	DWORD				nAudioSrcBufSize, nAudioDestBufSize;	// �����]�����^�]����T�C�Y
	DWORD				nAudioDestBlockSize;		// �𓀍ς݉����f�[�^�u���b�N�T�C�Y
	DWORD				nAudioBlockNum;				// �P�P�ʃo�b�t�@���̃u���b�N��
	DWORD				nCurrentAudioBlk;			// ���݂̃u���b�N�ʒu
	int					nAudioCount;				// �I�[�f�B�I����p�J�E���^
	long				nSamples, nBytes;			// ���������T���v���^�o�C�g��
	LPDIRECTSOUNDBUFFER	lpDSAVIAudio = NULL;		// DirectSound�Đ��o�b�t�@
	LPVOID				lpDSBuf1;
	DWORD				nDSSize1;
	DWORD				nCurrentPlay, nCurrentWrite;
	DWORD				nAudioEndLoc = 0;
	BOOL				bDoubleBuf;
	int					nNowKey;					// ���̃L�[
	int					nLastKey;					// �ЂƂO�̃L�[

	if ( !pMovieData->pVideoStream ) return AM_OK;		// �������`�F�b�N
	if ( !lpDS ) InitSound( 0 );						// �T�E���h�������`�F�b�N
	bDoubleBuf = true; //( lpDDSActive != lpDDSPrimary );		// �_�u���o�b�t�@�`�F�b�N
	if ( KBHit() ) GetCh();										// �L�[�o�b�t�@�N���A

//	pMovieData->pVideoFrame = AVIStreamGetFrameOpen( pMovieData->pVideoStream,	// �t���[���I�[�v��
//													 0 );//(LPBITMAPINFOHEADER)AVIGETFRAMEF_BESTDISPLAYFMT );
//	if ( !pMovieData->pVideoFrame ) return AMERR_NOTSUPPORTED;
//	fSampDelay = ( double )pMovieData->VideoInfo.dwScale / pMovieData->VideoInfo.dwRate;	// �T���v�����O�Ԋu

	if ( pMovieData->pAudioStream ) {				// �I�[�f�B�I�X�g���[������
		nRes = AVIStreamReadFormat( pMovieData->pAudioStream, 0, NULL, &nHeaderSize );	// �I�[�f�B�I�t�H�[�}�b�g�擾
		pwfSrcWave = ( WAVEFORMATEX * )malloc( nHeaderSize );
		nRes = AVIStreamReadFormat( pMovieData->pAudioStream, 0, pwfSrcWave, &nHeaderSize );

//		int nBits = 8;
		wfDestWave = *pwfSrcWave;					// �����𓀃t�H�[�}�b�g�Z�b�g
		wfDestWave.wFormatTag = WAVE_FORMAT_PCM;
		wfDestWave.nBlockAlign = wfDestWave.nChannels * 16 / 8;
		wfDestWave.nAvgBytesPerSec = wfDestWave.nSamplesPerSec * wfDestWave.nBlockAlign;
		wfDestWave.wBitsPerSample = 16;
		wfDestWave.cbSize = 0;
		nMMRes = acmStreamOpen( &hACMStream, NULL, pwfSrcWave, &wfDestWave, NULL, 0, 0, 0 );//ACM_STREAMOPENF_NONREALTIME );
		if ( !nMMRes ) {
			nAudioSrcBufSize = 0;
			nMMRes = acmStreamSize( hACMStream, pwfSrcWave->nBlockAlign/* * pMovieData->AudioInfo.dwLength*/,
									&nAudioDestBlockSize, ACM_STREAMSIZEF_SOURCE );
			nAudioBlockNum = ( int )( wfDestWave.nAvgBytesPerSec * ( 1.0 + pMovieData->fVidSampDelay ) )
							 / nAudioDestBlockSize + 1;
			nAudioSrcBufSize = pwfSrcWave->nBlockAlign * nAudioBlockNum; //pMovieData->AudioInfo.dwLength;
			nAudioDestBufSize = nAudioDestBlockSize * nAudioBlockNum;
			pAudioSrcBuf = ( BYTE * )malloc( nAudioSrcBufSize );	// �\�[�X����
			pAudioDestBuf = ( BYTE * )malloc( nAudioDestBufSize );	// �f�B�X�g����
			ZeroMemory( &acmHeader, sizeof( acmHeader ) );		// �`�b�l�g�d�`�c�d�q����
			acmHeader.cbStruct = sizeof( acmHeader );
			acmHeader.pbSrc = pAudioSrcBuf;
			acmHeader.cbSrcLength = nAudioSrcBufSize;
			acmHeader.cbSrcLengthUsed = nAudioSrcBufSize;
			acmHeader.pbDst = pAudioDestBuf;
			acmHeader.cbDstLength = nAudioDestBufSize;
			acmHeader.cbDstLengthUsed = nAudioDestBufSize;
			nMMRes = acmStreamPrepareHeader( hACMStream, &acmHeader, 0 );	// �𓀃w�b�_����
			MakeDSStreamBuf( &wfDestWave, nAudioDestBufSize * 2, &lpDSAVIAudio );	// DirectSound�o�b�t�@����

			nCurrentAudioBlk = 0;
			nAudioCount = 0;

			for ( i = 1; i <= 2; i++ ) {			// �����o�b�t�@�t�B��
				nRes = AVIStreamRead( pMovieData->pAudioStream,
									  nCurrentAudioBlk, nCurrentAudioBlk + nAudioBlockNum,
									  pAudioSrcBuf, nAudioSrcBufSize, &nBytes, &nSamples );
				nMMRes = acmStreamConvert( hACMStream, &acmHeader, ACM_STREAMCONVERTF_BLOCKALIGN );
				lpDSAVIAudio->Lock( ( nAudioCount & 1 ) ? nAudioDestBufSize : 0, nAudioDestBufSize,
									&lpDSBuf1, &nDSSize1, NULL, NULL, 0 );
				if ( nRes || ( DWORD )nSamples != nAudioBlockNum ) FillMemory( lpDSBuf1, nAudioDestBufSize, 0 );
				if ( !nRes ) CopyMemory( lpDSBuf1, pAudioDestBuf, nSamples * nAudioDestBlockSize );
				lpDSAVIAudio->Unlock( lpDSBuf1, nDSSize1, NULL, 0 );
				nCurrentAudioBlk += nAudioBlockNum;
				nAudioCount++;
			}
		}

	}
	nNowKey = CheckKey( AMINPUT_MULT );						// �ЂƂO�̃L�[
	fFirstTime = GetNowTime( 0 );
	for ( i = 0; i < ( int )pMovieData->VideoInfo.dwLength; i++ ) {
		nLastKey = nNowKey;									// �L�[���p��
		nNowKey = CheckKey( AMINPUT_MULT );					// ���݂̃L�[
		HCls();										// ��ʃN���A
		lpBMPInfoHeader = ( LPBITMAPINFOHEADER )AVIStreamGetFrame( pMovieData->pVideoFrame, i );	// �t���[���擾
		if ( !lpBMPInfoHeader ) {					// �擾�`�F�b�N
			nResult = AMERR_NOTSUPPORTED;
			break;
		}
		if ( !bDoubleBuf && i != 0 ) {				// �_�u���o�b�t�@�łȂ��ꍇ�����ő҂�
			do {									// ���ԃX�P�[������
				fNowTime = ( GetNowTime( 0 ) - fFirstTime ) / 1000.0;	// ���݂̎���
				if ( !ProcessEvent() ) break;
			 } while ( fNowTime <= fNextTime );
		}
		if ( FAILED( gg_pBackBuffer->GetDC( &hDCDest ) ) ) break;			// �]����c�b����
		pPicture = ( char * )lpBMPInfoHeader + sizeof( BITMAPINFOHEADER );
		if ( lpBMPInfoHeader->biBitCount <= 8 )
			pPicture += ( ( lpBMPInfoHeader->biClrUsed == 0 )
						  ? 1 << lpBMPInfoHeader->biBitCount : lpBMPInfoHeader->biClrUsed ) * 4;
		if ( 1 ) {//gg_bFullMode /*|| lpDDSActive != lpDDSPrimary*/ ) {
			nDispX = 0;  nDispY = 0;
		}
		else {
			nDispX = gg_piDispLocate.x;  nDispY = gg_piDispLocate.y;
		}
/*		if ( x == MOVIE_FULL_LOC && y == MOVIE_FULL_LOC ) {
			StretchDIBits( hDCDest, nDispX, nDispY, siDispSize.cx, siDispSize.cy,
						   0, 0, lpBMPInfoHeader->biWidth, lpBMPInfoHeader->biHeight,
						   pPicture, (LPBITMAPINFO)lpBMPInfoHeader, DIB_RGB_COLORS, SRCCOPY );
		}
		else {
			SetDIBitsToDevice( hDCDest, nDispX + x, nDispY + y, lpBMPInfoHeader->biWidth, lpBMPInfoHeader->biHeight,
							   0, 0, 0, lpBMPInfoHeader->biHeight,
							   pPicture, (LPBITMAPINFO)lpBMPInfoHeader, DIB_RGB_COLORS );
		}*/
		if ( x == MOVIE_FULL_LOC && y == MOVIE_FULL_LOC ) {		// �t�����
			hBitmap = CreateDIBitmap( hDCDest, lpBMPInfoHeader, CBM_INIT,	// �c�h�a����c�c�a�쐬
									  pPicture, ( BITMAPINFO * )lpBMPInfoHeader,
									  DIB_RGB_COLORS );
			hDCSrc = CreateCompatibleDC( hDCDest );		// �]�����c�b����
			SelectObject( hDCSrc, hBitmap );
			nRes = StretchBlt( hDCDest, nDispX, nDispY, gg_siDispSize.cx, gg_siDispSize.cy,	// �t����ʕ`��
							   hDCSrc, 0, 0, lpBMPInfoHeader->biWidth, lpBMPInfoHeader->biHeight,
							   SRCCOPY );
			DeleteDC( hDCSrc );							// �c�b���J��
			DeleteObject( hBitmap );
		}
		else {											// �ʏ�T�C�Y
			SetDIBitsToDevice( hDCDest, nDispX + x, nDispY + y, lpBMPInfoHeader->biWidth, lpBMPInfoHeader->biHeight,
							   0, 0, 0, lpBMPInfoHeader->biHeight,
							   pPicture, (LPBITMAPINFO)lpBMPInfoHeader, DIB_RGB_COLORS );
/*			hBitmap = CreateDIBitmap( hDCDest, lpBMPInfoHeader, CBM_INIT,	// �c�h�a����c�c�a�쐬
									  pPicture, ( BITMAPINFO * )lpBMPInfoHeader,
									  DIB_RGB_COLORS );
			hDCSrc = CreateCompatibleDC( hDCDest );		// �]�����c�b����
			SelectObject( hDCSrc, hBitmap );
			nRes = BitBlt( hDCDest, nDispX + x, nDispY + y,		// ���T�C�Y�`��
									lpBMPInfoHeader->biWidth, lpBMPInfoHeader->biHeight,
						   hDCSrc, 0, 0, SRCCOPY );
			DeleteDC( hDCSrc );						// �c�b���J��
			DeleteObject( hBitmap );*/
		}
		gg_pBackBuffer->ReleaseDC( hDCDest );
//		if ( !ProcessEvent() || ( nNowKey & ~nLastKey ) ) break;
		if ( !ProcessEvent() || KBHit() ) break;
		if ( i == 0 ) {
			fNowTime = ( GetNowTime( 0 ) - fFirstTime ) / 1000.0;	// ���݂̎���
			fNextTime = fNowTime;// + pMovieData->fVidSampDelay;	// ���̕\������
			if ( lpDSAVIAudio ) lpDSAVIAudio->Play( 0, 0, DSBPLAY_LOOPING );
		}
		else {
/*			char szOut[80];
			fNowTime = ( double )GetNowTime( 0 ) / 1000;		// ���݂̎���
//			DrawFBox( 0, 0, 320, 16, 0 );
			sprintf( szOut, "%d", ( int )( ( fNextTime - fNowTime ) * 1000 ) );
			DrawString( 0, 0, szOut, 0xffffffff );*/
			if ( bDoubleBuf ) {						// �_�u���o�b�t�@�̏ꍇ�����ő҂�
				do {									// ���ԃX�P�[������
					fNowTime = ( GetNowTime( 0 ) - fFirstTime ) / 1000.0;	// ���݂̎���
					if ( !ProcessEvent() ) break;
				 } while ( fNowTime <= fNextTime );
			}
		}
		fNextTime += pMovieData->fVidSampDelay;
		if ( fNowTime > fNextTime ) {				// �R�}��΂�
			i++;
			fNextTime += pMovieData->fVidSampDelay;
		}
		Flip();
		if ( lpDSAVIAudio ) {						// �����o�b�t�@���t���b�V��
			lpDSAVIAudio->GetCurrentPosition( &nCurrentPlay, &nCurrentWrite );
			if ( ( nCurrentPlay < nAudioDestBufSize && ( nAudioCount & 1 ) ) ||
				 ( nCurrentPlay >= nAudioDestBufSize && !( nAudioCount & 1 ) ) )
			{
				nRes = AVIStreamRead( pMovieData->pAudioStream,
									  nCurrentAudioBlk, nCurrentAudioBlk + nAudioBlockNum,
									  pAudioSrcBuf, nAudioSrcBufSize, &nBytes, &nSamples );
				nMMRes = acmStreamConvert( hACMStream, &acmHeader, 0 ); //ACM_STREAMCONVERTF_BLOCKALIGN );
				lpDSAVIAudio->Lock( ( nAudioCount & 1 ) ? nAudioDestBufSize : 0, nAudioDestBufSize,
									&lpDSBuf1, &nDSSize1, NULL, NULL, 0 );
				if ( nRes || ( DWORD )nSamples != nAudioBlockNum ) FillMemory( lpDSBuf1, nAudioDestBufSize, 0 );
				if ( !nRes ) CopyMemory( lpDSBuf1, pAudioDestBuf, nSamples * nAudioDestBlockSize );
				lpDSAVIAudio->Unlock( lpDSBuf1, nDSSize1, NULL, 0 );
				nCurrentAudioBlk += nAudioBlockNum;
				nAudioCount++;
			}
		}
	}
	while ( fNowTime <= fNextTime ) {				// ���ԑ҂�
		fNowTime = ( GetNowTime( 0 ) - fFirstTime ) / 1000.0;		// ���݂̎���
		if ( !ProcessEvent() ) break;
	}

	if ( lpDSAVIAudio ) {							// �����Đ�����ꍇ
		lpDSAVIAudio->Stop();						// DirectSound���I��
		lpDSAVIAudio->Release();
		nMMRes = acmStreamUnprepareHeader( hACMStream, &acmHeader, 0 );	// ACM���I��
		acmStreamClose( hACMStream, 0 );
		free( pAudioSrcBuf );						// �����o�b�t�@�J��
		free( pAudioDestBuf );
		free( pwfSrcWave );
	}

	return nResult;
}


// ���[�r�[�̓���t���[���擾
extern int AM_GetMovieFrame( AMMOVIE *pMovieData, int nFrameLoc,
							 AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo )
{
	HDC							hDCDest;					// �]����c�b
	LPBITMAPINFOHEADER			lpBMPInfoHeader;			// �t���[���̃r�b�g�}�b�v�|�C���^
	char						*pPicData;
	HRESULT						hRes;						// ����
	AM_LPDISPLAYBUFFER			pTempBuffer;				// �e���|�����o�b�t�@
	LPDIRECT3DSURFACE9			lpTopSurf;					// �e�N�X�`���g�b�v�T�[�t�F�X
	RECT						rcDest;						// �]�����E�]�����`

	lpBMPInfoHeader = ( LPBITMAPINFOHEADER )AVIStreamGetFrame( pMovieData->pVideoFrame,
															   nFrameLoc );	// �t���[���擾
	if ( !lpBMPInfoHeader ) return AMERR_NOTSUPPORTED;

	gg_pD3DDevice->CreateOffscreenPlainSurface( pMovieData->AVIInfo.dwWidth, pMovieData->AVIInfo.dwHeight,
												D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &pTempBuffer, NULL );
	pTempBuffer->GetDC( &hDCDest );

	pPicData = ( char * )lpBMPInfoHeader + sizeof( BITMAPINFOHEADER );
	if ( lpBMPInfoHeader->biBitCount <= 8 )
		pPicData += ( ( lpBMPInfoHeader->biClrUsed == 0 )
					  ? 1 << lpBMPInfoHeader->biBitCount : lpBMPInfoHeader->biClrUsed ) * 4;
	SetDIBitsToDevice( hDCDest, 0, 0, lpBMPInfoHeader->biWidth, lpBMPInfoHeader->biHeight,
					   0, 0, 0, lpBMPInfoHeader->biHeight,
					   pPicData, (LPBITMAPINFO)lpBMPInfoHeader, DIB_RGB_COLORS );
	pTempBuffer->ReleaseDC( hDCDest );

	hRes = pPicture->GetSurfaceLevel( 0, &lpTopSurf );		// �]����T�[�t�F�X�擾
	SetRect( &rcDest, 0, 0, pPicInfo->nWidth, pPicInfo->nHeight );	// �]�����`
	// �]��
	D3DXLoadSurfaceFromSurface( lpTopSurf, NULL, &rcDest, pTempBuffer, NULL, NULL, D3DX_FILTER_LINEAR, 0 );
	pTempBuffer->Release();									// �]�����T�[�t�F�X�J��
	lpTopSurf->Release();									// �]����T�[�t�F�X�J��

	return AM_OK;
}
// ---------------------------------------------------------------------------
//
// �t�@�C����: AM_Time.cpp
//
// �@�\: DirectX��ł̂`�l�f�`�l�d�̎��ԋ@�\�������B
//
// �v���b�g�t�H�[��: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#pragma warning( disable : 4819 )						// �R�[�h�y�[�W�̌x���}�~

#include <windows.h>
#include <mmsystem.h>

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// �L�[���́@�u���b�W���C���[
#include "AM_Time.h"											// ���ԋ@�\�@�@��ˑ����C���[

#endif


// ---------------------------------------------------------------------------
// �t�@�C�����O���[�o���ϐ���



//-----------------------------------------------------------------------------
// ���Ԋ֐���

// ���ݎ��Ԏ擾�֐�
extern int AM_GetPresentTime( void )
{
	return timeGetTime();
//	return GetTickCount();
}


// �����̎擾
extern int AM_GetDateTime( AMDATETIME *pDateTime )
{
	WORD		nDate, nTime;						// ����
	FILETIME	ftFileTime;							// �t�@�C���^�C���o�b�t�@
	SYSTEMTIME	stSystemTime;						// �V�X�e���^�C���o�b�t�@

	GetLocalTime( &stSystemTime );					// �V�X�e���^�C���擾
	SystemTimeToFileTime( &stSystemTime, &ftFileTime );	// �t�@�C���^�C���ϊ�
	FileTimeToDosDateTime( &ftFileTime, &nDate, &nTime );
	pDateTime->nPackedTime = ( (DWORD)nDate << 16 ) + nTime;	// �p�b�N�g����
	pDateTime->nYear   = (unsigned int )stSystemTime.wYear;		// �N
	pDateTime->nMonth  = (unsigned char)stSystemTime.wMonth;	// ��
	pDateTime->nDay    = (unsigned char)stSystemTime.wDay;		// ��
	pDateTime->nHour   = (unsigned char)stSystemTime.wHour;		// ��
	pDateTime->nMinute = (unsigned char)stSystemTime.wMinute;	// ��
	pDateTime->nSecond = (unsigned char)stSystemTime.wSecond;	// �b

	return AM_OK;
}


//-----------------------------------------------------------------------------
// �ȏ�

// �`�l�f�`�l�d�@�c�����������r�����������W���[��
// Version 2.4
// Programmed by K.Katoh, Amusement Media Academy

#pragma warning( disable : 4819 )						// �R�[�h�y�[�W�̌x���}�~

//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <process.h>
#include <ddraw.h>
#include <dsound.h>
#include "amgame.h"

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_Sound.h"								// �T�E���h�w�b�_

#endif

#define MAXSOUNDNUM			200						// �ő�T�E���h��
#define NOSOUNDSIZE			1000					// �����T�C�Y
#define	STREAM_BUF_SIZE		0x10000					// �X�g���[�~���O�o�b�t�@�T�C�Y

#define SOUND_WAV			0
#define SOUND_MIDI			1

#define DEF_VOLUME			80						// �f�t�H���g�̉���

#define BUFFERDESC_SIZE		20						// DSBUFFERDESC�\���̃T�C�Y

#define WAVE_H_BUF_SIZE		256						// �w�b�_�o�b�t�@�T�C�Y		

static HRESULT AMDSErr( HWND hOutWnd, HRESULT ErrorCode );	// �G���[����
static int LoadWavBuffer( char *szWavFile, LPDIRECTSOUNDBUFFER *lplpDSBDest );
static int MakeSilentBuffer( LPDIRECTSOUNDBUFFER *lplpDSBDest );	// �����o�b�t�@����
static int WaitEndWav( LPDIRECTSOUNDBUFFER lpWaitBuf );		// �v�����Đ��I���҂�
static int StartMciMidi( char *szFileName );				// MCI MIDI�Đ�

typedef struct tagMEMSOUND {							// �������v�����Ǘ��\����
	int		nSoundKind;
	int		nSBufLoc;
	char	*szSoundName;
} MEMSOUND;

typedef struct tagSTREAM_SOUND {						// �X�g���[�~���O�Ǘ��\����
	HANDLE			hWavFile;							// �t�@�C���n���h��
	int				nFileSize;							// �t�@�C���T�C�Y
	int				nFileRemain;						// �ǂݎc���o�C�g��
	int				nBufRemain;							// �o�b�t�@�c��o�C�g��
	WAVEFORMATEX	wfWaveFormat;						// WAV�t�H�[�}�b�g
	char			cHeadLevel;							// �擪���x��
	int				bProcessing;						// �������t���O
} STREAM_SOUND;

LPDIRECTSOUND			lpDS				= NULL;		// DirectSound�I�u�W�F
LPDIRECTSOUNDBUFFER		lpDSBPrimary		= NULL;		// �v���C�}���o�b�t�@
LPDIRECTSOUNDBUFFER		lpDSBSilent			= NULL;		// �����o�b�t�@
LPDIRECTSOUNDBUFFER		lpDSBWork			= NULL;		// ��Ɨp�o�b�t�@
LPDIRECTSOUNDBUFFER		lpDSBSecondary[MAXSOUNDNUM];	// �X�g�A�p�o�b�t�@

LPDIRECTSOUNDBUFFER		lpDSBStream         = NULL;		// �X�g���[���o�b�t�@
LPDIRECTSOUNDNOTIFY		lpDSStrNotify       = NULL;		// �ʒm�C���^�t�F�[�X


MEMSOUND		WavInfo[MAXSOUNDNUM];
int				hSoundLoc, nSBufLoc;
STREAM_SOUND	ssStreamData;							// �X�g���[���f�[�^
int				bStreamThread = false;					// �X�g���[���X���b�h
int				nStreamLastPos;							// �X�g���[���̂ЂƂO�ʒu
int				nStreamReadEnd = 0;						// �X�g���[���ǂݏo���I����

HMIDIOUT		hMidi;
UINT			wMidiID = NULL;							// MIDI�f�o�C�X�h�c
BOOL			bMidiPlaying = 0;						// MIDI���t���t���O
BOOL			bMidiLooping = FALSE;					// MIDI���[�v���t���O

void WaitForDSBufEnd( LPVOID lpv );						// �o�b�t�@�I���ʒm

#define DSCHKINIT if( dsrval != DD_OK ) return AMDSErr( gg_hWindow, dsrval );
extern int InitSound( int nSoundMode )				// �T�E���h�̏�����
{
	int				i, nResCode;
	HRESULT			dsrval;
	DSBUFFERDESC	dsbd;

	if ( gg_hWindow == NULL ) return AMERR_NOINITIALIZE;
	dsrval = DirectSoundCreate( NULL, &lpDS, NULL );	// �c�r�쐬
	DSCHKINIT
	dsrval = lpDS->SetCooperativeLevel( gg_hWindow, DSSCL_NORMAL );	// �������x��
	DSCHKINIT

	ZeroMemory( &dsbd, sizeof( dsbd ) );				// �v���C�}���o�b�t�@
	dsbd.dwSize = BUFFERDESC_SIZE;
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	dsrval = lpDS->CreateSoundBuffer( &dsbd, &lpDSBPrimary, NULL );
	DSCHKINIT

	for ( i = 0; i < nSBufLoc; i++ ) {				// �o�b�t�@������
		lpDSBSecondary[i] = NULL;
	}
	hSoundLoc = 1;  nSBufLoc = 0;

	if ( ( nResCode = MakeSilentBuffer( &lpDSBSilent ) ) < 0 ) {
		return nResCode;									// �����Đ�
	}
	lpDSBSilent->Play( 0, 0, DSBPLAY_LOOPING );

	SetPcmVolume( DEF_VOLUME );
	SetMusicVolume( DEF_VOLUME );

	ssStreamData.hWavFile = INVALID_HANDLE_VALUE;		// �X�g���[��Wav�t�@�C���n���h��������
	ssStreamData.bProcessing = false;					// �������t���O

	return 0;
}

// DirectDraw�G���[����
static HRESULT AMDSErr( HWND hOutWnd, HRESULT ErrorCode )
{
	char buf[80];

	if ( ErrorCode == DSERR_NODRIVER ) return AMERR_NODRIVER;

	wsprintf( buf, "DirectSound�G���[ (%08lx)\n", ErrorCode );
	MessageBox( hOutWnd, buf, "ERROR", MB_OK );

	return AMERR_INITIALIZE;
}

extern int EndSound( void )							// �T�E���h�g�p�I��
{
	StopPcm();										// ���y��~
	EndDSound();

	return 0;
}

extern int EndDSound( void )						// �����T�E���h�g�p�I��
{
	InitSoundMem();									// ��ʃT�E���h�o�b�t�@�̊J��

	if ( lpDSBStream ) {
		while ( bStreamThread &&    ssStreamData.bProcessing   );
		lpDSBStream->Stop();						// ���t��~
		lpDSBStream->Release();						// �T�E���h�o�b�t�@�J��
		lpDSBStream = NULL;
	}

	if ( lpDSBWork ) {								// ���[�N�p�o�b�t�@�J��
		lpDSBWork->Stop();
		lpDSBWork->Release();
		lpDSBWork = NULL;
	}
	if ( lpDSBSilent ) {							// �����o�b�t�@�J��
		lpDSBPrimary->Stop();
		lpDSBSilent->Stop();
		lpDSBSilent->Release();
		lpDSBSilent = NULL;
	}
	if ( lpDSBPrimary ) {							// �v���C�}���o�b�t�@�J��
		lpDSBPrimary->Release();
		lpDSBPrimary = NULL;
	}

	if ( lpDS ) {									// DirectSound�J��
		lpDS->Release();
		lpDS = NULL;
	}

	if ( wMidiID ) {								// �l�h�c�h��~
		mciSendCommand( wMidiID, MCI_CLOSE, 0, NULL );
		bMidiPlaying = 0;
	}

	return 0;
}

extern int InitSoundMem( void )						// �T�E���h�������̏�����
{
	int		i;

	if ( !lpDS ) return AMERR_NOINITIALIZE;			// �������`�F�b�N

	for ( i = 0; i < MAXSOUNDNUM; i++ ) {
		if ( lpDSBSecondary[i] ) {
			lpDSBSecondary[i]->Stop();
			lpDSBSecondary[i]->Release();
			lpDSBSecondary[i] = NULL;
		}
	}
	hSoundLoc = 1;  nSBufLoc = 0;
	StopMusic();

	return AM_OK;
}

extern int PlayWav( char *szFileName )				// �v�����t�@�C�����ڍĐ�
{
	int				nResCode;
	HRESULT			dsrval;

	if ( !lpDS ) return AMERR_NOINITIALIZE;
	if ( ( nResCode = LoadWavBuffer( szFileName, &lpDSBWork ) ) < 0 )
		return nResCode;

	dsrval = lpDSBWork->Play( 0, 0, 0 );			// �Đ��J�n
	if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
	WaitEndWav( lpDSBWork );
	return 0;
}

extern int PlayWavBack( char *szFileName )			// Wav���ڃo�b�N�Đ�
{
	int				nResCode;
	HRESULT			dsrval;

	if ( !lpDS ) return AMERR_NOINITIALIZE;
	if ( ( nResCode = LoadWavBuffer( szFileName, &lpDSBWork ) ) < 0 )
		return nResCode;

	dsrval = lpDSBWork->Play( 0, 0, 0 );			// �Đ��J�n
	if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;

	return 0;
}

extern int PlayWavStreamBack( char *szFileName )			// Wav�X�g���[���o�b�N�Đ�
{
	int				nResCode;
	HRESULT			dsrval;
	int				i;
    HRSRC			hrscr;
	int				nWavSize;
	char			*lpWavData;
	HANDLE			hWavFile;
	char			cHeadBuffer[WAVE_H_BUF_SIZE];
	DWORD			dwRead;
	int				nHeaderSize;							// �w�b�_�T�C�Y
	int				nReadSize;								// ���[�h�T�C�Y

	if ( !lpDS ) return AMERR_NOINITIALIZE;			// �������`�F�b�N

	if( ( hrscr = FindResource( NULL, szFileName, "WAVE" ) ) != NULL ) {
		if ( ( nResCode = LoadWavBuffer( szFileName, &lpDSBWork ) ) < 0 )
			return nResCode;

		dsrval = lpDSBWork->Play( 0, 0, 0 );			// �Đ��J�n
		if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
	}
	else {
		if ( lpDSBStream ) {
			if ( bStreamThread ) {
				int nFirstTime;
				nFirstTime = timeGetTime();
				while ( ( bStreamThread && !( ssStreamData.bProcessing ) ) ) {
					if ( !ProcessEvent() ) return AM_OK;
					if ( ( timeGetTime() - nFirstTime ) > 200 ) break;
				}
				nFirstTime = timeGetTime();
				while ( ( bStreamThread &&    ssStreamData.bProcessing   ) ) {
					if ( !ProcessEvent() ) return AM_OK;
					if ( ( timeGetTime() - nFirstTime ) > 200 ) break;
				}
			}
			dsrval = lpDSBStream->Stop();				// ���t��~
			if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
			dsrval = lpDSBStream->Release();			// �T�E���h�o�b�t�@�J��
			if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
			lpDSBStream = NULL;
			if ( lpDSStrNotify ) {						// �ʒmINT�J��
				lpDSStrNotify->Release();
				lpDSStrNotify = NULL;
			}
		}
		if ( ssStreamData.hWavFile != INVALID_HANDLE_VALUE ) {
			CloseHandle( ssStreamData.hWavFile );		// �t�@�C���N���[�Y
			ssStreamData.hWavFile = INVALID_HANDLE_VALUE;
		}
		hWavFile = CreateFile( szFileName, GENERIC_READ, FILE_SHARE_READ,	// ̧�ٵ����
							   NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		if ( hWavFile == INVALID_HANDLE_VALUE ) return AMERR_NOFILE;	// �����`�F�b�N
		ssStreamData.hWavFile = hWavFile;

		nWavSize = GetFileSize( hWavFile, NULL );		// �t�@�C���T�C�Y
		ssStreamData.nFileSize = nWavSize;

		ReadFile( hWavFile, cHeadBuffer, WAVE_H_BUF_SIZE, &dwRead, NULL );	// �f�[�^�ǂݏo��
		if ( *( DWORD * )cHeadBuffer != mmioFOURCC( 'R', 'I', 'F', 'F' ) )
			return AMERR_INVALIDFILE;
		if ( *( DWORD * )( &( cHeadBuffer[8] ) ) != mmioFOURCC( 'W', 'A', 'V', 'E' ) )
			return AMERR_INVALIDFILE;

		CopyMemory( &( ssStreamData.wfWaveFormat ), &( cHeadBuffer[20] ), sizeof( WAVEFORMATEX ) );	// �t�H�[�}�b�g�R�s�[

		lpWavData = NULL;								// �f�[�^�ʒu�T�[�`
		for ( i = 0; i < WAVE_H_BUF_SIZE - 10; i++ ) {
			if ( ( *( DWORD * )( &( cHeadBuffer[i] ) ) == mmioFOURCC( 'd', 'a', 't', 'a' ) ) &&
				 ( *( DWORD * )( &( cHeadBuffer[i + 4] ) ) != 0 ) )
			{
				nHeaderSize = i + 8;
				lpWavData = &( cHeadBuffer[i] );
				ssStreamData.cHeadLevel = *( lpWavData + 8 + 1 );	// �擪���x��
				break;
			}
		}
		if ( lpWavData == NULL ) return AMERR_INVALIDFILE;
		MakeDSStreamBuf( &( ssStreamData.wfWaveFormat ), STREAM_BUF_SIZE, &lpDSBStream );	// �o�b�t�@�쐬
		if ( !lpDSBStream && ( ssStreamData.hWavFile != INVALID_HANDLE_VALUE ) ) {
			CloseHandle( hWavFile );
			ssStreamData.hWavFile = INVALID_HANDLE_VALUE;
			return AMERR_SOUNDOUT;
		}
		ssStreamData.nFileRemain = *( DWORD * )( lpWavData + 4 );
		SetFilePointer( hWavFile, nHeaderSize, NULL, FILE_BEGIN );		// �f�[�^�̐擪��
		nReadSize = ( ssStreamData.nFileRemain < STREAM_BUF_SIZE / 2 ) ? ssStreamData.nFileRemain : STREAM_BUF_SIZE / 2;

		char		*lpDSBuf1;
		int			nDSSize1;
		lpDSBStream->Lock( 0, nReadSize, 
							( LPVOID * )&lpDSBuf1, ( LPDWORD )&nDSSize1, NULL, NULL, 0 );
		ReadFile( hWavFile, lpDSBuf1, nReadSize, &dwRead, NULL );	// �f�[�^�ǂݏo��
		if ( nReadSize < STREAM_BUF_SIZE / 2 ) FillMemory( lpDSBuf1 + nReadSize, STREAM_BUF_SIZE / 2 - nReadSize,
														   ssStreamData.cHeadLevel );
		lpDSBStream->Unlock( lpDSBuf1, nDSSize1, NULL, 0 );
		ssStreamData.nFileRemain -= nReadSize;
		if ( ( ssStreamData.nFileRemain == 0 ) && ( ssStreamData.hWavFile != INVALID_HANDLE_VALUE ) ) {
			CloseHandle( hWavFile );								// ̧�ٸ۰��
			ssStreamData.hWavFile = INVALID_HANDLE_VALUE;
		}
		ssStreamData.nBufRemain = nReadSize;
		nStreamLastPos = STREAM_BUF_SIZE / 2 + 1;
		nStreamReadEnd = 0;

		if ( !bStreamThread ) {
			_beginthread( WaitForDSBufEnd, 0, NULL );
			bStreamThread = true;
		}

		dsrval = lpDSBStream->Play( 0, 0, DSBPLAY_LOOPING );	// �Đ��J�n
		if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
	}

	return 0;
}


void WaitForDSBufEnd( LPVOID lpv )						// �o�b�t�@�I���ʒm
{
//    DWORD			dwResult;
	DWORD			nCurrentPlay, nCurrentWrite;
//	static DWORD	nLastPosition = STREAM_BUF_SIZE / 2;
	char			*lpDSBuf1;
	int				nDSSize1;
	int				nReadSize;
	int				nBufferReflesh;
//	static int		bReadEnd = false;
	DWORD			dwRead;
//    DMUS_NOTIFICATION_PMSG	*pPmsg;
//    char			szCount[4];

    while ( TRUE )
    {
		ssStreamData.bProcessing = true;								// ������
		if ( !lpDSBStream || gg_bEndFlag ) {
			bStreamThread = false;
			_endthread();
			break;
		}
		lpDSBStream->GetCurrentPosition( &nCurrentPlay, &nCurrentWrite );
		nBufferReflesh = -1;
		if ( nCurrentPlay >= STREAM_BUF_SIZE / 2 && nStreamLastPos <  STREAM_BUF_SIZE / 2 ) nBufferReflesh = 0;
		if ( nCurrentPlay <  STREAM_BUF_SIZE / 2 && nStreamLastPos >= STREAM_BUF_SIZE / 2 ) nBufferReflesh = STREAM_BUF_SIZE / 2;
		if ( nBufferReflesh >= 0 ) {
			nStreamLastPos = nCurrentPlay;
			if ( !lpDSBStream ) {
				bStreamThread = false;
				_endthread();
				break;
			}
			lpDSBStream->Lock( nBufferReflesh, STREAM_BUF_SIZE / 2,
								( LPVOID * )&lpDSBuf1, ( LPDWORD )&nDSSize1, NULL, NULL, 0 );
			if ( ssStreamData.nFileRemain > 0 ) {
				nReadSize = ( ssStreamData.nFileRemain < STREAM_BUF_SIZE / 2 ) ? ssStreamData.nFileRemain : STREAM_BUF_SIZE / 2;
				ReadFile( ssStreamData.hWavFile, lpDSBuf1, nReadSize, &dwRead, NULL );	// �f�[�^�ǂݏo��
				if ( nReadSize < STREAM_BUF_SIZE / 2 ) FillMemory( lpDSBuf1 + nReadSize, STREAM_BUF_SIZE / 2 - nReadSize,
																   ssStreamData.cHeadLevel );
				ssStreamData.nFileRemain -= nReadSize;
				if ( ( ssStreamData.nFileRemain == 0 ) && ( ssStreamData.hWavFile != INVALID_HANDLE_VALUE ) ) {
					CloseHandle( ssStreamData.hWavFile );	// ̧�ٸ۰��
					ssStreamData.hWavFile = INVALID_HANDLE_VALUE;
				}
				ssStreamData.nBufRemain = nReadSize;
			}
			else {
				FillMemory( lpDSBuf1, STREAM_BUF_SIZE / 2, ssStreamData.cHeadLevel );
				nStreamReadEnd++;
			}
			//if ( !lpDSBStream ) {
			//	bStreamThread = false;
			//	_endthread();
			//	break;
			//}
			if ( !lpDSBStream ) {
				bStreamThread = false;
				_endthread();
				break;
			}
			lpDSBStream->Unlock( lpDSBuf1, nDSSize1, NULL, 0 );
		}
		if ( nCurrentPlay > STREAM_BUF_SIZE / 2 ) nCurrentPlay -= STREAM_BUF_SIZE / 2;
		if ( ( ( ( int )nCurrentPlay >= ssStreamData.nBufRemain ) && ( nStreamReadEnd == 1 ) ) ||
			 ( nStreamReadEnd >= 2 ) )
		{
//        dwResult = WaitForSingleObject( ssStreamData.hNotifyEvent, INFINITE );
			lpDSBStream->Stop();
//			lpDSBStream->Release();
//			lpDSBStream = NULL;
			bStreamThread = false;
			_endthread();
//			CloseHandle( ssStreamData.hNotifyEvent );
			break;
		}
		Sleep( 20 );										// �^�X�N�x�~
		ssStreamData.bProcessing = false;					// �������łȂ���
		Sleep( 90 );										// �^�X�N�x�~
	}
	ssStreamData.bProcessing = false;						// �������łȂ���
}


extern int CheckPcm( void )							// �o�b�l���t���`�F�b�N
{
	DWORD			dwStat;

	if ( lpDSBWork ) {
		lpDSBWork->GetStatus( &dwStat );
		if ( dwStat & DSBSTATUS_PLAYING ) return TRUE;
	}

	if ( lpDSBStream ) {
		lpDSBStream->GetStatus( &dwStat );
		if ( dwStat & DSBSTATUS_PLAYING ) return TRUE;
	}

	return FALSE;
}


extern int StopPcm( void )							// �o�b�l���t��~
{
	HRESULT			dsrval;

	if ( lpDSBWork ) lpDSBWork->Stop();

	if ( lpDSBStream ) {
		if ( bStreamThread ) {
			int nFirstTime;
			nFirstTime = timeGetTime();
			while ( ( bStreamThread && !( ssStreamData.bProcessing ) ) ) {
				if ( !ProcessEvent() ) return AM_OK;
				if ( ( timeGetTime() - nFirstTime ) > 200 ) break;
			}
			nFirstTime = timeGetTime();
			while ( ( bStreamThread &&    ssStreamData.bProcessing   ) ) {
				if ( !ProcessEvent() ) return AM_OK;
				if ( ( timeGetTime() - nFirstTime ) > 200 ) break;
			}
		}
		//while ( ( bStreamThread && !( ssStreamData.bProcessing ) ) && ProcessEvent() );
		//while ( ( bStreamThread &&    ssStreamData.bProcessing   ) && ProcessEvent() );
		dsrval = lpDSBStream->Stop();				// ���t��~
		if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
		dsrval = lpDSBStream->Release();			// �T�E���h�o�b�t�@�J��
		if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
		lpDSBStream = NULL;
		if ( ssStreamData.hWavFile != INVALID_HANDLE_VALUE ) {
			CloseHandle( ssStreamData.hWavFile );	// �t�@�C���N���[�Y
			ssStreamData.hWavFile = INVALID_HANDLE_VALUE;
		}
	}

	return TRUE;
}


static int LoadWavBuffer( char *szWavFile, LPDIRECTSOUNDBUFFER *lplpDSBDest )
{
	int				i;
    HRSRC			hrscr;
	BOOL			bFileFlag;
	int				nWavSize;
	HANDLE			hWavBuff;
	char			*lpWavBuff, *lpWavData;
	HANDLE			hWavFile;
	DWORD			dwRead;
	int				nResult;

	if ( !lpDS ) return AMERR_NOINITIALIZE;			// �������`�F�b�N

	if( ( hrscr = FindResource( NULL, szWavFile, "WAVE" ) ) != NULL ) {
		lpWavBuff = ( char * )LockResource( LoadResource( NULL, hrscr ) );
		bFileFlag = FALSE;
	}
	else {
		hWavFile = CreateFile( szWavFile, GENERIC_READ, FILE_SHARE_READ,	// ̧�ٵ����
							   NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		if ( hWavFile == INVALID_HANDLE_VALUE ) return AMERR_NOFILE;	// �����`�F�b�N

		nWavSize = GetFileSize( hWavFile, NULL );		// �t�@�C���T�C�Y
		hWavBuff = GlobalAlloc( GHND, nWavSize );		// �������m��
		lpWavBuff  = ( char * )GlobalLock( hWavBuff );	// �|�C���^�擾

		ReadFile( hWavFile, lpWavBuff, nWavSize, &dwRead, NULL );	// �f�[�^�ǂݏo��
		CloseHandle( hWavFile );								// ̧�ٸ۰��
		if ( *( DWORD * )lpWavBuff != mmioFOURCC( 'R', 'I', 'F', 'F' ) )
			return AMERR_INVALIDFILE;
		if ( *( DWORD * )( lpWavBuff + 8 ) != mmioFOURCC( 'W', 'A', 'V', 'E' ) )
			return AMERR_INVALIDFILE;

		bFileFlag = TRUE;
	}

	lpWavData = NULL;								// �f�[�^�ʒu�T�[�`
	for ( i = 0; i < WAVE_H_BUF_SIZE - 10; i++ ) {
		if ( ( *( DWORD * )( lpWavBuff + i ) == mmioFOURCC( 'd', 'a', 't', 'a' ) ) &&
			 ( *( DWORD * )( lpWavBuff + i + 4 ) != 0 ) )
		{
			lpWavData = lpWavBuff + i;
			break;
		}
	}
	if ( lpWavData == NULL ) return AMERR_INVALIDFILE;

	if ( *lplpDSBDest ) {
		(*lplpDSBDest)->Stop();
		(*lplpDSBDest)->Release();
		*lplpDSBDest = NULL;
	}

	nResult = MakeDSBuffer( ( LPWAVEFORMATEX )( lpWavBuff + 20 ),
							lpWavData + 8, *( DWORD * )( lpWavData + 4 ), lplpDSBDest );

	if ( bFileFlag ) {
		GlobalUnlock( hWavBuff );					// �������J��
		GlobalFree( hWavBuff );
	}

	return nResult;
}


extern int MakeDSBuffer( LPWAVEFORMATEX lpWaveForm,		// DirectSound�o�b�t�@�쐬
						 char *pWavBuffer, DWORD nBufferSize,
						 LPDIRECTSOUNDBUFFER *lplpDSBDest )
{
	DSBUFFERDESC	dsbd;
	HRESULT			dsrval;
	LPVOID			lpDSBuf1, lpDSBuf2;
	DWORD			nDSSize1, nDSSize2;

	if ( !lpDS ) return AMERR_NOINITIALIZE;			// �������`�F�b�N

	ZeroMemory( &dsbd, sizeof( dsbd ) );			// �T�E���h�o�b�t�@�\���̃Z�b�g
	dsbd.dwSize = BUFFERDESC_SIZE;
	dsbd.dwFlags = DSBCAPS_STATIC;
	dsbd.dwBufferBytes = nBufferSize;				// �f�[�^�o�C�g��
	dsbd.lpwfxFormat = lpWaveForm;					// �t�H�[�}�b�g
	dsrval = lpDS->CreateSoundBuffer( &dsbd, lplpDSBDest, NULL );	// �ޯ̧����
	if ( dsrval != DS_OK ) return AMERR_NOMEMORY;

	( *lplpDSBDest )->Lock( 0, nBufferSize,
							&lpDSBuf1, &nDSSize1, &lpDSBuf2, &nDSSize2, 0 );
	CopyMemory( lpDSBuf1, pWavBuffer, nDSSize1 );
	if ( nDSSize2 != 0 ) {
		CopyMemory( lpDSBuf2, pWavBuffer + nDSSize1,
					nDSSize2 );
	}
	( *lplpDSBDest )->Unlock( lpDSBuf1, nDSSize1, lpDSBuf2, nDSSize2 );

	return 0;
}


extern int MakeDSStreamBuf( LPWAVEFORMATEX lpWaveForm,	// DirectSound�X�g���[�~���O�o�b�t�@�쐬
							int nBufferSize, LPDIRECTSOUNDBUFFER *lplpDSBDest )
{
	DSBUFFERDESC	dsbd;
	HRESULT			dsrval;

	if ( !lpDS ) return AMERR_NOINITIALIZE;			// �������`�F�b�N

	ZeroMemory( &dsbd, sizeof( dsbd ) );			// �T�E���h�o�b�t�@�\���̃Z�b�g
	dsbd.dwSize = BUFFERDESC_SIZE;
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = nBufferSize;				// �f�[�^�o�C�g��
	dsbd.lpwfxFormat = lpWaveForm;					// �t�H�[�}�b�g
	dsrval = lpDS->CreateSoundBuffer( &dsbd, lplpDSBDest, NULL );	// �ޯ̧����
	if ( dsrval != DS_OK ) return AMERR_NOMEMORY;

	return 0;
}


static int MakeSilentBuffer( LPDIRECTSOUNDBUFFER *lplpDSBDest )	// �����o�b�t�@����
{
	DSBUFFERDESC	dsbd;
	HRESULT			dsrval;
	LPVOID			lpDSBuf1, lpDSBuf2;
	DWORD			nDSSize1, nDSSize2;
	WAVEFORMATEX	wfBlank;

	if ( !lpDS ) return AMERR_NOINITIALIZE;

	wfBlank.wFormatTag = 1;								// WAVEFORMATEX�\����
	wfBlank.nChannels = 1;
	wfBlank.nSamplesPerSec = 8000;
	wfBlank.nAvgBytesPerSec = 8000;
	wfBlank.nBlockAlign = 1;
	wfBlank.wBitsPerSample = 8;
	wfBlank.cbSize = 0;

	if ( *lplpDSBDest ) {
		(*lplpDSBDest)->Stop();
		(*lplpDSBDest)->Release();
		*lplpDSBDest = NULL;
	}
	ZeroMemory( &dsbd, sizeof( dsbd ) );			// �T�E���h�o�b�t�@�\���̃Z�b�g
	dsbd.dwSize = BUFFERDESC_SIZE;
	dsbd.dwFlags = DSBCAPS_STATIC;
	dsbd.dwBufferBytes = NOSOUNDSIZE;
	dsbd.lpwfxFormat = &wfBlank;
	dsrval = lpDS->CreateSoundBuffer( &dsbd, lplpDSBDest, NULL );	// �ޯ̧����
	if ( dsrval != DS_OK ) return AMERR_NOMEMORY;

	(*lplpDSBDest)->Lock( 0, NOSOUNDSIZE, &lpDSBuf1, &nDSSize1,
						&lpDSBuf2, &nDSSize2, 0 );
	FillMemory( lpDSBuf1, nDSSize1, 0x80 );			// �����𖞂���
	if ( nDSSize2 != 0 ) {
		FillMemory( lpDSBuf2, nDSSize2, 0x80 );
	}
	(*lplpDSBDest)->Unlock( lpDSBuf1, nDSSize1, lpDSBuf2, nDSSize2 );

	return 0;
}

static int WaitEndWav( LPDIRECTSOUNDBUFFER lpWaitBuf )	// �v�`�u�Đ��I���҂�
{
	int				i;
	BOOL			bEndPlay;
	DWORD			dwStat;
	HRESULT			dsrval;

	if ( !lpDS || !lpWaitBuf ) return AMERR_NOINITIALIZE;
	bEndPlay = 0;
	while( ProcessEvent() ) {						// �Đ����`�F�b�N
		for ( i = 1; i <= 10; i++ ) {
			dsrval = lpWaitBuf->GetStatus( &dwStat );
			if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
			if ( ( dwStat & DSBSTATUS_PLAYING ) == 0 ) {
				bEndPlay = 1;
				break;
			}
		}
		if ( bEndPlay ) break;
	}

	return 0;
}

extern int LoadWavMem( char *szFileName )			// �v�`�u�̃������ւ̃��[�h
{
	int		nResCode;

	if ( !lpDS ) return AMERR_NOINITIALIZE;
	if ( hSoundLoc >= MAXSOUNDNUM ) return AMERR_NOFREEHANDLE;
	if ( ( nResCode = LoadWavBuffer( szFileName, &lpDSBSecondary[nSBufLoc] ) )
		 < 0 ) return nResCode;

    WavInfo[hSoundLoc].nSoundKind = SOUND_WAV;	// �T�E���h�Ǘ��\���̃Z�b�g
    WavInfo[hSoundLoc].nSBufLoc = nSBufLoc;
	
	hSoundLoc++;
	nSBufLoc++;

	return hSoundLoc - 1;
}

extern int PlayMem( int hSound )					// �����������̍Đ�
{
	HRESULT			dsrval;

	if ( !lpDS ) return AMERR_NOINITIALIZE;
	if ( hSound < 0 || hSound >= hSoundLoc ) return AMERR_INVALIDHANDLE;
	if ( WavInfo[ hSound ].nSoundKind != SOUND_WAV ) return AMERR_INVALIDFORM;

	dsrval = lpDSBSecondary[ WavInfo[hSound].nSBufLoc ]->Stop();
	if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
	dsrval = lpDSBSecondary[ WavInfo[hSound].nSBufLoc ]->Play( 0, 0, 0 );
	if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
	WaitEndWav( lpDSBSecondary[ WavInfo[hSound].nSBufLoc ] );

	return 0;
}

extern int PlayMemBack( int hSound )				// �������o�b�N�Đ�
{
	HRESULT			dsrval;

	if ( !lpDS ) return AMERR_NOINITIALIZE;
	if ( hSound < 0 || hSound >= hSoundLoc ) return AMERR_INVALIDHANDLE;
	if ( WavInfo[ hSound ].nSoundKind != SOUND_WAV ) return AMERR_INVALIDFORM;
	
	dsrval = lpDSBSecondary[ WavInfo[hSound].nSBufLoc ]->Stop();	// ��~
	dsrval = lpDSBSecondary[ WavInfo[hSound].nSBufLoc ]->		// �擪��
			 SetCurrentPosition( 0 );
	if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
	dsrval = lpDSBSecondary[ WavInfo[hSound].nSBufLoc ]->Play( 0, 0, 0 );
	if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;

	return 0;
}

extern int PlayMemLoop( int hSound )				// ���������[�v�Đ�
{
	HRESULT			dsrval;

	if ( !lpDS ) return AMERR_NOINITIALIZE;
	if ( hSound < 0 || hSound >= hSoundLoc ) return AMERR_INVALIDHANDLE;
	if ( WavInfo[ hSound ].nSoundKind != SOUND_WAV ) return AMERR_INVALIDFORM;
	
	dsrval = lpDSBSecondary[ WavInfo[hSound].nSBufLoc ]->		// �擪��
			 SetCurrentPosition( 0 );
	if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
	dsrval = lpDSBSecondary[ WavInfo[hSound].nSBufLoc ]->Play( 0, 0, DSBPLAY_LOOPING );
	if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;

	return 0;
}

extern int CheckPlayMem( int hSound )				// ���������t���`�F�b�N
{
	HRESULT			dsrval;
	DWORD			dwStat;

	if ( !lpDS ) return FALSE;
	if ( hSound < 0 || hSound >= hSoundLoc ) return AMERR_INVALIDHANDLE;
	if ( WavInfo[ hSound ].nSoundKind == SOUND_WAV ) {
		dsrval = lpDSBSecondary[ WavInfo[hSound].nSBufLoc ]->GetStatus( &dwStat );
		if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
		if ( dwStat & DSBSTATUS_PLAYING ) return TRUE;
									 else return FALSE;
	}
	return FALSE;
}

extern int StopPlayMem( int hSound )				// ���������t�X�g�b�v
{
	HRESULT			dsrval;

	if ( !lpDS ) return AMERR_NOINITIALIZE;
	if ( hSound < 0 || hSound >= hSoundLoc ) return AMERR_INVALIDHANDLE;
	if ( WavInfo[ hSound ].nSoundKind == SOUND_WAV ) {
		dsrval = lpDSBSecondary[ WavInfo[hSound].nSBufLoc ]->Stop();
		if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
		dsrval = lpDSBSecondary[ WavInfo[hSound].nSBufLoc ]->
				 SetCurrentPosition( 0 );
		if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
	}
	return 0;
}

extern int SetPcmVolume( int nVolume )				// �o�b�l�{���E���̃Z�b�g
{
	if ( !lpDSBPrimary ) return AMERR_NOINITIALIZE;
	if ( nVolume < 0 || nVolume > 100 ) return AMERR_INVALIDVALUE;

	if ( nVolume >= 50 )
		nVolume = - ( 100 - nVolume ) * 20;
	else
		nVolume = -50 * 20 - ( 50 - nVolume ) * 180;
	if ( lpDSBPrimary->SetVolume( nVolume ) == DS_OK )
		return 0;
	else
		return AMERR_SOUNDOUT;
}


extern int SetMemVolume( int nVolume, int hSound )	// �������o�b�l�{���E���̃Z�b�g
{
	HRESULT			dsrval;
	long			nDSVolume;						// DirectSound�{���E��
	long			nChkVolume;

	if ( !lpDS ) return AMERR_NOINITIALIZE;
	if ( hSound < 0 || hSound >= hSoundLoc ) return AMERR_INVALIDHANDLE;
	if ( nVolume < 0 || nVolume > 100 ) return AMERR_INVALIDVALUE;

	if ( WavInfo[ hSound ].nSoundKind == SOUND_WAV ) {
		nDSVolume = ( DSBVOLUME_MAX - DSBVOLUME_MIN ) * nVolume / 100 + DSBVOLUME_MIN;
		nChkVolume = DSERR_CONTROLUNAVAIL;
		dsrval = lpDSBSecondary[ WavInfo[hSound].nSBufLoc ]->SetVolume( nDSVolume );
		if ( dsrval != DS_OK ) {
			if ( dsrval == DSERR_CONTROLUNAVAIL ) return AMERR_CANNOT;
											 else return AMERR_SOUNDOUT;
		}
	}

	return AM_OK;
}


extern int PlayMidi( char *szFileName )				// MIDI�t�@�C���̉��t
{
	DWORD				dwReturn;

	if ( !lpDS ) return AMERR_NOINITIALIZE;
	if ( gg_bEndFlag ) return 0;
	if ( bMidiPlaying ) StopMusic();

	if ( ( dwReturn = StartMciMidi( szFileName ) ) ) {
		return dwReturn;
	}

	bMidiLooping = FALSE;
	while( ProcessEvent() && bMidiPlaying );

	return 0;
}

extern int PlayMidiBack( char *szFileName )			// MIDI�t�@�C���̃o�b�N���t
{
	DWORD				dwReturn;

	if ( !lpDS ) return AMERR_NOINITIALIZE;
	if ( gg_bEndFlag ) return 0;
	if ( bMidiPlaying ) StopMusic();

	if ( ( dwReturn = StartMciMidi( szFileName ) ) ) {
		return dwReturn;
	}
	bMidiLooping = FALSE;

	return 0;
}

extern int PlayMidiLoop( char *szFileName )			// MIDI�t�@�C���̃��[�v���t
{
	DWORD				dwReturn;

	if ( !lpDS ) return AMERR_NOINITIALIZE;
	if ( gg_bEndFlag ) return 0;
	if ( bMidiPlaying ) StopMusic();

	if ( ( dwReturn = StartMciMidi( szFileName ) ) ) {
		return dwReturn;
	}
	bMidiLooping = TRUE;

	return 0;
}


static int StartMciMidi( char *szFileName )			// MCI MIDI�Đ�
{
	DWORD				dwReturn;
	MCI_OPEN_PARMS		mciOpenParms;
	MCI_PLAY_PARMS		mciPlayParms;
	MCI_STATUS_PARMS	mciStatusParms;
	char				szMidiName[100];

	mciOpenParms.lpstrDeviceType = "sequencer";
	MakeFullPath( szMidiName, szFileName );
	mciOpenParms.lpstrElementName = szMidiName;
	if ( dwReturn = mciSendCommand( NULL, MCI_OPEN,
		 MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR) &mciOpenParms ) ) {
		return AMERR_NOFILE;
	}

	wMidiID = mciOpenParms.wDeviceID;

	mciStatusParms.dwItem = MCI_SEQ_STATUS_PORT;
	if ( dwReturn = mciSendCommand( wMidiID, MCI_STATUS,
		 MCI_STATUS_ITEM, (DWORD_PTR) &mciStatusParms ) ) {
		mciSendCommand( wMidiID, MCI_CLOSE, 0, NULL );
		return AMERR_SOUNDOUT;
	}

	if ( LOWORD( mciStatusParms.dwReturn ) != LOWORD( MIDI_MAPPER ) )
		return AMERR_NODRIVER;
	mciPlayParms.dwCallback = (DWORD_PTR) gg_hWindow;
	if ( dwReturn = mciSendCommand( wMidiID, MCI_PLAY, MCI_NOTIFY,
		 (DWORD_PTR) &mciPlayParms ) ) {
		mciSendCommand( wMidiID, MCI_CLOSE, 0, NULL );
		return AMERR_SOUNDOUT;
	}

	bMidiPlaying = TRUE;

	return 0;
}

extern void ProcessMciNortify( void )
{
	MCI_PLAY_PARMS		mciPlayParms;

	if ( bMidiLooping ) {
		mciPlayParms.dwCallback = (DWORD_PTR) gg_hWindow;
		mciPlayParms.dwFrom = 0;
		mciSendCommand( wMidiID, MCI_PLAY, MCI_NOTIFY | MCI_FROM,
						(DWORD_PTR) &mciPlayParms );
		bMidiPlaying = TRUE;
	}
	else {
		if ( bMidiPlaying )
			mciSendCommand( wMidiID, MCI_CLOSE, 0, NULL );
		bMidiPlaying = FALSE;
	}
}

extern int MakeFullPath( char *szDest, char *szSrc )
{
	int		i, nSrcLen;
	char	*cSearch;
	BOOL	bCurrent, bValidCh;

	cSearch = szSrc;
	nSrcLen = lstrlen( szSrc );
	bCurrent = TRUE;  bValidCh = FALSE;
	for ( i = 0; i < nSrcLen; i++ ) {
		if ( ( *cSearch == '\\' && !bValidCh )
		    || *cSearch == ':' ) {
			bCurrent = FALSE;
			break;
		}
		if ( *cSearch != ' ' ) bValidCh = TRUE;
		cSearch++;
	}
	if ( bCurrent ) {
		GetCurrentDirectory( 100, szDest );
		lstrcat( szDest, "\\" );
		lstrcat( szDest, szSrc );
	}
	else
		lstrcpy( szDest, szSrc );

	return bCurrent;
}

extern int CheckMusic( void )						// �l�h�c�h���t���`�F�b�N
{
	return bMidiPlaying;
}

extern int StopMusic( void )						// �l�h�c�h���t�X�g�b�v
{
	int		i;

	if ( bMidiPlaying ) {
		bMidiLooping = FALSE;
		mciSendCommand( wMidiID, MCI_CLOSE, 0, NULL );
		bMidiPlaying = FALSE;
		for ( i = 1; i <= 5; i++ ) ProcessEvent();
	}

	return 0;
}

extern int SetMusicVolume( DWORD nVolume )			// �l�h�c�h���ʃZ�b�g
{
	HMIDIOUT	hMidi;
	DWORD		nOutVol;
	int			ResCode;

	if ( !lpDS ) return AMERR_NOINITIALIZE;
	if ( nVolume < 0 || nVolume > 100 ) return AMERR_INVALIDVALUE;
	nVolume = nVolume * 0xffff / 100;
	if ( bMidiPlaying ) return AMERR_CANNOT;
	if ( ( ResCode = midiOutOpen( &hMidi, MIDI_MAPPER, NULL, NULL, CALLBACK_NULL ) )
		 == MMSYSERR_NOERROR ) {
		nOutVol = ( nVolume << 16 ) | nVolume;
		ResCode = midiOutSetVolume( hMidi, nOutVol );
		midiOutClose( hMidi );
		if ( ResCode != MMSYSERR_NOERROR ) return AMERR_SOUNDOUT;
	}
	else return AMERR_SOUNDOUT;

	return 0;
}
