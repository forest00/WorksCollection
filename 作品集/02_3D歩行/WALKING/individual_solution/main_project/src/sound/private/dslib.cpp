/*
** Direct Sound Lib.
**
** 「設定」 C/C++ の コード生成を「マルチスレッド」に変更
*/

#define	STRICT
#include <windows.h>
#include <stdio.h>
#include <dsound.h>
#include <mmsystem.h>
#include <string.h>
#include <process.h>
#include <math.h>

#include "dslib.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dsound.lib")


#ifndef DSBCAPS_CTRLDEFAULT
	// DirectX7以降対応用
	#define	DSBCAPS_CTRLDEFAULT	(DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY)
#endif


static LPDIRECTSOUND pDSound = NULL;
static LPDIRECTSOUNDBUFFER pDSPrimary;
static DWORD DSoundGetWaveHeader( LPSTR szFileName, WAVEFORMATEX *wfmtx );
//static BOOL DSoundReleaseChannel( int nChannel );

// チャンネル情報の格納構造体
typedef struct tagCHANNELSTRUCT {
	BOOL				bStream;				// ストリーム再生ならTRUE
	char				szFileName[MAX_PATH];	// 読み込んだファイルの名前
	LPDIRECTSOUNDBUFFER	lpBuffer;				// DirectSoundBuffer のポインタ
	DWORD				dwSize;					// Bufferのサイズ
	BOOL				bLoop;					// ループ再生か否か
	// ストリーム再生用
	HANDLE				hEvent;
	HANDLE				hThread;
	DWORD				dwEvent;
	void				*pDataTop;
	WAVEFORMATEX		wavFormatEx;
	DWORD				dwDataSize;
	HMMIO				hmmio;
} CHANNELSTRUCT;
static CHANNELSTRUCT *pChannel = NULL;
static int nChannelMax = 0;

enum {
	DSCTRL_EVENT_EXIT = 0,
	DSCTRL_EVENT_PLAY,
	DSCTRL_EVENT_STOP,
};

#define	DDS_STREAM_BUFFER	3	// バッファサイズ(sec)
static unsigned __stdcall thStreamPlay( LPVOID lpParam );
static BOOL DSoundLoadFileStream( LPSTR szFileName, int nChannel );





//////////////////////////////////////////////////////////
// DirectSoundの初期化
BOOL DSoundInit(
	HWND hWnd,			// 親ウィンドウのハンドル
	LPGUID lpGuid		// 使うデバイスのGUID、NULLだとデフォルト
) {	
	// オブジェクトを生成する
	if ( DirectSoundCreate( lpGuid, &pDSound, NULL ) != DS_OK ) {
		return FALSE;
	}

	// 協調レベルを設定
	if ( pDSound->SetCooperativeLevel( hWnd, DSSCL_PRIORITY ) != DS_OK ) {
		// 協調レベルの設定に失敗
		pDSound->Release(); pDSound = NULL;
		return FALSE;
	}

	// スピーカの設定
//	pDSound ->SetSpeakerConfig(DSSPEAKER_HEADPHONE);

	// プライマリバッファの作成
	DSBUFFERDESC dsdc;
	ZeroMemory( &dsdc, sizeof( DSBUFFERDESC ) );
	dsdc.dwSize			= sizeof( DSBUFFERDESC );
	dsdc.dwFlags		= DSBCAPS_CTRLVOLUME | DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLPAN ;
	dsdc.dwBufferBytes	= 0;
	dsdc.lpwfxFormat	= NULL;
	pDSound->CreateSoundBuffer( &dsdc, &pDSPrimary, NULL) ;

	// プライマリバッファの出力フォーマットを 44.1KHz, 16bit, stereo にする
	WAVEFORMATEX pcmwf;
	ZeroMemory( &pcmwf, sizeof( WAVEFORMATEX ) );
	pcmwf.wFormatTag		= WAVE_FORMAT_PCM;
	pcmwf.nChannels			= 2;
	pcmwf.nSamplesPerSec	= 44100;
	pcmwf.nBlockAlign		= 2*16/8;
	pcmwf.nAvgBytesPerSec	= pcmwf.nSamplesPerSec*pcmwf.nBlockAlign;
	pcmwf.wBitsPerSample	= 16;
	pDSPrimary->SetFormat( &pcmwf );

	return TRUE;
}



