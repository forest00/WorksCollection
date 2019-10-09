// ---------------------------------------------------------------------------
//
// ファイル名: AM_Connect.h
//
// 機能: ライブラリヘッダが結合された時のサービスを提供。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------


#ifndef __AM_CONNECT_INCLUDED__
#define __AM_CONNECT_INCLUDED__

// リンクライブラリ
#pragma comment( lib, "d3d9"   )								// D3D9ライブラリ
#pragma comment( lib, "d3dx9"   )								// D3DX9ライブラリ
#pragma comment( lib, "dsound"  )								// DirectSoundライブラリ
#pragma comment( lib, "dxguid"  )								// DirectX GUIDライブラリ
#pragma comment( lib, "winmm"   )								// マルチメディアライブラリ
#pragma comment( lib, "vfw32"   )								// デジタルビデオライブラリ
#pragma comment( lib, "msacm32" )								// 音声圧縮解凍ライブラリ

// VisualStudio2005対策
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE								// 旧形式の関数使用
#endif

#pragma warning( disable : 4819 )								// コードページの警告抑止

// ヘッダ
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#endif

// ---------------------------------------------------------------------------
//
// ファイル名: AM_DispEnv.h
//
// 機能: DirectX上でのＡＭＧＡＭＥの画面環境設定を実装。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AM_DISPENV_INCLUDED__
#define __AM_DISPENV_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif

#ifndef __AM_CONNECT_INCLUDED__

#include <d3d9.h>

#endif


// ---------------------------------------------------------------------------
// Ｄｉｒｅｃｔ３Ｄオブジェクト

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
// 共通リザルトコード

#define AM_OK					0								// 正常終了
#define AMERR_NOFILE			-1								// ファイルが見つからない
#define AMERR_INVALIDFILE		-2								// ファイル形式が不正
#define AMERR_NOMEMORY			-3								// メモリ不足
#define AMERR_INVALIDVALUE		-4								// 不正な値
#define AMERR_INITIALIZE		-5								// 既に初期化済み
#define AMERR_NODRIVER			-6								// ドライバが存在しない
#define AMERR_INVALIDMODE		-7								// 不正なモード
#define AMERR_NOINITIALIZE		-8								// 初期化がされていない
#define AMERR_DISPLAY			-9								// 画面周りのエラー
#define AMERR_NOFREEHANDLE		-10								// ハンドルの空きがない
#define AMERR_NOFREESURFACE		-11								// サーフェスの空きがない
#define AMERR_CANNOTCREATE		-12								// 生成不可
#define AMERR_SOUNDOUT			-13								// 音声出力ができない
#define AMERR_INVALIDFORM		-14								// 不正な形式
#define AMERR_INVALIDHANDLE		-15								// ハンドルが不正
#define AMERR_CANNOT			-16								// 命令を実行不可能
#define AMERR_OUTOFRANGE		-17								// 値が範囲外
#define AMERR_OVERBUFFER		-18								// バッファオーバー
#define AMERR_FILEIO			-19								// ファイル入出力エラー
#define AMERR_INVALIDSIZE		-20								// サイズが不正
#define AMERR_INVALIDPOINTER	-21								// ポインタが不正
#define AMERR_NOHARDWARE		-22								// ハードウェアなし
#define AMERR_NOSUITABLEFORMAT	-23								// 適切なフォーマットなし
#define AMERR_NOTSUPPORTED		-24								// サポートなし
#define AM_ENDOFSTREAM			-25								// ストリームの終わり
#define AMERR_ENDED				-26								// アプリは既に終了すべき


// ---------------------------------------------------------------------------
// 機能コード

#define AMMODE_WINDOWED			0x0001							// ウィンドウモード指定
#define AMMODE_NOTUSEVRAM		0x0002							// ＶＲＡＭ不使用指定
#define AMMODE_USEVRAM			0x0004							// ＶＲＡＭ使用指定（デフォルト）
#define AMMODE_NORESOURCE		0x0008							// リソース不使用指定
#define AMMODE_FINDRESOURCE		0x0010							// リソース使用指定（デフォルト）
#define AMMODE_NOCOLORMATCH		0x0020							// カラーマッチングせず指定
#define AMMODE_DOCOLORMATCH		0x0040							// カラーマッチング指定（デフォルト）
#define AMMODE_CHECKSAMEFILE	0x0080							// 同ファイル名チェック
#define AMMODE_LOADSAMEFILE		0x0100							// 同ファイル名もロード（デフォルト）
#define AMMODE_USE3D			0x0200							// ３Ｄ使用指定（デフォルト）
#define AMMODE_NOTUSE3D			0x0400							// ３Ｄ不使用指定
#define AMMODE_ALWAYSUSE3D		0x0800							// 常に３Ｄ使用指定


// ---------------------------------------------------------------------------
// 共通マクロ

// オブジェクトを安全に開放
#define AM_SAFE_RELEASE( Obj )		if ( Obj ) { Obj->Release();  Obj = NULL; }


// ---------------------------------------------------------------------------
// ハンドル定数

#define AM_PIC2D_HANDLE_HEAD	0x10000000						// 2Dピクチャハンドルヘッダ
#define AM_MOVIE_HANDLE_HEAD	0x20000000						// 2Dピクチャハンドルヘッダ
#define AM_HANDLE_HEAD_MASK		0xff000000						// ハンドルヘッダマスク値


// ---------------------------------------------------------------------------
// 固有定数

#define AM_MAX_WIN_TITLE		80								// 最大ウィンドウタイトル長


// ---------------------------------------------------------------------------
// 外部公開関数プロトタイプ宣言域

extern int AM_InitDisplay( void );										// ディスプレイ初期化
extern int AM_EndDisplay( void );										// ディスプレイ使用終了
extern int AM_SetAppliInstance( HINSTANCE hInstance );					// アプリケーションインスタンスの設定
extern int AM_SetDisplayMode( int nWidth, int nHeight, int nBitDepth );	// 解像度の設定
extern int AM_ProcessEvent( void );										// Windowsイベント処理
extern int AM_Flip( void );												// 画面の更新
extern int AM_WaitVBlankTrigger( void );								// VBlank信号のトリガを待つ
extern int AM_ClearBackBuffer( int r, int g, int b );					// バックバッファのクリア
extern int AM_SetViewport( int x1, int y1, int x2, int y2 );			// ビューポートの設定


// ---------------------------------------------------------------------------
// インターファイルグローバル変数プロトタイプ

// Window関係オブジェクト
extern HWND								gg_hWindow;						// ウィンドウハンドル
extern HINSTANCE						gg_hInstance;					// インスタンスハンドル

// DirectX関係オブジェクト
extern AM_LPDIRECT3D					gg_pD3D;						// D3Dオブジェクト
extern AM_LPDIRECT3DDEVICE				gg_pD3DDevice;					// D3DDeviceオブジェクト
extern AM_LPDISPLAYBUFFER				gg_pBackBuffer;					// BackBufferオブジェクト

