// ---------------------------------------------------------------------------
//
// ファイル名: AM_DispEnv.cpp
//
// 機能: DirectX上でのＡＭＧＡＭＥの画面環境設定を実装。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#pragma warning( disable : 4819 )						// コードページの警告抑止
#define _CRT_SECURE_NO_WARNINGS							// セキュリティ警告抑止(_vsnprintf、sscanf、_snprintf、sprintf使用のため)

// ---------------------------------------------------------------------------
// インクルード域

#include <stdio.h>
#include <stdarg.h>

#include <d3d9.h>												// D3D9オブジェクト
#include <d3dx9.h>												// D3D9Xオブジェクト

#include "amgame.h"												// AMGAMEヘッダ

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnvヘッダ
#include "AM_Input.h"											// ユーザ入力　機種依存レイヤー
#include "AB_Input.h"											// ユーザ入力　ブリッジ
#include "AM_Sound.h"											// サウンド　機種依存レイヤー
#include "AM_Movie.h"											// ムービー　機種依存レイヤー

#endif

// DirectX用旧関数エイリアス定義
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;
int (WINAPIV * _sscanf)(const char*, const char*, ...) = sscanf;
int (WINAPIV * __snprintf)(char *, size_t, const char*, ...) = _snprintf;
int (WINAPIV * _sprintf)(char*, const char*, ...) = sprintf;

// ---------------------------------------------------------------------------
// 内部使用定数定義域

#define WIN_CLASS_NAME			"AMWindow"						// ウィンドウクラス名
#define WIN_TITLE				"AMGAME Library version 3.20f"	// ウィンドウタイトル

//#define DISPENV_DEBUG_MODE										// デバッグモードか

#define AM_MIN_DISPLAY_WIDTH	320								// 最小ディスプレイ幅
#define AM_MIN_DISPLAY_HEIGHT	240								// 最小ディスプレイ高さ
#define AM_MAX_DISPLAY_WIDTH	1600							// 最大ディスプレイ幅
#define AM_MAX_DISPLAY_HEIGHT	1200							// 最大ディスプレイ高さ

#define AM_MAX_EVENTS			10								// 最大一括処理イベント数


// ---------------------------------------------------------------------------
// インターファイルグローバル変数域

// Window関係オブジェクト
HWND							gg_hWindow		= NULL;			// ウィンドウハンドル
HINSTANCE						gg_hInstance	= NULL;			// インスタンスハンドル

// DirectX関係オブジェクト
AM_LPDIRECT3D					gg_pD3D			= NULL;			// D3Dオブジェクト
AM_LPDIRECT3DDEVICE				gg_pD3DDevice	= NULL;			// D3DDeviceオブジェクト
AM_LPDISPLAYBUFFER				gg_pBackBuffer = NULL;				// BackBufferオブジェクト

// 画面環境保持変数
SIZE							gg_siDispSize	= { 640, 480 };	// クライアント領域の大きさ
SIZE							gg_siWholeDispSize	= { 640, 480 };	// 画面全体の大きさ
int								gg_nBitDepth	= 32;			// ビットデプス
BOOL							gg_bFullMode	= true;			// フルスクリーンモードか
POINT							gg_piDispLocate = { 0, 0 };		// クライアント領域の位置
D3DFORMAT						gg_dfDisplayFormat = D3DFMT_X8R8G8B8;	// ディスプレイフォーマット
BOOL							gg_bDispInitialized = false;	// 画面初期化済みか
BOOL							gg_bSceneActive = false;		// シーンはアクティブか
char							gg_szWinTitle[AM_MAX_WIN_TITLE] = WIN_TITLE;	// ウィンドウタイトル

// プロセス制御変数
BOOL							gg_bEndFlag = false;			// 終了フラグ


// ---------------------------------------------------------------------------
// ファイル内グローバル変数域

//AM_LPVERTEXBUFFER				gg_pVB			= NULL;			// Buffer to hold vertices
//AM_LP2DPICTUREBUFFER			fg_ppbPicture	= NULL;			// Buffer to hold 2D Picture

BOOL							fg_bWinClass_Reg	= false;	// ウィンドウクラスレジスタフラグ
BOOL							fg_bCloseFlag	= false;		// クローズフラグ


// ---------------------------------------------------------------------------
// ログ変数域

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
// 外部公開関数プロトタイプ宣言域
extern int AM_InitDisplay( void );								// ディスプレイ初期化
extern int AM_EndDisplay( void );								// ディスプレイ使用終了


// ---------------------------------------------------------------------------
// 内部関数プロトタイプ宣言域

// ウィンドウ制御
static HWND AM_InitWindow( HINSTANCE hInstance );				// 通常ウィンドウ初期化
static int AM_EndWindow( void );								// ウィンドウ使用終了

// Direct3D制御
static int AM_InitD3D( HWND hWnd );								// Direct3D初期化
static int AM_DecideDisplayForm( void );						// 画面ピクセルフォーマット決定
static VOID AM_EndD3D( void );									// Direct3D使用終了

LRESULT WINAPI AM_WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );


//-----------------------------------------------------------------------------
// 統合画面制御関数域

// ディスプレイ初期化
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

	gg_bDispInitialized = true;									// 初期化済み

	return AM_OK;
}


// ディスプレイ使用終了
extern int AM_EndDisplay( void )
{
    AM_EndD3D();												// Direct3D使用終了
	AM_EndWindow();												// Window使用終了
#ifdef DISPENV_DEBUG_MODE
	if ( fpLog ) {
		fclose( fpLog );
		fpLog = NULL;
	}
#endif
	return AM_OK;
}


// アプリケーションインスタンスの設定
// hInstance : アプリインスタンス
extern int AM_SetAppliInstance( HINSTANCE hInstance )
{
	gg_hInstance = hInstance;

	return AM_OK;
}


// 解像度の設定
// nWidth : ディスプレイ幅　nHeight : ディスプレイ高さ　nBitDepth : ビットデプス
extern int AM_SetDisplayMode( int nWidth, int nHeight, int nBitDepth )
{
	int					nResultCode = AM_OK;

	if ( gg_pD3DDevice ) return AMERR_CANNOT;					// 既にD3Dデバイスがあれば設定不可

	// 解像度設定
	if ( ( nWidth  >= AM_MIN_DISPLAY_WIDTH  && nWidth  <= AM_MAX_DISPLAY_WIDTH  ) &&
		 ( nHeight >= AM_MIN_DISPLAY_HEIGHT && nHeight <= AM_MAX_DISPLAY_HEIGHT ) )
	{
		gg_siDispSize.cx = nWidth;  gg_siDispSize.cy = nHeight;
	}
	else {
		nResultCode = AMERR_CANNOT;
	}

	// ビットデプス設定
	if ( nBitDepth == 16 || nBitDepth == 32 ) {
		if ( gg_bFullMode ) gg_nBitDepth = nBitDepth;			// フルスクリーンならビットデプスセット
	}
	else {
		nResultCode = AMERR_CANNOT;
	}

	return nResultCode;
}


//-----------------------------------------------------------------------------
// ウィンドウ制御関数域

