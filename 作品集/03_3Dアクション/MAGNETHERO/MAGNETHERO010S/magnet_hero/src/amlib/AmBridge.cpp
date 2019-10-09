// ---------------------------------------------------------------------------
//
// ファイル名: AB_DispEnv.cpp
//
// 機能: ＡＭＧＡＭＥの画面環境設定をブリッジ。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif

#pragma warning( disable : 4819 )						// コードページの警告抑止


// ---------------------------------------------------------------------------
// インクルード域

#include <windows.h>
#include <string.h>

#include "amgame.h"												// AMGAMEヘッダ

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// 画面初期化　機種依存レイヤー
#include "AB_DispEnv.h"											// 画面初期化　ブリッジ
#include "AM_2DGraph.h"											// 2D描画　機種依存レイヤー
#include "AB_2DGraph.h"											// 2D描画　ブリッジ
#include "AM_3DGraph.h"											// 3D描画　機種依存レイヤー
#include "AM_FigureDraw.h"										// 2D図形描画　機種依存レイヤー
#include "AM_Movie.h"											// ムービー再生　機種依存レイヤー
#include "AB_Movie.h"											// ムービー再生　ブリッジ
#include "AM_Input.h"											// キー入力　機種依存レイヤー
#include "AB_Debug.h"											// デバッグ　ブリッジ

#endif


// ---------------------------------------------------------------------------
// ファイル内グローバル変数域

int								fg_nFrameCount = 0;				// フレームカウンタ


//-----------------------------------------------------------------------------
// 画面制御関数域