// 画面環境保持変数
extern SIZE								gg_siDispSize;					// クライアント領域の大きさ
extern SIZE								gg_siWholeDispSize;				// 画面全体の大きさ
extern int								gg_nBitDepth;					// ビットデプス
extern BOOL								gg_bFullMode;					// フルスクリーンモードか
extern POINT							gg_piDispLocate;				// クライアント領域の位置
extern D3DFORMAT						gg_dfDisplayFormat;				// ディスプレイフォーマット
extern BOOL								gg_bDispInitialized;			// 画面初期化済みか
extern BOOL								gg_bSceneActive;				// シーンはアクティブか
extern char								gg_szWinTitle[AM_MAX_WIN_TITLE];	// ウィンドウタイトル

// プロセス制御変数
extern BOOL								gg_bEndFlag;					// 終了フラグ


// 暫定
//HRESULT InitVB();
//VOID Render();

#endif


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AB_DispEnv.h
//
// 機能: ＡＭＧＡＭＥの画面環境設定をブリッジ。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AB_DISPENV_INCLUDED__
#define __AB_DISPENV_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif

#ifndef __AM_CONNECT_INCLUDED__

#include <windows.h>

#endif


// ---------------------------------------------------------------------------
// 外部公開定数域

#define AM_TITLE_NAME			"Amgamew.sys"


// ---------------------------------------------------------------------------
// 外部公開関数プロトタイプ宣言域

extern HWND InitGraph( HINSTANCE hInstance );					// グラフィックスの初期化
extern int EndGraph( void );									// グラフィックス使用終了
extern int SetGlobalMode( int nModes );							// 環境依存グローバルモード設定

extern int SetGraphMode( int nWidth, int nHeight, int nBitDepth );	// グラフィックモード設定
extern int ProcessEvent( void );								// イベント処理
extern void SetPage( int nActivePage, int nDisplayPage );		// ページ設定（ダミー）
extern int Flip( void );										// 画面更新処理
extern int SetFrameCounter( int nCount );						// フレームカウンタセット
extern int GetFrameCounter( void );								// フレームカウンタ取得
extern void ClearVram( void );									//	VRAMクリア
extern int HCls( void );										// 画面クリア
extern void WaitVSync( int nWaitNum );							// ディスプレイの垂直同期信号待ち
extern void SetDrawArea( int x1, int y1, int x2, int y2 );		// 描画エリアの設定

extern int AB_SetWindowTitle( char *szTitle );					// ウィンドウタイトルの設定（非公開関数）


#endif


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AM_2DGraph.h
//
// 機能: DirectX上でのＡＭＧＡＭＥの２Ｄ画面描画機能を実装。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AM_2DGRAPH_INCLUDED__
#define __AM_2DGRAPH_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnvヘッダ
//#include "AB_2DGraph.h"											// AB2DGraphヘッダ

#endif


// ---------------------------------------------------------------------------
// 外部公開定数域

#define AM_MAX_BLEND_FACTOR		255								// ブレンドファクタ最大値
#define AM_MAX_COLOR_FACTOR		255								// 色ファクタ最大値

// 画面取得モード
#define AM_GET_FRONT_BUFFER		0								// フロントバッファ取得
#define AM_GET_BACK_BUFFER		1								// バックバッファ取得


// ---------------------------------------------------------------------------
// 外部公開構造体宣言域

// 2D頂点構造体
typedef struct tagAM_2DVERTEX {
	float				x, y, z, rhw;							// 頂点座標
	DWORD				color;									// 頂点色
	float				tu, tv;									// テクスチャ座標
} AM_2DVERTEX;

#define D3DFVF_AM2DTEX			( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )	// 2Dテクスチャ頂点フォーマット

// 画像情報構造体
typedef struct tagAM_2DPIC_INFO {
	int					nWidth, nHeight;						// 幅・高さ
	int					nRealWidth, nRealHeight;				// 実際の（物理的な）幅・高さ
	int					nBitDepth, nRealDepth;					// 元のビットデプスと実際のビットデプス
} AM_2DPIC_INFO;

// ブレンドステート構造体
typedef struct tagAM_BLEND_STATE {
	int					nBlendMode;								// ブレンドモード
	int					nBlendFactor;							// ブレンドファクタ
	int					nBrRed, nBrGreen, nBrBlue;				// 緑・赤・青の明るさ
} AM_BLEND_STATE;

// 2Dベクタ構造体
typedef struct tagAM_2DVECTOR {
	float				x, y;									// 成分
} AM_2DVECTOR;

// 整数矩形構造体
typedef struct tagAM_RECT {
	int					left, top;								// 左上成分
	int					right, bottom;							// 右下成分
} AM_RECT;


// ---------------------------------------------------------------------------
// インターファイルグローバル変数域

extern AM_BLEND_STATE			gg_bsBlendState;				// 現在のブレンドステート
extern AM_2DVERTEX				gg_v2DrawRect[4];				// 描画矩形用頂点


// ---------------------------------------------------------------------------
// 外部公開関数プロトタイプ宣言域

// ロード関係
extern AM_LP2DPICTUREBUFFER AM_Load2DGraph( char *szFileName, AM_2DPIC_INFO *pPicInfo );	// 2Dグラフィックロード
extern int AM_Reload2DGraph( char *szFileName, AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo );
																// 2Dグラフィック再ロード
extern AM_LP2DPICTUREBUFFER AM_Make2DGraphBuf( AM_2DPIC_INFO *pPicInfo );
																// 2Dグラフィックバッファ作成

// 描画関係
extern int AM_Draw2DGraph( int dx1, int dy1, int dx2, int dy2,
						   int sx1, int sy1, int sx2, int sy2, 
						   AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo );
																// 2Dグラフィック描画
extern int AM_Draw2DGraph4_2( AM_2DVECTOR *pDestVerts,
							  int sx1, int sy1, int sx2, int sy2, 
							  AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo );
																// 2Dグラフィック描画（描画先４点、描画元２点指定）
extern int AM_Draw2DBlockMaskEmu( int x, int y,
								  int sx1, int sy1, int sx2, int sy2, 
								  int nMaskWidth, int nMaskHeight, unsigned char *pMask,
								  AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo );
																// マスク描画エミュレーション

// 描画ステート関係
extern int AM_InitBlendState( void );							// ブレンドステート初期化
extern int AM_SetBlendState( int nBlendCode, int nBlendFactor );	// ブレンドステート設定
extern int AM_RestoreBlendState( AM_BLEND_STATE *pbsRestoreBlend );	// ブレンドステート復帰
extern int AM_GetBlendState( AM_BLEND_STATE *pbsBlendFactor );		// ブレンドステート取得
extern int AM_SetDrawColor( int nRed, int nGreen, int nBlue );		// 描画色セット
extern int AM_SetTexSampleState( int nSampleCode );					// テクスチャサンプルステート設定
extern int AM_SetTexWrapState( int nWrapCode );						// テクスチャラップステート設定