// 通常ウィンドウ表示
// Windowsの通常のウィンドウを表示する。
// hInstance : アプリケーションインスタンス
static HWND AM_InitWindow( HINSTANCE hInstance )
{
//	int					i;
    WNDCLASSEX			wc;
	HWND                hwnd;
	RECT				reClient;
	RECT				reWinLoc;						// ウィンドウ位置

	// ウィンドウクラス構造体の設定
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

    RegisterClassEx( &wc );                // ウィンドウクラスの登録
	fg_bWinClass_Reg = true;

    if ( gg_bFullMode ) 
		hwnd = CreateWindowEx(               // ウィンドウの生成
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
		hwnd = CreateWindowEx(						// ウィンドウの生成
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
    UpdateWindow( hwnd );							// ウィンドウの表示
    SetFocus( hwnd );								// フォーカスのセット
//	hMainWnd = hwnd;
//	hInst = hInstance;
/*	fg_bMoviePlaying = FALSE;							// ＡＶＩ初期化
	nMovieFirst = -1;  nMovieLast = -1;
	for ( i = 0; i < AM_MAXMOVIENUM; i++ )			// ＡＶＩバッファ初期化
		ZeroMemory( &MovieBuf[i], sizeof( MovieBuf[i] ) );

	AmInitJoypad( JOYSTICKID1, &Pad1 );				// パッド１初期化
	AmInitJoypad( JOYSTICKID2, &Pad2 );				// パッド２初期化*/

	return( hwnd );

}

// ウィンドウ使用終了
static int AM_EndWindow( void )
{
	if ( fg_bWinClass_Reg ) {
		UnregisterClass( WIN_CLASS_NAME, gg_hInstance );
		fg_bWinClass_Reg = false;
	}

	return AM_OK;
}


// Windowsイベント処理
extern int AM_ProcessEvent( void )
{
	int					nEventCnt;
    MSG					msg;

//	if ( bLockFlag ) return( FALSE );
	if ( gg_bEndFlag ) return false;							// すでに終了か

	nEventCnt = 0;
	while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) {
		if ( !GetMessage( &msg, NULL, 0, 0 ) ) {
			gg_bEndFlag = true;
//			StopPcm();											// 音楽停止
			return false;
		}
		TranslateMessage( &msg );
        DispatchMessage( &msg );
		nEventCnt++;
		if ( nEventCnt >= AM_MAX_EVENTS ) break;
	}
	return true;
}


// Windowsウィンドウプロシージャ
LRESULT WINAPI AM_WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
	// 初期処理
	case WM_ACTIVATEAPP:
		if ( !gg_bMouseCursor ) SetCursor( NULL );
		break;

	// マウスカーソルセット
    case WM_SETCURSOR:
		if ( gg_bFullMode && !gg_bMouseCursor ) {
			SetCursor( NULL );
			return TRUE;
		}
		break;

	// マウス入力
	case WM_LBUTTONDOWN:							// マウス左ボタン
		if ( gg_bMouseActive ) {
			AM_SetKeyPushed( MOUSECODE_LEFT );
//			bPushFlag = TRUE;
//			wKeyCode = MOUSECODE_LEFT;
		}
		break;

	case WM_RBUTTONDOWN:							// マウス右ボタン
		if ( gg_bMouseActive ) {
			AM_SetKeyPushed( MOUSECODE_RIGHT );
//			bPushFlag = TRUE;
//			wKeyCode = MOUSECODE_RIGHT;
		}
		break;

	case WM_MBUTTONDOWN:							// マウス中央ボタン
		if ( gg_bMouseActive ) {
			AM_SetKeyPushed( MOUSECODE_MIDDLE );
//			bPushFlag = TRUE;
//			wKeyCode = MOUSECODE_MIDDLE;
		}
		break;

	// キーボード入力
	case WM_CHAR:
		AM_SetKeyPushed( ( int )wParam );
		break;

/*	// 強制終了チェック
    case WM_KEYDOWN:
        if( wParam == VK_F12 && !fg_bCloseFlag ) {
		    fg_bCloseFlag = TRUE;
            PostMessage( hWnd, WM_CLOSE, 0, 0 );
//			wKeyCode = 0;
	    }
		break;*/

	case MM_MCINOTIFY:
		if ( lParam == (long)gg_wAVIDevice ) {					// ＡＶＩ
//			AM_StopMovie();
		}
		else {													// ＭＩＤＩ
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
// Direct3D制御関数域

// DirectGraph初期化
static int AM_InitD3D( HWND hWnd )
{
	D3DPRESENT_PARAMETERS		d3dpp;

	// D3Dオブジェクト作成
	if( !( gg_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return AMERR_DISPLAY;

	// ピクセルフォーマット決定
	AM_DecideDisplayForm();

    // D3DDeviceを作るための構造体セット
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.BackBufferWidth  = gg_siDispSize.cx;					// 画面の幅
	d3dpp.BackBufferHeight = gg_siDispSize.cy;					// 画面の高さ
	d3dpp.BackBufferFormat = gg_dfDisplayFormat;				// ピクセルフォーマット
	d3dpp.BackBufferCount  = 1;									// バックバッファ数（フロント含まず）
	d3dpp.MultiSampleType  = D3DMULTISAMPLE_NONE;				// マルチサンプル（アンチエリアシング？）
	d3dpp.SwapEffect       = D3DSWAPEFFECT_DISCARD; //D3DSWAPEFFECT_FLIP;	// 画面切り替えモード
	d3dpp.hDeviceWindow    = NULL;								// ターゲットウィンドウ
	d3dpp.Windowed         = gg_bFullMode ? FALSE : TRUE;		// ウィンドウモードか
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファフォーマット
	d3dpp.Flags            = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER; 	// フラグ
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	d3dpp.PresentationInterval = gg_bFullMode ? D3DPRESENT_INTERVAL_DEFAULT
											  : D3DPRESENT_INTERVAL_ONE;	// 表示周期

    // D3DDeviceの作成
    if ( FAILED( gg_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&d3dpp, &gg_pD3DDevice ) ) )
    {
		return AMERR_DISPLAY;
    }

    // Ｚバッファの有効化
    gg_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

    // カリングのOFF
    gg_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

/*	// スプライトの作成
	if ( FAILED( D3DXCreateSprite( g_pd3dDevice, &spSprite ) ) )
	{
		return LBERR_INITIALIZE;
	}*/

	// バックバッファ取得
	gg_pD3DDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &gg_pBackBuffer );

	// バックバッファクリア
//    gg_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
	AM_ClearBackBuffer( 0, 0, 0 );

	// 画面全体の大きさ設定
	if ( gg_bFullMode ) {
		gg_siWholeDispSize = gg_siDispSize;						// フルスクリーンならそのまま
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


// 画面ピクセルフォーマット決定
static int AM_DecideDisplayForm( void )
{
//	unsigned int		i;
	unsigned int		nDispModeNum;
	D3DDISPLAYMODE		D3DispMode;

	if( !gg_pD3D ) return AMERR_NOINITIALIZE;					// 初期化済みか

	// ピクセルフォーマット決定
	if ( gg_bFullMode ) {										// フルスクリーンの場合
		switch( gg_nBitDepth ) {
		case 32:												// ３２ビットモード
			nDispModeNum = gg_pD3D->GetAdapterModeCount( D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8 );
			if ( nDispModeNum ) {								// X8R8G8B8あり
				#ifdef DISPENV_DEBUG_MODE
					fprintf( fpLog, "Pixel format = D3DFMT_X8R8G8B8\n" );
				#endif
				gg_dfDisplayFormat = D3DFMT_X8R8G8B8;
			}
			else {												// X8R8G8B8なし
				#ifdef DISPENV_DEBUG_MODE
					fprintf( fpLog, "32-bit pixel format decision failed!\n" );
				#endif
			}
			break;

		case 16:												// １６ビットモード
			nDispModeNum = gg_pD3D->GetAdapterModeCount( D3DADAPTER_DEFAULT, D3DFMT_R5G6B5 );
			if ( nDispModeNum ) {								// R5G6B5あり
				#ifdef DISPENV_DEBUG_MODE
					fprintf( fpLog, "Pixel format = D3DFMT_R5G6B5\n" );
				#endif
				gg_dfDisplayFormat = D3DFMT_R5G6B5;
			}
			else {												// R5G6B5なし
				nDispModeNum = gg_pD3D->GetAdapterModeCount( D3DADAPTER_DEFAULT, D3DFMT_X1R5G5B5 );
				if ( nDispModeNum ) {							// X1R5G5B5あり
				#ifdef DISPENV_DEBUG_MODE
					fprintf( fpLog, "Pixel format = D3DFMT_X1R5G5B5\n" );
				#endif
					gg_dfDisplayFormat = D3DFMT_X1R5G5B5;
				}
			}
			break;
		}
	}
	else {														// ウィンドウモードの場合
	    if ( FAILED( gg_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &D3DispMode ) ) )
			return AMERR_CANNOT;
		gg_dfDisplayFormat = D3DispMode.Format;					// フォーマット格納

		// ビットデプスセット
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


// Direct3D使用終了
VOID AM_EndD3D( void )
{
	// シーン終了処理
	if ( gg_bSceneActive ) {
		gg_pD3DDevice->EndScene();								// シーン終了
		gg_bSceneActive = false;
	}

	AM_SAFE_RELEASE( gg_pBackBuffer );							// バックバッファ
	AM_SAFE_RELEASE( gg_pD3DDevice );							// D3Dデバイス

	AM_SAFE_RELEASE( gg_pD3D );									// D3D
}


// 画面の更新
extern int AM_Flip( void )
{
	D3DRASTER_STATUS			rsRasterData;

	if ( gg_bEndFlag ) return AMERR_ENDED;						// 終了か

    // End the scene
    gg_pD3DDevice->EndScene();
	gg_bSceneActive = false;

	// ウィンドウモードならVBlank待ち
	if ( !gg_bFullMode ) {
		do {													// VBlankになるまで待つ
			if ( !AM_ProcessEvent() ) break;					// とりあえずイベント処理
			gg_pD3DDevice->GetRasterStatus( 0, &rsRasterData );
		} while ( !rsRasterData.InVBlank );
	}

	// 終わってなければ表示
	if ( !gg_bEndFlag ) {
		gg_pD3DDevice->Present( NULL, NULL, NULL, NULL );		// バックバッファの内容を表示
		gg_pD3DDevice->BeginScene();							// シーン開始
		gg_bSceneActive = true;
	}
	else return AMERR_ENDED;

	return AM_OK;
}


// VBlank信号のトリガを待つ
extern int AM_WaitVBlankTrigger( void )
{
	D3DRASTER_STATUS			rsRasterData;

	if ( gg_bEndFlag ) return AMERR_ENDED;						// 終了か

	// VBlankでなくなるまで待つ
	do {
		if ( !AM_ProcessEvent() ) break;						// とりあえずイベント処理
		gg_pD3DDevice->GetRasterStatus( 0, &rsRasterData );
	} while ( rsRasterData.InVBlank );

	// VBlankになるまで待つ
	do {
		if ( !AM_ProcessEvent() ) break;						// とりあえずイベント処理
		gg_pD3DDevice->GetRasterStatus( 0, &rsRasterData );
	} while ( !rsRasterData.InVBlank );

	return AM_OK;
}


// バックバッファのクリア
extern int AM_ClearBackBuffer( int r, int g, int b )
{
	// 画面・デプスバッファクリア
//    gg_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
	gg_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						  D3DCOLOR_XRGB( r, g, b ), 1.0f, 0 );

	return AM_OK;
}


// ビューポートの設定
extern int AM_SetViewport( int x1, int y1, int x2, int y2 )
{
	D3DVIEWPORT9				dvViewport;

	if( !gg_pD3DDevice ) return AMERR_NOINITIALIZE;				// 初期化済みか

	// パラメータ設定
	dvViewport.X = x1;  dvViewport.Y = y1;						// 左上
	dvViewport.Width = x2 - x1;  dvViewport.Height = y2 - y1;	// 幅・高さ
	dvViewport.MinZ = 0.0f;  dvViewport.MaxZ = 1.0f;			// Zの範囲

	// ビューポート設定
	gg_pD3DDevice->SetViewport( &dvViewport );

	return AM_OK;
}


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AM_2DGraph.cpp
//
// 機能: DirectX上でのＡＭＧＡＭＥの２Ｄ画面描画機能を実装。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#pragma warning( disable : 4819 )						// コードページの警告抑止


// ---------------------------------------------------------------------------
// インクルード域

#include <d3d9.h>
#include <d3dx9.h>

#include "amgame.h"												// AMGAMEヘッダ

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnvヘッダ
#include "AM_2DGraph.h"											// AM2DGraphヘッダ
#include "AB_2DGraph.h"											// AB2DGraphヘッダ

#endif


// ---------------------------------------------------------------------------
// 内部使用定数定義域
//int								gg_nBlendMode = AMDRAW_NOBLEND;			// 現在のブレンドモード
//int								gg_nBlendFactor = AM_MAX_BLEND_FACTOR;	// 現在のブレンドファクタ


// ---------------------------------------------------------------------------
// インターファイルグローバル変数域

AM_BLEND_STATE					gg_bsBlendState;				// 現在のブレンドステート

// 描画矩形用頂点
extern AM_2DVERTEX				gg_v2DrawRect[4] = { {   0.0f,   0.0f, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f },
													 { 640.0f,   0.0f, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f },
													 {   0.0f, 480.0f, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f },
													 { 640.0f, 480.0f, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f } };


// ---------------------------------------------------------------------------
// ファイル内グローバル変数域

//AM_LP2DPICTUREBUFFER			fg_ppbPicture	= NULL;			// Buffer to hold 2D Picture


// ---------------------------------------------------------------------------
// 内部関数プロトタイプ宣言域


static int AM_Get2ExpNumber( int nSource );						// 元の数値以上の２のｎ乗の数値にする
static int AM_Calc2DUVCoord( float *du1, float *dv1, float *du2, float *dv2,
							 int sx1, int sy1, int sx2, int sy2, 
							 AM_2DPIC_INFO *pPicInfo );
																// 2Dグラフィック　テクスチャ座標計算
static int AM_SetBlendFactor( int nBlendFactor );				// ブレンドファクタセット


//-----------------------------------------------------------------------------
// 2D画像ロード関数域

// 2Dグラフィックロード
extern AM_LP2DPICTUREBUFFER AM_Load2DGraph( char *szFileName, AM_2DPIC_INFO *pPicInfo )
{
	AM_LP2DPICTUREBUFFER		p2dGraph = NULL;				// ロードテクスチャ
	HRESULT						hRes;
	D3DXIMAGE_INFO				iiImageInfo;
	LPDIRECT3DSURFACE9			lpTopSurf;
	D3DSURFACE_DESC				ddSurfInfo;
	D3DFORMAT					dfTexFormat;					// テクスチャフォーマット
	D3DCOLOR					nColorKey;						// カラーキー
//	D3DSURFACE_DESC				ddSurfInfo;

	// ピクセルフォーマット
	dfTexFormat = ( gg_nBitDepth == 32 ) ? D3DFMT_A8R8G8B8 : D3DFMT_A1R5G5B5;

	// カラーキー処理
	if ( strstr( szFileName, ".bmp" ) || strstr( szFileName, ".BMP" ) ) {	// ビットマップの場合
		nColorKey = 0xff000000;												// ビットマップならカラーキーあり
	}
	else {
		nColorKey = 0;														// ビットマップ以外ならカラーキーなし
	}

	// テクスチャロード
	hRes = D3DXCreateTextureFromFileEx( gg_pD3DDevice, ( LPCSTR )szFileName, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0,
										dfTexFormat, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
										nColorKey, &iiImageInfo, NULL,
										&p2dGraph );
//    hRes = D3DXCreateTextureFromFile( gg_pD3DDevice, ( LPCSTR )szFileName, &p2dGraph );

	if ( FAILED( hRes ) ) return NULL;

	// サーフェス情報取得
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


// 2Dグラフィックバッファ作成
extern AM_LP2DPICTUREBUFFER AM_Make2DGraphBuf( AM_2DPIC_INFO *pPicInfo )
{
	AM_LP2DPICTUREBUFFER		p2dGraph = NULL;				// ロードテクスチャ
	HRESULT						hRes;
	D3DFORMAT					dfTexFormat;					// テクスチャフォーマット

	dfTexFormat = ( pPicInfo->nBitDepth == 32 ) ? D3DFMT_A8R8G8B8 : D3DFMT_A1R5G5B5;

	// 実数値セット
	pPicInfo->nRealWidth  = AM_Get2ExpNumber( pPicInfo->nWidth  );
	pPicInfo->nRealHeight = AM_Get2ExpNumber( pPicInfo->nHeight );
	pPicInfo->nRealDepth = pPicInfo->nBitDepth;

	// テクスチャ作成
	hRes = D3DXCreateTexture( gg_pD3DDevice, pPicInfo->nRealWidth, pPicInfo->nRealHeight,
							  1, 0, dfTexFormat, D3DPOOL_MANAGED, &p2dGraph );
	if ( FAILED( hRes ) ) return NULL;

	return p2dGraph;
}


// 2Dグラフィック再ロード
extern int AM_Reload2DGraph( char *szFileName, AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo )
{
//	AM_LP2DPICTUREBUFFER		p2dGraph = NULL;				// ロードテクスチャ
	HRESULT						hRes;
//	D3DXIMAGE_INFO				iiImageInfo;
	LPDIRECT3DSURFACE9			lpTopSurf;
//	D3DSURFACE_DESC				ddSurfInfo;
//	D3DFORMAT					dfTexFormat;					// テクスチャフォーマット
//	D3DSURFACE_DESC				ddSurfInfo;
	RECT						rcDest;							// 転送先矩形

	// サーフェス取得
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


// 元の数値以上の２のｎ乗の数値にする
static int AM_Get2ExpNumber( int nSource )
{
	int							i;
	unsigned int				nUnsignedSrc;					// 符号なし元数値
//	unsigned int				nBitScale;						// ビットスケール
	int							bBit1 = false;					// 1ビットがあるか
	int							nResult;						// 結果

	if ( nSource == 0 ) return 0;
	nUnsignedSrc = ( unsigned int )nSource;

	for ( i = 0; i < 32; i++ ) {
		if ( nUnsignedSrc != 1 ) {								// 数値が最終（１）になっていない
			if ( nUnsignedSrc & 1 ) bBit1 = true;
		}
		else {													// 数値が１になっている
			nResult = ( bBit1 ) ? ( 1 << ( i + 1 ) ) : nSource;
		}
		nUnsignedSrc = nUnsignedSrc >> 1;
	}

	return nResult;
}


//-----------------------------------------------------------------------------
// 2D画像描画関数域

// 2Dグラフィック描画（描画先２点、描画元２点指定）
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


// 2Dグラフィック描画（描画先４点、描画元２点指定）
extern int AM_Draw2DGraph4_2( AM_2DVECTOR *pDestVerts,
							  int sx1, int sy1, int sx2, int sy2, 
							  AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo )
{
	int							i;
	float						tu1, tv1, tu2, tv2;

	AM_Calc2DUVCoord( &tu1, &tv1, &tu2, &tv2, sx1, sy1, sx2, sy2, pPicInfo );
//	tu1  = ( float )( sx1 + 0.5f ) / pPicInfo->nRealWidth;  tv1  = ( float )( sy1 + 0.5f ) / pPicInfo->nRealHeight;
//	tu2  = ( float )( sx2  /* - 0.5f */ ) / pPicInfo->nRealWidth;  tv2  = ( float )( sy2 /*- 0.5f*/ ) / pPicInfo->nRealHeight;

	// 頂点座標
	for ( i = 0; i < 4; i++ ) {
		gg_v2DrawRect[i].x = ( pDestVerts + i )->x;  gg_v2DrawRect[i].y = ( pDestVerts + i )->y;
	}
	// テクスチャ座標
	gg_v2DrawRect[0].tu = tu1;  gg_v2DrawRect[0].tv = tv1;  
	gg_v2DrawRect[1].tu = tu2;  gg_v2DrawRect[1].tv = tv1;  
 	gg_v2DrawRect[2].tu = tu1;  gg_v2DrawRect[2].tv = tv2;
 	gg_v2DrawRect[3].tu = tu2;  gg_v2DrawRect[3].tv = tv2;

	gg_pD3DDevice->SetTexture( 0, pPicture );
    gg_pD3DDevice->SetFVF( D3DFVF_AM2DTEX );
	gg_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, gg_v2DrawRect, sizeof( AM_2DVERTEX ) );

	return AM_OK;
}


// 2Dグラフィック　テクスチャ座標計算
static int AM_Calc2DUVCoord( float *du1, float *dv1, float *du2, float *dv2,
							 int sx1, int sy1, int sx2, int sy2, 
							 AM_2DPIC_INFO *pPicInfo )
{
	// u方向
	if ( sx1 <= sx2 ) {											// 左右反転なし
		*du1  = ( float )( sx1 + 0.5f ) / pPicInfo->nRealWidth;
		*du2  = ( float )( sx2 /* + 0.5f*/  ) / pPicInfo->nRealWidth;
	}
	else {														// 左右反転あり
		*du1  = ( float )( sx1 - 0.5f ) / pPicInfo->nRealWidth;
		*du2  = ( float )( sx2 /*  - 0.5f*/ ) / pPicInfo->nRealWidth;
	}

	// v方向
	if ( sy1 <= sy2 ) {											// 左右反転なし
		*dv1  = ( float )( sy1 + 0.5f ) / pPicInfo->nRealHeight;
		*dv2  = ( float )( sy2 /*- 0.5f*/ ) / pPicInfo->nRealHeight;
	}
	else {
		*dv1  = ( float )( sy1 - 0.5f ) / pPicInfo->nRealHeight;
		*dv2  = ( float )( sy2 /*- 0.5f*/ ) / pPicInfo->nRealHeight;
	}

	return AM_OK;
}


// マスク描画エミュレーション
int			nBitNumTable[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };	// ビット数テーブル

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
	int					nTLVBufSize;				// バッファサイズ
	AM_BLEND_STATE		bsBlendBuf;								// ブレンドステート退避場所
	int					nDrawWidth, nDrawHeight;	// 描画幅・高さ

	nTLVBufSize = ( ( gg_siDispSize.cx + 7 ) / 8 + 2 ) * 2;
	ptlvBuf = ( AM_2DVERTEX * )malloc( sizeof( AM_2DVERTEX ) * nTLVBufSize );
	if ( !ptlvBuf ) return AMERR_NOMEMORY;
	AM_GetBlendState( &bsBlendBuf );							// ブレンドステート取得
	AM_SetBlendState( AMDRAW_ALPHABLEND, bsBlendBuf.nBlendFactor );		// アルファブレンド
	gg_pD3DDevice->SetTexture( 0, pPicture );
	gg_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );	// 表面変更
	for ( i = 0; i < nTLVBufSize; i++ ) {			// 頂点バッファ初期化
		*( ptlvBuf + i ) = gg_v2DrawRect[0];
	}
	nDrawWidth = sx2 - sx1;  nDrawHeight = sy2 - sy1;	// 大きさ計算
	nLeft   = ( int )( x               + .0001 );
	nTop    = ( int )( y               + .0001 );
	nRight  = ( int )( x + nDrawWidth  + .0001 );
	nBottom = ( int )( y + nDrawHeight + .0001 );
	for ( dy = nTop; dy < nBottom + 8; dy += 8 ) {
		nLocTop = dy & 0xfff8;						// 上下ブロック化
		if ( nLocTop < nTop ) nLocTop = nTop;
		nLocBottom = ( dy + 8 ) & 0xfff8;
		if ( nLocBottom > nBottom ) nLocBottom = nBottom;
		ptlvPresent = ptlvBuf;
		for ( dx = nLeft; dx < nRight + 8; dx += 8 ) {
			nLocLeft = dx & 0xfff8;					// 左右ブロック化
			if ( nLocLeft < nLeft ) nLocLeft = nLeft;
			nLocRight = ( dx + 8 ) & 0xfff8;
			if ( nLocRight > nRight ) nLocRight = nRight;
			nMask_x = ( dx >> 3 ) % nMaskWidth;
			nMask_y = dy % nMaskHeight;
			nAlphaValue = 0; nAlphaValue2 = 0;
			for ( i = 0; i < 16; i++ ) {			// マスクアルファ算出
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
			ptlvPresent->x = ( float )nLocLeft;		// 座標セット
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
		ptlvPresent->x = ( float )nLocRight;		// 最後の２座標セット
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

	gg_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );	// 表面変更
	AM_RestoreBlendState( &bsBlendBuf );					// ブレンドステート復帰
	free( ptlvBuf );

	return AM_OK;
}


//-----------------------------------------------------------------------------
// 2D描画環境設定関数域

// ブレンドステート初期化
extern int AM_InitBlendState( void )
{
	// アルファブレンドなし
	gg_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	gg_bsBlendState.nBlendMode = AMDRAW_NOBLEND;
	AM_SetBlendFactor( AM_MAX_BLEND_FACTOR );					// ブレンドファクタ最大
	gg_bsBlendState.nBrRed = 255;  gg_bsBlendState.nBrGreen = 255;  gg_bsBlendState.nBrBlue = 255;	// 描画色は白

	// 色制御
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	// アルファ制御
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	// テクセルサンプラ設定
	gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );

	// テクスチャクランプ
	gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );


	return AM_OK;
}


// ブレンドステート設定
extern int AM_SetBlendState( int nBlendCode, int nBlendFactor )
{
	// ブレンドなし
	if ( nBlendCode == AMDRAW_NOBLEND ) {
		if ( gg_bsBlendState.nBlendMode != AMDRAW_NOBLEND ) {
			gg_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
//			gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			gg_bsBlendState.nBlendMode = AMDRAW_NOBLEND;
		}
		AM_SetBlendFactor( 255 );								// ブレンドファクタ設定
	}

	// アルファブレンディング
	if ( nBlendCode == AMDRAW_ALPHABLEND ) {
		if ( gg_bsBlendState.nBlendMode != AMDRAW_ALPHABLEND ) {
			if ( gg_bsBlendState.nBlendMode == AMDRAW_NOBLEND )
				gg_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );	// ブレンドＯＦＦならＯＮに
//			gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
//			gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
			gg_pD3DDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			gg_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			gg_bsBlendState.nBlendMode = AMDRAW_ALPHABLEND;
		}
		AM_SetBlendFactor( nBlendFactor );						// ブレンドファクタ設定
//		gg_nBlendFactor = nParam;
	}

	// 加算ブレンディング
	if ( nBlendCode == AMDRAW_ADDBLEND ) {
		if ( gg_bsBlendState.nBlendMode != AMDRAW_ADDBLEND ) {
			if ( gg_bsBlendState.nBlendMode == AMDRAW_NOBLEND )
				gg_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );	// ブレンドＯＦＦならＯＮに
//			gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
//			gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
			gg_pD3DDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			gg_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			gg_bsBlendState.nBlendMode = AMDRAW_ADDBLEND;
		}
		AM_SetBlendFactor( nBlendFactor );						// ブレンドファクタ設定
//		gg_nBlendFactor = nParam;
	}

	return AM_OK;
}


// ブレンドファクタセット
static int AM_SetBlendFactor( int nBlendFactor )
{
	int							i;

	// サチレーション
	if ( nBlendFactor < 0 ) nBlendFactor = 0;
	if ( nBlendFactor > AM_MAX_BLEND_FACTOR ) nBlendFactor = AM_MAX_BLEND_FACTOR;

	gg_bsBlendState.nBlendFactor = nBlendFactor;				// ブレンドファクタ保存

	// 描画頂点セット
	for ( i = 0; i < 4; i++ ) {
		gg_v2DrawRect[i].color = ( gg_v2DrawRect[0].color & 0x00ffffff ) | ( nBlendFactor << 24 );
	}

	return AM_OK;
}


// ブレンドステート復帰
extern int AM_RestoreBlendState( AM_BLEND_STATE *pbsRestoreBlend )
{
	AM_SetBlendState( pbsRestoreBlend->nBlendMode, pbsRestoreBlend->nBlendFactor );	// ブレンドファクタ設定
	// 必要なら色ファクタ設定
	if ( gg_bsBlendState.nBrRed   != pbsRestoreBlend->nBrRed   ||
		 gg_bsBlendState.nBrGreen != pbsRestoreBlend->nBrGreen ||
		 gg_bsBlendState.nBrBlue  != pbsRestoreBlend->nBrBlue )
	{
		AM_SetDrawColor( pbsRestoreBlend->nBrRed, pbsRestoreBlend->nBrGreen, pbsRestoreBlend->nBrBlue );
	}
	gg_bsBlendState = *pbsRestoreBlend;							// ブレンドステート保存

	return AM_OK;
}


// ブレンドステート取得
extern int AM_GetBlendState( AM_BLEND_STATE *pbsBlendFactor )
{
	*pbsBlendFactor = gg_bsBlendState;

	return AM_OK;
}


// テクスチャサンプルステート設定
extern int AM_SetTexSampleState( int nSampleCode )
{
	int							nResult = AM_OK;				// 結果コード

	switch ( nSampleCode ) {

	// ニアレストネイバー法
	case AMDRAW_NEAREST:										
		gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		break;

	// バイリニア法
	case AMDRAW_BILINEAR:										
		gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		break;

	// 該当コードなし
	default:
		nResult = AMERR_INVALIDMODE;
	}

	return nResult;
}


// 描画色セット
extern int AM_SetDrawColor( int nRed, int nGreen, int nBlue )
{
	int							i;
	int							nCompositColor;					// 合成済みの色

	// サチレーション
	if ( nRed   < 0 ) nRed = 0;
	if ( nRed   > AM_MAX_COLOR_FACTOR ) nRed   = AM_MAX_COLOR_FACTOR;
	if ( nGreen < 0 ) nGreen = 0;
	if ( nGreen > AM_MAX_COLOR_FACTOR ) nGreen = AM_MAX_COLOR_FACTOR;
	if ( nBlue  < 0 ) nBlue = 0;
	if ( nBlue  > AM_MAX_COLOR_FACTOR ) nBlue  = AM_MAX_COLOR_FACTOR;

	// 色情報保存
	gg_bsBlendState.nBrRed   = nRed;
	gg_bsBlendState.nBrGreen = nGreen;
	gg_bsBlendState.nBrBlue  = nBlue;

	nCompositColor = ( nRed << 16 ) + ( nGreen << 8 ) + nBlue;

	// 描画頂点セット
	for ( i = 0; i < 4; i++ ) {
		gg_v2DrawRect[i].color = ( gg_v2DrawRect[0].color & 0xff000000 ) | nCompositColor;
	}

	return AM_OK;
}


// テクスチャラップステート設定
extern int AM_SetTexWrapState( int nWrapCode )
{
	int							nResult = AM_OK;				// 結果コード

	switch ( nWrapCode ) {

	// テクスチャクランプ
	case AMDRAW_TEX_NOREPEAT:
		gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
		break;

	// テクスチャリピート
	case AMDRAW_TEX_REPEAT:
		gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
		gg_pD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
		break;

	// 該当コードなし
	default:
		nResult = AMERR_INVALIDMODE;
	}

	return nResult;
}


//-----------------------------------------------------------------------------
// 2D画像周辺関数域

// 表示用バッファの内容取得
extern int AM_GetDisplayBuffer( AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo, int nGetMode )
{
	HRESULT						hRes;							// 結果
	AM_LPDISPLAYBUFFER			pTempBuffer;					// テンポラリバッファ
	LPDIRECT3DSURFACE9			lpTopSurf;						// テクスチャトップサーフェス
	RECT						rcSrc, rcDest;					// 転送元・転送先矩形

	switch ( nGetMode ) {
	case AM_GET_FRONT_BUFFER:									// フロントバッファ取得
		gg_pD3DDevice->CreateOffscreenPlainSurface( gg_siWholeDispSize.cx, gg_siWholeDispSize.cy,
													D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pTempBuffer, NULL );
		gg_pD3DDevice->GetFrontBufferData( 0, pTempBuffer );

		hRes = pPicture->GetSurfaceLevel( 0, &lpTopSurf );		// 転送先サーフェス取得
		SetRect( &rcSrc, gg_piDispLocate.x, gg_piDispLocate.y,
						 gg_piDispLocate.x + gg_siDispSize.cx, gg_piDispLocate.y + gg_siDispSize.cy );	// 転送元矩形
		SetRect( &rcDest, 0, 0, pPicInfo->nWidth, pPicInfo->nHeight );	// 転送先矩形
		// 転送
		D3DXLoadSurfaceFromSurface( lpTopSurf, NULL, &rcDest, pTempBuffer, NULL, &rcSrc, D3DX_FILTER_LINEAR, 0 );
//		gg_pD3DDevice->StretchRect( pTempBuffer, NULL, lpTopSurf, NULL, D3DTEXF_LINEAR );
		pTempBuffer->Release();									// 転送元サーフェス開放
		lpTopSurf->Release();									// 転送先サーフェス開放
	}

    return AM_OK;
}


// 表示用バッファのファイルへのセーブ
extern int AM_SaveDisplayBuffer( AM_RECT *pSaveRect, char *szFileName, int nGetMode )
{
	int							i, j;
	HRESULT						hRes;							// 結果
	AM_LPDISPLAYBUFFER			pTempBuffer;					// テンポラリバッファ
	D3DLOCKED_RECT				lrLockedRect;					// ロックされた領域情報
	BITMAPFILEHEADER			bfFileHeader;					// BMPファイルヘッダ
	BITMAPINFOHEADER			biInfoHeader;					// BMPインフォヘッダ
	HANDLE						hWriteFile;						// 書き込みファイルハンドル
	DWORD						nWritten;						// 書き込み済みバイト数
	int							nHeaderSize;					// ヘッダサイズ
	int							nFileSize;						// ファイルサイズ
	int							nSaveWidth, nSaveHeight;		// セーブ幅・高さ
	DWORD						*pStartAddr;					// 制御スタートアドレス
	DWORD						*pSwap1, *pSwap2;				// スワップポイント1,2
	DWORD						nTempBuf;						// 一時バッファ
	DWORD						*pSurfRead;						// サーフェス読み出し
	char						*pSurfWrite;					// サーフェス書き込み
	int							nDWORDPitch;					// DWORD単位ピッチ

	// ファイルオープン
	hWriteFile = CreateFile( szFileName, GENERIC_WRITE,
							 0, NULL, CREATE_ALWAYS,
							 FILE_ATTRIBUTE_NORMAL, NULL );
	if ( hWriteFile == INVALID_HANDLE_VALUE )		// 成功チェック
		return AMERR_CANNOTCREATE;

	// 位置制御
	if ( !gg_bFullMode ) {
		pSaveRect->left   += gg_piDispLocate.x;
		pSaveRect->top    += gg_piDispLocate.y;
		pSaveRect->right  += gg_piDispLocate.x;
		pSaveRect->bottom += gg_piDispLocate.y;
	}

	// 大きさセット
	nSaveWidth  = pSaveRect->right  - pSaveRect->left;
	nSaveHeight = pSaveRect->bottom - pSaveRect->top;

	// ヘッダ・ファイルサイズ
	nHeaderSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER );
	nFileSize = nHeaderSize + nSaveWidth * nSaveHeight * 3;

	// ファイルヘッダ
	bfFileHeader.bfType = 'MB';
	bfFileHeader.bfSize = nFileSize;
	bfFileHeader.bfOffBits = nHeaderSize;
	WriteFile( hWriteFile, &bfFileHeader, sizeof( BITMAPFILEHEADER ), &nWritten, NULL );

	// インフォヘッダ
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
	case AM_GET_FRONT_BUFFER:									// フロントバッファ取得
		gg_pD3DDevice->CreateOffscreenPlainSurface( gg_siWholeDispSize.cx, gg_siWholeDispSize.cy,
													D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pTempBuffer, NULL );
		gg_pD3DDevice->GetFrontBufferData( 0, pTempBuffer );

		// ファイルへセーブ
		hRes = pTempBuffer->LockRect( &lrLockedRect, NULL, 0 );
		if ( SUCCEEDED( hRes ) ) {
			// 上下反転
			nDWORDPitch = lrLockedRect.Pitch >> 2;				// DWORDでのピッチ
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
			// フォーマット変換
			pSurfRead  = pStartAddr;
			pSurfWrite = ( char  * )lrLockedRect.pBits;
			for ( i = 0; i < nSaveHeight; i++ ) {
				for ( j = 0; j < nSaveWidth; j++ ) {
					*( ( DWORD * )pSurfWrite ) = *( pSurfRead + j );
					pSurfWrite += 3;
				}
				pSurfRead += nDWORDPitch;
			}
			// 書き込み
			WriteFile( hWriteFile, lrLockedRect.pBits, nSaveWidth * nSaveHeight * 3, &nWritten, NULL );
			pTempBuffer->UnlockRect();
		}
		pTempBuffer->Release();
	}

	CloseHandle( hWriteFile );									// ファイルクローズ

    return AM_OK;
}


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AM_3DGraph.cpp
//
// 機能: DirectX上でのＡＭＧＡＭＥの３Ｄ画面描画機能を実装。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#pragma warning( disable : 4819 )						// コードページの警告抑止