// グラフィックスの初期化
extern HWND InitGraph( HINSTANCE hInstance )
{
	int					nResult;								// リザルトコード
	int					nPresDebugMode;							// 現在のデバッグモード

	AM_SetAppliInstance( hInstance );							// アプリインスタンスの設定

	// ディスプレイ初期化
	nResult = AM_InitDisplay();
	if ( nResult != AM_OK ) {									// 初期化失敗の場合
		AM_EndDisplay();
		return NULL;
	}

	AM_Init3DGraph();											// 3Dグラフィック初期化

	AM_InitBlendState();										// ブレンドステート初期化

	AM_PrepareFontObject();										// 文字列フォントの準備

	// デバッグ機能抑制
	nPresDebugMode = GetAMDebugMode();							// 現在のデバッグモードを保存
	SetAMDebugMode( AMDEBUG_ALL_OFF );							// デバッグ機能ＯＦＦ

	int							hTitle;							// タイトル

	hTitle = LoadTexture( AM_TITLE_NAME );
	if ( hTitle < 0 ) {
		if ( gg_bFullMode ) {
			DrawString( 0, 0, "Amgamew.sysがありません！！", GetColor( 255, 0, 128 ) );
			Flip();
			GetCh();
		}
		else {
			MessageBox( gg_hWindow, "Amgamew.sysがありません。", "Error", MB_OK );
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

	AmVertex		avVertices[480 * 2];			// 頂点

	// 頂点情報の初期化（ラスタスクロール用）
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
		Draw3DLines( avVertices, 480 * 2, hTitle );	// 線描画
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
		Draw3DLines( avVertices, 480 * 2, hTitle );	// 線描画
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

	AB_Release2DGraphs();										// 画像メモリ開放
	nResult = AM_ClearBackBuffer( 0, 0, 0 );					// 画面更新処理

	AM_InitJoyPad( AM_MAX_PAD_NUM );							// パッド初期化

	if ( KBHit() ) GetCh();										// キーバッファクリア

	RestoreAMDebugMode( nPresDebugMode );						// デバッグモード復帰
	DeleteFile( AB_DEBUG_FILE_NAME );							// デバッグファイル消去

	SetFrameCounter( 1 );										// フレームカウンタセット

	return gg_hWindow;											// ウィンドウハンドルを返す
}


// グラフィックス使用終了
extern int EndGraph( void )
{
	int					nResult;								// リザルトコード

	AM_EndJoyPad();												// パッド使用終了
	AM_EndMouse();												// マウス使用終了

//	AM_StopMovie();												// ムービー再生ストップ
	InitMovieMem();												// ムービーメモリ開放

	AB_Release2DGraphs();										// 2Dグラフィック開放

	AM_ReleaseFontObject();										// 文字列フォントの開放

	AM_End3DGraph();											// 3Dグラフィック使用終了

	nResult = AM_EndDisplay();									// ディスプレイ使用終了

	return nResult;
}


// 環境依存グローバルモード設定
extern int SetGlobalMode( int nModes )
{
	int							nResult = AM_OK;				// 結果

	switch ( nModes ) {
	case AMMODE_WINDOWED:										// ウィンドウモードに設定
		if ( !gg_bDispInitialized ) {							// 画面初期化がまだ
			gg_bFullMode = false;
		}
		nResult = AM_OK;
		break;
	}

	return nResult;
}


// イベント処理
extern int ProcessEvent( void )
{
	int					nResult;								// リザルトコード

	nResult = AM_ProcessEvent();								// イベント処理

	return nResult;
}


// グラフィックモード設定
extern int SetGraphMode( int nWidth, int nHeight, int nBitDepth )
{
	int					nResult;								// リザルトコード

	nResult = AM_SetDisplayMode( nWidth, nHeight, nBitDepth );	// 解像度設定

	return nResult;
}


// ページ設定（ダミー）
extern void SetPage( int nActivePage, int nDisplayPage )
{

	return;
}


// 画面更新処理
extern int Flip( void )
{
	int					nResult;								// リザルトコード

	// デバッグ情報の表示
	if ( gg_bDisplayDebug ) {
		AB_DisplayDebugInfo();									// デバッグ情報表示
	}

	nResult = AM_Flip();										// 画面更新処理

	// デバッグキー・ポーズコントロール
	if ( gg_bDebugKey ) {
		AB_ControlDebugPause();									// デバッグキー・ポーズコントロール
	}

	fg_nFrameCount++;

	return nResult;
}


// フレームカウンタセット
extern int SetFrameCounter( int nCount )
{
	int							nResult;						// リザルトコード

	nResult = fg_nFrameCount;
	fg_nFrameCount = nCount;

	return nResult;
}


// フレームカウンタ取得
extern int GetFrameCounter( void )
{
	return fg_nFrameCount;
}


//	VRAMクリア
extern void ClearVram( void )
{
	int					nResult;								// リザルトコード

	nResult = AM_ClearBackBuffer( 0, 0, 0 );					// 画面更新処理

	return;
}


// 画面クリア
extern int HCls( void )
{
	int					nResult;								// リザルトコード

	nResult = AM_ClearBackBuffer( 0, 0, 0 );					// 画面更新処理

	return nResult;
}


// ディスプレイの垂直同期信号待ち
extern void WaitVSync( int nWaitNum )
{
	int					i;
	int					nResult;								// リザルトコード

	if ( nWaitNum <= 0 ) return;								// 回数チェック

	for ( i = 0; i < nWaitNum; i++ ) {
		nResult = AM_WaitVBlankTrigger();						// VBlankトリガ待ち
	}

	return;
}


// 描画エリアの設定
extern void SetDrawArea( int x1, int y1, int x2, int y2 )
{
	int							nTemp;

	// 大小関係補正
	if ( x1 > x2 ) { nTemp = x1;  x1 = x2;  x2 = nTemp; }
	if ( y1 > y2 ) { nTemp = y1;  y1 = y2;  y2 = nTemp; }

	// サチレーション
	if ( x1 < 0 ) x1 = 0;
	if ( y1 < 0 ) y1 = 0;
	if ( x2 > gg_siDispSize.cx ) x2 = gg_siDispSize.cx;
	if ( y2 > gg_siDispSize.cy ) y2 = gg_siDispSize.cy;

	// ビューポート設定
	AM_SetViewport( x1, y1, x2, y2 );

	return;
}


// ウィンドウタイトルの設定（非公開関数）
extern int AB_SetWindowTitle( char *szTitle )
{
	strncpy( gg_szWinTitle, szTitle, AM_MAX_WIN_TITLE );		// タイトルコピー
	gg_szWinTitle[AM_MAX_WIN_TITLE - 1] = 0;					// 必ず終端がつくよう

	return AM_OK;
}


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AB_2DGraph.cpp
//
// 機能: DirectX上でのＡＭＧＡＭＥの２Ｄ画面描画機能をブリッジ。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif

#pragma warning( disable : 4819 )						// コードページの警告抑止


// ---------------------------------------------------------------------------
// インクルード域

#include <math.h>

#include "amgame.h"												// AMGAMEヘッダ

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnvヘッダ
#include "AB_Movie.h"											// AMMovieヘッダ
#include "AB_Debug.h"											// ABDebugヘッダ
#include "AM_2DGraph.h"											// AM2DGraphヘッダ
#include "AB_2DGraph.h"											// AB2DGraphヘッダ

#endif


// ---------------------------------------------------------------------------
// ファイル内定数域

#define MAX_UNIT_PICTURES		2000							// 最大単位ピクチャ数
#define MAX_USER_PICTURES		5000							// 最大ユーザピクチャ数

#define MAX_DIRECTPIC_NAME		300								// 最大直接ロード画像名


// ---------------------------------------------------------------------------
// ファイル内グローバル変数域

// 画像バッファ関係
AM_UNIT_PICTURE					fg_UnitPictureBuf[MAX_UNIT_PICTURES];	// 単位ピクチャバッファ
int								fg_nUnitPicNum = 0;						// 単位ピクチャ数
AM_USER_PICTURE					fg_UserPictureBuf[MAX_USER_PICTURES];	// ユーザピクチャバッファ
int								fg_nUserPicNum = 0;						// ユーザピクチャ数

// 画像直接ロード関係
int								hDirectPic = -1;						// 直接画像ハンドル
char							szDirectName[MAX_DIRECTPIC_NAME];		// 直接画像ファイル名


// ---------------------------------------------------------------------------
// ファイル内関数プロトタイプ宣言域

static int AB_LoadBlockedTexture( char *szFileName, int nXSize, int nYSize,
								  int nXNum, int nYNum, int nAllNum, int *phHandleBuf, int bFlags );
																// ブロック化2Dグラフィックロード(内部版)
static AM_UNIT_PICTURE *AB_MakeUnitPicture( AM_LP2DPICTUREBUFFER ppbPicture, AM_2DPIC_INFO *ppiPicInfo, char *szFileName );
																// 新規単位ピクチャ
static int AB_Make2DHandle( AM_UNIT_PICTURE *pUnitPic, int nLeft, int nTop, int nWidth, int nHeight, int bFlags );
																// 新規絵ハンドル
static int AB_DirectLoad2DGraph( int x, int y, char *szFileName, int nConds );	// 2Dグラフィック擬似直接ロード

static int AM_DrawUserPicRotCond( int nCenX1, int nCenY1, int nCenX2, int nCenY2,
								  double fAngle, double fExRate,
								  AM_USER_PICTURE *pUserPicture, int bConds );
																// 2D条件付き回転描画(左右反転自動付加)

static int AM_DrawPictureCond( int dx1, int dy1, int dx2, int dy2,
							   int sx1, int sy1, int sx2, int sy2,
							   AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo, int bConds );
																// 条件付き描画
static int AM_DrawPictureCond4_2( AM_2DVECTOR *pDestVerts,
								  int sx1, int sy1, int sx2, int sy2,
								  AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo, int bConds );
																// 条件付き描画（描画先４点、描画元２点指定）


//-----------------------------------------------------------------------------
// 2D画像ロード関数域

// 2Dグラフィックロード(旧)
extern int LoadBmpMem( char *szFileName )
{
	return LoadTexture( szFileName );
}


// 2Dグラフィックロード
extern int LoadTexture( char *szFileName )
{
	AM_LP2DPICTUREBUFFER		ppbPicture;								// Buffer to hold 2D Picture
	AM_2DPIC_INFO				piPicInfo;
	AM_UNIT_PICTURE				*pUnitPic;								// 単位ピクチャ
	int							hPicture;								// ピクチャハンドル
	AB_LOADING_DEBUG_INFO		ldDebugInfo;							// デバッグ情報

	ppbPicture = AM_Load2DGraph( szFileName, &piPicInfo );				// グラフィックロード
	// エラー処理
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

	pUnitPic = AB_MakeUnitPicture( ppbPicture, &piPicInfo, szFileName );	// 単位ピクチャ登録
	if ( !pUnitPic ) return AMERR_NOFREESURFACE;

	hPicture = AB_Make2DHandle( pUnitPic, 0, 0, piPicInfo.nWidth, piPicInfo.nHeight, 0 );	// ハンドル作成

	// デバッグ情報出力
	if ( gg_bDebugFile ) {
		ldDebugInfo.nObjectKind = AB_DEBUG_LOADTEXTURE;
		ldDebugInfo.szFileName  = szFileName;
		ldDebugInfo.nMadeHandle = hPicture;
		ldDebugInfo.bError = false;
		AB_OutLoadingDebugInfo( &ldDebugInfo );
	}

	return hPicture;
}


// ブロック化2Dグラフィックロード(旧)
extern int LoadBlkBmpMem( char *szFileName, int nXSize, int nYSize,
						  int nXNum, int nYNum, int nAllNum, int *phHandleBuf )
{
	return AB_LoadBlockedTexture( szFileName, nXSize, nYSize,
								  nXNum, nYNum, nAllNum, phHandleBuf, 0 );
}


// ブロック化2Dグラフィックロード(新)
extern int LoadBlkTexture( char *szFileName, int nXSize, int nYSize,
						   int nXNum, int nYNum, int nAllNum, int *phHandleBuf )
{
	return AB_LoadBlockedTexture( szFileName, nXSize, nYSize,
								  nXNum, nYNum, nAllNum, phHandleBuf, 0 );
}


// ブロック化2Dグラフィックロード(旧、左右反転)
extern int LoadBlkBmpMemLR( char *szFileName, int nXSize, int nYSize,
							int nXNum, int nYNum, int nAllNum, int *phHandleBuf )
{
	return AB_LoadBlockedTexture( szFileName, nXSize, nYSize,
								  nXNum, nYNum, nAllNum, phHandleBuf, AM_PICTURE_LR );
}

// ブロック化2Dグラフィックロード(左右反転)
extern int LoadBlkTextureLR( char *szFileName, int nXSize, int nYSize,
							 int nXNum, int nYNum, int nAllNum, int *phHandleBuf )
{
	return AB_LoadBlockedTexture( szFileName, nXSize, nYSize,
								  nXNum, nYNum, nAllNum, phHandleBuf, AM_PICTURE_LR );
}


// ブロック化2Dグラフィックロード(内部版)
static int AB_LoadBlockedTexture( char *szFileName, int nXSize, int nYSize,
								  int nXNum, int nYNum, int nAllNum, int *phHandleBuf, int bFlags )
{
	int							i, j;
	int							x, y;									// ブロックの左上座標
	int							nBlockNum;								// ブロック数
	AM_LP2DPICTUREBUFFER		ppbPicture;								// Buffer to hold 2D Picture
	AM_2DPIC_INFO				piPicInfo;
	AM_UNIT_PICTURE				*pUnitPic;								// 単位ピクチャ
	AB_LOADING_DEBUG_INFO		ldDebugInfo;							// デバッグ情報

	ppbPicture = AM_Load2DGraph( szFileName, &piPicInfo );				// グラフィックロード
	// エラー処理
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

	pUnitPic = AB_MakeUnitPicture( ppbPicture, &piPicInfo, szFileName );	// 単位ピクチャ登録
	if ( !pUnitPic ) return AMERR_NOFREESURFACE;

	if ( nXSize > piPicInfo.nWidth || nYSize > piPicInfo.nHeight ) return AMERR_INVALIDSIZE;	// 1ブロックよりも絵が小さい場合
	nBlockNum = 0;														// 初期ブロック数
	y = 0;
	for ( i = 0; i < nYNum; i++ ) {
		x = 0;
		for ( j = 0; j < nXNum; j++ ) {
			if ( 1 ) {//( x + nXSize <= piPicInfo.nWidth ) && ( y + nYSize <= piPicInfo.nHeight ) ) {	// ブロックが絵の外に掛からない
				*( phHandleBuf + nBlockNum ) = AB_Make2DHandle( pUnitPic, x, y, nXSize, nYSize, 0 );	// ハンドル作成
				nBlockNum++;
				if ( bFlags & AM_PICTURE_LR ) {
					*( phHandleBuf + nBlockNum ) = AB_Make2DHandle( pUnitPic, x, y, nXSize, nYSize, AM_PICTURE_LR );	// ハンドル作成
					nBlockNum++;
				}
			}
			else {														// ブロックが絵の外に掛かる
				if ( bFlags & AM_PICTURE_LR ) {							// 左右反転ありか
					*( phHandleBuf + nBlockNum ) = *( phHandleBuf + nBlockNum - 2 );	// ふたつ前コピー
					*( phHandleBuf + nBlockNum + 1 ) = *( phHandleBuf + nBlockNum - 1 );	// ふたつ前コピー
					nBlockNum += 2;
				}
				else {
					*( phHandleBuf + nBlockNum ) = *( phHandleBuf + nBlockNum - 1 );	// ひとつ前コピー
					nBlockNum++;
				}
			}
			// 規定数まで達していれば終了
			if ( ( !( bFlags & AM_PICTURE_LR ) && nBlockNum >= nAllNum     ) ||
				 (  ( bFlags & AM_PICTURE_LR ) && nBlockNum >= nAllNum * 2 ) )
			{
				break;
			}
			x += nXSize;
		}
		y += nYSize;
	}

	// デバッグ情報出力
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


// 新規単位ピクチャ
static AM_UNIT_PICTURE *AB_MakeUnitPicture( AM_LP2DPICTUREBUFFER ppbPicture, AM_2DPIC_INFO *ppiPicInfo, char *szFileName )
{
	if ( fg_nUnitPicNum >= MAX_UNIT_PICTURES ) return NULL;				// 空きチェック

	fg_UnitPictureBuf[fg_nUnitPicNum].a2PicInfo = *ppiPicInfo;			// 絵情報
	fg_UnitPictureBuf[fg_nUnitPicNum].pPicture  = ppbPicture;			// 絵オブジェクト
	strncpy( fg_UnitPictureBuf[fg_nUnitPicNum].szFileName, szFileName, AM_MAX_PICTURE_NAME - 1 );	// 絵ファイル名
	fg_UnitPictureBuf[fg_nUnitPicNum].szFileName[AM_MAX_PICTURE_NAME - 1] = 0;	// ファイル名の終端を確実に置く
	fg_nUnitPicNum++;

	return &fg_UnitPictureBuf[fg_nUnitPicNum - 1];
}


// 新規絵ハンドル
static int AB_Make2DHandle( AM_UNIT_PICTURE *pUnitPic, int nLeft, int nTop, int nWidth, int nHeight, int bFlags )
{
	int							hPicture = AM_PIC2D_HANDLE_HEAD;

	if ( fg_nUserPicNum >= MAX_USER_PICTURES ) return AMERR_NOFREEHANDLE;	// ハンドル空きチェック

	fg_UserPictureBuf[fg_nUserPicNum].pUnitPic = pUnitPic;
	fg_UserPictureBuf[fg_nUserPicNum].nLeft = nLeft;
	fg_UserPictureBuf[fg_nUserPicNum].nTop = nTop;
	fg_UserPictureBuf[fg_nUserPicNum].nWidth = nWidth;
	fg_UserPictureBuf[fg_nUserPicNum].nHeight = nHeight;
	fg_UserPictureBuf[fg_nUserPicNum].bFlags = bFlags;
	fg_nUserPicNum++;

	hPicture |= fg_nUserPicNum - 1;										// ハンドル生成

	return hPicture;
}


// 2Dグラフィックバッファ作成
extern int MakeGraphMem( int nWidth, int nHeight )
{
	return MakeTextureMem( nWidth, nHeight, 0 );
}


// 2Dグラフィックバッファ作成
extern int MakeTextureMem( int nWidth, int nHeight, int nBitDepth )
{
	AM_LP2DPICTUREBUFFER		ppbPicture;								// Buffer to hold 2D Picture
	AM_2DPIC_INFO				piPicInfo;
	AM_UNIT_PICTURE				*pUnitPic;								// 単位ピクチャ
	int							hPicture;								// ピクチャハンドル

	// 絵構造体データセット
	piPicInfo.nWidth = nWidth;  piPicInfo.nHeight = nHeight;			// 幅・高さ
	if ( nBitDepth == 0 ) nBitDepth = gg_nBitDepth;						// ビットデプス
	piPicInfo.nBitDepth = nBitDepth;

	ppbPicture = AM_Make2DGraphBuf( &piPicInfo );						// グラフィックロード
	if ( !ppbPicture ) return AMERR_NOMEMORY;

	pUnitPic = AB_MakeUnitPicture( ppbPicture, &piPicInfo, "User-Made(MakeTextureMem)" );	// 単位ピクチャ登録
	if ( !pUnitPic ) return AMERR_NOFREESURFACE;

	hPicture = AB_Make2DHandle( pUnitPic, 0, 0, piPicInfo.nWidth, piPicInfo.nHeight, 0 );	// ハンドル作成

	return hPicture;
}


// 2Dグラフィック再ロード
extern int ReloadGraph( char *szFileName, int hPicture )
{
	int							nResult;								// リザルトコード
	int							nUserPicLoc;							// ユーザー絵位置
	AM_USER_PICTURE				*pupUserPic;							// ユーザー絵
//	AM_LP2DPICTUREBUFFER		ppbPicture;								// Buffer to hold 2D Picture
//	AM_2DPIC_INFO				piPicInfo;
//	AM_UNIT_PICTURE				*pUnitPic;								// 単位ピクチャ
//	int							hPicture;								// ピクチャハンドル
	AB_LOADING_DEBUG_INFO		ldDebugInfo;							// デバッグ情報

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;						// 位置
	// 妥当性チェック
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

	// デバッグ情報出力
	if ( gg_bDebugFile ) {
		ldDebugInfo.nObjectKind = AB_DEBUG_RELOADGRAPH;
		ldDebugInfo.szFileName  = szFileName;
		ldDebugInfo.nMadeHandle = hPicture;
		ldDebugInfo.bError = nResult < 0 ? true : false;
		AB_OutLoadingDebugInfo( &ldDebugInfo );
	}

	return nResult;
}


// 2Dグラフィック擬似直接ロード
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


// 2Dグラフィック擬似直接ロード(カラーキー付き)
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


// 2Dグラフィック擬似直接ロード(内部版)
static int AB_DirectLoad2DGraph( int x, int y, char *szFileName, int nConds )
{
	if ( hDirectPic < 0 ) {												// 初めて直接ロード
		hDirectPic = LoadTexture( szFileName );							// 画像ロード
		strcpy( szDirectName, szFileName );								// 名前記録
	}
	else {																// 2回目以降
		if ( strcmp( szDirectName, szFileName ) != 0 ) {				// 前回と同じでない
			ReloadGraph( szFileName, hDirectPic );						// リロード
			strcpy( szDirectName, szFileName );							// 名前記録
		}
	}

	// 描画
	if ( nConds == 0 ) {												// ノーマル
		DrawMem( x, y, hDirectPic );
	}
	else {																// カラーキー付き
		DrawMemTh( x, y, hDirectPic );
	}

	return hDirectPic;
}


// 全ビットマップ開放
extern int InitGraphMem( void )
{
	return AB_Release2DGraphs();
}


// 全テクスチャ開放
extern int InitTexture( void )
{
	return AB_Release2DGraphs();
}


// 2Dグラフィック開放
extern int AB_Release2DGraphs( void )
{
	int							i;

	// グラフィック開放
	for ( i = 0; i < fg_nUnitPicNum; i++ ) {
		AM_SAFE_RELEASE( fg_UnitPictureBuf[i].pPicture );
	}

	// 定数リセット
	fg_nUnitPicNum = 0;													// 単位ピクチャリセット
	fg_nUserPicNum = 0;													// ユーザーピクチャリセット
	hDirectPic = -1;													// 直接画像ハンドルリセット

	return AM_OK;
}


// 最後に作られたグラフィック開放（単位ピクチャとユーザーピクチャが1:1対応している必要あり）
extern int AB_ReleaseLastGraph( int hPicture )
{
	if ( fg_nUnitPicNum <= 0 || fg_nUserPicNum <= 0 ) return AMERR_CANNOT;
	if ( ( hPicture & ~AM_HANDLE_HEAD_MASK ) != fg_nUserPicNum - 1 ) return AMERR_CANNOT;	// 位置

	AM_SAFE_RELEASE( fg_UnitPictureBuf[fg_nUnitPicNum - 1].pPicture );

	fg_nUnitPicNum--;
	fg_nUserPicNum--;

	return AM_OK;
}


//-----------------------------------------------------------------------------
// 2D画像描画関数域

// 2D描画
extern int DrawMem( int x, int y, int hPicture )
{
	int					nResult;								// リザルトコード
	int					nUserPicLoc;							// ユーザー絵位置
	AM_USER_PICTURE		*pupUserPic;							// ユーザー絵
	int					bDrawConds = 0;							// 描画条件
	AB_DRAWING_DEBUG_INFO		adDebugInfo;					// デバッグ情報

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// 位置
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// 妥当性チェック
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEM;			// DrawMem命令
			adDebugInfo.nUsedHandle = hPicture;					// ハンドル
			adDebugInfo.nBlendKind = 0;							// ブレンド
			adDebugInfo.bError = true;							// エラーあり
			AB_SetDrawingDebugInfo( &adDebugInfo );				// デバッグ情報セット
		}
		return AMERR_INVALIDHANDLE;
	}

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	if ( pupUserPic->bFlags & AM_PICTURE_LR ) {					// 左右反転あり
		bDrawConds |= AM_DRAWPIC_LR;
	}

	nResult = AM_DrawPictureCond( x, y, x + pupUserPic->nWidth, y + pupUserPic->nHeight,
								  pupUserPic->nLeft, pupUserPic->nTop,
								  pupUserPic->nLeft + pupUserPic->nWidth, pupUserPic->nTop  + pupUserPic->nHeight,
								  pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ), bDrawConds );

	// デバッグ情報をセット
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEM;				// DrawMem命令
		adDebugInfo.nUsedHandle = hPicture;						// ハンドル
		adDebugInfo.pUserPicture = pupUserPic;					// ユーザーピクチャ
		adDebugInfo.x1 = x;										// 描画x座標
		adDebugInfo.y1 = y;										// 描画y座標
		adDebugInfo.nBlendKind = 0;								// ブレンド
		adDebugInfo.bError = false;								// エラーなし
		AB_SetDrawingDebugInfo( &adDebugInfo );					// デバッグ情報セット
	}

	return nResult;
}