// 2D画像周辺関数
extern int AM_GetDisplayBuffer( AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo, int nGetMode );
																// 表示用バッファの内容取得
extern int AM_SaveDisplayBuffer( AM_RECT *pSaveRect, char *szFileName, int nGetMode );
// 表示用バッファのファイルへのセーブ

#endif


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AB_2DGraph.h
//
// 機能: DirectX上でのＡＭＧＡＭＥの２Ｄ画面描画機能をブリッジ。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AB_2DGRAPH_INCLUDED__
#define __AB_2DGRAPH_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_DispEnv.h"											// AMDispEnvヘッダ
#include "AM_2DGraph.h"											// AM2DGraphヘッダ

#endif


// ---------------------------------------------------------------------------
// 外部公開定数域

// 汎用定数
#define AM_MAX_PICTURE_NAME		200								// 最大絵ファイル名長

// サンプルモード
#define AMDRAW_NEAREST			0x0001							// ニアレストネイバー
#define AMDRAW_BILINEAR			0x0002							// バイリニアフィルタリング

// ブレンドモード
#define AMDRAW_NOBLEND			0x0004							// ブレンドなし
#define AMDRAW_ALPHABLEND		0x0008							// アルファブレンディング
#define AMDRAW_ADDBLEND			0x0010							// 加算ブレンディング

// ラップモード
#define AMDRAW_TEX_NOREPEAT		0x0020							// テクスチャリピートなし
#define AMDRAW_TEX_REPEAT		0x0040							// テクスチャリピートあり

// DrawMemTrans関数用ブレンドモード
#define TRANS_AVERAGE		0									// 平均による半透明
#define TRANS_AVERAGE_TH	1									// 平均による半透明（透明色付き）
#define TRANS_ADD			2									// 加算による半透明

// 絵の属性
#define AM_PICTURE_LR			0x0001							// 左右反転フラグ

// 描画属性
#define AM_DRAWPIC_LR			0x0001							// 描画左右反転フラグ
#define AM_DRAWPIC_PICALPHA		0x0002							// 絵のアルファ値でアルファブレンド
#define AM_DRAWPIC_AVEALPHA		0x0004							// 平均アルファブレンド
#define AM_DRAWPIC_ADDALPHA		0x0008							// 加算ブレンド


// ---------------------------------------------------------------------------
// 外部公開構造体宣言域

// 単位ピクチャ構造体
typedef struct tagAM_UNIT_PICTURE {
	AM_2DPIC_INFO				a2PicInfo;						// ピクチャ情報
	AM_LP2DPICTUREBUFFER		pPicture;						// ピクチャ
	char						szFileName[AM_MAX_PICTURE_NAME];	// ピクチャファイル名
} AM_UNIT_PICTURE;

// ユーザピクチャ構造体
typedef struct tagAM_USER_PICTURE {
	AM_UNIT_PICTURE				*pUnitPic;						// 単位ピクチャ
	int							nLeft, nTop;					// 単位ピクチャ内での位置
	int							nWidth, nHeight;				// 幅と高さ
	int							bFlags;							// 属性フラグ
} AM_USER_PICTURE;

/*// 2Dベクタ構造体
typedef struct tagAM_2DVECTOR {
	float						x, y;							// 成分
} AM_2DVECTOR;*/


// ---------------------------------------------------------------------------
// 外部公開関数プロトタイプ宣言域

// 2D画像ロード関数
extern int LoadBmpMem( char *szFileName );						// 2Dグラフィックロード(旧)
extern int LoadTexture( char *szFileName );						// 2Dグラフィックロード
extern int LoadBlkBmpMem( char *szFileName, int nXSize, int nYSize,
						  int nXNum, int nYNum, int nAllNum, int *phHandleBuf );
																// ブロック化2Dグラフィックロード(旧)
extern int LoadBlkTexture( char *szFileName, int nXSize, int nYSize,
						   int nXNum, int nYNum, int nAllNum, int *phHandleBuf );
																// ブロック化2Dグラフィックロード
extern int LoadBlkBmpMemLR( char *szFileName, int nXSize, int nYSize,
							int nXNum, int nYNum, int nAllNum, int *phHandleBuf );
																// ブロック化2Dグラフィックロード(旧、左右反転)
extern int LoadBlkTextureLR( char *szFileName, int nXSize, int nYSize,
							 int nXNum, int nYNum, int nAllNum, int *phHandleBuf );
																// ブロック化2Dグラフィックロード(左右反転)
extern int MakeGraphMem( int nWidth, int nHeight );				// 2Dグラフィックバッファ作成
extern int MakeTextureMem( int nWidth, int nHeight, int nBitDepth );	// 2Dグラフィックバッファ作成
extern int ReloadGraph( char *szFileName, int hPicture );		// 2Dグラフィック再ロード
extern int LoadBmp( int x, int y, char *szFileName );			// 2Dグラフィック擬似直接ロード
extern int LoadBmpNP( int x, int y, char *szFileName );
extern int LoadBmpTh( int x, int y, char *szFileName );			// 2Dグラフィック擬似直接ロード(カラーキー付き)
extern int LoadBmpThNP( int x, int y, char *szFileName );

// 開放関数
extern int InitGraphMem( void );								// 全ビットマップ開放
extern int InitTexture( void );									// 全テクスチャ開放
extern int AB_Release2DGraphs( void );							// 2Dグラフィック開放
extern int AB_ReleaseLastGraph( int hPicture );					// 最後に作られたグラフィック開放

// 2D画像描画関数
extern int DrawMem( int x, int y, int hPicture );				// 2D描画
extern int DrawMemTh( int x, int y, int hPicture );				// アルファブレンド（カラーキー）付き2D描画
extern int DrawMemLR( int x, int y, int hPicture );				// 2D左右反転描画
extern int DrawMemThLR( int x, int y, int hPicture );			// 2D左右反転描画（カラーキー付き）
extern int DrawMemExp( int x1, int y1, int x2, int y2, int hPicture );	// 2D拡大縮小描画
extern int DrawMemThExp( int x1, int y1, int x2, int y2, int hPicture );	// アルファブレンド（カラーキー）付き2D拡大縮小描画
extern int DrawMemRot( int nCenX1, int nCenY1, int nCenX2, int nCenY2,
					   double fAngle, double fExRate,
					   int hAmTexture );
																// 2D拡大縮小描画
extern int DrawMemThRot( int nCenX1, int nCenY1, int nCenX2, int nCenY2,
						 double fAngle, double fExRate,
						 int hPicture );
																// 2D回転描画（カラーキー付き）
extern int DrawMemFlex( int x1, int y1, int x2, int y2,
						int x3, int y3, int x4, int y4,
						int hPicture );
																// 2D変形描画