// ---------------------------------------------------------------------------
// インクルード域

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "amgame.h"												// AMGAMEヘッダ

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnvヘッダ
#include "AM_3DGraph.h"											// AMDispEnvヘッダ
#include "AM_Input.h"											// AMInputヘッダ

#endif


// ---------------------------------------------------------------------------
// ファイル内グローバル変数域

int								fg_bLighting = false;			// 光源処理ありか


// ---------------------------------------------------------------------------
// 内部関数プロトタイプ宣言域

static int AM_InitMatrices( void );								// 行列初期化
static int AM_InitLights( void );								// 光源初期化


//-----------------------------------------------------------------------------
// 3D環境設定関数域

// 3Dグラフィックス初期化
extern int AM_Init3DGraph( void )
{
	AM_InitLights();											// 光源
	AM_InitMatrices();											// 行列

	return AM_OK;
}


// 行列初期化
static int AM_InitMatrices( void )
{
	D3DXVECTOR3		v3CamLocation, v3CamLookAt, v3CamUp;		// カメラ位置、注視点、上方向

	// ワールド行列
    D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );
	gg_pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );

    // ビュー行列
    D3DXMATRIX matView;
	v3CamLocation = D3DXVECTOR3( gg_siDispSize.cx / 2.0f, gg_siDispSize.cy / 2.0f,
								 ( float )( 2.0f * 290.0f * ( gg_siDispSize.cy / 480.0f ) ) );	// カメラ位置
	v3CamLookAt = D3DXVECTOR3( gg_siDispSize.cx / 2.0f, gg_siDispSize.cy / 2.0f, 0.0f );	// 注視点
	v3CamUp = D3DXVECTOR3( 0.0f, -1.0f, 0.0f );					// 上方向ベクトル
    D3DXMatrixLookAtLH( &matView, &v3CamLocation, 
                                  &v3CamLookAt,
                                  &v3CamUp );
    gg_pD3DDevice->SetTransform( D3DTS_VIEW, &matView );

	// プロジェクション行列
    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, ( float )gg_siDispSize.cx / gg_siDispSize.cy, 1.0f, 2000.0f );
    gg_pD3DDevice->SetTransform( D3DTS_PROJECTION, &matProj );