BOOL DSoundRelease() {
	if ( pDSound == NULL ) {
		// DirectSoundを初期化していない
		return TRUE;
	}

	// 作ったチャンネルを解放
	for( int i = 0; i < nChannelMax; i++ ) {
		DSoundReleaseChannel( i );
	}
	// DirectSoundの解放
	pDSPrimary->Release(); pDSPrimary = NULL;
	pDSound->Release(); pDSound = NULL;

	// チャンネルテーブルを解放
	free( pChannel ); pChannel = NULL;

	return TRUE;
}



//////////////////////////////////////////////////////////
// 指定のチャンネルを確保する
static int DSoundInitChannel( int nChannel ) {
	if ( nChannel < 0 ) { nChannel = nChannelMax; }
	if ( nChannelMax <= nChannel ) {
		// チャンネルの追加
		if ( pChannel == NULL ) {
			// チャンネル新規
			pChannel = ( CHANNELSTRUCT* )malloc( sizeof( CHANNELSTRUCT ) * ( nChannel + 1 ) );
		} else {
			// チャンネル追加
			pChannel = ( CHANNELSTRUCT* )realloc( pChannel, sizeof( CHANNELSTRUCT ) * ( nChannel + 1 ) );
		}
		// 追加した分を初期化
		for ( int i = nChannelMax; i <= nChannel; i++ ) {
			pChannel[i].bStream		= FALSE;
			strcpy( pChannel[i].szFileName, "" );
			pChannel[i].lpBuffer	= NULL;
			pChannel[i].dwSize		= 0;
			pChannel[i].bLoop		= FALSE;
		}
		nChannelMax = nChannel + 1;
	} else {
		// 現状すでにあるチャンネルを使う
		DSoundReleaseChannel( nChannel );
	}

	return nChannel;
}


//////////////////////////////////////////////////////////
// ファイルの読み込み

static BOOL CreateBuffer( int nChannel,int iHeletz,int iBit,int iMSFlg, DWORD dwSize ) {
	DSBUFFERDESC dsbdesc;
	PCMWAVEFORMAT pcmwf;

	memset( &pcmwf, 0, sizeof( PCMWAVEFORMAT ) );
	pcmwf.wf.wFormatTag			= WAVE_FORMAT_PCM;
	pcmwf.wf.nChannels			= iMSFlg;
	pcmwf.wf.nSamplesPerSec		= iHeletz;
	pcmwf.wf.nBlockAlign		= iMSFlg * iBit / 8;
	pcmwf.wf.nAvgBytesPerSec	= pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign;
	pcmwf.wBitsPerSample		= iBit;

	memset( &dsbdesc, 0, sizeof( DSBUFFERDESC ) );
	dsbdesc.dwSize			= sizeof( DSBUFFERDESC );
	dsbdesc.dwFlags			= DSBCAPS_CTRLDEFAULT | DSBCAPS_STICKYFOCUS;
	dsbdesc.dwBufferBytes	= dwSize;
	dsbdesc.lpwfxFormat		= ( LPWAVEFORMATEX )&pcmwf;

	if ( pDSound->CreateSoundBuffer( &dsbdesc, &pChannel[nChannel].lpBuffer, NULL ) != DS_OK ) {
		return FALSE;
	}
	pChannel[nChannel].dwSize = dwSize;

	return TRUE;
}