extern int DrawMemThFlex( int x1, int y1, int x2, int y2,
						  int x3, int y3, int x4, int y4,
						  int hPicture );
																// 2D変形描画（カラーキー付き）
extern int DrawMemMask( int x, int y, int hPicture,
						int nWidth, int nHeight, unsigned char *pMask );
																// 2Dマスク付き描画(擬似)
extern int DrawMemThMask( int x, int y, int hPicture,
						  int nWidth, int nHeight, unsigned char *pMask );
																// 2Dマスク、カラーキー付き描画(擬似)
extern int  DrawMemTrans( int x, int y, int hPicture, int nMode );		// 2D半透明描画


// 2D画像周辺関数
extern int GetPictureSize( int hPicture, int *pnWidth, int *pnHeight );
																// 画像サイズ取得
extern int GetDispGraph( int hPicture );						// 現在表示されているグラフィック取得
extern int SaveBmp( int x1, int y1, int x2, int y2, char *szFileName );
																// ビットマップのセーブ
extern int SetBmpOffset( int nOffset );							// ロード画像データオフセット設定(ダミー)
extern int SetDrawMode( int nDrawMode, int nParam );			// 描画モードの設定
extern int AB_GetUserPicture( int hPicture, AM_USER_PICTURE **ppupUserPicture );
																// ハンドルからユーザーピクチャ取得


#endif


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AB_Palette.cpp
//
// 機能: DirectX上でのＡＭＧＡＭＥの２Ｄ画面描画機能を実装。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AB_PALETTE_INCLUDED__
#define __AB_PALETTE_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_2DGraph.h"											// AM2DGraphヘッダ

#endif

// ---------------------------------------------------------------------------
// 外部公開定数域

#define AM_MAX_DRAW_BRIGHT		1000							// 最大描画輝度


//-----------------------------------------------------------------------------
// ダミー関数域

extern void SetPalette( int pnum, int gbl, int rbl, int bbl );	// パレット設定（ダミー）
extern int GetPalette( int pnum, int *gbl, int *rbl, int *bbl );
																// 取得（ダミー）
extern int SetBmpPal( char *szBitmap );							// ＢＭＰパレット設定（ダミー）
extern int SetBmpPalPart( char *szBitmap, int FirstEnt, int EntNum );
																// ＢＭＰパレット部分設定（ダミー）
extern int LoadPalMem( char *szFileName );						// パレットロード（ダミー）
extern int MakePalMem( void );									// パレット用メモリ作成（ダミー）
extern int SetPalToMem( int hPalette, int nEntry, int gbr, int rbr, int bbr );
																// メモリパレットへパレットセット（ダミー）
extern int GetPalFromMem( int hPalette, int nEntry, int *gbr, int *rbr, int *bbr );
																// メモリパレットからパレット取得（ダミー）
extern int SetPalMem( int hPalette );							// メモリパレットセット（ダミー）
extern int SetPalMemPart( int hPalette, int nFirstEnt, int nEntNum );
																// メモリパレット部分セット（ダミー）
extern int FreePalMem( int hPalette );							// パレットメモリの開放（ダミー）
extern int SetPalArea( int under, int over );					// パレット設定範囲指定（ダミー）


//-----------------------------------------------------------------------------
// トーンコントロール関数域

extern int SetDrawBright( int nRedBright, int nGreenBright, int nBlueBright );
																// 描画明度の設定
extern int ControlPal( int nBrGreen, int nBrRed, int nBrBlue );	// パレット色調コントロール
extern int MovePalette( int nSGreen, int nSRed, int nSBlue,
						int nDGreen, int nDRed, int nDBlue, int nSpeed );
																// 画面明度の変化


#endif


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AB_3DGraph.h
//
// 機能: DirectX上でのＡＭＧＡＭＥの３Ｄ画面描画機能をブリッジ。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------


#ifndef __AB_3DGRAPH_INCLUDED__
#define __AB_3DGRAPH_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif


// ---------------------------------------------------------------------------
// インクルード域

#ifndef __AM_CONNECT_INCLUDED__

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#endif


// ---------------------------------------------------------------------------
// 外部公開定数域

#define AM3D_VALUE				float							// ３Ｄ値

// ポリゴン描画定数
#define AMPOLY_NOLIGHT		( 1 << 0 )							// ライティングなし
#define AMPOLY_LIGHTING		( 1 << 1 )							// ライティングあり
#define AMPOLY_QUADRANGLE	( 1 << 2 )							// 四角ポリゴン描画
#define AMPOLY_TRIANGLE		( 1 << 3 )							// 三角ポリゴン描画

// 行列定数
#define AM_MATRIX_LW			0								// ローカル→ワールド
#define AM_MATRIX_WV			1								// ワールド→ビュー


// ---------------------------------------------------------------------------
// 外部公開構造体宣言域

// 3D頂点構造体
typedef struct tagAmVertex {
	AM3D_VALUE					x, y, z;						// 座標
	AM3D_VALUE					u, v;							// テクスチャ座標
	AM3D_VALUE					nx, ny, nz;						// 法線ベクトル
	unsigned char				r, g, b, a;						// 色・アルファ値
} AmVertex;

// 行列構造体
typedef struct tagAmMatrix {
	AM3D_VALUE				m[4][4];							// 行列要素
} AmMatrix;

// ベクトル構造体
typedef struct tagAmVector {
	AM3D_VALUE				x, y, z;							// ベクトル要素
} AmVector;


// ---------------------------------------------------------------------------
// 外部公開関数プロトタイプ宣言域

// 描画関数
extern int DrawPolygons( AmVertex *pVertices, int nVertexNum, int hTexture, int nFlags );
																// ポリゴン描画
extern int Draw3DLines( AmVertex *pVertices, int nVertexNum, int hTexture );
																// 線列描画
// ツール関数
extern int SetLightVertex( AmVertex *pVertex, AM3D_VALUE x, AM3D_VALUE y, AM3D_VALUE z,
						   AM3D_VALUE u, AM3D_VALUE v,
						   AM3D_VALUE nx, AM3D_VALUE ny, AM3D_VALUE nz );
																// ライティングあり頂点データセット
extern int SetColorVertex( AmVertex *pVertex, AM3D_VALUE x, AM3D_VALUE y, AM3D_VALUE z,
						   AM3D_VALUE u, AM3D_VALUE v,
						   unsigned char r, unsigned char g, unsigned char b, unsigned char a );
																// ライティングなし頂点データセット
extern int SetLWMatrix( AmMatrix *pMatrix );					// ローカル→ワールド行列セット
extern int SetWVMatrix( AmMatrix *pMatrix );					// ワールド→ビュー行列セット
extern int GetLWMatrix( AmMatrix *pMatrix );					// ローカル→ワールド行列取得
extern int GetWVMatrix( AmMatrix *pMatrix );					// ワールド→ビュー行列取得
extern int MultMatrix( AmMatrix *pDest, AmMatrix *pSrc1, AmMatrix *pSrc2 );
																// 行列の積計算