/*	// チェック
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

	// 色制御
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2 );
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	// アルファ制御
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

	gg_pD3DDevice->SetFVF( D3DFVF_AM3DVERTEX );
	gg_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, cvRectVerts, sizeof( AM_3DVERTEX ) );

	AM_Flip();
	AM_GetSingleChar();*/

	return AM_OK;
}


// 行列初期化
extern int AM_SetTransformMatrix( int nKind, AmMatrix *pmaMatrix )
{
//	D3DXMATRIX					mat;
	int							nResult = AM_OK;

	switch ( nKind ) {
	case AM_MATRIX_LW:											// ローカル→ワールド
//		mat = *( ( D3DXMATRIX * )pmaMatrix );
		gg_pD3DDevice->SetTransform( D3DTS_WORLD, ( D3DMATRIX * )pmaMatrix );
		break;

	case AM_MATRIX_WV:											// ワールド→ビュー
//		mat = *( ( D3DXMATRIX * )pmaMatrix );
		gg_pD3DDevice->SetTransform( D3DTS_VIEW, ( D3DMATRIX * )pmaMatrix );
		break;

	default:													// 値は不適切
		nResult = AMERR_INVALIDVALUE;
		break;
	}

	return nResult;
}


// 行列取得
extern int AM_GetTransformMatrix( int nKind, AmMatrix *pmaMatrix )
{
	int							nResult = AM_OK;

	switch ( nKind ) {
	case AM_MATRIX_LW:											// ローカル→ワールド
		gg_pD3DDevice->GetTransform( D3DTS_WORLD, ( D3DMATRIX * )pmaMatrix );
		break;

	case AM_MATRIX_WV:											// ワールド→ビュー
		gg_pD3DDevice->GetTransform( D3DTS_VIEW, ( D3DMATRIX * )pmaMatrix );
		break;

	default:													// 値は不適切
		nResult = AMERR_INVALIDVALUE;
		break;
	}

	return nResult;
}


// 光源初期化
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


// 3Dグラフィックス使用終了
extern int AM_End3DGraph( void )
{
	return AM_OK;
}


//-----------------------------------------------------------------------------
// 3D描画関数域

// 3Dポリゴン描画
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

	if ( nFlags & AMPOLY_NOLIGHT ) {				// ライティング無しの場合
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
		if ( nFlags & AMPOLY_QUADRANGLE ) {			// 四角ポリゴンの場合
			for ( i = 0; i < nVertexNum; i += 4 ) {
				gg_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &pVertBuf[i], sizeof( AM_3DVERTEX ) );
			}
		}
		else {										// 三角ポリゴンの場合
			gg_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, nVertexNum / 3, pVertBuf, sizeof( AM_3DVERTEX ) );
		}
		delete []pVertBuf;
	}
	else {											// ライティングありの場合
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
		if ( nFlags & AMPOLY_QUADRANGLE ) {			// 四角ポリゴンの場合
			for ( i = 0; i < nVertexNum; i += 4 ) {
				gg_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &pVertBuf[i], sizeof( AM_3DVERTEX ) );
			}
		}
		else {										// 三角ポリゴンの場合
			gg_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, nVertexNum / 3, pVertBuf, sizeof( AM_3DVERTEX ) );
		}
		delete []pVertBuf;
	}

	// ライティングOFF
	if ( fg_bLighting ) {
		gg_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		fg_bLighting = false;
	}

//	delete []pVertBuf;

	return AM_OK;
}


// 3D線列描画
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
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AM_FigureDraw.cpp
//
// 機能: DirectX上でのＡＭＧＡＭＥの２Ｄ画面描画機能を実装。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#pragma warning( disable : 4819 )						// コードページの警告抑止


// ---------------------------------------------------------------------------
// インクルード域

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string.h>

#include "amgame.h"												// AMGAMEヘッダ

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnvヘッダ
#include "AM_2DGraph.h"											// AM2DGraphヘッダ
#include "AM_FigureDraw.h"										// AMFigureDrawヘッダ

#endif


// ---------------------------------------------------------------------------
// ファイル内構造体宣言域


// 2D頂点構造体
typedef struct tagAM_POINTVERTEX {
	float				x, y, z, rhw;							// 頂点座標
	int					color;									// 頂点色
} AM_POINTVERTEX;

#define D3DFVF_AMPOINT			( D3DFVF_XYZRHW | D3DFVF_DIFFUSE )	// 2Dテクスチャ頂点フォーマット


// ---------------------------------------------------------------------------
// ファイル内グローバル変数域

// バッファ関係
AM_POINTVERTEX					fg_apPointBuf[AM_MAX_POINT_NUM];	// ポイントバッファ

// フォント関係
HFONT							fg_hStringFont = NULL;				// 文字列のフォント
int								fg_nStringFontSize = -1;			// 文字列フォントサイズ
LPD3DXFONT						fg_pD3DXFont = NULL;				// D3DXFontオブジェクト


// ---------------------------------------------------------------------------
// 内部関数プロトタイプ宣言域

static int AM_GetModulateColor( int nColor1, int nColor2 );			// 色の掛け算計算


//-----------------------------------------------------------------------------
// 図形描画関数域

// 点列の描画
extern int AM_DrawPointArray( AM_POINT_DATA *papPoints, int nPointNum )
{
	int							i;
	int							nResult = AM_OK;

	if ( nPointNum > AM_MAX_POINT_NUM ) {
		nPointNum = AM_MAX_POINT_NUM;
		nResult = AMERR_OUTOFRANGE;
	}

	// 点座標コピー
	for ( i = 0; i < nPointNum; i++ ) {
		fg_apPointBuf[i].x = ( float )( papPoints + i )->x;
		fg_apPointBuf[i].y = ( float )( papPoints + i )->y;
		fg_apPointBuf[i].z = ( float )( papPoints + i )->z;
		fg_apPointBuf[i].rhw = 1.0f;
		fg_apPointBuf[i].color = ( DWORD )( papPoints + i )->nColor;
	}

	// 描画
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2 );	// ディフューズ色のみ
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2 );	// ディフューズアルファのみ
	gg_pD3DDevice->SetFVF( D3DFVF_AMPOINT );
	gg_pD3DDevice->DrawPrimitiveUP( D3DPT_POINTLIST, ( DWORD )nPointNum,
									fg_apPointBuf, sizeof( AM_POINTVERTEX ) );
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );	// テクスチャも復帰
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );	// テクスチャアルファも復帰

	return nResult;
}


// 色コードの取得
extern int AM_GetColorCode( int r, int g, int b )
{
	int							nResult;

	nResult = ( int )D3DCOLOR_ARGB( AM_MAX_BLEND_FACTOR, r, g, b );

	return nResult;
}


// バックバッファの指定点の色取得
extern int AM_GetBackbufferPointColor( int x, int y )
{
	int							nResult = AMERR_CANNOT;			// 関数の結果
	HRESULT						hRes;							// 呼び出し結果
	RECT						rcSrc;							// 転送元矩形
	D3DLOCKED_RECT				lrLockedRect;					// ロックされた領域情報

	SetRect( &rcSrc, x, y, x + 1, y + 1 );						// 転送元矩形
	hRes = gg_pBackBuffer->LockRect( &lrLockedRect, &rcSrc, D3DLOCK_READONLY );
	if ( SUCCEEDED( hRes ) ) {
		if ( gg_nBitDepth == 32 ) {
			nResult = *( ( int * )( lrLockedRect.pBits ) );// + y * ( lrLockedRect.Pitch >> 2 ) + x );		// 32ビット色
		}
		else {
			nResult = ( int ) *( ( short * )( lrLockedRect.pBits ) );// + y * ( lrLockedRect.Pitch >> 1 ) + x );		// 16ビット色
			if ( gg_dfDisplayFormat == D3DFMT_R5G6B5 ) {		// R5G6B5の場合
				nResult = 0xff000000 + ( ( nResult & 0xf800 ) << 8 ) + ( ( nResult & 0x07e0 ) << 5 ) + ( ( nResult & 0x001f ) << 3 );
			}
			else {												// X1R5G5B5の場合
				nResult = 0xff000000 + ( ( nResult & 0x7c00 ) << 9 ) + ( ( nResult & 0x03e0 ) << 6 ) + ( ( nResult & 0x001f ) << 3 );
			}
		}
		gg_pBackBuffer->UnlockRect();
	}

	return nResult;
}


// 長方形の描画
extern int AM_DrawRectangleArray( AM_POINT_DATA *papPoints, int nRectNum )
{
	int							i;
	int							nResult = AM_OK;
	AM_POINT_DATA				*pPresPoint;					// 現在の点

	if ( nRectNum > AM_MAX_RECT_NUM ) {
		nRectNum = AM_MAX_RECT_NUM;
		nResult = AMERR_OUTOFRANGE;
	}

	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2 );	// ディフューズ色のみ
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2 );	// ディフューズアルファのみ
//	gg_pD3DDevice->SetTexture( 0, NULL );							// テクスチャなしに

	pPresPoint = papPoints;
	for ( i = 0; i < nRectNum; i++ ) {
		// 左上
		fg_apPointBuf[0].x = ( float )pPresPoint->x;
		fg_apPointBuf[0].y = ( float )pPresPoint->y;
		fg_apPointBuf[0].z = ( float )pPresPoint->z;
		fg_apPointBuf[0].rhw = 1.0f;
		fg_apPointBuf[0].color = AM_GetModulateColor( pPresPoint->nColor, gg_v2DrawRect[0].color );

		// 右上
		fg_apPointBuf[1] = fg_apPointBuf[0];
		fg_apPointBuf[1].x = ( float )( pPresPoint + 1 )->x;

		// 左下
		fg_apPointBuf[2].x = ( float )pPresPoint->x;
		fg_apPointBuf[2].y = ( float )( pPresPoint + 1 )->y;
		fg_apPointBuf[2].z = ( float )( pPresPoint + 1 )->z;
		fg_apPointBuf[2].rhw = 1.0f;
		fg_apPointBuf[2].color = AM_GetModulateColor( ( pPresPoint + 1 )->nColor, gg_v2DrawRect[2].color );

		// 右下
		fg_apPointBuf[3] = fg_apPointBuf[2];
		fg_apPointBuf[3].x = ( float )( pPresPoint + 1 )->x;

		// 描画
		gg_pD3DDevice->SetFVF( D3DFVF_AMPOINT );
		gg_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, nRectNum * 2,
										fg_apPointBuf, sizeof( AM_POINTVERTEX ) );

		pPresPoint += 2;
	}

	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );	// テクスチャ色も復帰
	gg_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );	// テクスチャアルファも復帰

	return nResult;
}


