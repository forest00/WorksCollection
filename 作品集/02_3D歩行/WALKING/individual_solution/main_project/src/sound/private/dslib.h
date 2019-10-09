#ifndef __DSLIB_H__
#define	__DSLIB_H__
/*
** DirectSound Lib.
*/


// 初期化
BOOL DSoundInit( HWND hWnd, LPGUID lpGuid );

// 解放
BOOL DSoundRelease();

// wavファイルの読み込み
enum {
	DSLF_FLG_DEFAULT = 0,
	DSLF_FLG_STATIC,
	DSLF_FLG_STREAM,
};
int DSoundLoadFile(
	LPSTR szfilename,		// ファイル名 or リソース
	int nChannel,			// 読み込みチャンネル番号(-1で空いてるチャンネルに追加)
	DWORD dwFlg				// フラグ DSLF_FLG_xxxx
);

// wavファイルの解放
BOOL DSoundReleaseChannel( int nChannel );

// 再生
BOOL DSoundPlay( int nChannel, BOOL bLoop );

// 停止
void DSoundStop( int nChannel );

// ボリュームセット
BOOL DSoundSetVolume( int nChannel,int nVolume );

// パンセット
BOOL DSoundSetPan( int nChannel, int nCmd );

// 再生中か？
BOOL DSoundIsStop( int nChannel );



#endif