// アルファブレンド（カラーキー）付き2D描画
extern int DrawMemTh( int x, int y, int hPicture )
{
	int					nResult;								// リザルトコード
	int					nUserPicLoc;							// ユーザー絵位置
	AM_USER_PICTURE		*pupUserPic;							// ユーザー絵
	int					bDrawConds = AM_DRAWPIC_PICALPHA;		// 描画条件
	AB_DRAWING_DEBUG_INFO		adDebugInfo;					// デバッグ情報

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// 位置
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// 妥当性チェック
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEM;			// DrawMem命令
			adDebugInfo.nUsedHandle = hPicture;					// ハンドル
			adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;		// ブレンド
			adDebugInfo.bError = true;							// エラーあり
			AB_SetDrawingDebugInfo( &adDebugInfo );				// デバッグ情報セット
		}
		return AMERR_INVALIDHANDLE;
	}

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	if ( pupUserPic->bFlags & AM_PICTURE_LR ) {					// 左右反転あり
		bDrawConds |= AM_DRAWPIC_LR;
	}

	nResult = AM_DrawPictureCond( x, y, x + pupUserPic->nWidth, y + pupUserPic->nHeight,
								  pupUserPic->nLeft, pupUserPic->nTop,
								  pupUserPic->nLeft + pupUserPic->nWidth, pupUserPic->nTop  + pupUserPic->nHeight,
								  pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ), bDrawConds );

	// デバッグ情報をセット
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEM;				// DrawMem命令
		adDebugInfo.nUsedHandle = hPicture;						// ハンドル
		adDebugInfo.pUserPicture = pupUserPic;					// ユーザーピクチャ
		adDebugInfo.x1 = x;										// 描画x座標
		adDebugInfo.y1 = y;										// 描画y座標
		adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;			// ブレンド
		adDebugInfo.bError = false;								// エラーなし
		AB_SetDrawingDebugInfo( &adDebugInfo );					// デバッグ情報セット
	}

	return nResult;
}


// 2D左右反転描画
extern int DrawMemLR( int x, int y, int hPicture )
{
	int					nResult;								// リザルトコード
	int					nUserPicLoc;							// ユーザー絵位置
	AM_USER_PICTURE		*pupUserPic;							// ユーザー絵
	int					bDrawConds = 0;							// 描画条件
	AB_DRAWING_DEBUG_INFO		adDebugInfo;					// デバッグ情報

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// 位置
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// 妥当性チェック
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMLR;		// DrawMem命令
			adDebugInfo.nUsedHandle = hPicture;					// ハンドル
			adDebugInfo.nBlendKind = 0;							// ブレンド
			adDebugInfo.bError = true;							// エラーあり
			AB_SetDrawingDebugInfo( &adDebugInfo );				// デバッグ情報セット
		}
		return AMERR_INVALIDHANDLE;
	}

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	if ( !( pupUserPic->bFlags & AM_PICTURE_LR ) ) {			// 左右反転なし
		bDrawConds |= AM_DRAWPIC_LR;
	}

	nResult = AM_DrawPictureCond( x, y, x + pupUserPic->nWidth, y + pupUserPic->nHeight,
								  pupUserPic->nLeft, pupUserPic->nTop,
								  pupUserPic->nLeft + pupUserPic->nWidth, pupUserPic->nTop  + pupUserPic->nHeight,
								  pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ), bDrawConds );

	// デバッグ情報をセット
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMLR;			// DrawMemLR命令
		adDebugInfo.nUsedHandle = hPicture;						// ハンドル
		adDebugInfo.pUserPicture = pupUserPic;					// ユーザーピクチャ
		adDebugInfo.x1 = x;										// 描画x座標
		adDebugInfo.y1 = y;										// 描画y座標
		adDebugInfo.nBlendKind = 0;								// ブレンド
		adDebugInfo.bError = false;								// エラーなし
		AB_SetDrawingDebugInfo( &adDebugInfo );					// デバッグ情報セット
	}

	return nResult;
}


// 2D左右反転描画（カラーキー付き）
extern int DrawMemThLR( int x, int y, int hPicture )
{
	int					nResult;								// リザルトコード
	int					nUserPicLoc;							// ユーザー絵位置
	AM_USER_PICTURE		*pupUserPic;							// ユーザー絵
	int					bDrawConds = AM_DRAWPIC_PICALPHA;		// 描画条件
	AB_DRAWING_DEBUG_INFO		adDebugInfo;					// デバッグ情報

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// 位置
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// 妥当性チェック
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMLR;		// DrawMem命令
			adDebugInfo.nUsedHandle = hPicture;					// ハンドル
			adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;		// ブレンド
			adDebugInfo.bError = true;							// エラーあり
			AB_SetDrawingDebugInfo( &adDebugInfo );				// デバッグ情報セット
		}
		return AMERR_INVALIDHANDLE;
	}

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	if ( !( pupUserPic->bFlags & AM_PICTURE_LR ) ) {			// 左右反転なし
		bDrawConds |= AM_DRAWPIC_LR;
	}

	nResult = AM_DrawPictureCond( x, y, x + pupUserPic->nWidth, y + pupUserPic->nHeight,
								  pupUserPic->nLeft, pupUserPic->nTop,
								  pupUserPic->nLeft + pupUserPic->nWidth, pupUserPic->nTop  + pupUserPic->nHeight,
								  pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ), bDrawConds );

	// デバッグ情報をセット
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMLR;			// DrawMem命令
		adDebugInfo.nUsedHandle = hPicture;						// ハンドル
		adDebugInfo.pUserPicture = pupUserPic;					// ユーザーピクチャ
		adDebugInfo.x1 = x;										// 描画x座標
		adDebugInfo.y1 = y;										// 描画y座標
		adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;			// ブレンド
		adDebugInfo.bError = false;								// エラーなし
		AB_SetDrawingDebugInfo( &adDebugInfo );					// デバッグ情報セット
	}

	return nResult;
}


// 2D拡大縮小描画
extern int DrawMemExp( int x1, int y1, int x2, int y2, int hPicture )
{
	int					nResult;								// リザルトコード
	int					nUserPicLoc;							// ユーザー絵位置
	AM_USER_PICTURE		*pupUserPic;							// ユーザー絵
	int					bDrawConds = 0;							// 描画条件
	AB_DRAWING_DEBUG_INFO		adDebugInfo;					// デバッグ情報

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// 位置
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// 妥当性チェック
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMEXP;		// DrawMem命令
			adDebugInfo.nUsedHandle = hPicture;					// ハンドル
			adDebugInfo.nBlendKind = 0;							// ブレンド
			adDebugInfo.bError = true;							// エラーあり
			AB_SetDrawingDebugInfo( &adDebugInfo );				// デバッグ情報セット
		}
		return AMERR_INVALIDHANDLE;
	}

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	if ( pupUserPic->bFlags & AM_PICTURE_LR ) {					// 左右反転あり
		bDrawConds |= AM_DRAWPIC_LR;
	}

	nResult = AM_DrawPictureCond( x1, y1, x2, y2,
								  pupUserPic->nLeft, pupUserPic->nTop,
								  pupUserPic->nLeft + pupUserPic->nWidth, pupUserPic->nTop  + pupUserPic->nHeight,
								  pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ), bDrawConds );

	// デバッグ情報をセット
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMEXP;			// DrawMem命令
		adDebugInfo.nUsedHandle = hPicture;						// ハンドル
		adDebugInfo.pUserPicture = pupUserPic;					// ユーザーピクチャ
		adDebugInfo.x1 = x1;									// 描画x座標
		adDebugInfo.y1 = y1;									// 描画y座標
		adDebugInfo.x2 = x2;									// 描画x座標2
		adDebugInfo.y2 = y2;									// 描画y座標2
		adDebugInfo.nBlendKind = 0;								// ブレンド
		adDebugInfo.bError = false;								// エラーなし
		AB_SetDrawingDebugInfo( &adDebugInfo );					// デバッグ情報セット
	}

	return nResult;
}


// アルファブレンド（カラーキー）付き2D拡大縮小描画
extern int DrawMemThExp( int x1, int y1, int x2, int y2, int hPicture )
{
	int					nResult;								// リザルトコード
	int					nUserPicLoc;							// ユーザー絵位置
	AM_USER_PICTURE		*pupUserPic;							// ユーザー絵
	int					bDrawConds = AM_DRAWPIC_PICALPHA;		// 描画条件
	AB_DRAWING_DEBUG_INFO		adDebugInfo;					// デバッグ情報

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// 位置
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// 妥当性チェック
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMEXP;		// DrawMem命令
			adDebugInfo.nUsedHandle = hPicture;					// ハンドル
			adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;		// ブレンド
			adDebugInfo.bError = true;							// エラーあり
			AB_SetDrawingDebugInfo( &adDebugInfo );				// デバッグ情報セット
		}
		return AMERR_INVALIDHANDLE;
	}

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	if ( pupUserPic->bFlags & AM_PICTURE_LR ) {					// 左右反転あり
		bDrawConds |= AM_DRAWPIC_LR;
	}

	nResult = AM_DrawPictureCond( x1, y1, x2, y2,
								  pupUserPic->nLeft, pupUserPic->nTop,
								  pupUserPic->nLeft + pupUserPic->nWidth, pupUserPic->nTop  + pupUserPic->nHeight,
								  pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ), bDrawConds );

	// デバッグ情報をセット
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMEXP;			// DrawMem命令
		adDebugInfo.nUsedHandle = hPicture;						// ハンドル
		adDebugInfo.pUserPicture = pupUserPic;					// ユーザーピクチャ
		adDebugInfo.x1 = x1;									// 描画x座標
		adDebugInfo.y1 = y1;									// 描画y座標
		adDebugInfo.x2 = x2;									// 描画x座標2
		adDebugInfo.y2 = y2;									// 描画y座標2
		adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;			// ブレンド
		adDebugInfo.bError = false;								// エラーなし
		AB_SetDrawingDebugInfo( &adDebugInfo );					// デバッグ情報セット
	}

	return nResult;
}


// 2D回転描画
extern int DrawMemRot( int nCenX1, int nCenY1, int nCenX2, int nCenY2,
					   double fAngle, double fExRate,
					   int hPicture )
{
	int					nResult;								// リザルトコード
	int					nUserPicLoc;							// ユーザー絵位置
	AB_DRAWING_DEBUG_INFO		adDebugInfo;					// デバッグ情報

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// 位置
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// 妥当性チェック
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMROT;		// DrawMem命令
			adDebugInfo.nUsedHandle = hPicture;					// ハンドル
			adDebugInfo.nBlendKind = 0;							// ブレンド
			adDebugInfo.bError = true;							// エラーあり
			AB_SetDrawingDebugInfo( &adDebugInfo );				// デバッグ情報セット
		}
		return AMERR_INVALIDHANDLE;
	}

	// 描画
	nResult = AM_DrawUserPicRotCond( nCenX1, nCenY1, nCenX2, nCenY2, fAngle, fExRate,
									 &fg_UserPictureBuf[nUserPicLoc], 0 );

	// デバッグ情報をセット
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMROT;			// DrawMem命令
		adDebugInfo.nUsedHandle = hPicture;						// ハンドル
		adDebugInfo.pUserPicture = &fg_UserPictureBuf[nUserPicLoc];	// ユーザーピクチャ
		adDebugInfo.x1 = nCenX1;								// 描画x座標
		adDebugInfo.y1 = nCenY1;								// 描画y座標
		adDebugInfo.x2 = nCenX2;								// 描画x座標2
		adDebugInfo.y2 = nCenY2;								// 描画y座標2
		adDebugInfo.fAngle = ( float )fAngle;					// 描画角度
		adDebugInfo.fExRate = ( float )fExRate;					// 描画拡大率
		adDebugInfo.nBlendKind = 0;								// ブレンド
		adDebugInfo.bError = false;								// エラーなし
		AB_SetDrawingDebugInfo( &adDebugInfo );					// デバッグ情報セット
	}

	return nResult;
}