// 色の掛け算計算
static int AM_GetModulateColor( int nColor1, int nColor2 )
{
	int				nAlpha, nRed, nGreen, nBlue;

	nAlpha = ( ( ( nColor1 >> 24 ) & 0xff ) * ( ( nColor2 >> 24 ) & 0xff ) ) / 0xff;
	nRed =   ( ( ( nColor1 >> 16 ) & 0xff ) * ( ( nColor2 >> 16 ) & 0xff ) ) / 0xff;
	nGreen = ( ( ( nColor1 >>  8 ) & 0xff ) * ( ( nColor2 >>  8 ) & 0xff ) ) / 0xff;
	nBlue =  ( ( ( nColor1       ) & 0xff ) * ( ( nColor2       ) & 0xff ) ) / 0xff;

	return ( nAlpha << 24 ) + ( nRed << 16 ) + ( nGreen << 8 ) + ( nBlue );
}


// 文字列フォントの準備
extern int AM_PrepareFontObject( void )
{
	int							nResult;
//	HRESULT						hRes;

	// Windowsフォントオブジェクト作成
	nResult = AM_SetStringFontSize( AM_DEFAULT_FONT_SIZE );
	if ( nResult < 0 ) return nResult;

/*	// D3DXフォントオブジェクト作成
	hRes = D3DXCreateFont( gg_pD3DDevice, fg_hStringFont, &fg_pD3DXFont );
	if ( FAILED( hRes ) ) return AMERR_DISPLAY;
*/
	return AM_OK;
}


// 文字列フォントの開放
extern int AM_ReleaseFontObject( void )
{
	AM_SAFE_RELEASE( fg_pD3DXFont );							// D3DXフォント開放

	if ( fg_hStringFont != NULL ) {
		DeleteObject( fg_hStringFont );							// Windowsフォント削除
		fg_hStringFont = NULL;
	}

	return AM_OK;
}

// 文字列フォントサイズの設定
extern int AM_SetStringFontSize( int nFontSize )
{
    HFONT						hNewFont;						// 新規フォント
	int							nNWeight;						// 文字のウェイト（太さ）
	HRESULT						hRes;							// 結果

	if ( nFontSize == fg_nStringFontSize ) return AM_OK;			// サイズが同じなら何もしない

	// 太さ制御
    if ( nFontSize <= 22 ) nNWeight = FW_SEMIBOLD;
	                  else nNWeight = FW_NORMAL;

	// フォント生成
	hNewFont = CreateFont( nFontSize, 0,
		                   0, 0, nNWeight, FALSE, FALSE, FALSE,
		                   SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
						   CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY,
						   FIXED_PITCH, NULL );
	if ( hNewFont == NULL ) return AMERR_INVALIDVALUE;

    if ( fg_hStringFont != NULL ) DeleteObject( fg_hStringFont );	// 既にあれば削除

	// 共有変数セット
    fg_hStringFont = hNewFont;
	fg_nStringFontSize = nFontSize;

	// D3DXフォントオブジェクト作成
	AM_SAFE_RELEASE( fg_pD3DXFont );								// D3DXフォント開放
#if D3DX_VERSION == 0x0900
	hRes = D3DXCreateFont( gg_pD3DDevice, fg_hStringFont, &fg_pD3DXFont );	// D3DXフォント作成
#else
	hRes = D3DXCreateFont( gg_pD3DDevice, nFontSize, nFontSize / 2, nNWeight, 1, false,
						   SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, NONANTIALIASED_QUALITY,
						   FIXED_PITCH, NULL, &fg_pD3DXFont );
#endif
	if ( FAILED( hRes ) ) return AMERR_DISPLAY;

    return AM_OK;
}


// 文字列フォントサイズの取得
extern int AM_GetStringFontSize( void )
{
	return fg_nStringFontSize;
}


// 文字列の描画
extern int AM_DrawString( int x, int y, char *szString, int nColor )
{
	HRESULT						hRes;
	int							nStrLength;						// 文字列の長さ
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
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AM_Input.cpp
//
// 機能: DirectX上でのＡＭＧＡＭＥのユーザ入力機能を実装。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#pragma warning( disable : 4819 )						// コードページの警告抑止

#include <windows.h>

#include "amgame.h"												// AMGAMEヘッダ

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// 画面初期化　機種依存レイヤー
#include "AM_Input.h"											// キー入力　機種依存レイヤー
#include "AB_Input.h"											// キー入力　ブリッジレイヤー

#endif


// ---------------------------------------------------------------------------
// 構造体定義域

typedef struct tagAM_JOYPAD {									// ジョイパッド構造体
	UINT		uJoyID;											// ジョイスティックＩＤ
	BOOL		bValid;											// ジョイスティック有効か
	DWORD		nMinX, nMaxX;									// スティックの最大、最小値
	DWORD		nMinY, nMaxY;
	DWORD		nCenterX, nCenterY;								// 中央値
	DWORD		nLeft, nRight;									// 左右しきい値
	DWORD		nTop, nBottom;									// 上下しきい値
} AM_JOYPAD;


// ---------------------------------------------------------------------------
// インターファイルグローバル変数域

int								gg_nJoyPadNum = -1;				// ジョイパッド数
int								gg_bMouseCursor = false;		// マウスカーソル
int								gg_bMouseActive = false;		// マウス有効か


// ---------------------------------------------------------------------------
// ファイル内グローバル変数域

int								fg_bKeyPushed = false;			// キーボードのキーが押されたか
int								fg_nKeyCode;					// キーコード
AM_JOYPAD						fg_jpPads[AM_MAX_PAD_NUM];		// ジョイパッド


// ---------------------------------------------------------------------------
// 内部関数プロトタイプ宣言域

static int AM_InitSinglePad( UINT nJoyID, AM_JOYPAD *pJoyPad );	// ジョイパッド初期化


//-----------------------------------------------------------------------------
// キーボード入力制御関数域

// キー入力待ち・入力キー取得関数
extern int AM_GetSingleChar( void )
{
	while( AM_ProcessEvent() && !AM_IsKeyHit() );
	fg_bKeyPushed = false;

	return fg_nKeyCode;
}

// キーボード押下チェック関数
extern int AM_IsKeyHit( void )
{
	if ( !AM_ProcessEvent() ) return ( int )( VK_ESCAPE );		// 既に終了の場合

	if ( fg_bKeyPushed ) return fg_nKeyCode;
					else return false;
}

// キー入力通知関数
extern int AM_SetKeyPushed( int nKeyCode )
{
	fg_nKeyCode = nKeyCode;
	fg_bKeyPushed = true;

	return AM_OK;
}

// キー入力付加
extern int AM_AddKeyInput( int nKeyState )
{
	BYTE			cKeyBuf[256];

	GetKeyboardState( cKeyBuf );								// キーボード

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

	// デバッグ用入力
	if ( ( cKeyBuf[ 'P'        ] & 0x80 ) ) nKeyState |= KEYIN_DEBUG_PAUSE;
	if ( ( cKeyBuf[ 'O'        ] & 0x80 ) ) nKeyState |= KEYIN_DEBUG_DISPLAY;
	if ( ( cKeyBuf[ 'I'        ] & 0x80 ) ) nKeyState |= KEYIN_DEBUG_COLOR;
	
	// マウス
	if ( gg_bMouseActive ) {
		if ( ( cKeyBuf[ VK_LBUTTON ] & 0x80 ) ) nKeyState |= MOUSEKEY_LEFT;
		if ( ( cKeyBuf[ VK_RBUTTON ] & 0x80 ) ) nKeyState |= MOUSEKEY_RIGHT;
		if ( ( cKeyBuf[ VK_MBUTTON ] & 0x80 ) ) nKeyState |= MOUSEKEY_MIDDLE;
	}

	return nKeyState;
}


//-----------------------------------------------------------------------------
// パッド入力制御関数域

UINT						nPadIDs[AM_MAX_PAD_NUM] = { JOYSTICKID1, JOYSTICKID2 };

// パッド初期化
extern int AM_InitJoyPad( int nMaxPadNum )
{
	int							i;
	int							nActivePadNum = 0;				// アクティブなパッド数
	int							nResult;

	if ( nMaxPadNum > AM_MAX_PAD_NUM ) nMaxPadNum = AM_MAX_PAD_NUM;
	for ( i = 0; i < nMaxPadNum; i++ ) {
		nResult = AM_InitSinglePad( nPadIDs[i], &fg_jpPads[i] );	// パッド初期化
		if ( nResult != AM_OK ) break;							// 無ければ終了
		nActivePadNum++;
	}

	gg_nJoyPadNum = nActivePadNum;								// パッド数登録
//	if ( nActivePadNum > nMaxPadNum ) nActivePadNum = nMaxPadNum;	// パッド数制限

	return nActivePadNum;
}


// 単体ジョイパッド初期化
static int AM_InitSinglePad( UINT nJoyID, AM_JOYPAD *pJoyPad )
{
	JOYCAPS						jCaps;
	JOYINFOEX					jJoyInfo;
	int							nResult = AMERR_NOHARDWARE;

	pJoyPad->bValid = false;									// 無効にしておく
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
		jJoyInfo.dwSize = sizeof( jJoyInfo );					// Joy構造体セット
		jJoyInfo.dwFlags = JOY_RETURNALL;
		if ( joyGetPosEx( pJoyPad->uJoyID, &jJoyInfo ) == JOYERR_NOERROR ) {
			pJoyPad->bValid = TRUE;								// パッド有効に
			nResult = AM_OK;
		}
	}

	return nResult;
}


// パッド使用終了
extern int AM_EndJoyPad( void )
{
	return AM_OK;
}