extern int MultCross( AmVector *pDest, AmVector *pSrc1, AmVector *pSrc2 );
																// 外積計算
extern int SetIdentityMatrix( AmMatrix *pDest );				// 単位行列セット
extern int SetRTMatrix( AmMatrix *pDest, AM3D_VALUE tx, AM3D_VALUE ty, AM3D_VALUE tz,
										 AM3D_VALUE rx, AM3D_VALUE ry, AM3D_VALUE rz );
																// 回転＋平行移動行列セット


#endif


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AM_3DGraph.h
//
// 機能: DirectX上でのＡＭＧＡＭＥの３Ｄ画面描画機能を実装。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------


#ifndef __AM_3DGRAPH_INCLUDED__
#define __AM_3DGRAPH_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif


// ---------------------------------------------------------------------------
// インクルード域

#ifndef __AM_CONNECT_INCLUDED__

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "AM_2DGraph.h"
#include "AB_3DGraph.h"

#endif


// ---------------------------------------------------------------------------
// 外部公開構造体宣言域

// 2D頂点構造体
typedef struct tagAM_3DVERTEX {
    D3DXVECTOR3					vPos;							// 位置
    D3DXVECTOR3					vNorm;							// 法線ベクトル
	DWORD						color;							// 頂点色
    float						tu, tv;							// テクスチャ座標
} AM_3DVERTEX;

#define D3DFVF_AM3DVERTEX		( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )	// 3D頂点フォーマット


// ---------------------------------------------------------------------------
// 外部公開関数プロトタイプ宣言域

extern int AM_Init3DGraph( void );								// 3D設定初期化
extern int AM_End3DGraph( void );								// 3Dグラフィックス使用終了
extern int AM_SetTransformMatrix( int nKind, AmMatrix *pmaMatrix );
																// 行列初期化
extern int AM_GetTransformMatrix( int nKind, AmMatrix *pmaMatrix );
																// 行列取得

extern int AM_Draw3DPolygons( AmVertex *pVertices, int nVertexNum,
							  int sx, int sy,
							  AM_LP3DTEXTUREBUFFER pTexture, AM_2DPIC_INFO *pPicInfo, int nFlags );
																// 3Dポリゴン描画
extern int AM_Draw3DLines( AmVertex *pVertexes, int nVertexNum,
						   int sx, int sy,
						   AM_LP3DTEXTUREBUFFER pTexture, AM_2DPIC_INFO *pPicInfo );
																// 3D線列描画

#endif


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AM_FigureDraw.h
//
// 機能: DirectX上でのＡＭＧＡＭＥの２Ｄ画面描画機能を実装。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AM_FIGUREDRAW_INCLUDED__
#define __AM_FIGUREDRAW_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif

// ---------------------------------------------------------------------------
// インクルード域

//#include <d3d9.h>
//#include <d3dx9.h>

//#include "AM_DispEnv.h"											// AMDispEnvヘッダ
//#include "AM_2DGraph.h"											// AMDispEnvヘッダ


// ---------------------------------------------------------------------------
// 外部公開定数

#define AM_MAX_POINT_NUM		640								// 描画最大点数
#define AM_MAX_RECT_NUM			( AM_MAX_POINT_NUM / 2 )		// 描画最大長方形数

#define AM_DEFAULT_FONT_SIZE	16								// デフォルトフォントサイズ


// ---------------------------------------------------------------------------
// 外部公開構造体

// 点データ
typedef struct tagAM_POINT_DATA {
	int							x, y, z;						// 座標
	unsigned int				nColor;							// 色
} AM_POINT_DATA;


//-----------------------------------------------------------------------------
// 図形描画関数

extern int AM_DrawPointArray( AM_POINT_DATA *pPoints, int nPointNum );
																// 点列の描画
extern int AM_GetColorCode( int r, int g, int b );				// 色コードの取得
extern int AM_GetBackbufferPointColor( int x, int y );			// バックバッファの指定点の色取得
extern int AM_DrawRectangleArray( AM_POINT_DATA *papPoints, int nRectNum );
																// 長方形の描画
// 文字列関係
extern int AM_PrepareFontObject( void );						// 文字列フォントの準備
extern int AM_ReleaseFontObject( void );						// 文字列フォントの開放
extern int AM_SetStringFontSize( int nFontSize );				// 文字列フォントサイズの設定

extern int AM_GetStringFontSize( void );						// 文字列フォントサイズの取得
extern int AM_DrawString( int x, int y, char *szString, int nColor );
																// 文字列の描画


#endif


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AB_FigureDraw.h
//
// 機能: DirectX上でのＡＭＧＡＭＥの２Ｄ画面描画機能をブリッジ。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AB_FIGUREDRAW_INCLUDED__
#define __AB_FIGUREDRAW_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif

// ---------------------------------------------------------------------------
// インクルード域


// ---------------------------------------------------------------------------
// 外部公開定数域

#define AM_MIN_FONT_SIZE			4							// 最小フォントサイズ
#define AM_MAX_FONT_SIZE			8192						// 最大フォントサイズ


//-----------------------------------------------------------------------------
// 図形描画関数

extern void DrawPixel( int x, int y, int color );				// 1点の描画
extern int GetColor( int nRed, int nGreen, int nBlue );			// 色コードの取得
extern int GetPixel( int x, int y );							// 指定点の色取得
extern void DrawFBox( int x1, int y1, int x2, int y2, int color );
																// 長方形の描画
extern void DrawTBox( int x1, int y1, int x2, int y2, int color );
																// 半透明長方形の描画
// 文字列関係
extern int DrawString( int x, int y, char *szString, int nColor );
																// 文字列の描画
extern int SetFontSize( int nFontSize );						// ﾌｫﾝﾄｻｲｽﾞの変更


#endif
// ---------------------------------------------------------------------------
//
// ファイル名: AM_Input.h
//
// 機能: DirectX上でのＡＭＧＡＭＥのユーザ入力機能を実装。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------


#ifndef __AM_INPUT_INCLUDED__
#define __AM_INPUT_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif

// ---------------------------------------------------------------------------
// 外部公開定数域

#define AM_MAX_PAD_NUM			2										// 最大パッド数


// ---------------------------------------------------------------------------
// インターファイルグローバル変数域

extern int						gg_nJoyPadNum;					// ジョイパッド数
extern int						gg_bMouseCursor;				// マウスカーソル
extern int						gg_bMouseActive;				// マウス有効か


// ---------------------------------------------------------------------------
// 外部公開関数プロトタイプ宣言域

// キーボード関係
extern int AM_GetSingleChar( void );									// キー入力待ち・入力キー取得関数
extern int AM_IsKeyHit( void );											// キーボード押下チェック関数
extern int AM_SetKeyPushed( int nKeyCode );								// キー入力通知関数
extern int AM_AddKeyInput( int nKeyState );								// キー入力付加