// 2D回転描画（カラーキー付き）
extern int DrawMemThRot( int nCenX1, int nCenY1, int nCenX2, int nCenY2,
						 double fAngle, double fExRate,
						 int hPicture )
{
	int					nResult;								// リザルトコード
	int					nUserPicLoc;							// ユーザー絵位置
	AB_DRAWING_DEBUG_INFO		adDebugInfo;					// デバッグ情報

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// 位置
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// 妥当性チェック
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMROT;		// DrawMem命令
			adDebugInfo.nUsedHandle = hPicture;					// ハンドル
			adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;		// ブレンド
			adDebugInfo.bError = true;							// エラーあり
			AB_SetDrawingDebugInfo( &adDebugInfo );				// デバッグ情報セット
		}
		return AMERR_INVALIDHANDLE;
	}

	// 描画
	nResult = AM_DrawUserPicRotCond( nCenX1, nCenY1, nCenX2, nCenY2, fAngle, fExRate,
									 &fg_UserPictureBuf[nUserPicLoc], AM_DRAWPIC_PICALPHA );

	// デバッグ情報をセット
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMROT;			// DrawMem命令
		adDebugInfo.nUsedHandle = hPicture;						// ハンドル
		adDebugInfo.pUserPicture = &fg_UserPictureBuf[nUserPicLoc];		// ユーザーピクチャ
		adDebugInfo.x1 = nCenX1;								// 描画x座標
		adDebugInfo.y1 = nCenY1;								// 描画y座標
		adDebugInfo.x2 = nCenX2;								// 描画x座標2
		adDebugInfo.y2 = nCenY2;								// 描画y座標2
		adDebugInfo.fAngle = ( float )fAngle;					// 描画角度
		adDebugInfo.fExRate = ( float )fExRate;					// 描画拡大率
		adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;			// ブレンド
		adDebugInfo.bError = false;								// エラーなし
		AB_SetDrawingDebugInfo( &adDebugInfo );					// デバッグ情報セット
	}

	return nResult;
}


// 2D条件付き回転描画(左右反転自動付加)
static int AM_DrawUserPicRotCond( int nCenX1, int nCenY1, int nCenX2, int nCenY2,
								  double fAngle, double fExRate,
								  AM_USER_PICTURE *pUserPicture, int bConds )
{
	int					i;
	int					nResult;								// リザルトコード
	AM_2DVECTOR			v2Vertices[4];							// 4頂点
	float				fCost, fSint;							// サインコサイン
	float				tx, ty;									// 一時座標

	if ( pUserPicture->bFlags & AM_PICTURE_LR ) {					// 左右反転あり
		bConds |= AM_DRAWPIC_LR;
	}

	// サインコサイン
	fCost = (float)cos( fAngle );  fSint = (float)sin( fAngle );

	// 基本座標
	v2Vertices[0].x = ( float )( 0.0f                 - nCenX1 );  v2Vertices[0].y = ( float )( 0.0f                  - nCenY1 );
	v2Vertices[1].x = ( float )( pUserPicture->nWidth - nCenX1 );  v2Vertices[1].y = ( float )( 0.0f                  - nCenY1 );
	v2Vertices[2].x = ( float )( 0.0f                 - nCenX1 );  v2Vertices[2].y = ( float )( pUserPicture->nHeight - nCenY1 );
	v2Vertices[3].x = ( float )( pUserPicture->nWidth - nCenX1 );  v2Vertices[3].y = ( float )( pUserPicture->nHeight - nCenY1 );

	for ( i = 0; i < 4; i++ ) {								// 四角形座標複写
		// 回転
		tx = ( v2Vertices[i].x * fCost - v2Vertices[i].y * fSint );
		ty = ( v2Vertices[i].x * fSint + v2Vertices[i].y * fCost );
		// 拡縮・平行移動
		v2Vertices[i].x = tx * ( float )fExRate + nCenX2;
		v2Vertices[i].y = ty * ( float )fExRate + nCenY2;
	}

	// 描画
	nResult = AM_DrawPictureCond4_2( v2Vertices,
									 pUserPicture->nLeft, pUserPicture->nTop,
									 pUserPicture->nLeft + pUserPicture->nWidth, pUserPicture->nTop  + pUserPicture->nHeight,
									 pUserPicture->pUnitPic->pPicture, &( pUserPicture->pUnitPic->a2PicInfo ), bConds );

	return nResult;
}


// 2D変形描画
extern int DrawMemFlex( int x1, int y1, int x2, int y2,
						int x3, int y3, int x4, int y4,
						int hPicture )
{
	int					nResult;								// リザルトコード
	int					nUserPicLoc;							// ユーザー絵位置
	AM_USER_PICTURE		*pupUserPic;							// ユーザー絵
	AM_2DVECTOR			v2Vertices[4];							// 4頂点
	int					bDrawConds = 0;							// 描画条件
	AB_DRAWING_DEBUG_INFO		adDebugInfo;					// デバッグ情報

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// 位置
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// 妥当性チェック
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMFLEX;		// DrawMemFlex命令
			adDebugInfo.nUsedHandle = hPicture;					// ハンドル
			adDebugInfo.nBlendKind = 0;							// ブレンド
			adDebugInfo.bError = true;							// エラーあり
			AB_SetDrawingDebugInfo( &adDebugInfo );				// デバッグ情報セット
		}
		return AMERR_INVALIDHANDLE;
	}

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	if ( pupUserPic->bFlags & AM_PICTURE_LR ) {					// 左右反転あり
		bDrawConds |= AM_DRAWPIC_LR;
	}

	// 座標
	v2Vertices[0].x = ( float )x1;  v2Vertices[0].y = ( float )y1;
	v2Vertices[1].x = ( float )x2;  v2Vertices[1].y = ( float )y2;
	v2Vertices[2].x = ( float )x3;  v2Vertices[2].y = ( float )y3;
	v2Vertices[3].x = ( float )x4;  v2Vertices[3].y = ( float )y4;

	// 描画
	nResult = AM_DrawPictureCond4_2( v2Vertices,
									 pupUserPic->nLeft, pupUserPic->nTop,
									 pupUserPic->nLeft + pupUserPic->nWidth, pupUserPic->nTop  + pupUserPic->nHeight,
									 pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ), bDrawConds );

	// デバッグ情報をセット
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMFLEX;			// DrawMem命令
		adDebugInfo.nUsedHandle = hPicture;						// ハンドル
		adDebugInfo.pUserPicture = pupUserPic;					// ユーザーピクチャ
		adDebugInfo.x1 = x1;									// 描画x座標
		adDebugInfo.y1 = y1;									// 描画y座標
		adDebugInfo.x2 = x2;									// 描画x座標2
		adDebugInfo.y2 = y2;									// 描画y座標2
		adDebugInfo.x3 = x3;									// 描画x座標3
		adDebugInfo.y3 = y3;									// 描画y座標3
		adDebugInfo.x4 = x4;									// 描画x座標4
		adDebugInfo.y4 = y4;									// 描画y座標4
		adDebugInfo.nBlendKind = 0;								// ブレンド
		adDebugInfo.bError = false;								// エラーなし
		AB_SetDrawingDebugInfo( &adDebugInfo );					// デバッグ情報セット
	}

	return nResult;
}


// 2D変形描画（カラーキー付き）
extern int DrawMemThFlex( int x1, int y1, int x2, int y2,
						  int x3, int y3, int x4, int y4,
						  int hPicture )
{
	int					nResult;								// リザルトコード
	int					nUserPicLoc;							// ユーザー絵位置
	AM_USER_PICTURE		*pupUserPic;							// ユーザー絵
	AM_2DVECTOR			v2Vertices[4];							// 4頂点
	int					bDrawConds = AM_DRAWPIC_PICALPHA;		// 描画条件
	AB_DRAWING_DEBUG_INFO		adDebugInfo;				// デバッグ情報

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// 位置
	if ( nUserPicLoc >= fg_nUserPicNum ) {						// 妥当性チェック
		if ( gg_bRegisterDebug ) {
			adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMFLEX;		// DrawMemFlex命令
			adDebugInfo.nUsedHandle = hPicture;					// ハンドル
			adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;		// ブレンド
			adDebugInfo.bError = true;							// エラーあり
			AB_SetDrawingDebugInfo( &adDebugInfo );				// デバッグ情報セット
		}
		return AMERR_INVALIDHANDLE;
	}

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	if ( pupUserPic->bFlags & AM_PICTURE_LR ) {					// 左右反転あり
		bDrawConds |= AM_DRAWPIC_LR;
	}

	// 座標
	v2Vertices[0].x = ( float )x1;  v2Vertices[0].y = ( float )y1;
	v2Vertices[1].x = ( float )x2;  v2Vertices[1].y = ( float )y2;
	v2Vertices[2].x = ( float )x3;  v2Vertices[2].y = ( float )y3;
	v2Vertices[3].x = ( float )x4;  v2Vertices[3].y = ( float )y4;

	// 描画
	nResult = AM_DrawPictureCond4_2( v2Vertices,
									 pupUserPic->nLeft, pupUserPic->nTop,
									 pupUserPic->nLeft + pupUserPic->nWidth, pupUserPic->nTop  + pupUserPic->nHeight,
									 pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ), bDrawConds );

	// デバッグ情報をセット
	if ( gg_bRegisterDebug ) {
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWMEMFLEX;			// DrawMem命令
		adDebugInfo.nUsedHandle = hPicture;						// ハンドル
		adDebugInfo.pUserPicture = pupUserPic;					// ユーザーピクチャ
		adDebugInfo.x1 = x1;									// 描画x座標
		adDebugInfo.y1 = y1;									// 描画y座標
		adDebugInfo.x2 = x2;									// 描画x座標2
		adDebugInfo.y2 = y2;									// 描画y座標2
		adDebugInfo.x3 = x3;									// 描画x座標3
		adDebugInfo.y3 = y3;									// 描画y座標3
		adDebugInfo.x4 = x4;									// 描画x座標4
		adDebugInfo.y4 = y4;									// 描画y座標4
		adDebugInfo.nBlendKind = AM_DRAWPIC_PICALPHA;			// ブレンド
		adDebugInfo.bError = false;								// エラーなし
		AB_SetDrawingDebugInfo( &adDebugInfo );					// デバッグ情報セット
	}

	return nResult;
}


// 2Dマスク付き描画(擬似)
extern int DrawMemMask( int x, int y, int hPicture,
						int nWidth, int nHeight, unsigned char *pMask )
{
	int					nResult;								// リザルトコード
	int					nUserPicLoc;							// ユーザー絵位置
	AM_USER_PICTURE		*pupUserPic;							// ユーザー絵

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// 位置
	if ( nUserPicLoc >= fg_nUserPicNum ) return AMERR_INVALIDHANDLE;	// 妥当性チェック]

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];

	nResult = AM_Draw2DBlockMaskEmu( x, y, 
									 pupUserPic->nLeft, pupUserPic->nTop,
									 pupUserPic->nLeft + pupUserPic->nWidth, pupUserPic->nTop  + pupUserPic->nHeight,
									 nWidth, nHeight, pMask,
									 pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ) );

	return nResult;
}


// 2Dマスク、カラーキー付き描画(擬似)
extern int DrawMemThMask( int x, int y, int hPicture,
						  int nWidth, int nHeight, unsigned char *pMask )
{
	int					nResult;								// リザルトコード

	nResult = DrawMemMask( x, y, hPicture, nWidth, nHeight, pMask );

	return nResult;
}