// 単体パッド入力付加
extern int AM_AddJoyInput( int nPadIndex, int nKeyState )
{
	AM_JOYPAD					*pJoyPad;						// パッド
	JOYINFOEX					jJoyInfo;						// パッド情報

	// 値チェックとパッド取得
	if ( nPadIndex < 0 || nPadIndex >= gg_nJoyPadNum ) return AMERR_NOHARDWARE;
	pJoyPad = &fg_jpPads[nPadIndex];

	if ( pJoyPad->bValid ) {									// 有効か
		jJoyInfo.dwSize = sizeof( jJoyInfo );					// Joy構造体セット
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
// マウス入力制御関数域

// マウス初期化
extern int AM_InitMouse( int bMouseState )
{
	if ( !gg_hWindow ) return AMERR_NOINITIALIZE;

	gg_bMouseActive = true;
	AM_SetMouseState( bMouseState );					// 状態設定

	return AM_OK;
}


// マウス使用終了
extern int AM_EndMouse( void )
{

	return AM_OK;
}


// マウス状態設定
extern int AM_SetMouseState( int bMouseState )
{
	if ( !gg_bMouseActive ) return AMERR_NOINITIALIZE;	// 初期化チェック
	if ( bMouseState ) {
		gg_bMouseCursor = true;
	}
	else {
		gg_bMouseCursor = false;
	}
	PostMessage( gg_hWindow, WM_SETCURSOR, ( WPARAM )gg_hWindow, 0 );

	return AM_OK;
}


// マウス位置取得
extern int AM_GetMousePosition( int *nXLoc, int *nYLoc )
{
	POINT		MousePnt;

	if ( !gg_bMouseActive ) {							// 初期化チェック
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


// マウス位置設定
extern int AM_SetMouseLoc( int nXLoc, int nYLoc )
{
	POINT		MousePnt;

	if ( !gg_bMouseActive ) return AMERR_NOINITIALIZE;		// 初期化チェック

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
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AM_Movie.cpp
//
// 機能: DirectX上でのＡＭＧＡＭＥのムービー再生機能を実装。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#pragma warning( disable : 4819 )						// コードページの警告抑止


// ---------------------------------------------------------------------------
// インクルード域

#include <windows.h>
#include <digitalv.h>
#include <vfw.h>
#include <msacm.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dsound.h>

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnvヘッダ
#include "AB_DispEnv.h"											// ABDispEnvヘッダ
#include "AM_2DGraph.h"											// AM2DGraphヘッダ
#include "AM_Sound.h"											// AMSoundヘッダ
#include "AB_Time.h"											// ABTimeヘッダ
#include "AB_Input.h"											// ABInputヘッダ
#include "AM_Movie.h"											// AMMovieヘッダ

#endif


// ---------------------------------------------------------------------------
// インターファイルグローバル変数域

MCIDEVICEID						gg_wAVIDevice;					// ＡＶＩデバイスＩＤ


// ---------------------------------------------------------------------------
// ファイル内グローバル変数域

int								fg_bMoviePlaying = false;		// ムービー再生中か
int								fg_nMovieFirst = -1;			// ムービー再生起点
int								fg_nMovieLast = -1;				// ムービー再生終点
int								fg_bAmAVIInit = false;			// AVIムービー再生初期化済みか


//-----------------------------------------------------------------------------
// ムービー再生関数域

/*// 通常ムービー再生
extern int AM_PlayMovie( char *szFileName, int x, int y, int nWidth, int nHeight )
{
	MCI_DGV_OPEN_PARMS		mciOpen;
	MCI_DGV_RECT_PARMS		mciRect;
	MCI_DGV_WINDOW_PARMS	mciWindow;
	MCI_DGV_PUT_PARMS		mciPut;
	MCI_DGV_PLAY_PARMS		mciPlay;
	DWORD					lPlayFlag;
	MCIERROR				nResult;

//	while ( bActiveFlag == FALSE && ProcessEvent() );			// アクティブ待ち
	if ( fg_bMoviePlaying == TRUE ) AM_StopMovie();				// 前の終了

//	SetPage( 0, 0 );
	if ( x == 0 && y == 0 &&									// 全画面ならクリア
		 nWidth == gg_siDispSize.cx && nHeight == gg_siDispSize.cy )
	{
		HCls();
	}
	mciOpen.dwCallback = ( DWORD_PTR )gg_hWindow;				// イベントハンドラ
	mciOpen.lpstrElementName = szFileName;						// ファイルオープン
	nResult = mciSendCommand( 0, MCI_OPEN,
							  (DWORD)( MCI_OPEN_ELEMENT | MCI_DGV_OPEN_NOSTATIC ),
							  (DWORD_PTR)&mciOpen );
	if ( nResult != 0 ) return AMERR_NOFILE;
	gg_wAVIDevice = mciOpen.wDeviceID;

	if ( nWidth == 0 || nHeight == 0 ) {			// サイズ決定
		mciWindow.hWnd = gg_hWindow;					// 実サイズ
		mciSendCommand( gg_wAVIDevice, MCI_WHERE,
						(DWORD)MCI_DGV_WHERE_SOURCE, (DWORD_PTR)&mciRect);
	}
	else {											// 指定サイズ
		mciRect.rc.right = nWidth;
		mciRect.rc.bottom = nHeight;
	}

	mciWindow.hWnd = gg_hWindow;						// ウィンドウ関連付け
	mciSendCommand( gg_wAVIDevice, MCI_WINDOW,
					(DWORD)MCI_DGV_WINDOW_HWND, (DWORD_PTR)&mciWindow );

	mciPut.dwCallback = (DWORD_PTR)gg_hWindow;		// 転送先指定
	mciPut.rc.left = x;  mciPut.rc.top = y;
	mciPut.rc.right = mciRect.rc.right;  mciPut.rc.bottom = mciRect.rc.bottom;
	mciSendCommand( gg_wAVIDevice, MCI_PUT,
					MCI_DGV_PUT_DESTINATION | MCI_DGV_RECT, (DWORD_PTR)&mciPut );

	mciPlay.dwCallback = (DWORD_PTR)gg_hWindow;
	lPlayFlag = MCI_NOTIFY;
	if ( fg_nMovieFirst >= 0 && fg_nMovieLast >= 0 ) {	// タイムリージョン指定あるか
		mciPlay.dwFrom = fg_nMovieFirst;
		mciPlay.dwTo = fg_nMovieLast;
		lPlayFlag |= MCI_FROM | MCI_TO;
		fg_nMovieFirst = -1;  fg_nMovieLast = -1;
	}
	nResult = mciSendCommand( gg_wAVIDevice, MCI_PLAY, lPlayFlag,	// 再生
							  (DWORD_PTR)&mciPlay );
	if ( nResult == 0 ) fg_bMoviePlaying = TRUE;
				   else return AMERR_INVALIDVALUE;

	return AM_OK;
}


// 通常ムービー再生ストップ
extern int AM_StopMovie( void )
{
	if ( fg_bMoviePlaying ) {
		fg_bMoviePlaying = FALSE;
		mciSendCommand( gg_wAVIDevice, MCI_CLOSE, 0, NULL );
//		nMovieFirst = -1;  nMovieLast = -1;
	}

	return AM_OK;
}*/


// ムービーファイルオープン
extern int AM_OpenMovieFile( char *szFileName, AMMOVIE *pMovieData )
{
	int					i;
//	int					bError;
	HRESULT				nRes;						// 結果
	PAVIFILE			pAVIFile;
	AVIFILEINFO			AVIInfo;
	PAVISTREAM			pAVIStream;
	AVISTREAMINFO		StreamInfo;
	PAVISTREAM			pVideoStream;
	AVISTREAMINFO		VideoInfo;
	PAVISTREAM			pAudioStream;
	AVISTREAMINFO		AudioInfo;
//	double				fAudioLen;					// 音楽長さ(秒単位)

	if ( !fg_bAmAVIInit ) {							// イニシャライズチェック
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
	pMovieData->pVideoFrame = AVIStreamGetFrameOpen( pVideoStream,	// フレームオープン
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
	pMovieData->fVidSampDelay = ( double )pMovieData->VideoInfo.dwScale / pMovieData->VideoInfo.dwRate;	// サンプリング間隔
	//// ビデオが短かすぎればオーディオに合わせる
	//fAudioLen = ( ( double )pMovieData->AudioInfo.dwScale / pMovieData->AudioInfo.dwRate ) * pMovieData->AudioInfo.dwLength;
	//if ( ( pMovieData->fVidSampDelay * pMovieData->VideoInfo.dwLength ) < fAudioLen ) {
	//	pMovieData->fVidSampDelay = fAudioLen / pMovieData->VideoInfo.dwLength;
	//}
	pMovieData->fFirstTime    = -1;
	pMovieData->nPresFrame    = -1;
//	pMovieData->pVideoFrame   = NULL;

	return AM_OK;
}


// ムービーファイルクローズ
extern int AM_CloseMovieFile( AMMOVIE *pMovieData )
{
	if ( !pMovieData->pVideoStream ) return AM_OK;

	if ( pMovieData->pVideoFrame ) {				// ビデオフレーム開放
		AVIStreamGetFrameClose( pMovieData->pVideoFrame );
		pMovieData->pVideoFrame = NULL;
	}
	if ( pMovieData->pVideoStream ) {				// ビデオストリーム開放
		AVIStreamRelease( pMovieData->pVideoStream );
		pMovieData->pVideoStream = NULL;
	}

	if ( pMovieData->pAudioStream ) {				// オーディオストリーム開放
		AVIStreamRelease( pMovieData->pAudioStream );
		pMovieData->pAudioStream = NULL;
	}

	AVIFileRelease( pMovieData->pAVIFile );			// AVIファイル開放

	return AM_OK;
}


extern int AM_PlayMovieFile( int x, int y, AMMOVIE *pMovieData )	// ムービーファイル再生
{
	int					i;
	LPBITMAPINFOHEADER	lpBMPInfoHeader;			// フレームのビットマップポインタ
	HBITMAP				hBitmap;					// 中間ＤＤＢハンドル
	HDC					hDCSrc;						// 転送元ＤＣ
	HDC					hDCDest;					// 転送先ＤＣ
	int					nDispX, nDispY;				// 表示座標
//	int					fNowTime;					// 現在時刻
//	int					fNextTime;					// 次の表示時刻
//	int					fSampDelay;					// サンプリング間隔（時間単位はms）
	double				fNowTime = 1.0f;			// 現在時刻
	double				fNextTime = 0.0f;			// 次の表示時刻
	double				fFirstTime = 0.0f;			// 最初の表示時刻
//	double				fSampDelay;					// サンプリング間隔
	char				*pPicture;					// 画像データポインタ
	WAVEFORMATEX		*pwfSrcWave;				// 元データWaveフォーマット
	WAVEFORMATEX		wfDestWave;					// 解凍Waveフォーマット
	long				nHeaderSize;				// 音声ヘッダサイズバッファ
	int					nResult = AM_OK;			// 実行結果
	HACMSTREAM			hACMStream;					// ＡＣＭストリームハンドル
	ACMSTREAMHEADER		acmHeader;					// ＡＣＭストリームヘッダ
	BYTE				*pAudioSrcBuf, *pAudioDestBuf;	// 音声バッファ
	int					nRes, nRes2 = AVIERR_NODATA;
	MMRESULT			nMMRes;						// ＭＭ　ＡＰＩ結果
	DWORD				nAudioSrcBufSize, nAudioDestBufSize;	// 音声転送元／転送先サイズ
	DWORD				nAudioDestBlockSize;		// 解凍済み音声データブロックサイズ
	DWORD				nAudioBlockNum;				// １単位バッファ中のブロック数
	DWORD				nCurrentAudioBlk;			// 現在のブロック位置
	int					nAudioCount;				// オーディオ制御用カウンタ
	long				nSamples, nBytes;			// 処理したサンプル／バイト数
	LPDIRECTSOUNDBUFFER	lpDSAVIAudio = NULL;		// DirectSound再生バッファ
	LPVOID				lpDSBuf1;
	DWORD				nDSSize1;
	DWORD				nCurrentPlay, nCurrentWrite;
	DWORD				nAudioEndLoc = 0;
	BOOL				bDoubleBuf;
	int					nNowKey;					// 今のキー
	int					nLastKey;					// ひとつ前のキー

	if ( !pMovieData->pVideoStream ) return AM_OK;		// 初期化チェック
	if ( !lpDS ) InitSound( 0 );						// サウンド初期化チェック
	bDoubleBuf = true; //( lpDDSActive != lpDDSPrimary );		// ダブルバッファチェック
	if ( KBHit() ) GetCh();										// キーバッファクリア

//	pMovieData->pVideoFrame = AVIStreamGetFrameOpen( pMovieData->pVideoStream,	// フレームオープン
//													 0 );//(LPBITMAPINFOHEADER)AVIGETFRAMEF_BESTDISPLAYFMT );
//	if ( !pMovieData->pVideoFrame ) return AMERR_NOTSUPPORTED;
//	fSampDelay = ( double )pMovieData->VideoInfo.dwScale / pMovieData->VideoInfo.dwRate;	// サンプリング間隔

	if ( pMovieData->pAudioStream ) {				// オーディオストリーム制御
		nRes = AVIStreamReadFormat( pMovieData->pAudioStream, 0, NULL, &nHeaderSize );	// オーディオフォーマット取得
		pwfSrcWave = ( WAVEFORMATEX * )malloc( nHeaderSize );
		nRes = AVIStreamReadFormat( pMovieData->pAudioStream, 0, pwfSrcWave, &nHeaderSize );

//		int nBits = 8;
		wfDestWave = *pwfSrcWave;					// 音声解凍フォーマットセット
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
			pAudioSrcBuf = ( BYTE * )malloc( nAudioSrcBufSize );	// ソース準備
			pAudioDestBuf = ( BYTE * )malloc( nAudioDestBufSize );	// ディスト準備
			ZeroMemory( &acmHeader, sizeof( acmHeader ) );		// ＡＣＭＨＥＡＤＥＲ準備
			acmHeader.cbStruct = sizeof( acmHeader );
			acmHeader.pbSrc = pAudioSrcBuf;
			acmHeader.cbSrcLength = nAudioSrcBufSize;
			acmHeader.cbSrcLengthUsed = nAudioSrcBufSize;
			acmHeader.pbDst = pAudioDestBuf;
			acmHeader.cbDstLength = nAudioDestBufSize;
			acmHeader.cbDstLengthUsed = nAudioDestBufSize;
			nMMRes = acmStreamPrepareHeader( hACMStream, &acmHeader, 0 );	// 解凍ヘッダ準備
			MakeDSStreamBuf( &wfDestWave, nAudioDestBufSize * 2, &lpDSAVIAudio );	// DirectSoundバッファ準備

			nCurrentAudioBlk = 0;
			nAudioCount = 0;

			for ( i = 1; i <= 2; i++ ) {			// 初期バッファフィル
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
	nNowKey = CheckKey( AMINPUT_MULT );						// ひとつ前のキー
	fFirstTime = GetNowTime( 0 );
	for ( i = 0; i < ( int )pMovieData->VideoInfo.dwLength; i++ ) {
		nLastKey = nNowKey;									// キー引継ぎ
		nNowKey = CheckKey( AMINPUT_MULT );					// 現在のキー
		HCls();										// 画面クリア
		lpBMPInfoHeader = ( LPBITMAPINFOHEADER )AVIStreamGetFrame( pMovieData->pVideoFrame, i );	// フレーム取得
		if ( !lpBMPInfoHeader ) {					// 取得チェック
			nResult = AMERR_NOTSUPPORTED;
			break;
		}
		if ( !bDoubleBuf && i != 0 ) {				// ダブルバッファでない場合ここで待つ
			do {									// 時間スケール調整
				fNowTime = ( GetNowTime( 0 ) - fFirstTime ) / 1000.0;	// 現在の時刻
				if ( !ProcessEvent() ) break;
			 } while ( fNowTime <= fNextTime );
		}
		if ( FAILED( gg_pBackBuffer->GetDC( &hDCDest ) ) ) break;			// 転送先ＤＣ準備
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
		if ( x == MOVIE_FULL_LOC && y == MOVIE_FULL_LOC ) {		// フル画面
			hBitmap = CreateDIBitmap( hDCDest, lpBMPInfoHeader, CBM_INIT,	// ＤＩＢからＤＤＢ作成
									  pPicture, ( BITMAPINFO * )lpBMPInfoHeader,
									  DIB_RGB_COLORS );
			hDCSrc = CreateCompatibleDC( hDCDest );		// 転送元ＤＣ準備
			SelectObject( hDCSrc, hBitmap );
			nRes = StretchBlt( hDCDest, nDispX, nDispY, gg_siDispSize.cx, gg_siDispSize.cy,	// フル画面描画
							   hDCSrc, 0, 0, lpBMPInfoHeader->biWidth, lpBMPInfoHeader->biHeight,
							   SRCCOPY );
			DeleteDC( hDCSrc );							// ＤＣ等開放
			DeleteObject( hBitmap );
		}
		else {											// 通常サイズ
			SetDIBitsToDevice( hDCDest, nDispX + x, nDispY + y, lpBMPInfoHeader->biWidth, lpBMPInfoHeader->biHeight,
							   0, 0, 0, lpBMPInfoHeader->biHeight,
							   pPicture, (LPBITMAPINFO)lpBMPInfoHeader, DIB_RGB_COLORS );
/*			hBitmap = CreateDIBitmap( hDCDest, lpBMPInfoHeader, CBM_INIT,	// ＤＩＢからＤＤＢ作成
									  pPicture, ( BITMAPINFO * )lpBMPInfoHeader,
									  DIB_RGB_COLORS );
			hDCSrc = CreateCompatibleDC( hDCDest );		// 転送元ＤＣ準備
			SelectObject( hDCSrc, hBitmap );
			nRes = BitBlt( hDCDest, nDispX + x, nDispY + y,		// 実サイズ描画
									lpBMPInfoHeader->biWidth, lpBMPInfoHeader->biHeight,
						   hDCSrc, 0, 0, SRCCOPY );
			DeleteDC( hDCSrc );						// ＤＣ等開放
			DeleteObject( hBitmap );*/
		}
		gg_pBackBuffer->ReleaseDC( hDCDest );
//		if ( !ProcessEvent() || ( nNowKey & ~nLastKey ) ) break;
		if ( !ProcessEvent() || KBHit() ) break;
		if ( i == 0 ) {
			fNowTime = ( GetNowTime( 0 ) - fFirstTime ) / 1000.0;	// 現在の時刻
			fNextTime = fNowTime;// + pMovieData->fVidSampDelay;	// 次の表示時刻
			if ( lpDSAVIAudio ) lpDSAVIAudio->Play( 0, 0, DSBPLAY_LOOPING );
		}
		else {
/*			char szOut[80];
			fNowTime = ( double )GetNowTime( 0 ) / 1000;		// 現在の時刻
//			DrawFBox( 0, 0, 320, 16, 0 );
			sprintf( szOut, "%d", ( int )( ( fNextTime - fNowTime ) * 1000 ) );
			DrawString( 0, 0, szOut, 0xffffffff );*/
			if ( bDoubleBuf ) {						// ダブルバッファの場合ここで待つ
				do {									// 時間スケール調整
					fNowTime = ( GetNowTime( 0 ) - fFirstTime ) / 1000.0;	// 現在の時刻
					if ( !ProcessEvent() ) break;
				 } while ( fNowTime <= fNextTime );
			}
		}
		fNextTime += pMovieData->fVidSampDelay;
		if ( fNowTime > fNextTime ) {				// コマ飛ばし
			i++;
			fNextTime += pMovieData->fVidSampDelay;
		}
		Flip();
		if ( lpDSAVIAudio ) {						// 音声バッファリフレッシュ
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
	while ( fNowTime <= fNextTime ) {				// 時間待ち
		fNowTime = ( GetNowTime( 0 ) - fFirstTime ) / 1000.0;		// 現在の時刻
		if ( !ProcessEvent() ) break;
	}

	if ( lpDSAVIAudio ) {							// 音声再生ある場合
		lpDSAVIAudio->Stop();						// DirectSound側終了
		lpDSAVIAudio->Release();
		nMMRes = acmStreamUnprepareHeader( hACMStream, &acmHeader, 0 );	// ACM側終了
		acmStreamClose( hACMStream, 0 );
		free( pAudioSrcBuf );						// 音声バッファ開放
		free( pAudioDestBuf );
		free( pwfSrcWave );
	}

	return nResult;
}


// ムービーの特定フレーム取得
extern int AM_GetMovieFrame( AMMOVIE *pMovieData, int nFrameLoc,
							 AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo )
{
	HDC							hDCDest;					// 転送先ＤＣ
	LPBITMAPINFOHEADER			lpBMPInfoHeader;			// フレームのビットマップポインタ
	char						*pPicData;
	HRESULT						hRes;						// 結果
	AM_LPDISPLAYBUFFER			pTempBuffer;				// テンポラリバッファ
	LPDIRECT3DSURFACE9			lpTopSurf;					// テクスチャトップサーフェス
	RECT						rcDest;						// 転送元・転送先矩形

	lpBMPInfoHeader = ( LPBITMAPINFOHEADER )AVIStreamGetFrame( pMovieData->pVideoFrame,
															   nFrameLoc );	// フレーム取得
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

	hRes = pPicture->GetSurfaceLevel( 0, &lpTopSurf );		// 転送先サーフェス取得
	SetRect( &rcDest, 0, 0, pPicInfo->nWidth, pPicInfo->nHeight );	// 転送先矩形
	// 転送
	D3DXLoadSurfaceFromSurface( lpTopSurf, NULL, &rcDest, pTempBuffer, NULL, NULL, D3DX_FILTER_LINEAR, 0 );
	pTempBuffer->Release();									// 転送元サーフェス開放
	lpTopSurf->Release();									// 転送先サーフェス開放

	return AM_OK;
}
// ---------------------------------------------------------------------------
//
// ファイル名: AM_Time.cpp
//
// 機能: DirectX上でのＡＭＧＡＭＥの時間機能を実装。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#pragma warning( disable : 4819 )						// コードページの警告抑止

#include <windows.h>
#include <mmsystem.h>

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// キー入力　ブリッジレイヤー
#include "AM_Time.h"											// 時間機能　機種依存レイヤー

#endif


// ---------------------------------------------------------------------------
// ファイル内グローバル変数域



//-----------------------------------------------------------------------------
// 時間関数域

// 現在時間取得関数
extern int AM_GetPresentTime( void )
{
	return timeGetTime();
//	return GetTickCount();
}


// 日時の取得
extern int AM_GetDateTime( AMDATETIME *pDateTime )
{
	WORD		nDate, nTime;						// 日時
	FILETIME	ftFileTime;							// ファイルタイムバッファ
	SYSTEMTIME	stSystemTime;						// システムタイムバッファ

	GetLocalTime( &stSystemTime );					// システムタイム取得
	SystemTimeToFileTime( &stSystemTime, &ftFileTime );	// ファイルタイム変換
	FileTimeToDosDateTime( &ftFileTime, &nDate, &nTime );
	pDateTime->nPackedTime = ( (DWORD)nDate << 16 ) + nTime;	// パックト時間
	pDateTime->nYear   = (unsigned int )stSystemTime.wYear;		// 年
	pDateTime->nMonth  = (unsigned char)stSystemTime.wMonth;	// 月
	pDateTime->nDay    = (unsigned char)stSystemTime.wDay;		// 日
	pDateTime->nHour   = (unsigned char)stSystemTime.wHour;		// 時
	pDateTime->nMinute = (unsigned char)stSystemTime.wMinute;	// 分
	pDateTime->nSecond = (unsigned char)stSystemTime.wSecond;	// 秒

	return AM_OK;
}


//-----------------------------------------------------------------------------
// 以上

// ＡＭＧＡＭＥ　ＤｉｒｅｃｔＳｏｕｎｄモジュール
// Version 2.4
// Programmed by K.Katoh, Amusement Media Academy

#pragma warning( disable : 4819 )						// コードページの警告抑止

//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <process.h>
#include <ddraw.h>
#include <dsound.h>
#include "amgame.h"

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_Sound.h"								// サウンドヘッダ

#endif

#define MAXSOUNDNUM			200						// 最大サウンド数
#define NOSOUNDSIZE			1000					// 無音サイズ
#define	STREAM_BUF_SIZE		0x10000					// ストリーミングバッファサイズ

#define SOUND_WAV			0
#define SOUND_MIDI			1

#define DEF_VOLUME			80						// デフォルトの音量

#define BUFFERDESC_SIZE		20						// DSBUFFERDESC構造体サイズ

#define WAVE_H_BUF_SIZE		256						// ヘッダバッファサイズ		

static HRESULT AMDSErr( HWND hOutWnd, HRESULT ErrorCode );	// エラー処理
static int LoadWavBuffer( char *szWavFile, LPDIRECTSOUNDBUFFER *lplpDSBDest );
static int MakeSilentBuffer( LPDIRECTSOUNDBUFFER *lplpDSBDest );	// 無音バッファ生成
static int WaitEndWav( LPDIRECTSOUNDBUFFER lpWaitBuf );		// Ｗａｖ再生終了待ち
static int StartMciMidi( char *szFileName );				// MCI MIDI再生

typedef struct tagMEMSOUND {							// メモリＷａｖ管理構造体
	int		nSoundKind;
	int		nSBufLoc;
	char	*szSoundName;
} MEMSOUND;

typedef struct tagSTREAM_SOUND {						// ストリーミング管理構造体
	HANDLE			hWavFile;							// ファイルハンドル
	int				nFileSize;							// ファイルサイズ
	int				nFileRemain;						// 読み残しバイト数
	int				nBufRemain;							// バッファ残りバイト数
	WAVEFORMATEX	wfWaveFormat;						// WAVフォーマット
	char			cHeadLevel;							// 先頭レベル
	int				bProcessing;						// 処理中フラグ
} STREAM_SOUND;

LPDIRECTSOUND			lpDS				= NULL;		// DirectSoundオブジェ
LPDIRECTSOUNDBUFFER		lpDSBPrimary		= NULL;		// プライマリバッファ
LPDIRECTSOUNDBUFFER		lpDSBSilent			= NULL;		// 無音バッファ
LPDIRECTSOUNDBUFFER		lpDSBWork			= NULL;		// 作業用バッファ
LPDIRECTSOUNDBUFFER		lpDSBSecondary[MAXSOUNDNUM];	// ストア用バッファ

LPDIRECTSOUNDBUFFER		lpDSBStream         = NULL;		// ストリームバッファ
LPDIRECTSOUNDNOTIFY		lpDSStrNotify       = NULL;		// 通知インタフェース


MEMSOUND		WavInfo[MAXSOUNDNUM];
int				hSoundLoc, nSBufLoc;
STREAM_SOUND	ssStreamData;							// ストリームデータ
int				bStreamThread = false;					// ストリームスレッド
int				nStreamLastPos;							// ストリームのひとつ前位置
int				nStreamReadEnd = 0;						// ストリーム読み出し終了回数

HMIDIOUT		hMidi;
UINT			wMidiID = NULL;							// MIDIデバイスＩＤ
BOOL			bMidiPlaying = 0;						// MIDI演奏中フラグ
BOOL			bMidiLooping = FALSE;					// MIDIループ中フラグ

void WaitForDSBufEnd( LPVOID lpv );						// バッファ終了通知

#define DSCHKINIT if( dsrval != DD_OK ) return AMDSErr( gg_hWindow, dsrval );
extern int InitSound( int nSoundMode )				// サウンドの初期化
{
	int				i, nResCode;
	HRESULT			dsrval;
	DSBUFFERDESC	dsbd;

	if ( gg_hWindow == NULL ) return AMERR_NOINITIALIZE;
	dsrval = DirectSoundCreate( NULL, &lpDS, NULL );	// ＤＳ作成
	DSCHKINIT
	dsrval = lpDS->SetCooperativeLevel( gg_hWindow, DSSCL_NORMAL );	// 協調レベル
	DSCHKINIT

	ZeroMemory( &dsbd, sizeof( dsbd ) );				// プライマリバッファ
	dsbd.dwSize = BUFFERDESC_SIZE;
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	dsrval = lpDS->CreateSoundBuffer( &dsbd, &lpDSBPrimary, NULL );
	DSCHKINIT

	for ( i = 0; i < nSBufLoc; i++ ) {				// バッファ初期化
		lpDSBSecondary[i] = NULL;
	}
	hSoundLoc = 1;  nSBufLoc = 0;

	if ( ( nResCode = MakeSilentBuffer( &lpDSBSilent ) ) < 0 ) {
		return nResCode;									// 無音再生
	}
	lpDSBSilent->Play( 0, 0, DSBPLAY_LOOPING );

	SetPcmVolume( DEF_VOLUME );
	SetMusicVolume( DEF_VOLUME );

	ssStreamData.hWavFile = INVALID_HANDLE_VALUE;		// ストリームWavファイルハンドル初期化
	ssStreamData.bProcessing = false;					// 処理中フラグ

	return 0;
}

// DirectDrawエラー処理
static HRESULT AMDSErr( HWND hOutWnd, HRESULT ErrorCode )
{
	char buf[80];

	if ( ErrorCode == DSERR_NODRIVER ) return AMERR_NODRIVER;

	wsprintf( buf, "DirectSoundエラー (%08lx)\n", ErrorCode );
	MessageBox( hOutWnd, buf, "ERROR", MB_OK );

	return AMERR_INITIALIZE;
}

extern int EndSound( void )							// サウンド使用終了
{
	StopPcm();										// 音楽停止
	EndDSound();

	return 0;
}

extern int EndDSound( void )						// 内部サウンド使用終了
{
	InitSoundMem();									// 一般サウンドバッファの開放

	if ( lpDSBStream ) {
		while ( bStreamThread &&    ssStreamData.bProcessing   );
		lpDSBStream->Stop();						// 演奏停止
		lpDSBStream->Release();						// サウンドバッファ開放
		lpDSBStream = NULL;
	}

	if ( lpDSBWork ) {								// ワーク用バッファ開放
		lpDSBWork->Stop();
		lpDSBWork->Release();
		lpDSBWork = NULL;
	}
	if ( lpDSBSilent ) {							// 無音バッファ開放
		lpDSBPrimary->Stop();
		lpDSBSilent->Stop();
		lpDSBSilent->Release();
		lpDSBSilent = NULL;
	}
	if ( lpDSBPrimary ) {							// プライマリバッファ開放
		lpDSBPrimary->Release();
		lpDSBPrimary = NULL;
	}

	if ( lpDS ) {									// DirectSound開放
		lpDS->Release();
		lpDS = NULL;
	}

	if ( wMidiID ) {								// ＭＩＤＩ停止
		mciSendCommand( wMidiID, MCI_CLOSE, 0, NULL );
		bMidiPlaying = 0;
	}

	return 0;
}

extern int InitSoundMem( void )						// サウンドメモリの初期化
{
	int		i;

	if ( !lpDS ) return AMERR_NOINITIALIZE;			// 初期化チェック

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

extern int PlayWav( char *szFileName )				// Ｗａｖファイル直接再生
{
	int				nResCode;
	HRESULT			dsrval;

	if ( !lpDS ) return AMERR_NOINITIALIZE;
	if ( ( nResCode = LoadWavBuffer( szFileName, &lpDSBWork ) ) < 0 )
		return nResCode;

	dsrval = lpDSBWork->Play( 0, 0, 0 );			// 再生開始
	if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
	WaitEndWav( lpDSBWork );
	return 0;
}

extern int PlayWavBack( char *szFileName )			// Wav直接バック再生
{
	int				nResCode;
	HRESULT			dsrval;

	if ( !lpDS ) return AMERR_NOINITIALIZE;
	if ( ( nResCode = LoadWavBuffer( szFileName, &lpDSBWork ) ) < 0 )
		return nResCode;

	dsrval = lpDSBWork->Play( 0, 0, 0 );			// 再生開始
	if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;

	return 0;
}

extern int PlayWavStreamBack( char *szFileName )			// Wavストリームバック再生
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
	int				nHeaderSize;							// ヘッダサイズ
	int				nReadSize;								// リードサイズ

	if ( !lpDS ) return AMERR_NOINITIALIZE;			// 初期化チェック

	if( ( hrscr = FindResource( NULL, szFileName, "WAVE" ) ) != NULL ) {
		if ( ( nResCode = LoadWavBuffer( szFileName, &lpDSBWork ) ) < 0 )
			return nResCode;

		dsrval = lpDSBWork->Play( 0, 0, 0 );			// 再生開始
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
			dsrval = lpDSBStream->Stop();				// 演奏停止
			if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
			dsrval = lpDSBStream->Release();			// サウンドバッファ開放
			if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
			lpDSBStream = NULL;
			if ( lpDSStrNotify ) {						// 通知INT開放
				lpDSStrNotify->Release();
				lpDSStrNotify = NULL;
			}
		}
		if ( ssStreamData.hWavFile != INVALID_HANDLE_VALUE ) {
			CloseHandle( ssStreamData.hWavFile );		// ファイルクローズ
			ssStreamData.hWavFile = INVALID_HANDLE_VALUE;
		}
		hWavFile = CreateFile( szFileName, GENERIC_READ, FILE_SHARE_READ,	// ﾌｧｲﾙｵｰﾌﾟﾝ
							   NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		if ( hWavFile == INVALID_HANDLE_VALUE ) return AMERR_NOFILE;	// 成功チェック
		ssStreamData.hWavFile = hWavFile;

		nWavSize = GetFileSize( hWavFile, NULL );		// ファイルサイズ
		ssStreamData.nFileSize = nWavSize;

		ReadFile( hWavFile, cHeadBuffer, WAVE_H_BUF_SIZE, &dwRead, NULL );	// データ読み出し
		if ( *( DWORD * )cHeadBuffer != mmioFOURCC( 'R', 'I', 'F', 'F' ) )
			return AMERR_INVALIDFILE;
		if ( *( DWORD * )( &( cHeadBuffer[8] ) ) != mmioFOURCC( 'W', 'A', 'V', 'E' ) )
			return AMERR_INVALIDFILE;

		CopyMemory( &( ssStreamData.wfWaveFormat ), &( cHeadBuffer[20] ), sizeof( WAVEFORMATEX ) );	// フォーマットコピー

		lpWavData = NULL;								// データ位置サーチ
		for ( i = 0; i < WAVE_H_BUF_SIZE - 10; i++ ) {
			if ( ( *( DWORD * )( &( cHeadBuffer[i] ) ) == mmioFOURCC( 'd', 'a', 't', 'a' ) ) &&
				 ( *( DWORD * )( &( cHeadBuffer[i + 4] ) ) != 0 ) )
			{
				nHeaderSize = i + 8;
				lpWavData = &( cHeadBuffer[i] );
				ssStreamData.cHeadLevel = *( lpWavData + 8 + 1 );	// 先頭レベル
				break;
			}
		}
		if ( lpWavData == NULL ) return AMERR_INVALIDFILE;
		MakeDSStreamBuf( &( ssStreamData.wfWaveFormat ), STREAM_BUF_SIZE, &lpDSBStream );	// バッファ作成
		if ( !lpDSBStream && ( ssStreamData.hWavFile != INVALID_HANDLE_VALUE ) ) {
			CloseHandle( hWavFile );
			ssStreamData.hWavFile = INVALID_HANDLE_VALUE;
			return AMERR_SOUNDOUT;
		}
		ssStreamData.nFileRemain = *( DWORD * )( lpWavData + 4 );
		SetFilePointer( hWavFile, nHeaderSize, NULL, FILE_BEGIN );		// データの先頭に
		nReadSize = ( ssStreamData.nFileRemain < STREAM_BUF_SIZE / 2 ) ? ssStreamData.nFileRemain : STREAM_BUF_SIZE / 2;

		char		*lpDSBuf1;
		int			nDSSize1;
		lpDSBStream->Lock( 0, nReadSize, 
							( LPVOID * )&lpDSBuf1, ( LPDWORD )&nDSSize1, NULL, NULL, 0 );
		ReadFile( hWavFile, lpDSBuf1, nReadSize, &dwRead, NULL );	// データ読み出し
		if ( nReadSize < STREAM_BUF_SIZE / 2 ) FillMemory( lpDSBuf1 + nReadSize, STREAM_BUF_SIZE / 2 - nReadSize,
														   ssStreamData.cHeadLevel );
		lpDSBStream->Unlock( lpDSBuf1, nDSSize1, NULL, 0 );
		ssStreamData.nFileRemain -= nReadSize;
		if ( ( ssStreamData.nFileRemain == 0 ) && ( ssStreamData.hWavFile != INVALID_HANDLE_VALUE ) ) {
			CloseHandle( hWavFile );								// ﾌｧｲﾙｸﾛｰｽﾞ
			ssStreamData.hWavFile = INVALID_HANDLE_VALUE;
		}
		ssStreamData.nBufRemain = nReadSize;
		nStreamLastPos = STREAM_BUF_SIZE / 2 + 1;
		nStreamReadEnd = 0;

		if ( !bStreamThread ) {
			_beginthread( WaitForDSBufEnd, 0, NULL );
			bStreamThread = true;
		}

		dsrval = lpDSBStream->Play( 0, 0, DSBPLAY_LOOPING );	// 再生開始
		if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
	}

	return 0;
}


void WaitForDSBufEnd( LPVOID lpv )						// バッファ終了通知
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
		ssStreamData.bProcessing = true;								// 処理中
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
				ReadFile( ssStreamData.hWavFile, lpDSBuf1, nReadSize, &dwRead, NULL );	// データ読み出し
				if ( nReadSize < STREAM_BUF_SIZE / 2 ) FillMemory( lpDSBuf1 + nReadSize, STREAM_BUF_SIZE / 2 - nReadSize,
																   ssStreamData.cHeadLevel );
				ssStreamData.nFileRemain -= nReadSize;
				if ( ( ssStreamData.nFileRemain == 0 ) && ( ssStreamData.hWavFile != INVALID_HANDLE_VALUE ) ) {
					CloseHandle( ssStreamData.hWavFile );	// ﾌｧｲﾙｸﾛｰｽﾞ
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
		Sleep( 20 );										// タスク休止
		ssStreamData.bProcessing = false;					// 処理中でなくす
		Sleep( 90 );										// タスク休止
	}
	ssStreamData.bProcessing = false;						// 処理中でなくす
}


extern int CheckPcm( void )							// ＰＣＭ演奏中チェック
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


extern int StopPcm( void )							// ＰＣＭ演奏停止
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
		dsrval = lpDSBStream->Stop();				// 演奏停止
		if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
		dsrval = lpDSBStream->Release();			// サウンドバッファ開放
		if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
		lpDSBStream = NULL;
		if ( ssStreamData.hWavFile != INVALID_HANDLE_VALUE ) {
			CloseHandle( ssStreamData.hWavFile );	// ファイルクローズ
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

	if ( !lpDS ) return AMERR_NOINITIALIZE;			// 初期化チェック

	if( ( hrscr = FindResource( NULL, szWavFile, "WAVE" ) ) != NULL ) {
		lpWavBuff = ( char * )LockResource( LoadResource( NULL, hrscr ) );
		bFileFlag = FALSE;
	}
	else {
		hWavFile = CreateFile( szWavFile, GENERIC_READ, FILE_SHARE_READ,	// ﾌｧｲﾙｵｰﾌﾟﾝ
							   NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		if ( hWavFile == INVALID_HANDLE_VALUE ) return AMERR_NOFILE;	// 成功チェック

		nWavSize = GetFileSize( hWavFile, NULL );		// ファイルサイズ
		hWavBuff = GlobalAlloc( GHND, nWavSize );		// メモリ確保
		lpWavBuff  = ( char * )GlobalLock( hWavBuff );	// ポインタ取得

		ReadFile( hWavFile, lpWavBuff, nWavSize, &dwRead, NULL );	// データ読み出し
		CloseHandle( hWavFile );								// ﾌｧｲﾙｸﾛｰｽﾞ
		if ( *( DWORD * )lpWavBuff != mmioFOURCC( 'R', 'I', 'F', 'F' ) )
			return AMERR_INVALIDFILE;
		if ( *( DWORD * )( lpWavBuff + 8 ) != mmioFOURCC( 'W', 'A', 'V', 'E' ) )
			return AMERR_INVALIDFILE;

		bFileFlag = TRUE;
	}

	lpWavData = NULL;								// データ位置サーチ
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
		GlobalUnlock( hWavBuff );					// メモリ開放
		GlobalFree( hWavBuff );
	}

	return nResult;
}


extern int MakeDSBuffer( LPWAVEFORMATEX lpWaveForm,		// DirectSoundバッファ作成
						 char *pWavBuffer, DWORD nBufferSize,
						 LPDIRECTSOUNDBUFFER *lplpDSBDest )
{
	DSBUFFERDESC	dsbd;
	HRESULT			dsrval;
	LPVOID			lpDSBuf1, lpDSBuf2;
	DWORD			nDSSize1, nDSSize2;

	if ( !lpDS ) return AMERR_NOINITIALIZE;			// 初期化チェック

	ZeroMemory( &dsbd, sizeof( dsbd ) );			// サウンドバッファ構造体セット
	dsbd.dwSize = BUFFERDESC_SIZE;
	dsbd.dwFlags = DSBCAPS_STATIC;
	dsbd.dwBufferBytes = nBufferSize;				// データバイト数
	dsbd.lpwfxFormat = lpWaveForm;					// フォーマット
	dsrval = lpDS->CreateSoundBuffer( &dsbd, lplpDSBDest, NULL );	// ﾊﾞｯﾌｧ生成
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


extern int MakeDSStreamBuf( LPWAVEFORMATEX lpWaveForm,	// DirectSoundストリーミングバッファ作成
							int nBufferSize, LPDIRECTSOUNDBUFFER *lplpDSBDest )
{
	DSBUFFERDESC	dsbd;
	HRESULT			dsrval;

	if ( !lpDS ) return AMERR_NOINITIALIZE;			// 初期化チェック

	ZeroMemory( &dsbd, sizeof( dsbd ) );			// サウンドバッファ構造体セット
	dsbd.dwSize = BUFFERDESC_SIZE;
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = nBufferSize;				// データバイト数
	dsbd.lpwfxFormat = lpWaveForm;					// フォーマット
	dsrval = lpDS->CreateSoundBuffer( &dsbd, lplpDSBDest, NULL );	// ﾊﾞｯﾌｧ生成
	if ( dsrval != DS_OK ) return AMERR_NOMEMORY;

	return 0;
}


static int MakeSilentBuffer( LPDIRECTSOUNDBUFFER *lplpDSBDest )	// 無音バッファ生成
{
	DSBUFFERDESC	dsbd;
	HRESULT			dsrval;
	LPVOID			lpDSBuf1, lpDSBuf2;
	DWORD			nDSSize1, nDSSize2;
	WAVEFORMATEX	wfBlank;

	if ( !lpDS ) return AMERR_NOINITIALIZE;

	wfBlank.wFormatTag = 1;								// WAVEFORMATEX構造体
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
	ZeroMemory( &dsbd, sizeof( dsbd ) );			// サウンドバッファ構造体セット
	dsbd.dwSize = BUFFERDESC_SIZE;
	dsbd.dwFlags = DSBCAPS_STATIC;
	dsbd.dwBufferBytes = NOSOUNDSIZE;
	dsbd.lpwfxFormat = &wfBlank;
	dsrval = lpDS->CreateSoundBuffer( &dsbd, lplpDSBDest, NULL );	// ﾊﾞｯﾌｧ生成
	if ( dsrval != DS_OK ) return AMERR_NOMEMORY;

	(*lplpDSBDest)->Lock( 0, NOSOUNDSIZE, &lpDSBuf1, &nDSSize1,
						&lpDSBuf2, &nDSSize2, 0 );
	FillMemory( lpDSBuf1, nDSSize1, 0x80 );			// 無音を満たす
	if ( nDSSize2 != 0 ) {
		FillMemory( lpDSBuf2, nDSSize2, 0x80 );
	}
	(*lplpDSBDest)->Unlock( lpDSBuf1, nDSSize1, lpDSBuf2, nDSSize2 );

	return 0;
}

static int WaitEndWav( LPDIRECTSOUNDBUFFER lpWaitBuf )	// ＷＡＶ再生終了待ち
{
	int				i;
	BOOL			bEndPlay;
	DWORD			dwStat;
	HRESULT			dsrval;

	if ( !lpDS || !lpWaitBuf ) return AMERR_NOINITIALIZE;
	bEndPlay = 0;
	while( ProcessEvent() ) {						// 再生中チェック
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

extern int LoadWavMem( char *szFileName )			// ＷＡＶのメモリへのロード
{
	int		nResCode;

	if ( !lpDS ) return AMERR_NOINITIALIZE;
	if ( hSoundLoc >= MAXSOUNDNUM ) return AMERR_NOFREEHANDLE;
	if ( ( nResCode = LoadWavBuffer( szFileName, &lpDSBSecondary[nSBufLoc] ) )
		 < 0 ) return nResCode;

    WavInfo[hSoundLoc].nSoundKind = SOUND_WAV;	// サウンド管理構造体セット
    WavInfo[hSoundLoc].nSBufLoc = nSBufLoc;
	
	hSoundLoc++;
	nSBufLoc++;

	return hSoundLoc - 1;
}

extern int PlayMem( int hSound )					// メモリ音声の再生
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

extern int PlayMemBack( int hSound )				// メモリバック再生
{
	HRESULT			dsrval;

	if ( !lpDS ) return AMERR_NOINITIALIZE;
	if ( hSound < 0 || hSound >= hSoundLoc ) return AMERR_INVALIDHANDLE;
	if ( WavInfo[ hSound ].nSoundKind != SOUND_WAV ) return AMERR_INVALIDFORM;
	
	dsrval = lpDSBSecondary[ WavInfo[hSound].nSBufLoc ]->Stop();	// 停止
	dsrval = lpDSBSecondary[ WavInfo[hSound].nSBufLoc ]->		// 先頭へ
			 SetCurrentPosition( 0 );
	if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
	dsrval = lpDSBSecondary[ WavInfo[hSound].nSBufLoc ]->Play( 0, 0, 0 );
	if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;

	return 0;
}

extern int PlayMemLoop( int hSound )				// メモリループ再生
{
	HRESULT			dsrval;

	if ( !lpDS ) return AMERR_NOINITIALIZE;
	if ( hSound < 0 || hSound >= hSoundLoc ) return AMERR_INVALIDHANDLE;
	if ( WavInfo[ hSound ].nSoundKind != SOUND_WAV ) return AMERR_INVALIDFORM;
	
	dsrval = lpDSBSecondary[ WavInfo[hSound].nSBufLoc ]->		// 先頭へ
			 SetCurrentPosition( 0 );
	if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;
	dsrval = lpDSBSecondary[ WavInfo[hSound].nSBufLoc ]->Play( 0, 0, DSBPLAY_LOOPING );
	if ( dsrval != DS_OK ) return AMERR_SOUNDOUT;

	return 0;
}

extern int CheckPlayMem( int hSound )				// メモリ演奏中チェック
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

extern int StopPlayMem( int hSound )				// メモリ演奏ストップ
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

extern int SetPcmVolume( int nVolume )				// ＰＣＭボリウムのセット
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


extern int SetMemVolume( int nVolume, int hSound )	// メモリＰＣＭボリウムのセット
{
	HRESULT			dsrval;
	long			nDSVolume;						// DirectSoundボリウム
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


extern int PlayMidi( char *szFileName )				// MIDIファイルの演奏
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

extern int PlayMidiBack( char *szFileName )			// MIDIファイルのバック演奏
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

extern int PlayMidiLoop( char *szFileName )			// MIDIファイルのループ演奏
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


static int StartMciMidi( char *szFileName )			// MCI MIDI再生
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

extern int CheckMusic( void )						// ＭＩＤＩ演奏中チェック
{
	return bMidiPlaying;
}

extern int StopMusic( void )						// ＭＩＤＩ演奏ストップ
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

extern int SetMusicVolume( DWORD nVolume )			// ＭＩＤＩ音量セット
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