// パッド関係
extern int AM_InitJoyPad( int nMaxPadNum );								// パッド初期化
extern int AM_EndJoyPad( void );										// パッド使用終了
extern int AM_AddJoyInput( int nPadIndex, int nKeyState );				// 単体パッド入力付加

// マウス関係
extern int AM_InitMouse( int bMouseState );								// マウス初期化
extern int AM_EndMouse( void );											// マウス使用終了
extern int AM_SetMouseState( int bMouseState );							// マウス状態設定
extern int AM_GetMousePosition( int *nXLoc, int *nYLoc );				// マウス位置取得
extern int AM_SetMouseLoc( int nXLoc, int nYLoc );						// マウス位置設定


#endif


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
//
// 機能: DirectX上でのＡＭＧＡＭＥのユーザ入力機能をブリッジ。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------


#ifndef __AB_INPUT_INCLUDED__
#define __AB_INPUT_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif

// ---------------------------------------------------------------------------
// 外部公開定数域

// キー定数
#define KEYIN_2					0x0001					// ２キーチェックマスク
#define KEYIN_DOWN				0x0001					// ↓キーチェックマスク
#define KEYIN_4					0x0002					// ４キーチェックマスク
#define KEYIN_LEFT				0x0002					// ←キーチェックマスク
#define KEYIN_6					0x0004					// ６キーチェックマスク
#define KEYIN_RIGHT				0x0004					// →キーチェックマスク
#define KEYIN_8					0x0008					// ８キーチェックマスク
#define KEYIN_UP				0x0008					// ↑キーチェックマスク
#define KEYIN_SPC				0x0010					// スペースチェックマスク
#define KEYIN_ESC				0x0020					// ｴｽｹｰﾌﾟチェックマスク
#define KEYIN_START				0x0020					// スタートキーチェックマスク
#define KEYIN_SFT				0x0040					// シフトチェックマスク
#define KEYIN_RET				0x0080					// リターンチェックマスク
#define KEYIN_Z					0x0100					// Ｚチェックマスク
#define KEYIN_X					0x0200					// Ｘチェックマスク
#define KEYIN_C					0x0400					// Ｃチェックマスク
#define KEYIN_A					0x0800					// Ａチェックマスク
#define KEYIN_S					0x1000					// Ｓチェックマスク
#define KEYIN_D					0x2000					// Ｄチェックマスク
#define KEYIN_Q					0x4000					// Ｑチェックマスク
#define KEYIN_W					0x8000					// Ｗチェックマスク

// デバッキングキー定数
#define KEYIN_DEBUG_PAUSE		0x01000000				// ポーズチェックマスク
#define KEYIN_DEBUG_DISPLAY		0x02000000				// デバッグ表示チェックマスク
#define KEYIN_DEBUG_COLOR		0x04000000				// デバッグ色変更チェックマスク

// マウスボタン定数
#define MOUSEKEY_LEFT			0x00010000				// マウス左ボタン
#define MOUSEKEY_RIGHT			0x00020000				// マウス右ボタン
#define MOUSEKEY_MIDDLE			0x00040000				// マウス中央ボタン

// マウスボタン　キーコード
#define MOUSECODE_LEFT			0x101					// マウス左ボタンコード
#define MOUSECODE_RIGHT			0x102					// マウス右ボタンコード
#define MOUSECODE_MIDDLE		0x103					// マウス中央ボタンコード

// キー入力モード
#define AMINPUT_MULT			0								// マルチプレックス入力
#define AMINPUT_KEYBOARD		1								// キーボード入力
#define AMINPUT_PAD1			2								// パッド１入力
#define AMINPUT_PAD2			3								// パッド２入力


// ---------------------------------------------------------------------------
// 外部公開関数プロトタイプ宣言域

extern int GetCh( void );												// 一文字入力
extern int KBHit( void );												// キー入力チェック
extern int CheckKey( int nInputMode );									// キーが押されているかチェック

// マウス関係
extern int InitMouse( int bMouseState );								// マウス初期化
extern int SetMouseState( int bMouseState );							// マウス状態設定
extern int GetMouseLoc( int *nXLoc, int *nYLoc );						// マウス位置取得
extern int SetMouseLoc( int nXLoc, int nYLoc );							// マウス位置設定


#endif
// ---------------------------------------------------------------------------
//
// ファイル名: AM_Movie.h
//
// 機能: DirectX上でのＡＭＧＡＭＥのムービー再生機能を実装。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AM_MOVIE_INCLUDED__
#define __AM_MOVIE_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif

#include <windows.h>
#include <digitalv.h>
#include <vfw.h>
#include <msacm.h>
#include <mmsystem.h>
#include <dsound.h>

#ifndef __AM_CONNECT_INCLUDED__

#include "AM_2DGraph.h"											// AM2DGraphヘッダ

#endif


#define MOVIE_FULL_LOC	-1000000					// ムービーフル再生位置


typedef struct tagAMAVIAUDIO {						// 圧縮音声制御構造体
	PAVISTREAM			pAudioStream;				// オーディオストリーム
	HACMSTREAM			hACMStream;					// ＡＣＭストリームハンドル
	ACMSTREAMHEADER		acmHeader;					// ＡＣＭストリームヘッダ
//	char				*pOutBuffer1, *pOutBuffer2;	// 出力データバッファ1,2
	LPDIRECTSOUNDBUFFER	lpDSBPlay;					// DirectSound再生バッファ
	BYTE				*pAudioSrcBuf;				// 転送元音声バッファ
	DWORD				nAudioSrcBufSize;			// 音声転送元サイズ
	BYTE				*pAudioDestBuf;				// 転送先音声バッファ
	DWORD				nAudioDestBufSize;			// 音声転送先サイズ
	LONG				nBlockNum;					// バッファブロック数
	LONG				nCurrentBlock;				// 現在の再生ブロック

} AMAVIAUDIO;


typedef struct tagAMMOVIE {							// ムービー構造体
	PAVIFILE			pAVIFile;					// ＡＶＩファイル
	AVIFILEINFO			AVIInfo;					// ＡＶＩファイル情報
	PAVISTREAM			pVideoStream;				// ビデオストリーム
	AVISTREAMINFO		VideoInfo;					// ビデオストリーム情報
	PGETFRAME			pVideoFrame;				// ビデオフレーム
	PAVISTREAM			pAudioStream;				// オーディオストリーム
	AVISTREAMINFO		AudioInfo;					// オーディオストリーム情報
//	int					nWidth, nHeight;			// 幅と高さ
	double				fVidSampDelay;				// ビデオサンプル間隔
	double				fFirstTime;					// 最初の時刻
	int					nPresFrame;					// 現在のフレーム
} AMMOVIE;


extern MCIDEVICEID				gg_wAVIDevice;					// ＡＶＩデバイスＩＤ