// 2D半透明描画
extern int DrawMemTrans( int x, int y, int hPicture, int nMode )
{
	int					nResult;								// リザルトコード
	int					nUserPicLoc;							// ユーザー絵位置
	AM_USER_PICTURE		*pupUserPic;							// ユーザー絵
	int					bDrawConds = 0;							// 描画条件

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// 位置
	if ( nUserPicLoc >= fg_nUserPicNum ) return AMERR_INVALIDHANDLE;	// 妥当性チェック

	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	if ( pupUserPic->bFlags & AM_PICTURE_LR ) {					// 左右反転あり
		bDrawConds |= AM_DRAWPIC_LR;
	}

	// 描画モード制御
	switch ( nMode ) {
	// 平均半透明
	case TRANS_AVERAGE:
	case TRANS_AVERAGE_TH:
		bDrawConds |= AM_DRAWPIC_AVEALPHA;
		break;

	// 加算半透明
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


// 条件付き描画
static int AM_DrawPictureCond( int dx1, int dy1, int dx2, int dy2,
							   int sx1, int sy1, int sx2, int sy2,
							   AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo, int bConds )
{
	int					nResult;								// リザルトコード
	int					nTemp;									// 一時変数
	AM_BLEND_STATE		bsBlendBuf;								// ブレンドステート退避場所

	if ( bConds & AM_DRAWPIC_LR ) {								// 左右反転ありか
		nTemp = sx1;  sx1 = sx2;  sx2 = nTemp;
	}

	if ( bConds & ( AM_DRAWPIC_PICALPHA | AM_DRAWPIC_AVEALPHA | AM_DRAWPIC_ADDALPHA ) ) {	// 絵のアルファ値考慮か
		AM_GetBlendState( &bsBlendBuf );							// ブレンドステート取得
		if ( bConds & AM_DRAWPIC_ADDALPHA ) {
			if ( bConds & AM_DRAWPIC_AVEALPHA ) {
				AM_SetBlendState( AMDRAW_ADDBLEND, AM_MAX_BLEND_FACTOR / 2 );		// 加算ブレンド
			}
			else {
				AM_SetBlendState( AMDRAW_ADDBLEND, AM_MAX_BLEND_FACTOR );			// 加算ブレンド
			}
		}
		else {
			if ( bConds & AM_DRAWPIC_AVEALPHA ) {
				AM_SetBlendState( AMDRAW_ALPHABLEND, AM_MAX_BLEND_FACTOR / 2 );		// アルファブレンド
			}
			else {
				AM_SetBlendState( AMDRAW_ALPHABLEND, bsBlendBuf.nBlendFactor );		// アルファブレンド
			}
		}
	}

	// 実描画
	nResult = AM_Draw2DGraph( dx1, dy1, dx2, dy2,
							  sx1, sy1, sx2, sy2,
							  pPicture, pPicInfo );

	if ( bConds & ( AM_DRAWPIC_PICALPHA | AM_DRAWPIC_AVEALPHA | AM_DRAWPIC_ADDALPHA ) ) {	// 絵のアルファ値考慮か
		AM_RestoreBlendState( &bsBlendBuf );					// ブレンドステート復帰
	}

	return nResult;
}


// 条件付き描画（描画先４点、描画元２点指定）
static int AM_DrawPictureCond4_2( AM_2DVECTOR *pDestVerts,
								  int sx1, int sy1, int sx2, int sy2,
								  AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo, int bConds )
{
	int					nResult;								// リザルトコード
	int					nTemp;									// 一時変数
	AM_BLEND_STATE		bsBlendBuf;								// ブレンドステート退避場所

	if ( bConds & AM_DRAWPIC_LR ) {								// 左右反転ありか
		nTemp = sx1;  sx1 = sx2;  sx2 = nTemp;
	}

	if ( bConds & AM_DRAWPIC_PICALPHA ) {						// 絵のアルファ値考慮か
		AM_GetBlendState( &bsBlendBuf );							// ブレンドステート取得
		AM_SetBlendState( AMDRAW_ALPHABLEND, bsBlendBuf.nBlendFactor );		// アルファブレンド
	}

	// 実描画
	nResult = AM_Draw2DGraph4_2( pDestVerts,
								 sx1, sy1, sx2, sy2,
								 pPicture, pPicInfo );

	if ( bConds & AM_DRAWPIC_PICALPHA ) {						// 絵のアルファ値考慮か
		AM_RestoreBlendState( &bsBlendBuf );					// ブレンドステート復帰
	}

	return nResult;
}


//-----------------------------------------------------------------------------
// 2D画像周辺関数域

// 画像サイズ取得
extern int GetPictureSize( int hPicture, int *pnWidth, int *pnHeight )
{
	int					nResult = AM_OK;						// リザルトコード
	int					nUserPicLoc;							// ユーザー絵位置
	AM_USER_PICTURE		*pupUserPic;							// ユーザー絵
//	int					nHandleKind;							// ハンドル種類

	switch ( hPicture & AM_HANDLE_HEAD_MASK ) {
	// 2D絵の場合
	case AM_PIC2D_HANDLE_HEAD:
		nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// 位置
		if ( nUserPicLoc >= fg_nUserPicNum ) return AMERR_INVALIDHANDLE;	// 妥当性チェック

		pupUserPic = &fg_UserPictureBuf[nUserPicLoc];

		*pnWidth = pupUserPic->nWidth;  *pnHeight = pupUserPic->nHeight;
		break;

	// ムービーの場合
	case AM_MOVIE_HANDLE_HEAD:
		return GetMovieSize( hPicture, pnWidth, pnHeight );
		break;

	default:
		nResult = AMERR_INVALIDHANDLE;
		break;
	}

	return nResult;
}


// 現在表示されているグラフィック取得
extern int GetDispGraph( int hPicture )
{
	int					nResult;								// リザルトコード
	int					nUserPicLoc;							// ユーザー絵位置
	AM_USER_PICTURE		*pupUserPic;							// ユーザー絵

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// 位置
	if ( nUserPicLoc >= fg_nUserPicNum ) return AMERR_INVALIDHANDLE;	// 妥当性チェック

	// 表示画面取得
	pupUserPic = &fg_UserPictureBuf[nUserPicLoc];
	nResult = AM_GetDisplayBuffer( pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ),
								   AM_GET_FRONT_BUFFER );

	return nResult;
}


// ビットマップのセーブ
extern int SaveBmp( int x1, int y1, int x2, int y2, char *szFileName )
{
	int							nResult;
	AM_RECT						rcSaveRect;
	int							nTemp;

	// 大小関係整合
	if ( x1 > x2 ) { nTemp = x1;  x1 = x2;  x2 = nTemp; }		
	if ( y1 > y2 ) { nTemp = y1;  y1 = y2;  y2 = nTemp; }

	// サチレーション
	if ( x1 < 0 ) x1 = 0;
	if ( y1 < 0 ) y1 = 0;
	if ( x2 > gg_siDispSize.cx ) x2 = gg_siDispSize.cx;
	if ( y2 > gg_siDispSize.cy ) y2 = gg_siDispSize.cy;

	// 大きさチェック
	if ( x1 == x2 || y1 == y2 ) return AMERR_INVALIDSIZE;

	// コピー
	rcSaveRect.left  = x1;  rcSaveRect.top    = y1;
	rcSaveRect.right = x2;  rcSaveRect.bottom = y2;

	// セーブ
	nResult = AM_SaveDisplayBuffer( &rcSaveRect, szFileName, AM_GET_FRONT_BUFFER );

	return nResult;
}


// ロード画像データオフセット設定(ダミー)
extern int SetBmpOffset( int nOffset )
{
	return AM_OK;
}


// 描画モードの設定
extern int SetDrawMode( int nDrawMode, int nParam )
{
	if ( !gg_bDispInitialized ) return AMERR_NOINITIALIZE;		// 初期化チェック

	// テクスチャサンプルモード
	if ( nDrawMode & AMDRAW_NEAREST ) {							// ニアレストネイバー
			AM_SetTexSampleState( AMDRAW_NEAREST );
	}
	if ( nDrawMode & AMDRAW_BILINEAR ) {						// バイリニアフィルタリング
			AM_SetTexSampleState( AMDRAW_BILINEAR );
	}

	// ブレンドモード
	if ( nDrawMode & AMDRAW_NOBLEND ) {							// ブレンドなし
			AM_SetBlendState( AMDRAW_NOBLEND, 0 );
	}
	if ( nDrawMode & AMDRAW_ALPHABLEND ) {						// アルファブレンディング
			AM_SetBlendState( AMDRAW_ALPHABLEND, nParam );
	}
	if ( nDrawMode & AMDRAW_ADDBLEND ) {						// 加算ブレンディング
			AM_SetBlendState( AMDRAW_ADDBLEND, nParam );
	}

	// テクスチャラップモード
	if ( nDrawMode & AMDRAW_TEX_NOREPEAT ) {					// テクスチャリピートなし
		AM_SetTexWrapState( AMDRAW_TEX_NOREPEAT );				// テクスチャクランプ
	}
	if ( nDrawMode & AMDRAW_TEX_REPEAT ) {						// テクスチャリピートあり
		AM_SetTexWrapState( AMDRAW_TEX_REPEAT );				// テクスチャクランプ
	}

	return AM_OK;
}


// ハンドルからユーザーピクチャ取得
extern int AB_GetUserPicture( int hPicture, AM_USER_PICTURE **ppupUserPicture )
{
	int					nUserPicLoc;							// ユーザー絵位置

	nUserPicLoc = hPicture & ~AM_HANDLE_HEAD_MASK;				// 位置
	if ( nUserPicLoc >= fg_nUserPicNum ) return AMERR_INVALIDHANDLE;	// 妥当性チェック

	// 絵と情報取得
	*ppupUserPicture = &fg_UserPictureBuf[nUserPicLoc];

	return AM_OK;
}


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AB_3DGraph.cpp
//
// 機能: DirectX上でのＡＭＧＡＭＥの３Ｄ画面描画機能をブリッジ。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#pragma warning( disable : 4819 )						// コードページの警告抑止


// ---------------------------------------------------------------------------
// インクルード域

#include "amgame.h"												// AMGAMEヘッダ

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnvヘッダ
#include "AB_2DGraph.h"
#include "AM_3DGraph.h"
#include "AB_3DGraph.h"

#endif


//-----------------------------------------------------------------------------
// 3D画像描画関数域

// ポリゴン描画
extern int DrawPolygons( AmVertex *pVertices, int nVertexNum, int hTexture, int nFlags )	
{
	int							nResult = AM_OK;
	AM_USER_PICTURE				*pUserPicture;

	if ( hTexture >= 0 && ( nResult = AB_GetUserPicture( hTexture, &pUserPicture ) ) == AM_OK ) {	// テクスチャありの場合
		AM_Draw3DPolygons( pVertices, nVertexNum,
						   pUserPicture->nLeft, pUserPicture->nTop,
						   pUserPicture->pUnitPic->pPicture, &( pUserPicture->pUnitPic->a2PicInfo ), nFlags );
	}
	else {														// テクスチャ無しの場合
		AM_Draw3DPolygons( pVertices, nVertexNum,
						   0, 0,
						   NULL, NULL, nFlags );
	}

	return nResult;
}


// 線列描画
extern int Draw3DLines( AmVertex *pVertices, int nVertexNum, int hTexture )	
{
	int							nResult = AM_OK;
	AM_USER_PICTURE				*pUserPicture;

	if ( hTexture >= 0 && ( nResult = AB_GetUserPicture( hTexture, &pUserPicture ) ) == AM_OK ) {	// テクスチャありの場合
		AM_Draw3DLines( pVertices, nVertexNum,
						pUserPicture->nLeft, pUserPicture->nTop,
						pUserPicture->pUnitPic->pPicture, &( pUserPicture->pUnitPic->a2PicInfo ) );
	}
	else {														// テクスチャ無しの場合
		AM_Draw3DLines( pVertices, nVertexNum,
						0, 0,
						NULL, NULL );
	}

	return nResult;
}


//-----------------------------------------------------------------------------
// 3Dツール関数域

// ライティングあり頂点データセット
extern int SetLightVertex( AmVertex *pVertex, AM3D_VALUE x, AM3D_VALUE y, AM3D_VALUE z,
						   AM3D_VALUE u, AM3D_VALUE v,
						   AM3D_VALUE nx, AM3D_VALUE ny, AM3D_VALUE nz )
{
	pVertex->x = x;  pVertex->y = y;  pVertex->z = z;  
	pVertex->u = u;  pVertex->v = v;
	pVertex->nx = nx;  pVertex->ny = ny;  pVertex->nz = nz;

	return AM_OK;
}


// ライティングなし頂点データセット
extern int SetColorVertex( AmVertex *pVertex, AM3D_VALUE x, AM3D_VALUE y, AM3D_VALUE z,
						   AM3D_VALUE u, AM3D_VALUE v,
						   unsigned char r, unsigned char g, unsigned char b, unsigned char a )
{
	pVertex->x = x;  pVertex->y = y;  pVertex->z = z;  
	pVertex->u = u;  pVertex->v = v;
	pVertex->r = r;  pVertex->g = g;  pVertex->b = b;  pVertex->a = a;

	return AM_OK;
}


// ローカル→ワールド行列セット
extern int SetLWMatrix( AmMatrix *pMatrix )
{
	int							nResult;

	nResult = AM_SetTransformMatrix( AM_MATRIX_LW, pMatrix );

	return nResult;
}


// ワールド→ビュー行列セット
extern int SetWVMatrix( AmMatrix *pMatrix )
{
	int							nResult;

	nResult = AM_SetTransformMatrix( AM_MATRIX_WV, pMatrix );

	return nResult;
}


// ローカル→ワールド行列取得
extern int GetLWMatrix( AmMatrix *pMatrix )
{
	int							nResult;

	nResult = AM_GetTransformMatrix( AM_MATRIX_LW, pMatrix );

	return nResult;
}


// ワールド→ビュー行列取得
extern int GetWVMatrix( AmMatrix *pMatrix )
{
	int							nResult;

	nResult = AM_GetTransformMatrix( AM_MATRIX_WV, pMatrix );

	return nResult;
}


// 行列の積計算
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


// 外積計算
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
// 行列セット関数域

// 単位行列セット
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


// 回転＋平行移動行列セット
extern int SetRTMatrix( AmMatrix *pDest, AM3D_VALUE tx, AM3D_VALUE ty, AM3D_VALUE tz,
										 AM3D_VALUE rx, AM3D_VALUE ry, AM3D_VALUE rz )
{
//	int				i, j;
	AmMatrix		mtRes;							// 結果行列
	AmMatrix		mtPres;							// 現在行列

	// x軸回転
	SetIdentityMatrix( &mtRes );					// 単位行列に
	mtRes.m[1][1] =  cosf( rx );  mtRes.m[1][2] =  sinf( rx );
	mtRes.m[2][1] = -sinf( rx );  mtRes.m[2][2] =  cosf( rx );

	// y軸回転
	SetIdentityMatrix( &mtPres );					// 単位行列に
	mtPres.m[0][0] =  cosf( ry );  mtPres.m[0][2] = -sinf( ry );
	mtPres.m[2][0] =  sinf( ry );  mtPres.m[2][2] =  cosf( ry );
	MultMatrix( &mtRes, &mtRes, &mtPres );			// 変換を加える

	// z軸回転
	SetIdentityMatrix( &mtPres );					// 単位行列に
	mtPres.m[0][0] =  cosf( rz );  mtPres.m[0][1] =  sinf( rz );
	mtPres.m[1][0] = -sinf( rz );  mtPres.m[1][1] =  cosf( rz );
	MultMatrix( &mtRes, &mtRes, &mtPres );			// 変換を加える

	mtRes.m[3][0] = tx;  mtRes.m[3][1] = ty;  mtRes.m[3][2] = tz;

	*pDest = mtRes;

	return AM_OK;
}


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AB_Palette.cpp
//
// 機能: DirectX上でのＡＭＧＡＭＥの２Ｄ画面描画機能をブリッジ。
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

#include "amgame.h"												// AMGAMEヘッダ

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnvヘッダ
#include "AB_DispEnv.h"											// AMDispEnvヘッダ
#include "AM_2DGraph.h"											// AM2DGraphヘッダ
#include "AB_2DGraph.h"											// AB2DGraphヘッダ
#include "AB_Palette.h"											// ABPaletteヘッダ

#endif


// ---------------------------------------------------------------------------
// 内部定数定義域



//-----------------------------------------------------------------------------
// パレット制御（廃止済み、ダミー）関数域

// パレット設定（ダミー）
extern void SetPalette( int nEntry, int gbr, int rbr, int bbr )
{
	return;
}


// パレット取得（ダミー）
extern int GetPalette( int nEntry, int *gbr, int *rbr, int *bbr )
{
	return AM_OK;
}


// ＢＭＰパレット設定（ダミー）
extern int SetBmpPal( char *filename )
{
	return AM_OK;
}


// ＢＭＰパレット部分設定（ダミー）
extern int SetBmpPalPart( char *filename, int FirstEnt, int EntNum )
{
	return AM_OK;
}


// メモリへのパレットロード（ダミー）
extern int LoadPalMem( char *szFileName )
{
	return AM_OK;
}


// メモリパレットセット（ダミー）
extern int SetPalMem( int hPalette )
{
	return AM_OK;
}


// メモリパレット部分セット（ダミー）
extern int SetPalMemPart( int hPalette, int nFirstEnt, int nEntNum )
{
	return AM_OK;
}


// パレット用メモリ取得（ダミー）
extern int MakePalMem( void )
{
	return AM_OK;
}


// メモリパレットへパレットセット（ダミー）
extern int SetPalToMem( int hPalette, int nEntry, int gbr, int rbr, int bbr )
{
	return AM_OK;
}


// メモリパレットからパレット取得（ダミー）
extern int GetPalFromMem( int hPalette, int nEntry,
						  int *gbr, int *rbr, int *bbr )
{
	return AM_OK;
}


// パレットメモリの開放（ダミー）
extern int FreePalMem( int hPalette )
{
	return AM_OK;
}


// パレットリージョンの設定（ダミー）
extern int SetPalArea( int under, int over )
{
	return AM_OK;
}


//-----------------------------------------------------------------------------
// トーンコントロール関数域

// 描画明度の設定
extern int SetDrawBright( int nRedBright, int nGreenBright, int nBlueBright )
{
	int							nResult;

	nResult = AM_SetDrawColor( nRedBright   * AM_MAX_COLOR_FACTOR / AM_MAX_DRAW_BRIGHT,
							   nGreenBright * AM_MAX_COLOR_FACTOR / AM_MAX_DRAW_BRIGHT,
							   nBlueBright  * AM_MAX_COLOR_FACTOR / AM_MAX_DRAW_BRIGHT );

	return nResult;
}


// パレット色調コントロール
extern int ControlPal( int nBrGreen, int nBrRed, int nBrBlue )
{
	int							nResult;

	nResult = SetDrawBright( nBrRed, nBrGreen, nBrBlue );		// これから描く絵の色調のみ

	return nResult;
}


// 画面明度の変化
extern int MovePalette( int nSGreen, int nSRed, int nSBlue,
						int nDGreen, int nDRed, int nDBlue, int nSpeed )
{
	int				i;
	int				hDispBuffer;
	float			nPresRed, nPresGreen, nPresBlue;
	float			nRedDel, nGreenDel, nBlueDel;
	AM_BLEND_STATE		bsBlendBuf;								// ブレンドステート退避場所

	if ( !gg_pD3DDevice ) return AMERR_CANNOT;					// イニシャライズチェック
	if ( nSpeed <= 0 ) return AMERR_INVALIDVALUE;				// 速度チェック

	// 輝度保存
	AM_GetBlendState( &bsBlendBuf );							// ブレンドステート取得

	// 現在の色・色速度計算
	nPresRed   = (float)( nSRed   * 10 );
	nPresGreen = (float)( nSGreen * 10 );
	nPresBlue  = (float)( nSBlue  * 10 );
	nRedDel   = ( nDRed   - nSRed   ) * 10.0f / nSpeed;
	nGreenDel = ( nDGreen - nSGreen ) * 10.0f / nSpeed;
	nBlueDel  = ( nDBlue  - nSBlue  ) * 10.0f / nSpeed;

	hDispBuffer = MakeTextureMem( gg_siDispSize.cx, gg_siDispSize.cy, 0 );	// 画像バッファ作成
	if ( hDispBuffer < 0 ) return hDispBuffer;
	GetDispGraph( hDispBuffer );								// 現在の画像取得

	for ( i = 0; i < nSpeed; i++ ) {
		nPresRed   += nRedDel;
		nPresGreen += nGreenDel;
		nPresBlue  += nBlueDel;

		SetDrawBright( ( int )nPresRed, ( int )nPresGreen, ( int )nPresBlue );
		DrawMem( 0, 0, hDispBuffer );
		Flip();
		if ( !ProcessEvent() ) break;							// 終了なら抜ける
	}

	AB_ReleaseLastGraph( hDispBuffer );							// 一時画像バッファ開放
	AM_RestoreBlendState( &bsBlendBuf );						// ブレンドステート復帰

	return AM_OK;
}


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AB_FigureDraw.cpp
//
// 機能: DirectX上でのＡＭＧＡＭＥの２Ｄ画面描画機能をブリッジ。
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
//#include <d3dx9.h>

#include "amgame.h"												// AMGAMEヘッダ

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnvヘッダ
#include "AM_2DGraph.h"											// AM2DGraphヘッダ
#include "AB_2DGraph.h"											// AB2DGraphヘッダ
#include "AB_Debug.h"											// ABDebugヘッダ
#include "AM_FigureDraw.h"										// AMFigureDrawヘッダ
#include "AB_FigureDraw.h"										// AMFigureDrawヘッダ

#endif


//-----------------------------------------------------------------------------
// 図形描画関数域

// 1点の描画
extern void DrawPixel( int x, int y, int color )
{
	int							nResult;
	AM_POINT_DATA				apDrawPoint;					// 描画ポイント

	apDrawPoint.x = x;  apDrawPoint.y = y;  apDrawPoint.z = 0;	// 座標
	apDrawPoint.nColor = color;

	nResult = AM_DrawPointArray( &apDrawPoint, 1 );				// 点描画

	return;
}


// 色コードの取得
extern int GetColor( int nRed, int nGreen, int nBlue )
{
	int							nResult;

	// 色サチレーション
	if ( nRed   < 0 ) nRed   = 0;  if ( nRed   > 255 ) nRed   = 255;
	if ( nGreen < 0 ) nGreen = 0;  if ( nGreen > 255 ) nGreen = 255;
	if ( nBlue  < 0 ) nBlue  = 0;  if ( nBlue  > 255 ) nBlue  = 255;

	nResult = AM_GetColorCode( nRed, nGreen, nBlue );			// 色ゲット

	return nResult;
}


// 指定点の色取得
extern int GetPixel( int x, int y )
{
	int							nResult;

	// エラーチェック
	if ( x < 0 || x > gg_siDispSize.cx || y < 0 || y > gg_siDispSize.cy ) return AMERR_OUTOFRANGE;

	nResult = AM_GetBackbufferPointColor( x, y );

	return nResult;
}


// 長方形の描画
extern void DrawFBox( int x1, int y1, int x2, int y2, int color )
{
	AM_POINT_DATA				apRectPoint[2];					// 左上・右下の点

	// 左上
	apRectPoint[0].x = x1;  apRectPoint[0].y = y1;  apRectPoint[0].z = 0;
	apRectPoint[0].nColor = color;

	// 右下
	apRectPoint[1].x = x2;  apRectPoint[1].y = y2;  apRectPoint[1].z = 0;
	apRectPoint[1].nColor = color;

	AM_DrawRectangleArray( apRectPoint, 1 );					// 描画

	// デバッグ情報をセット
	if ( gg_bRegisterDebug ) {
		AB_DRAWING_DEBUG_INFO		adDebugInfo;				// デバッグ情報
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWFBOX;			// DrawFBox命令
		adDebugInfo.x1 = x1;									// 描画x座標
		adDebugInfo.y1 = y1;									// 描画y座標
		adDebugInfo.x2 = x2;									// 描画x座標2
		adDebugInfo.y2 = y2;									// 描画y座標2
		adDebugInfo.nColor = color;								// 描画色
		adDebugInfo.nBlendKind = 0;								// ブレンド
		AB_SetDrawingDebugInfo( &adDebugInfo );					// デバッグ情報セット
	}

	return;
}


// 半透明長方形の描画
extern void DrawTBox( int x1, int y1, int x2, int y2, int color )
{
	AM_POINT_DATA				apRectPoint[2];					// 左上・右下の点
	AM_BLEND_STATE				bsBlendBuf;						// ブレンドステート退避場所

	// 左上
	apRectPoint[0].x = x1;  apRectPoint[0].y = y1;  apRectPoint[0].z = 0;
	apRectPoint[0].nColor = ( color & 0xffffff ) | 0x80000000;

	// 右下
	apRectPoint[1].x = x2;  apRectPoint[1].y = y2;  apRectPoint[1].z = 0;
	apRectPoint[1].nColor = ( color & 0xffffff ) | 0x80000000;

	AM_GetBlendState( &bsBlendBuf );							// ブレンドステート取得
	AM_SetBlendState( AMDRAW_ALPHABLEND, AM_MAX_BLEND_FACTOR / 2 );		// アルファブレンド

	AM_DrawRectangleArray( apRectPoint, 1 );					// 描画

	AM_RestoreBlendState( &bsBlendBuf );						// ブレンドステート復帰

	// デバッグ情報をセット
	if ( gg_bRegisterDebug ) {
		AB_DRAWING_DEBUG_INFO		adDebugInfo;				// デバッグ情報
		adDebugInfo.nObjectKind = AB_DEBUG_DRAWFBOX;			// DrawFBox命令
		adDebugInfo.x1 = x1;									// 描画x座標
		adDebugInfo.y1 = y1;									// 描画y座標
		adDebugInfo.x2 = x2;									// 描画x座標2
		adDebugInfo.y2 = y2;									// 描画y座標2
		adDebugInfo.nColor = color;								// 描画色
		adDebugInfo.nBlendKind = AMDRAW_ALPHABLEND;				// ブレンド
		AB_SetDrawingDebugInfo( &adDebugInfo );					// デバッグ情報セット
	}

	return;
}


// 文字列の描画
extern int DrawString( int x, int y, char *szString, int nColor )
{
	int							nResult;

	nResult = AM_DrawString( x, y, szString, nColor );

	return nResult;
}


// ﾌｫﾝﾄｻｲｽﾞの変更
extern int SetFontSize( int nFontSize )
{
	int							nResult;

	// サチレーション
	if ( nFontSize < AM_MIN_FONT_SIZE ) {
		nFontSize = AM_MIN_FONT_SIZE;
	}
	if ( nFontSize > AM_MAX_FONT_SIZE ) {
		nFontSize = AM_MAX_FONT_SIZE;
	}

	nResult = AM_SetStringFontSize( nFontSize );				// フォントサイズ設定

	return AM_OK;
}


//-----------------------------------------------------------------------------
// 以上
//
// 機能: DirectX上でのＡＭＧＡＭＥのユーザ入力機能をブリッジ。
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

#include "AM_DispEnv.h"											// AMDispEnvヘッダ
#include "AM_Input.h"											// キー入力　機種依存レイヤー
#include "AB_Input.h"											// キー入力　ブリッジ

#endif


// ---------------------------------------------------------------------------
// キーボード制御関数域

// 一文字入力
extern int GetCh( void )
{
	return AM_GetSingleChar();
}


// キー入力チェック
extern int KBHit( void )
{
	return AM_IsKeyHit();
}


// キーが押されているかチェック
extern int CheckKey( int nInputMode )
{
	int							i;
	int							nResult = 0;

	switch ( nInputMode ) {
	// マルチプル入力
	case AMINPUT_MULT:
		
		nResult = AM_AddKeyInput( 0 );							// キーボード入力

		for ( i = 0; i < gg_nJoyPadNum; i++ ) {					// パッド入力
			nResult = AM_AddJoyInput( i, nResult );
		}
		break;

	// キーボード入力
	case AMINPUT_KEYBOARD:										
		nResult = AM_AddKeyInput( 0 );
		break;

	// パッド1入力
	case AMINPUT_PAD1:
		nResult = AM_AddJoyInput( 0, 0 );
		break;

	// パッド2入力
	case AMINPUT_PAD2:										
		nResult = AM_AddJoyInput( 1, 0 );
		break;

	// 不正入力
	default:
		nResult = AMERR_INVALIDVALUE;
		break;
	}


	return nResult;
}


// ---------------------------------------------------------------------------
// マウス制御関数域

// マウス初期化
extern int InitMouse( int bMouseState )
{
	int							nResult;
	
	nResult = AM_InitMouse( bMouseState );

	return nResult;
}


// マウス状態設定
extern int SetMouseState( int bMouseState )
{
	int							nResult;
	
	nResult = AM_SetMouseState( bMouseState );

	return nResult;
}


// マウス位置取得
extern int GetMouseLoc( int *nXLoc, int *nYLoc )
{
	int							nResult;
	
	nResult = AM_GetMousePosition( nXLoc, nYLoc );

	return nResult;
}


// マウス位置設定
extern int SetMouseLoc( int nXLoc, int nYLoc )
{
	int							nResult;
	
	nResult = AM_SetMouseLoc( nXLoc, nYLoc );

	return nResult;
}


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AB_Movie.cpp
//
// 機能: DirectX上でのＡＭＧＡＭＥのムービー再生機能をブリッジ。
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

#include "amgame.h"												// AMGAMEヘッダ

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnvヘッダ
#include "AM_2DGraph.h"											// AM2DGraphヘッダ
#include "AB_2DGraph.h"											// AB2DGraphヘッダ
#include "AB_Time.h"											// AMMovieヘッダ
#include "AM_Movie.h"											// AMMovieヘッダ
#include "AB_Movie.h"											// AMMovieヘッダ

#endif


// ---------------------------------------------------------------------------
// ファイル内グローバル変数域

AMMOVIE					MovieBuf[AM_MAX_MOVIE_NUM];				// ムービーバッファ
int						fg_nMovieNum = 0;						// ムービー数


//-----------------------------------------------------------------------------
// ムービー再生関数域

// ムービー再生
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


// バックムービー再生(ダミー)
extern int PlayMovieBack( int x, int y, char *szFileName )
{
	int		nResult;

	nResult = PlayMovie( x, y, szFileName );
//	nResult = AM_PlayMovie( szFileName, x, y, 0, 0 );

	return nResult;
}


// ﾌﾙｽｸﾘｰﾝムービー再生
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


// ﾌﾙｽｸﾘｰﾝﾊﾞｯｸﾑｰﾋﾞｰ再生(ダミー)
extern int PlayMovieFullBack( char *szFileName )
{
	int		nResult;

	nResult = PlayMovieFull( szFileName );

	return nResult;
}


/*extern int SetMovieTime( int nFirst, int nLast )	// ムービー再生時間設定
{
	nMovieFirst = nFirst;
	nMovieLast = nLast;

	return AM_OK;
}*/


extern int LoadMovieMem( char *szFileName )			// ムービーのメモリへのロード
{
	int					nResult;

	if ( fg_nMovieNum >= AM_MAX_MOVIE_NUM ) return AMERR_NOFREEHANDLE;

	nResult = AM_OpenMovieFile( szFileName, &MovieBuf[fg_nMovieNum] );
	if ( nResult != AM_OK ) return nResult;
	fg_nMovieNum++;

	return AM_MOVIE_HANDLE_HEAD + fg_nMovieNum - 1;
}


extern int PlayMovieMem( int x, int y, int hMovie )		// メモリムービーの再生
{
	int					nResult;

	if ( ( hMovie & AM_HANDLE_HEAD_MASK ) != AM_MOVIE_HANDLE_HEAD ) return AMERR_INVALIDHANDLE;
	hMovie &= ~AM_HANDLE_HEAD_MASK;
	if ( hMovie < 0 || hMovie >= fg_nMovieNum ) return AMERR_INVALIDHANDLE;

	nResult = AM_PlayMovieFile( x, y, &MovieBuf[hMovie] );

	return nResult;
}


extern int PlayMovieMemFull( int hMovie )			// メモリムービーのフル再生
{
	int					nResult;

	if ( ( hMovie & AM_HANDLE_HEAD_MASK ) != AM_MOVIE_HANDLE_HEAD ) return AMERR_INVALIDHANDLE;
	hMovie &= ~AM_HANDLE_HEAD_MASK;
	if ( hMovie < 0 || hMovie >= fg_nMovieNum ) return AMERR_INVALIDHANDLE;

	nResult = AM_PlayMovieFile( MOVIE_FULL_LOC, MOVIE_FULL_LOC, &MovieBuf[hMovie] );

	return nResult;
}


extern int GetMovieFrame( int hPicture, int hMovie, int nFrameNum )	// ムービーの特定フレーム取得
{
	int							nResult;
	AM_USER_PICTURE				*pupUserPic;					// ユーザー絵

	// ムービーハンドル処理
	if ( ( hMovie & AM_HANDLE_HEAD_MASK ) != AM_MOVIE_HANDLE_HEAD ) return AMERR_INVALIDHANDLE;
	hMovie &= ~AM_HANDLE_HEAD_MASK;
	if ( hMovie < 0 || hMovie >= fg_nMovieNum ) return AMERR_INVALIDHANDLE;

	// 絵ハンドル処理
	nResult = AB_GetUserPicture( hPicture, &pupUserPic );		// ユーザー絵取得
	if ( nResult < 0 ) return nResult;

	nResult = AM_GetMovieFrame( &MovieBuf[hMovie], nFrameNum,
								pupUserPic->pUnitPic->pPicture, &( pupUserPic->pUnitPic->a2PicInfo ) );

	return nResult;
}


extern int GetMovieSize( int hMovie, int *nWidth, int *nHeight )	// ムービーサイズの取得
{
	if ( ( hMovie & AM_HANDLE_HEAD_MASK ) != AM_MOVIE_HANDLE_HEAD ) return AMERR_INVALIDHANDLE;
	hMovie &= ~AM_HANDLE_HEAD_MASK;
	if ( hMovie < 0 || hMovie >= fg_nMovieNum ) return AMERR_INVALIDHANDLE;
	
	*nWidth  = MovieBuf[hMovie].AVIInfo.dwWidth;
	*nHeight = MovieBuf[hMovie].AVIInfo.dwHeight;

	return AM_OK;
}


extern int GetMovieCount( int hMovie )				// ムービーフレームカウント数の取得
{
	int				i;
	double			fNowTime;

	if ( ( hMovie & AM_HANDLE_HEAD_MASK ) != AM_MOVIE_HANDLE_HEAD ) return AMERR_INVALIDHANDLE;
	hMovie &= ~AM_HANDLE_HEAD_MASK;
	if ( hMovie < 0 || hMovie >= fg_nMovieNum ) return AMERR_INVALIDHANDLE;

	if ( MovieBuf[hMovie].fFirstTime < 0 ) {		// フレーム初期化なしの場合
		MovieBuf[hMovie].fFirstTime = GetNowTime( 0 ) / 1000.0;
		MovieBuf[hMovie].nPresFrame = 0;
	}
	else {
		fNowTime = GetNowTime( 0 ) / 1000.0 - MovieBuf[hMovie].fFirstTime;
		for ( i = 1; i <= 2; i++ ) { 
			if ( fNowTime >= ( MovieBuf[hMovie].nPresFrame + 1 ) * MovieBuf[hMovie].fVidSampDelay )	// フレーム進むか
			{
				MovieBuf[hMovie].nPresFrame++;
			}
		}
	}
	if ( MovieBuf[hMovie].nPresFrame >= ( int )MovieBuf[hMovie].VideoInfo.dwLength ) {	// 終了ならリセット
		MovieBuf[hMovie].fFirstTime = -1;
		MovieBuf[hMovie].nPresFrame = -1;
	}

	if ( MovieBuf[hMovie].nPresFrame < 0 ) return AM_ENDOFSTREAM;
	return MovieBuf[hMovie].nPresFrame;
}


extern int InitMovieMem( void )						// ムービー用メモリの開放
{
	int					i;

	for ( i = 0; i < fg_nMovieNum; i++ ) {
		AM_CloseMovieFile( &MovieBuf[i] );
	}
	fg_nMovieNum = 0;

	return AM_OK;
}


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AB_Time.cpp
//
// 機能: DirectX上でのＡＭＧＡＭＥの時間機能をブリッジ。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#pragma warning( disable : 4819 )						// コードページの警告抑止


#include "amgame.h"												// AMGAMEヘッダ

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// キー入力　ブリッジレイヤー
#include "AB_Input.h"											// キー入力　ブリッジレイヤー
#include "AM_Time.h"											// 時間機能　ブリッジレイヤー

#endif


// ---------------------------------------------------------------------------
// ファイル内グローバル変数域



//-----------------------------------------------------------------------------
// 時間関数域

// キー入力待ち・入力キー取得関数
extern int GetNowTime( int nDummy )
{
	int							nResult;

	nResult = AM_GetPresentTime();

	return nResult;
}


// タイマで待つ（１／１０００秒単位）
extern int WaitByTimer( int nWaitTime )
{
	int			nFirstTime;

	nFirstTime = AM_GetPresentTime();
	while( AM_ProcessEvent() && AM_GetPresentTime() - nFirstTime < nWaitTime ) {
		if ( CheckKey( AMINPUT_MULT ) ) break;
	}

	return AM_OK;
}


// 日時の取得
extern int GetDateTime( AMDATETIME *pDateTime )
{
	int							nResult;

	nResult = AM_GetDateTime( pDateTime );

	return nResult;
}


// 日時のアンパック
extern int UnpackDateTime( AMDATETIME *pDateTime )
{
	int		nDate, nTime;										// 日時

	nDate = pDateTime->nPackedTime >> 16;						// 年月日
	nTime = pDateTime->nPackedTime & 0xffff;					// 時分秒

	pDateTime->nYear   = (unsigned int )( ( nDate >> 9  ) & 0x7f );	// 年
	pDateTime->nYear   += 1980;									// 年オフセット
	pDateTime->nMonth  = (unsigned char)( ( nDate >> 5  ) & 0xf 	);	// 月
	pDateTime->nDay    = (unsigned char)(   nDate         & 0x1f );	// 日
	pDateTime->nHour   = (unsigned char)( ( nTime >> 11 ) & 0x1f );	// 時
	pDateTime->nMinute = (unsigned char)( ( nTime >> 5  ) & 0x3f );	// 分
	pDateTime->nSecond = (unsigned char)( ( nTime         & 0x1f ) << 1 );	// 秒

	return AM_OK;
}


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AB_Debug.cpp
//
// 機能: DirectX上でのＡＭＧＡＭＥのデバッグ機能をブリッジ。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2005 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif

#pragma warning( disable : 4819 )						// コードページの警告抑止


#include <stdio.h>
#include "amgame.h"												// AMGAMEヘッダ

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// ディスプレイ環境　ブリッジレイヤー
#include "AB_Input.h"											// キー入力　ブリッジレイヤー
#include "AM_2DGraph.h"											// AM2DGraphヘッダ
#include "AM_FigureDraw.h"										// AMFigureDrawヘッダ
#include "AB_Debug.h"											// 自分ヘッダ

#endif

#define MAX_DEBUG_STRING_LEN		100							// 最大デバッグ文字列長
#define MAX_DEBUG_STRING_NUM		( 8 * 60 )					// 最大デバッグオブジェクト数
#define MAX_DEBUG_DRAWING_OBJECTS	( 8 * 60 )					// 最大描画デバッグオブジェクト数
#define DEBUG_COLOR_VALS			5							// デバッグ色バリエーション数


// ---------------------------------------------------------------------------
// インターファイルグローバル変数域

int								gg_bRegisterDebug = false;		// デバッグ情報を登録するか
int								gg_bDisplayDebug = false;		// デバッグ情報を表示するか
int								gg_bDebugKey = true;			// デバッグ用キー入力を有効にするか
int								gg_bDebugFile = true;			// デバッグファイル出力を有効にするか


// ---------------------------------------------------------------------------
// ファイル内グローバル変数域

// デバッグ文字列関係
char							fg_szMyDebugStrings[MAX_DEBUG_STRING_NUM][MAX_DEBUG_STRING_LEN];	// 表示デバッグ文字列
int								fg_nMyDebugStringNum = 0;			// デバッグ文字列数

// 描画系デバッグオブジェクト関係
AB_DRAWING_DEBUG_INFO			fg_adDrawingDebugObjects[MAX_DEBUG_DRAWING_OBJECTS];	// 表示する描画デバッグオブジェクト
int								fg_nMyDrawingDebugObjects = 0;		// 描画デバッグオブジェクト数

// デバッグ表示属性
int								fg_nDebugColor = 0xffffffff;	// デバッグ表示の色
int								fg_nDebugColorVals[DEBUG_COLOR_VALS] = { 0xffffffff, 0xffff0000, 0xff00ff00, 0xff0000ff, 0xff000000 };
int								fg_nPresentDebugColor = 0;		// 現在のデバッグ色


// ---------------------------------------------------------------------------
// 内部関数プロトタイプ宣言域

static int ToggleDisplayDebug( void );							// デバッグ表示のＯＮ／ＯＦＦ切り替え
static int ChangeDebugColor( void );							// デバッグ表示色の切り替え



//-----------------------------------------------------------------------------
// ユーザー公開関数域

// デバッグモードのセット
extern int SetAMDebugMode( int nDebugMode )
{
	switch( nDebugMode ) {
	// 全てのデバッグ機能をオフ
	case AMDEBUG_ALL_OFF:
		gg_bRegisterDebug = false;								// 登録
		gg_bDisplayDebug  = false;								// 表示
		gg_bDebugKey      = false;								// キー
		gg_bDebugFile     = false;								// ファイル
		break;

	// 全てのデバッグ機能をオン
	case AMDEBUG_ALL_ON:
		gg_bRegisterDebug = true;								// 登録
		gg_bDisplayDebug  = true;								// 表示
		gg_bDebugKey      = true;								// キー
		gg_bDebugFile     = true;								// ファイル
		break;

	// デバッグ情報登録をオフ
	case AMDEBUG_REGISTER_OFF:
		gg_bRegisterDebug = false;								// 登録
		break;

	// デバッグ情報登録をオン
	case AMDEBUG_REGISTER_ON:
		gg_bRegisterDebug = true;								// 登録
		break;

	// デバッグ情報表示をオフ
	case AMDEBUG_DISPLAY_OFF:
		gg_bDisplayDebug = false;								// 表示
		break;

	// デバッグ情報表示をオン
	case AMDEBUG_DISPLAY_ON:
		gg_bDisplayDebug = true;								// 表示
		break;

	// デバッグキーをオフ
	case AMDEBUG_KEY_OFF:
		gg_bDebugKey = false;									// キー
		break;

	// デバッグキーをオン
	case AMDEBUG_KEY_ON:
		gg_bDebugKey = true;									// キー
		break;

	// ファイルをオフ
	case AMDEBUG_FILE_OFF:
		gg_bDebugFile = false;									// ファイル
		break;

	// ファイルをオン
	case AMDEBUG_FILE_ON:
		gg_bDebugFile = true;									// ファイル
		break;

	}

	return AM_OK;
}


// デバッグモードの取得
extern int GetAMDebugMode( void )
{
	int							nResult;

	nResult = 0;
	if ( gg_bRegisterDebug ) nResult += AMDEBUG_REGISTER;		// 登録
	if ( gg_bDisplayDebug  ) nResult += AMDEBUG_DISPLAY;		// 表示
	if ( gg_bDebugKey      ) nResult += AMDEBUG_KEY;			// キー
	if ( gg_bDebugFile     ) nResult += AMDEBUG_FILE;			// ファイル

	return nResult;
}


// デバッグモードの復帰
extern int RestoreAMDebugMode( int nDebugFlags )
{
	// 登録
	if ( nDebugFlags & AMDEBUG_REGISTER ) gg_bRegisterDebug = true;
									 else gg_bRegisterDebug = false;
	// 表示
	if ( nDebugFlags & AMDEBUG_DISPLAY  ) gg_bDisplayDebug = true;
									 else gg_bDisplayDebug = false;
	// キー
	if ( nDebugFlags & AMDEBUG_KEY      ) gg_bDebugKey = true;
									 else gg_bDebugKey = false;
	// ファイル
	if ( nDebugFlags & AMDEBUG_FILE     ) gg_bDebugFile = true;
									 else gg_bDebugFile = false;

	return AM_OK;
}


// デバッグ文字列の出力
extern int OutputAMDebugString( char *szOutString )
{
	// 登録可能か
	if ( fg_nMyDebugStringNum >= MAX_DEBUG_STRING_NUM ||
		 fg_nMyDrawingDebugObjects >= MAX_DEBUG_DRAWING_OBJECTS )
	{
		return AMERR_OVERBUFFER;
	}

	// デバッグオブジェクト登録
	fg_adDrawingDebugObjects[fg_nMyDrawingDebugObjects].nObjectKind = AB_DEBUG_OUT_STRING;
	fg_adDrawingDebugObjects[fg_nMyDrawingDebugObjects].nUsedHandle = fg_nMyDebugStringNum;

	// デバッグ文字列登録
	strncpy( fg_szMyDebugStrings[fg_nMyDebugStringNum], szOutString, MAX_DEBUG_STRING_LEN );
	fg_szMyDebugStrings[fg_nMyDebugStringNum][MAX_DEBUG_STRING_LEN - 1] = 0;	// 確実に終端を置く

	fg_nMyDrawingDebugObjects++;
	fg_nMyDebugStringNum++;

	// デバッグ表示を強制ＯＮに
	gg_bDisplayDebug = true;									// 表示
//	gg_bRegisterDebug = true;									// 登録

	return AM_OK;
}



//-----------------------------------------------------------------------------
// 内部関数域

// 描画系デバッグ情報セット関数
extern int AB_SetDrawingDebugInfo( AB_DRAWING_DEBUG_INFO *pDebugInfo )
{
	// バッファの空きチェック
	if ( fg_nMyDrawingDebugObjects >= MAX_DEBUG_DRAWING_OBJECTS ) return AMERR_OVERBUFFER;

	// 情報をバッファに蓄積
	fg_adDrawingDebugObjects[fg_nMyDrawingDebugObjects] = *pDebugInfo;
	fg_nMyDrawingDebugObjects++;

	return AM_OK;
}


// デバッグ情報表示関数
extern int AB_DisplayDebugInfo( void )
{
	int							i;
	int							x, y;
	char						szCommandName[100];
	char						szOut[MAX_DEBUG_STRING_LEN];
	int							nFontSize;						// フォントサイズ

	// 定数セット
	nFontSize = AM_GetStringFontSize();							// フォントサイズ取得
	x = 0;  y = 0;

	// キャプション表示
	sprintf( szOut, "Frame:%d", GetFrameCounter() );
	AM_DrawString( x, y, szOut, fg_nDebugColor );
	y += nFontSize;

	// 描画系デバッグオブジェクト表示
	for ( i = 0; i < fg_nMyDrawingDebugObjects; i++ ) {
		strcpy( szOut, "Undefined Error!" );					// 出力初期化
		switch( fg_adDrawingDebugObjects[i].nObjectKind ) {
		case AB_DEBUG_DRAWMEM:									// DrawMemの場合
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

		case AB_DEBUG_DRAWMEMLR:								// DrawMemLRの場合
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

		case AB_DEBUG_DRAWMEMEXP:								// DrawMemExpの場合
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

		case AB_DEBUG_DRAWMEMROT:								// DrawMemRotの場合
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

		case AB_DEBUG_DRAWMEMFLEX:								// DrawMemFlexの場合
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

		case AB_DEBUG_DRAWFBOX:									// DrawFBoxの場合
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

		case AB_DEBUG_OUT_STRING:								// デバッグ文字列出力の場合
			strcpy( szOut, fg_szMyDebugStrings[fg_adDrawingDebugObjects[i].nUsedHandle] );
			break;
		}
		// エラーの場合の文字列
		if ( fg_adDrawingDebugObjects[i].bError ) {
			sprintf( szOut, "%s( ERROR ) Handle = %d", szCommandName, fg_adDrawingDebugObjects[i].nUsedHandle );
		}
		AM_DrawString( x, y, szOut, fg_nDebugColor );
		y += nFontSize;
	}

	// オブジェクト初期化
	fg_nMyDrawingDebugObjects = 0;
	fg_nMyDebugStringNum = 0;

	return AM_OK;
}


// デバッグ機能・ポーズコントロール関数
extern int AB_ControlDebugPause( void )
{
	static int					nLastInKey = 0;					// ひとつ前のキー入力
	int							nInKey;							// 現在のキー入力
	int							nPushedKey;						// 今押されたキー
	int							nGetKey;						// ゲットしたキー
	static int					bPauseDemand = false;			// ポーズ要求

	// 押下キーチェック
	nInKey = CheckKey( AMINPUT_MULT );
	nPushedKey = nInKey & ~nLastInKey;
	nLastInKey = nInKey;

	// デバッグ表示キーチェック
	if ( nPushedKey & KEYIN_DEBUG_DISPLAY ) ToggleDisplayDebug();

	// デバッグ色キーチェック
	if ( nPushedKey & KEYIN_DEBUG_COLOR ) ChangeDebugColor();

	// ポーズキー押下チェック
	if ( !bPauseDemand ) {
		if ( !( nPushedKey & KEYIN_DEBUG_PAUSE ) ) return AM_OK;
	}

	// キー待ち
	if ( KBHit() ) GetCh();
	nGetKey = GetCh();

	nLastInKey = 0;

	// 次回ポーズするかチェック
	if ( nGetKey == 'P' || nGetKey == 'p' ) bPauseDemand = true;
									   else bPauseDemand = false;

	// デバッグ表示キーチェック(ポーズ中)
	if ( nGetKey == 'O' || nGetKey == 'o' ) {
		ToggleDisplayDebug();
		nLastInKey |= KEYIN_DEBUG_DISPLAY;
		bPauseDemand = true;									// 再びポーズに
	}

	// デバッグ色キーチェック(ポーズ中)
	if ( nGetKey == 'I' || nGetKey == 'i' ) {
		ChangeDebugColor();
		nLastInKey |= KEYIN_DEBUG_COLOR;
		bPauseDemand = true;									// 再びポーズに
	}

	return AM_OK;
}


// デバッグ表示のＯＮ／ＯＦＦ切り替え
static int ToggleDisplayDebug( void )
{
	if ( gg_bDisplayDebug ) {
		gg_bDisplayDebug  = false;							// 表示
		if ( gg_bRegisterDebug ) gg_bRegisterDebug = false;	// 登録
							else gg_bRegisterDebug = true;
//		gg_bRegisterDebug = false;							// 登録
	}
	else {
		gg_bDisplayDebug  = true;							// 表示
		gg_bRegisterDebug = true;							// 登録
	}

	// オブジェクト初期化
	fg_nMyDrawingDebugObjects = 0;
	fg_nMyDebugStringNum = 0;

	return AM_OK;
}


// デバッグ表示色の切り替え
static int ChangeDebugColor( void )
{
	// 色を進める
	fg_nPresentDebugColor++;
	if ( fg_nPresentDebugColor >= DEBUG_COLOR_VALS ) fg_nPresentDebugColor = 0;

	// 色をセット
	fg_nDebugColor = fg_nDebugColorVals[fg_nPresentDebugColor];

	return AM_OK;
}


// ロード系デバッグ情報出力関数
extern int AB_OutLoadingDebugInfo( AB_LOADING_DEBUG_INFO *pLoadDebugInfo )
{
	FILE						*fp;
	
	if ( ( fp = fopen( AB_DEBUG_FILE_NAME, "a" ) ) == NULL ) return AMERR_FILEIO;

	// コマンド名・ファイル名表示
	switch( pLoadDebugInfo->nObjectKind ) {
	case AB_DEBUG_LOADTEXTURE:									// LoadTexture関数
		fprintf( fp, "LoadTexture ファイル名：\"%s\" ", pLoadDebugInfo->szFileName );
		if ( pLoadDebugInfo->bError ) {
			fprintf( fp, "失敗 " );
			fprintf( fp, "Handle = %d", pLoadDebugInfo->nMadeHandle );
		}
		else {
			fprintf( fp, "成功 " );
			fprintf( fp, "Handle = 0x%x", pLoadDebugInfo->nMadeHandle );
		}
		break;

	case AB_DEBUG_LOADBLKTEXTURE:								// LoadBlkTexture関数
	case AB_DEBUG_LOADBLKTEXTURELR:								// LoadBlkTextureLR関数
		if ( pLoadDebugInfo->nObjectKind == AB_DEBUG_LOADBLKTEXTURE ) fprintf( fp, "LoadBlkTexture " );
																 else fprintf( fp, "LoadBlkTextureLR " );
		fprintf( fp, "ファイル名：\"%s\" ", pLoadDebugInfo->szFileName );
		if ( pLoadDebugInfo->bError ) {
			fprintf( fp, "失敗 " );
			fprintf( fp, "戻り値 = %d", pLoadDebugInfo->nMadeHandle );
		}
		else {
			fprintf( fp, "成功 " );
			fprintf( fp, "Handle = 0x%x～0x%x", pLoadDebugInfo->nMadeHandle, pLoadDebugInfo->nMadeHandleEnd );
		}
		break;

	case AB_DEBUG_RELOADGRAPH:									// ReloadGraph関数
		fprintf( fp, "ReloadGraph ファイル名：\"%s\" ", pLoadDebugInfo->szFileName );
		if ( pLoadDebugInfo->bError ) {
			fprintf( fp, "失敗 " );
		}
		else {
			fprintf( fp, "成功 " );
		}
		fprintf( fp, "Handle = 0x%x", pLoadDebugInfo->nMadeHandle );
		break;
	}

	fprintf( fp, "\n" );
	fclose( fp );

	return AM_OK;
}


//-----------------------------------------------------------------------------
// 以上