static BOOL FillBuffer( int nChannel, char *pData ) {
	LPVOID write1;
	DWORD length1;
	LPVOID write2;
	DWORD length2;
	HRESULT hr;
	hr = pChannel[nChannel].lpBuffer->Lock( 0, pChannel[nChannel].dwSize, &write1, &length1, &write2, &length2, 0 );
	if ( hr == DSERR_BUFFERLOST ) {
		pChannel[nChannel].lpBuffer->Restore();
		hr = pChannel[nChannel].lpBuffer->Lock( 0, pChannel[nChannel].dwSize, &write1, &length1, &write2, &length2, 0 );
	}
	if ( hr != DS_OK ) {
		return FALSE;
	}
	CopyMemory( write1, pData, length1 );
	if (write2 != NULL) {
		CopyMemory( write2, pData + length1, length2 );
	}
	hr = pChannel[nChannel].lpBuffer->Unlock( write1, length1, write2, length2 );
	if ( hr != DS_OK ) {
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
// static play.

static BOOL DSoundLoadFileStatic( LPSTR szFileName, int nChannel ) {
	HMMIO 			hmfr;
	MMCKINFO		parent,child;
	WAVEFORMATEX	wfmtx;

	// 構造体のセット
	parent.ckid 		= (FOURCC)0;
	parent.cksize 		= 0;
	parent.fccType		= (FOURCC)0;
	parent.dwDataOffset = 0;
	parent.dwFlags		= 0;

	// 構造体のコピー
	child=parent;

	// ファイルのオープン
	hmfr = mmioOpen( szFileName, NULL, MMIO_READ | MMIO_ALLOCBUF );
	if ( hmfr == NULL ) {
		// ファイルオープン失敗
		return FALSE;
	}

	parent.fccType = mmioFOURCC( 'W','A','V','E' );
	if ( mmioDescend( hmfr, &parent, NULL, MMIO_FINDRIFF ) ) {
		// RIFFディセンド失敗
		mmioClose( hmfr, 0 );
		return FALSE;
	}

	child.ckid = mmioFOURCC( 'f','m','t',' ' );
	if ( mmioDescend( hmfr, &child, &parent, 0 ) ) {
		// RIFFディセンド失敗
		mmioClose(hmfr, 0);
		return FALSE;
	}

	// WAVEヘッダの読みこみ
	if ( mmioRead( hmfr, (char*)&wfmtx, sizeof(wfmtx) ) != sizeof(wfmtx) ) {
		// ヘッダ読みこみの失敗
		mmioClose( hmfr, 0 );
		return FALSE;
	}

	if ( wfmtx.wFormatTag != WAVE_FORMAT_PCM ) {
		// PCMフォーマットじゃない
		mmioClose( hmfr, 0 );
		return FALSE;
	}

	if ( mmioAscend( hmfr, &child, 0 ) ) {
		// アセンド失敗
		mmioClose(hmfr, 0);
		return FALSE;
	}

	child.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
	if ( mmioDescend( hmfr, &child, &parent, MMIO_FINDCHUNK ) ) {
		// ディセンド失敗
		mmioClose( hmfr, 0 );
		return FALSE;
	}

	// メモリの確保(サウンドバッファ)
	char *pBuffer = (char*)malloc( child.cksize );

	// mmioOpenされたファイルからバッファにWAVEデータを読み込む
	if ( (DWORD)mmioRead( hmfr, pBuffer, child.cksize ) != child.cksize ) {
		// 読みこみ失敗
		mmioClose( hmfr, 0 );
		return FALSE;
	}

	// ファイルを閉じる
	mmioClose( hmfr, 0 );

	// セカンダリバッファの作成
	CreateBuffer( nChannel, wfmtx.nSamplesPerSec,
		wfmtx.wBitsPerSample, wfmtx.nChannels, child.cksize );

	// サウンドバッファにデータをコピーする
	FillBuffer( nChannel, pBuffer );

	// メモリの解放
	free( pBuffer );

	return TRUE;

}

// ファイルからWAVEヘッダを読み込む。データサイズが戻る
static DWORD DSoundGetWaveHeader( LPSTR szFileName, WAVEFORMATEX *wfmtx ) {
	HMMIO 			hmfr;
	MMCKINFO		parent,child;

	// 構造体のセット
	parent.ckid 		= (FOURCC)0;
	parent.cksize 		= 0;
	parent.fccType		= (FOURCC)0;
	parent.dwDataOffset = 0;
	parent.dwFlags		= 0;

	// 構造体のコピー
	child = parent;

	// ファイルのオープン
	hmfr = mmioOpen( szFileName, NULL,MMIO_READ | MMIO_ALLOCBUF );
	if ( hmfr == NULL ) {
		// ファイルオープン失敗
		return 0;
	}

	parent.fccType = mmioFOURCC( 'W','A','V','E' );
	if ( mmioDescend( hmfr, &parent, NULL, MMIO_FINDRIFF ) ) {
		// RIFFディセンド失敗
		mmioClose( hmfr, 0 );
		return 0;
	}

	child.ckid = mmioFOURCC( 'f','m','t',' ' );
	if ( mmioDescend( hmfr, &child, &parent, 0 ) ) {
		// RIFFディセンド失敗
		mmioClose( hmfr, 0 );
		return 0;
	}

	// WAVEヘッダの読みこみ
	if ( mmioRead( hmfr, (char*)wfmtx, sizeof( WAVEFORMATEX )) != sizeof( WAVEFORMATEX ) ) {
		// ヘッダ読みこみの失敗
		mmioClose( hmfr, 0 );
		return 0;
	}

	if ( wfmtx->wFormatTag != WAVE_FORMAT_PCM ) {
		// PCMフォーマットじゃない
		mmioClose( hmfr, 0 );
		return 0;
	}

	if ( mmioAscend( hmfr, &child, 0 ) ) {
		// アセンド失敗
		mmioClose(hmfr, 0);
		return 0;
	}

	child.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
	if ( mmioDescend( hmfr, &child, &parent, MMIO_FINDCHUNK ) ) {
		// ディセンド失敗
		mmioClose( hmfr, 0 );
		return 0;
	}

	// ファイルを閉じる
	mmioClose( hmfr,0 );

	return child.cksize;
}


//////////////////////////////////////////////////////////////////////////////////////////
// stream play.

static BOOL DSoundLoadFileStream( LPSTR szFileName, int nChannel ) {
	HMMIO 			hmfr;
	MMCKINFO		parent,child;
	WAVEFORMATEX	wfmtx;

	// 構造体のセット
	parent.ckid 		= (FOURCC)0;
	parent.cksize 		= 0;
	parent.fccType		= (FOURCC)0;
	parent.dwDataOffset = 0;
	parent.dwFlags		= 0;

	// 構造体のコピー
	child=parent;

	// ファイルのオープン
	hmfr = mmioOpen( szFileName, NULL, MMIO_READ | MMIO_ALLOCBUF );
	if ( hmfr == NULL ) {
		// ファイルオープン失敗
		return FALSE;
	}

	parent.fccType = mmioFOURCC( 'W','A','V','E' );
	if ( mmioDescend( hmfr, &parent, NULL, MMIO_FINDRIFF ) ) {
		// RIFFディセンド失敗
		mmioClose( hmfr, 0 );
		return FALSE;
	}

	child.ckid = mmioFOURCC( 'f','m','t',' ' );
	if ( mmioDescend( hmfr, &child, &parent, 0 ) ) {
		// RIFFディセンド失敗
		mmioClose(hmfr, 0);
		return FALSE;
	}

	// WAVEヘッダの読みこみ
	if ( mmioRead( hmfr, (char*)&wfmtx, sizeof(wfmtx) ) != sizeof(wfmtx) ) {
		// ヘッダ読みこみの失敗
		mmioClose( hmfr, 0 );
		return FALSE;
	}

	if ( wfmtx.wFormatTag != WAVE_FORMAT_PCM ) {
		// PCMフォーマットじゃない
		mmioClose( hmfr, 0 );
		return FALSE;
	}

	if ( mmioAscend( hmfr, &child, 0 ) ) {
		// アセンド失敗
		mmioClose(hmfr, 0);
		return FALSE;
	}

	child.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
	if ( mmioDescend( hmfr, &child, &parent, MMIO_FINDCHUNK ) ) {
		// ディセンド失敗
		mmioClose( hmfr, 0 );
		return FALSE;
	}

	// WAVEヘッダの保管
	memcpy( &pChannel[ nChannel ].wavFormatEx, &wfmtx, sizeof( wfmtx ) );

	// データサイズ、先頭位置の保管
	pChannel[ nChannel ].dwDataSize = child.cksize;
	pChannel[ nChannel ].pDataTop = (void*)child.dwDataOffset;

	// HMMIOの保管
	pChannel[ nChannel ].hmmio = hmfr;

	// ファイルを閉じる
//	mmioClose( hmfr, 0 );

	// セカンダリバッファサイズの確定
	pChannel[ nChannel ].dwSize = wfmtx.nAvgBytesPerSec * DDS_STREAM_BUFFER;

	// セカンダリバッファの作成
	DSBUFFERDESC	dsbdesc;
	ZeroMemory( &dsbdesc, sizeof( dsbdesc ) );
	dsbdesc.dwSize			= sizeof( dsbdesc );
	dsbdesc.dwFlags			= DSBCAPS_CTRLDEFAULT | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STICKYFOCUS | DSBCAPS_GLOBALFOCUS ;
	dsbdesc.lpwfxFormat		= &wfmtx;
	dsbdesc.dwBufferBytes	= pChannel[ nChannel ].dwSize;
	if ( pDSound->CreateSoundBuffer( &dsbdesc, &pChannel[nChannel].lpBuffer, NULL ) != DS_OK ) {
		return FALSE;
	}

	return TRUE;

}




static unsigned __stdcall thStreamPlay( LPVOID lpParam ) {
	int nChannel = (int)lpParam;
	BOOL bEndFlg = FALSE;	// 演奏終了フラグ
    BOOL bPlaying  = FALSE;	// 演奏停止フラグ

    DWORD dwBufPosition = 0;
	// DSBufferをLockするために必要
    LPVOID lplpvAudioPtr1, lplpvAudioPtr2;
    DWORD dwAudioBytes1, dwAudioBytes2;
	// ファイルの読み込み位置
    DWORD dwFilePosition = 0;
	// 再生終了用( 0:通常, 1:最後の再生位置, 2:再生終了 )
    DWORD dwEnding = 0;

	// 最初のデータをバッファに書き込む（リングバッファだけれどスタートからなので分割はあり得ない）
    if( pChannel[nChannel].lpBuffer->Lock( 0,0, &lplpvAudioPtr1, &dwAudioBytes1, &lplpvAudioPtr2,&dwAudioBytes2,DSBLOCK_ENTIREBUFFER ) == DS_OK) {
		// データの先頭にシーク
		mmioSeek( pChannel[nChannel].hmmio, (long)pChannel[nChannel].pDataTop, SEEK_SET );
		// データを読み込む
		mmioRead( pChannel[nChannel].hmmio, (char *)lplpvAudioPtr1, dwAudioBytes1 );
		pChannel[nChannel].lpBuffer->Unlock( lplpvAudioPtr1, dwAudioBytes1, lplpvAudioPtr2, dwAudioBytes2 );
		// 次の読み込み位置まで進める
		dwFilePosition += dwAudioBytes1;
    } else {
		// Lock失敗したので終了
		bEndFlg = TRUE;
	}

	// 再生ループに入る
	while( !bEndFlg ) {
		// 何かイベントが発生するまで待つ
		DWORD ev = WaitForSingleObject( pChannel[nChannel].hEvent, 100 );
		// イベント発生
		if( ev == WAIT_OBJECT_0 ) {
			if( pChannel[nChannel].dwEvent == DSCTRL_EVENT_PLAY ) {
				// 演奏開始
				pChannel[nChannel].lpBuffer->Play( 0, 0, DSBPLAY_LOOPING );
				bPlaying = TRUE;
			} else if( pChannel[nChannel].dwEvent == DSCTRL_EVENT_STOP ) {
				// 演奏停止
				pChannel[nChannel].lpBuffer->Stop();
				bPlaying = FALSE;
			} else {
				// 演奏終了
				bEndFlg = TRUE;
				break;
			}
		} else if( bPlaying ) {
			// 再生中の処理

			// dwPosに現在の再生ポジションを得る
			DWORD dwPos;
			pChannel[nChannel].lpBuffer->GetCurrentPosition( &dwPos, NULL );

			// 再生ポジションがdwBufPositionより小さかったら、再生ポジションにバッファの長さを加える
			if( dwPos < dwBufPosition ) {
				dwPos += pChannel[nChannel].dwSize;
			}

			// 再生カーソル位置が半分以上(今回の場合1.5秒)先になければ何もしない
			if( ( dwPos - dwBufPosition ) < ( pChannel[nChannel].wavFormatEx.nAvgBytesPerSec * DDS_STREAM_BUFFER / 2 ) ) {
				continue;
			}

			if( dwEnding == 0 ) {
				BOOL bMp3RetFlg=TRUE;
				// ロックするサイズを求める（バッファの半分か、データの残り分か、より少ない方）
				DWORD Len;
				Len = min( pChannel[ nChannel ].dwDataSize - dwFilePosition, pChannel[ nChannel ].wavFormatEx.nAvgBytesPerSec * DDS_STREAM_BUFFER / 2 );
				// バッファのロック
				if( pChannel[nChannel].lpBuffer->Lock( dwBufPosition, Len, &lplpvAudioPtr1, &dwAudioBytes1, &lplpvAudioPtr2, &dwAudioBytes2, 0 ) == DS_OK ) {
					// 第１ロックバッファに読み込み
					mmioRead(pChannel[nChannel].hmmio, (char*)lplpvAudioPtr1, dwAudioBytes1);
					// 第２ロックバッファは有効か？
					if( lplpvAudioPtr2 ) {
						// 第２ロックバッファに読み込み
						mmioRead( pChannel[nChannel].hmmio, (char*)lplpvAudioPtr2, dwAudioBytes2 );
						dwBufPosition = dwAudioBytes2;
					} else {
						// 第２ロックバッファは必要ない
						dwBufPosition += dwAudioBytes1;
						if( dwBufPosition == pChannel[nChannel].dwSize ) {
							dwBufPosition = 0;
						}
					}
					// アンロック
					pChannel[nChannel].lpBuffer->Unlock( lplpvAudioPtr1, dwAudioBytes1, lplpvAudioPtr2, dwAudioBytes2 );
					// 読み込んだ分進める
					dwFilePosition += Len;
				}
				// データは最後まで読み込んだか？
				if( dwFilePosition >= pChannel[nChannel].dwDataSize ) {
					// データの先頭にシーク
					mmioSeek( pChannel[nChannel].hmmio, (long)pChannel[nChannel].pDataTop, SEEK_SET );
					dwFilePosition = 0;
					// ループ再生を要求されているか？
					if ( !pChannel[nChannel].bLoop ) {
						// ループしないので演奏終了
						dwEnding = 1;
					}
				}
			} else if( dwEnding == 2 ) {
				// 再生終了
				pChannel[nChannel].lpBuffer->Stop();
				bPlaying = FALSE;
			} else {
				// 終了時処理
				if( pChannel[nChannel].lpBuffer->Lock( dwBufPosition, pChannel[nChannel].wavFormatEx.nAvgBytesPerSec * DDS_STREAM_BUFFER / 2,
						&lplpvAudioPtr1, &dwAudioBytes1, &lplpvAudioPtr2, &dwAudioBytes2, 0 ) == DS_OK ) {
					// ダミーデータ０で埋める（こうしないとリングバッファのゴミを再生してしまう
					ZeroMemory( lplpvAudioPtr1, dwAudioBytes1 );
					// 第２ロックバッファは有効か？
					if( lplpvAudioPtr2 ) {
						// 有効なので０で埋める
						ZeroMemory( lplpvAudioPtr2, dwAudioBytes2 );
						dwBufPosition  = dwAudioBytes2;
					} else {
						// 有効ではない
						dwBufPosition += dwAudioBytes1;
						if( dwBufPosition == pChannel[nChannel].dwSize ) {
							dwBufPosition = 0;
						}
					}
					// アンロック
					pChannel[nChannel].lpBuffer->Unlock( lplpvAudioPtr1, dwAudioBytes1, lplpvAudioPtr2, dwAudioBytes2 );
					// 再生終了に進める
					dwEnding++;
				}
			}
		}
    }
	Sleep( 60 );
    _endthreadex( 0 );
    return 0;
}



//////////////////////////////////////////////////////////////////////////////////////////
// controll func.

int DSoundLoadFile(
	LPSTR szFileName2,		// ファイル名 or リソース
	int nChannel,			// 読み込みチャンネル番号(-1で空いてるチャンネルに追加)
	DWORD dwFlg				// フラグ
) {
	if ( pDSound == NULL ) { return TRUE; }

	// EPS特例処理
	char szFileName[ MAX_PATH ];
	strcpy( szFileName, szFileName2 );

	// 使うチャンネルを初期化
	nChannel = DSoundInitChannel( nChannel );
	if ( dwFlg == DSLF_FLG_DEFAULT ) {
		WAVEFORMATEX wfmtx;
		DWORD dwSize;
		dwSize = DSoundGetWaveHeader( szFileName, &wfmtx );
		// ストリームバッファの倍以下なら
		if ( wfmtx.nAvgBytesPerSec * DDS_STREAM_BUFFER * 2 > dwSize ) {
			dwFlg = DSLF_FLG_STATIC;
		} else {
			dwFlg = DSLF_FLG_STREAM;
		}
	}
	if ( dwFlg == DSLF_FLG_STATIC ) {
		// スタティック
		DSoundLoadFileStatic( szFileName, nChannel );
		pChannel[ nChannel ].bStream = FALSE;
	} else if ( dwFlg == DSLF_FLG_STREAM ) {
		// ストリーム
		// イベント作成
		pChannel[ nChannel ].hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
		if ( !pChannel[ nChannel ].hEvent ) {
			// 作成失敗
			DSoundReleaseChannel( nChannel );
			return -1;
		}
		// ファイルの読み込み（実際にはバッファ作成までで読み込みはしない）
		DSoundLoadFileStream( szFileName, nChannel );
		// ストリーム再生用スレッドの作成
		unsigned dummy;
		pChannel[ nChannel ].hThread = (HANDLE)_beginthreadex( NULL, 0, thStreamPlay, (void*)nChannel, 0, &dummy );
		if( !pChannel[ nChannel ].hThread ) {
			// 失敗
			DSoundReleaseChannel( nChannel );
			return -1;
		}
		pChannel[ nChannel ].bStream = TRUE;
	}

	return nChannel;
}

BOOL DSoundPlay( int nChannel, BOOL bLoop ) {
	if ( pDSound == NULL ) { return TRUE; }
	if ( nChannel < 0 || nChannel > nChannelMax ) { return TRUE; }
	
	if ( pChannel[nChannel].lpBuffer == NULL ) { return TRUE; }

	pChannel[nChannel].bLoop = bLoop;
	if( pChannel[nChannel].bStream ) {
		// ストリーム
		pChannel[nChannel].dwEvent = DSCTRL_EVENT_PLAY;
		SetEvent( pChannel[nChannel].hEvent );
		WaitForSingleObject( pChannel[nChannel].hThread, 100 );
	} else {
		// スタティック
		pChannel[nChannel].lpBuffer->Play( 0, 0, bLoop == TRUE ? DSBPLAY_LOOPING : 0 );
	}

	return TRUE;
}

void DSoundStop( int nChannel ) {
	if ( pDSound == NULL ) { return; }
	if ( nChannel < 0 || nChannel > nChannelMax ) { return; }
	
	if ( pChannel[nChannel].lpBuffer == NULL ) { return; }

	if( pChannel[nChannel].bStream ) {
		// ストリーム
		pChannel[nChannel].dwEvent = DSCTRL_EVENT_STOP;
		SetEvent( pChannel[nChannel].hEvent );
		WaitForSingleObject( pChannel[nChannel].hThread, 100 );
	} else {
		// スタティック
		pChannel[nChannel].lpBuffer->Stop();
		pChannel[nChannel].lpBuffer->SetCurrentPosition( 0 );
	}

	return;
}

BOOL DSoundReleaseChannel( int nChannel ) {
	if ( nChannel < 0 ||  nChannelMax <= nChannel ) {
		return TRUE;
	}
	DSoundStop( nChannel );
	if ( pChannel[nChannel].lpBuffer != NULL ) {
		if ( pChannel[nChannel].bStream ) {
			pChannel[nChannel].bStream	= FALSE;
			pChannel[nChannel].dwEvent	= DSCTRL_EVENT_EXIT;
			SetEvent( pChannel[nChannel].hEvent );
			WaitForSingleObject( pChannel[nChannel].hThread, 100 );
			CloseHandle( pChannel[nChannel].hThread ); pChannel[nChannel].hThread = NULL;
			CloseHandle( pChannel[nChannel].hEvent ); pChannel[nChannel].hEvent = NULL;
		}
		pChannel[nChannel].lpBuffer->Release();
		pChannel[nChannel].lpBuffer = NULL;
	}
	return TRUE;
}

// 最大１００％
// DSBVOLUME_MAXとDSBVOLUME_MINの値が変わったらまずいかも。
BOOL DSoundSetVolume( int nChannel,int nVolume ) {
	if ( pDSound == NULL ) return TRUE;
	if ( nChannel < 0 || nChannel > nChannelMax ) { return TRUE; }
	if ( pChannel[nChannel].lpBuffer != NULL ) {
		static long lVoltbl[]=
		{//   0,    1,    2,    3,    4,    5,    6,    7,    8,    9,
			    0,  -25,  -50,  -75, -100, -125, -150, -175, -200, -225,
			 -250, -275, -300, -325, -350, -375, -400, -425, -450, -475,
			 -500, -525, -550, -575, -600, -625, -650, -675, -700, -725,
			 -750, -775, -800, -825, -850, -875, -900, -925, -950, -975,
			-1000,-1025,-1050,-1075,-1100,-1125,-1150,-1175,-1200,-1225,
			-1250,-1275,-1300,-1325,-1350,-1375,-1400,-1425,-1450,-1475,
			-1500,-1525,-1550,-1575,-1600,-1625,-1650,-1675,-1700,-1725,
			-1750,-1775,-1800,-1825,-1850,-1875,-1900,-1925,-1950,-1975,
			-2000,-2100,-2200,-2300,-2400,-2500,-2600,-2700,-2800,-2900,
			-3000,-3500,-4000,-4500,-5000,-6000,-7000,-8000,-9000,-9500,-10000
		};
		pChannel[nChannel].lpBuffer->SetVolume( lVoltbl[100-nVolume] );
	}
	return TRUE;
}

// 左最大-100～100右最大
// DSBPAN_LEFTとDSBPAN_RIGHTの値が変わったらまずい
BOOL DSoundSetPan( int nChannel, int nCmd ) {
	int nPan = nCmd * 100;
	if ( pDSound == NULL ) return TRUE;
	if ( nChannel < 0 || nChannel > nChannelMax ) { return TRUE; }
	if ( pChannel[nChannel].lpBuffer != NULL ) {
		pChannel[nChannel].lpBuffer->SetPan( nPan );
	}
	return TRUE;
}

// Stop中ならTRUE / Play中ならFALSE
BOOL DSoundIsStop( int nChannel ) {
	if ( pDSound == NULL ) return TRUE;
	BOOL bFlg = TRUE;
	if ( nChannel < 0 || nChannel > nChannelMax ) { return TRUE; }
	if ( pChannel[nChannel].lpBuffer != NULL ) {
		DWORD dwData;
		pChannel[nChannel].lpBuffer->GetStatus( &dwData );
		if ( dwData & DSBSTATUS_PLAYING ) {
			// 再生中
			bFlg = FALSE;
		}
	}
	return bFlg;
}