extern int AM_PlayMovie( char *szFileName, int x, int y, int nWidth, int nHeight );
																// 通常ムービー再生
extern int AM_StopMovie( void );								// 通常ムービー再生ストップ

extern int AM_OpenMovieFile( char *szFileName, AMMOVIE *pMovieData );
																// ムービーファイルオープン
extern int AM_CloseMovieFile( AMMOVIE *pMovieData );			// ムービーファイルクローズ
extern int AM_PlayMovieFile( int x, int y, AMMOVIE *pMovieData );	// ムービーファイル再生
extern int AM_GetMovieFrame( AMMOVIE *pMovieData, int nFrameLoc,
							 AM_LP2DPICTUREBUFFER pPicture, AM_2DPIC_INFO *pPicInfo );
																// ムービーの特定フレーム取得


#endif
// ---------------------------------------------------------------------------
//
// ファイル名: AB_Movie.h
//
// 機能: DirectX上でのＡＭＧＡＭＥのムービー再生機能をブリッジ。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AB_MOVIE_INCLUDED__
#define __AB_MOVIE_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif


// ---------------------------------------------------------------------------
// 外部公開定数域

#define AM_MAX_MOVIE_NUM		10								// 最大ムービー数



// ---------------------------------------------------------------------------
// 外部公開関数プロトタイプ宣言域

extern int PlayMovie( int x, int y, char *szFileName );			// ムービー再生
extern int PlayMovieBack( int x, int y, char *szFileName );		// バックムービー再生
extern int PlayMovieFull( char *szFileName );					// ﾌﾙｽｸﾘｰﾝムービー再生
extern int PlayMovieFullBack( char *szFileName );				// ﾌﾙｽｸﾘｰﾝﾊﾞｯｸﾑｰﾋﾞｰ再生(ダミー)
extern int SetMovieTime( int nFirst, int nLast );				// ムービー再生時間設定

extern int LoadMovieMem( char *szFileName );					// ムービーのメモリへのロード
extern int PlayMovieMem( int x, int y, int hMovie );			// メモリムービーの再生
extern int PlayMovieMemFull( int hMovie );						// メモリムービーのフル再生
extern int GetMovieFrame( int hPicture, int hMovie, int nFrameNum );
																// ムービーの特定フレーム取得
extern int GetMovieSize( int hMovie, int *nWidth, int *nHeight );	// ムービーサイズの取得
extern int GetMovieCount( int hMovie );							// ムービーフレームカウント数の取得
extern int InitMovieMem( void );								// ムービー用メモリの開放



#endif
// ---------------------------------------------------------------------------
//
// ファイル名: AB_Time.h
//
// 機能: DirectX上でのＡＭＧＡＭＥの時間機能をブリッジ。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AB_TIME_INCLUDED__
#define __AB_TIME_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif


// ---------------------------------------------------------------------------
// 外部公開構造体宣言域

typedef struct tagAMDATETIME {									// 日時構造体
	DWORD						nPackedTime;					// パックト時間
	unsigned int				nYear;							// 更新年（西暦）
	unsigned char				nMonth;							// 更新月
	unsigned char				nDay;							// 更新日
	unsigned char				nHour;							// 更新時
	unsigned char				nMinute;						// 更新分
	unsigned char				nSecond;						// 更新秒
	unsigned char				dPadding;						// パディング
	WORD						dPadding2;						// パディング
} AMDATETIME;


//-----------------------------------------------------------------------------
// 時間関数域

extern int GetNowTime( int nDummy );							// キー入力待ち・入力キー取得関数
extern int WaitByTimer( int nWaitTime );						// タイマで待つ（１／１０００秒単位）
extern int GetDateTime( AMDATETIME *pDateTime );				// 日時の取得
extern int UnpackDateTime( AMDATETIME *pDateTime );				// 日時のアンパック


#endif


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
//
// ファイル名: AM_Time.h
//
// 機能: DirectX上でのＡＭＧＡＭＥの時間機能を実装。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------

#ifndef __AM_TIME_INCLUDED__
#define __AM_TIME_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif

//#include <windows.h>
//#include <mmsystem.h>

#ifndef __AM_CONNECT_INCLUDED__

#include "AB_Time.h"											// 時間機能　ブリッジレイヤー

#endif



//-----------------------------------------------------------------------------
// キーボード入力制御関数域

extern int AM_GetPresentTime( void );							// 現在時間取得関数
extern int AM_GetDateTime( AMDATETIME *pDateTime );				// 日時の取得


#endif


//-----------------------------------------------------------------------------
// 以上
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

//             アミューズメントメディア総合学院　ゲームライブラリ

//                          ”ＡＭＧＡＭＥ”

//                                Ver. 3.00

//         Programmed 1996-2004 by K.Katoh, Academy of Amusementmedia

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ＡＭＧＡＭＥ　ＴＤ　ＤｉｒｅｃｔＳｏｕｎｄモジュール


#ifndef __AMDSOUND_INCLUDED__						// ｲﾝｸﾙｰﾃﾞｯﾄﾞチェック
#define __AMDSOUND_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif

//#define DIRECTSOUND_VERSION			0x0500
#include <mmsystem.h>
#include <dsound.h>

// リンクライブラリ
#pragma comment( lib, "dsound"  )					// DirectSoundライブラリ
#pragma comment( lib, "dxguid"  )					// DirectX GUIDライブラリ
#pragma comment( lib, "winmm"   )					// マルチメディアライブラリ

// ---------------------------------------------------------------------------
// ライブラリ共通変数
extern BOOL		bMidiPlaying;


// ---------------------------------------------------------------------------
// ＰＣＭライブラリ関数

extern int InitSound( int nSoundMode );				// サウンドの初期化
extern int EndSound( void );						// サウンド使用終了
extern int EndDSound( void );						// 内部サウンド使用終了
extern int InitSoundMem( void );					// サウンドメモリの初期化
extern int PlayWav( char *szFileName );				// Wav直接再生
extern int PlayWavBack( char *szFileName );			// Wav直接バック再生
extern int PlayWavStreamBack( char *szFileName );	// Wavストリームバック再生
extern int CheckPcm( void );						// ＰＣＭ演奏中チェック
extern int StopPcm( void );							// ＰＣＭ演奏停止
extern int MakeDSBuffer( LPWAVEFORMATEX lpWaveForm,		// DirectSoundバッファ作成
						 char *pWavBuffer, DWORD nBufferSize,
						 LPDIRECTSOUNDBUFFER *lplpDSBDest );
extern int MakeDSStreamBuf( LPWAVEFORMATEX lpWaveForm,	// DirectSoundストリーミングバッファ作成
							int nBufferSize, LPDIRECTSOUNDBUFFER *lplpDSBDest );
extern int LoadWavMem( char *szFileName );			// メモリへのＷＡＶロード
extern int PlayMem( int hSound );					// メモリ再生
extern int PlayMemBack( int hSound );				// メモリバック再生
extern int PlayMemLoop( int hSound );				// メモリループ再生
extern int CheckPlayMem( int hSound );				// メモリ演奏中チェック
extern int StopPlayMem( int hSound );				// メモリ演奏ストップ
extern int SetPcmVolume( int nVolume );				// ＰＣＭボリウムのセット
extern int SetMemVolume( int nVolume, int hSound );	// メモリＰＣＭボリウムのセット


// ---------------------------------------------------------------------------
// ミュージックライブラリ関数

extern int PlayMidi( char *szFileName );			// MIDIファイルの演奏
extern int PlayMidiBack( char *szFileName );		// MIDIファイルのバック演奏
extern int PlayMidiLoop( char *szFileName );		// MIDIファイルのループ演奏
extern int CheckMusic( void );						// ＭＩＤＩ演奏中チェック
extern int StopMusic( void );						// ＭＩＤＩ演奏ストップ
extern int SetMusicVolume( DWORD nVolume );			// ＭＩＤＩ音量セット
extern int MakeFullPath( char *szDest, char *szSrc );	// フルパスの生成
extern void ProcessMciNortify( void );				// ＭＣＩ通知処理


// ---------------------------------------------------------------------------
// 外部公開変数

extern LPDIRECTSOUND			lpDS;				// DirectSoundオブジェ

#endif
// ---------------------------------------------------------------------------
//
// 機能: DirectX上でのＡＭＧＡＭＥのデバッグ機能をブリッジ。
//
// プラットフォーム: Windows XP, Windows 2000 + DirectX9
//
// Copyright (c) 2004 K.Kato. All rights reserved.
//
// ---------------------------------------------------------------------------


#ifndef __AB_DEBUG_INCLUDED__
#define __AB_DEBUG_INCLUDED__

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE						// 旧形式の関数使用
#endif

// ---------------------------------------------------------------------------
// 外部公開定数域

// デバッグ情報ファイル名
#define AB_DEBUG_FILE_NAME			"AM_DEBUG.txt"				// デバッグファイル名
#define AB_DEBUG_FILE_NAME_L		L"AM_DEBUG.txt"				// デバッグファイル名

// デバッグモードセット定数
#define AMDEBUG_ALL_OFF				0							// 全てのデバッグ機能をオフ
#define AMDEBUG_ALL_ON				1							// 全てのデバッグ機能をオン
#define AMDEBUG_REGISTER_OFF		2							// デバッグ情報登録をオフ
#define AMDEBUG_REGISTER_ON			3							// デバッグ情報登録をオン
#define AMDEBUG_DISPLAY_OFF			4							// デバッグ情報表示をオフ
#define AMDEBUG_DISPLAY_ON			5							// デバッグ情報表示をオン
#define AMDEBUG_KEY_OFF				6							// デバッグキー操作をオフ
#define AMDEBUG_KEY_ON				7							// デバッグキー操作をオン
#define AMDEBUG_FILE_OFF			8							// デバッグキー操作をオフ
#define AMDEBUG_FILE_ON				9							// デバッグキー操作をオン

// デバッグモード定数マスク
#define AMDEBUG_REGISTER			1							// デバッグ情報登録
#define AMDEBUG_DISPLAY				2							// デバッグ情報表示
#define AMDEBUG_KEY					4							// デバッグキー操作
#define AMDEBUG_FILE				8							// デバッグファイル出力

// 描画デバッグオブジェクト種類
#define AB_DEBUG_DRAWMEM			0							// DrawMem系(DrawMem,DrawMemTh)命令
#define AB_DEBUG_DRAWMEMLR			1							// DrawMemLR系命令
#define AB_DEBUG_DRAWMEMEXP			2							// DrawMemExp系命令
#define AB_DEBUG_DRAWMEMROT			3							// DrawMemRot系命令
#define AB_DEBUG_DRAWMEMFLEX		4							// DrawMemFlex系命令
#define AB_DEBUG_DRAWFBOX			5							// DrawFBox系命令
#define AB_DEBUG_OUT_STRING			0x1000						// 文字列出力

// ロードデバッグオブジェクト種類
#define AB_DEBUG_LOADTEXTURE		0x100						// LoadTexture命令
#define AB_DEBUG_LOADBLKTEXTURE		0x101						// LoadBlkTexture命令
#define AB_DEBUG_LOADBLKTEXTURELR	0x102						// LoadBlkTextureLR命令
#define AB_DEBUG_RELOADGRAPH		0x103						// ReloadGraph命令

// ---------------------------------------------------------------------------
// 外部公開構造体宣言域

// 描画デバッグ情報構造体
typedef struct tagAB_DRAWING_DEBUG_INFO {
	int							nObjectKind;					// オブジェクト種類
	int							nUsedHandle;					// 使用ハンドル
	AM_USER_PICTURE				*pUserPicture;					// ピクチャ情報
	int							x1, y1, x2, y2, x3, y3, x4, y4;	// 描画位置
	float						fAngle, fExRate;				// 回転角・拡大率
	int							nBlendKind;						// ブレンド種類
	int							nColor;							// 描画色
	int							bError;							// エラーの有無
} AB_DRAWING_DEBUG_INFO;

// ロードデバッグ情報構造体
typedef struct tagAB_LOADING_DEBUG_INFO {
	int							nObjectKind;					// オブジェクト種類
	char						*szFileName;					// ファイル名
	int							nMadeHandle;					// 作成されたハンドル
	int							nMadeHandleEnd;					// 作成された最後のハンドル
	int							bError;							// エラーの有無
} AB_LOADING_DEBUG_INFO;


// ---------------------------------------------------------------------------
// インターファイルグローバル変数域

extern int						gg_bRegisterDebug;				// デバッグ情報を登録するか
extern int						gg_bDisplayDebug;				// デバッグ情報を表示するか
extern int						gg_bDebugKey;					// デバッグ用キーを有効にするか
extern int						gg_bDebugFile;					// デバッグファイル出力を有効にするか


// ---------------------------------------------------------------------------
// 外部公開関数プロトタイプ宣言域

extern int SetAMDebugMode( int nDebugMode );					// デバッグモードのセット
extern int GetAMDebugMode( void );								// デバッグモードの取得
extern int RestoreAMDebugMode( int nDebugFlags );				// デバッグモードの復帰
extern int OutputAMDebugString( char *szOutString );			// デバッグ文字列の出力

extern int AB_SetDrawingDebugInfo( AB_DRAWING_DEBUG_INFO *pDebugInfo );
																// 描画系デバッグ情報セット関数
extern int AB_DisplayDebugInfo( void );							// デバッグ情報表示関数
extern int AB_ControlDebugPause( void );						// デバッグ機能・ポーズコントロール関数

extern int AB_OutLoadingDebugInfo( AB_LOADING_DEBUG_INFO *pLoadDebugInfo );	// ロード系デバッグ情報出力関数



#endif
