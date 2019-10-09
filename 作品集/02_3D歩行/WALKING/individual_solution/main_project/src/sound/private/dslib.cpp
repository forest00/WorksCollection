/*
** Direct Sound Lib.
**
** �u�ݒ�v C/C++ �� �R�[�h�������u�}���`�X���b�h�v�ɕύX
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
	// DirectX7�ȍ~�Ή��p
	#define	DSBCAPS_CTRLDEFAULT	(DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY)
#endif


static LPDIRECTSOUND pDSound = NULL;
static LPDIRECTSOUNDBUFFER pDSPrimary;
static DWORD DSoundGetWaveHeader( LPSTR szFileName, WAVEFORMATEX *wfmtx );
//static BOOL DSoundReleaseChannel( int nChannel );

// �`�����l�����̊i�[�\����
typedef struct tagCHANNELSTRUCT {
	BOOL				bStream;				// �X�g���[���Đ��Ȃ�TRUE
	char				szFileName[MAX_PATH];	// �ǂݍ��񂾃t�@�C���̖��O
	LPDIRECTSOUNDBUFFER	lpBuffer;				// DirectSoundBuffer �̃|�C���^
	DWORD				dwSize;					// Buffer�̃T�C�Y
	BOOL				bLoop;					// ���[�v�Đ����ۂ�
	// �X�g���[���Đ��p
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

#define	DDS_STREAM_BUFFER	3	// �o�b�t�@�T�C�Y(sec)
static unsigned __stdcall thStreamPlay( LPVOID lpParam );
static BOOL DSoundLoadFileStream( LPSTR szFileName, int nChannel );





//////////////////////////////////////////////////////////
// DirectSound�̏�����
BOOL DSoundInit(
	HWND hWnd,			// �e�E�B���h�E�̃n���h��
	LPGUID lpGuid		// �g���f�o�C�X��GUID�ANULL���ƃf�t�H���g
) {	
	// �I�u�W�F�N�g�𐶐�����
	if ( DirectSoundCreate( lpGuid, &pDSound, NULL ) != DS_OK ) {
		return FALSE;
	}

	// �������x����ݒ�
	if ( pDSound->SetCooperativeLevel( hWnd, DSSCL_PRIORITY ) != DS_OK ) {
		// �������x���̐ݒ�Ɏ��s
		pDSound->Release(); pDSound = NULL;
		return FALSE;
	}

	// �X�s�[�J�̐ݒ�
//	pDSound ->SetSpeakerConfig(DSSPEAKER_HEADPHONE);

	// �v���C�}���o�b�t�@�̍쐬
	DSBUFFERDESC dsdc;
	ZeroMemory( &dsdc, sizeof( DSBUFFERDESC ) );
	dsdc.dwSize			= sizeof( DSBUFFERDESC );
	dsdc.dwFlags		= DSBCAPS_CTRLVOLUME | DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLPAN ;
	dsdc.dwBufferBytes	= 0;
	dsdc.lpwfxFormat	= NULL;
	pDSound->CreateSoundBuffer( &dsdc, &pDSPrimary, NULL) ;

	// �v���C�}���o�b�t�@�̏o�̓t�H�[�}�b�g�� 44.1KHz, 16bit, stereo �ɂ���
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
		// DirectSound�����������Ă��Ȃ�
		return TRUE;
	}

	// ������`�����l�������
	for( int i = 0; i < nChannelMax; i++ ) {
		DSoundReleaseChannel( i );
	}
	// DirectSound�̉��
	pDSPrimary->Release(); pDSPrimary = NULL;
	pDSound->Release(); pDSound = NULL;

	// �`�����l���e�[�u�������
	free( pChannel ); pChannel = NULL;

	return TRUE;
}



//////////////////////////////////////////////////////////
// �w��̃`�����l�����m�ۂ���
static int DSoundInitChannel( int nChannel ) {
	if ( nChannel < 0 ) { nChannel = nChannelMax; }
	if ( nChannelMax <= nChannel ) {
		// �`�����l���̒ǉ�
		if ( pChannel == NULL ) {
			// �`�����l���V�K
			pChannel = ( CHANNELSTRUCT* )malloc( sizeof( CHANNELSTRUCT ) * ( nChannel + 1 ) );
		} else {
			// �`�����l���ǉ�
			pChannel = ( CHANNELSTRUCT* )realloc( pChannel, sizeof( CHANNELSTRUCT ) * ( nChannel + 1 ) );
		}
		// �ǉ���������������
		for ( int i = nChannelMax; i <= nChannel; i++ ) {
			pChannel[i].bStream		= FALSE;
			strcpy( pChannel[i].szFileName, "" );
			pChannel[i].lpBuffer	= NULL;
			pChannel[i].dwSize		= 0;
			pChannel[i].bLoop		= FALSE;
		}
		nChannelMax = nChannel + 1;
	} else {
		// ���󂷂łɂ���`�����l�����g��
		DSoundReleaseChannel( nChannel );
	}

	return nChannel;
}


//////////////////////////////////////////////////////////
// �t�@�C���̓ǂݍ���

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

	// �\���̂̃Z�b�g
	parent.ckid 		= (FOURCC)0;
	parent.cksize 		= 0;
	parent.fccType		= (FOURCC)0;
	parent.dwDataOffset = 0;
	parent.dwFlags		= 0;

	// �\���̂̃R�s�[
	child=parent;

	// �t�@�C���̃I�[�v��
	hmfr = mmioOpen( szFileName, NULL, MMIO_READ | MMIO_ALLOCBUF );
	if ( hmfr == NULL ) {
		// �t�@�C���I�[�v�����s
		return FALSE;
	}

	parent.fccType = mmioFOURCC( 'W','A','V','E' );
	if ( mmioDescend( hmfr, &parent, NULL, MMIO_FINDRIFF ) ) {
		// RIFF�f�B�Z���h���s
		mmioClose( hmfr, 0 );
		return FALSE;
	}

	child.ckid = mmioFOURCC( 'f','m','t',' ' );
	if ( mmioDescend( hmfr, &child, &parent, 0 ) ) {
		// RIFF�f�B�Z���h���s
		mmioClose(hmfr, 0);
		return FALSE;
	}

	// WAVE�w�b�_�̓ǂ݂���
	if ( mmioRead( hmfr, (char*)&wfmtx, sizeof(wfmtx) ) != sizeof(wfmtx) ) {
		// �w�b�_�ǂ݂��݂̎��s
		mmioClose( hmfr, 0 );
		return FALSE;
	}

	if ( wfmtx.wFormatTag != WAVE_FORMAT_PCM ) {
		// PCM�t�H�[�}�b�g����Ȃ�
		mmioClose( hmfr, 0 );
		return FALSE;
	}

	if ( mmioAscend( hmfr, &child, 0 ) ) {
		// �A�Z���h���s
		mmioClose(hmfr, 0);
		return FALSE;
	}

	child.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
	if ( mmioDescend( hmfr, &child, &parent, MMIO_FINDCHUNK ) ) {
		// �f�B�Z���h���s
		mmioClose( hmfr, 0 );
		return FALSE;
	}

	// �������̊m��(�T�E���h�o�b�t�@)
	char *pBuffer = (char*)malloc( child.cksize );

	// mmioOpen���ꂽ�t�@�C������o�b�t�@��WAVE�f�[�^��ǂݍ���
	if ( (DWORD)mmioRead( hmfr, pBuffer, child.cksize ) != child.cksize ) {
		// �ǂ݂��ݎ��s
		mmioClose( hmfr, 0 );
		return FALSE;
	}

	// �t�@�C�������
	mmioClose( hmfr, 0 );

	// �Z�J���_���o�b�t�@�̍쐬
	CreateBuffer( nChannel, wfmtx.nSamplesPerSec,
		wfmtx.wBitsPerSample, wfmtx.nChannels, child.cksize );

	// �T�E���h�o�b�t�@�Ƀf�[�^���R�s�[����
	FillBuffer( nChannel, pBuffer );

	// �������̉��
	free( pBuffer );

	return TRUE;

}

// �t�@�C������WAVE�w�b�_��ǂݍ��ށB�f�[�^�T�C�Y���߂�
static DWORD DSoundGetWaveHeader( LPSTR szFileName, WAVEFORMATEX *wfmtx ) {
	HMMIO 			hmfr;
	MMCKINFO		parent,child;

	// �\���̂̃Z�b�g
	parent.ckid 		= (FOURCC)0;
	parent.cksize 		= 0;
	parent.fccType		= (FOURCC)0;
	parent.dwDataOffset = 0;
	parent.dwFlags		= 0;

	// �\���̂̃R�s�[
	child = parent;

	// �t�@�C���̃I�[�v��
	hmfr = mmioOpen( szFileName, NULL,MMIO_READ | MMIO_ALLOCBUF );
	if ( hmfr == NULL ) {
		// �t�@�C���I�[�v�����s
		return 0;
	}

	parent.fccType = mmioFOURCC( 'W','A','V','E' );
	if ( mmioDescend( hmfr, &parent, NULL, MMIO_FINDRIFF ) ) {
		// RIFF�f�B�Z���h���s
		mmioClose( hmfr, 0 );
		return 0;
	}

	child.ckid = mmioFOURCC( 'f','m','t',' ' );
	if ( mmioDescend( hmfr, &child, &parent, 0 ) ) {
		// RIFF�f�B�Z���h���s
		mmioClose( hmfr, 0 );
		return 0;
	}

	// WAVE�w�b�_�̓ǂ݂���
	if ( mmioRead( hmfr, (char*)wfmtx, sizeof( WAVEFORMATEX )) != sizeof( WAVEFORMATEX ) ) {
		// �w�b�_�ǂ݂��݂̎��s
		mmioClose( hmfr, 0 );
		return 0;
	}

	if ( wfmtx->wFormatTag != WAVE_FORMAT_PCM ) {
		// PCM�t�H�[�}�b�g����Ȃ�
		mmioClose( hmfr, 0 );
		return 0;
	}

	if ( mmioAscend( hmfr, &child, 0 ) ) {
		// �A�Z���h���s
		mmioClose(hmfr, 0);
		return 0;
	}

	child.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
	if ( mmioDescend( hmfr, &child, &parent, MMIO_FINDCHUNK ) ) {
		// �f�B�Z���h���s
		mmioClose( hmfr, 0 );
		return 0;
	}

	// �t�@�C�������
	mmioClose( hmfr,0 );

	return child.cksize;
}


//////////////////////////////////////////////////////////////////////////////////////////
// stream play.

static BOOL DSoundLoadFileStream( LPSTR szFileName, int nChannel ) {
	HMMIO 			hmfr;
	MMCKINFO		parent,child;
	WAVEFORMATEX	wfmtx;

	// �\���̂̃Z�b�g
	parent.ckid 		= (FOURCC)0;
	parent.cksize 		= 0;
	parent.fccType		= (FOURCC)0;
	parent.dwDataOffset = 0;
	parent.dwFlags		= 0;

	// �\���̂̃R�s�[
	child=parent;

	// �t�@�C���̃I�[�v��
	hmfr = mmioOpen( szFileName, NULL, MMIO_READ | MMIO_ALLOCBUF );
	if ( hmfr == NULL ) {
		// �t�@�C���I�[�v�����s
		return FALSE;
	}

	parent.fccType = mmioFOURCC( 'W','A','V','E' );
	if ( mmioDescend( hmfr, &parent, NULL, MMIO_FINDRIFF ) ) {
		// RIFF�f�B�Z���h���s
		mmioClose( hmfr, 0 );
		return FALSE;
	}

	child.ckid = mmioFOURCC( 'f','m','t',' ' );
	if ( mmioDescend( hmfr, &child, &parent, 0 ) ) {
		// RIFF�f�B�Z���h���s
		mmioClose(hmfr, 0);
		return FALSE;
	}

	// WAVE�w�b�_�̓ǂ݂���
	if ( mmioRead( hmfr, (char*)&wfmtx, sizeof(wfmtx) ) != sizeof(wfmtx) ) {
		// �w�b�_�ǂ݂��݂̎��s
		mmioClose( hmfr, 0 );
		return FALSE;
	}

	if ( wfmtx.wFormatTag != WAVE_FORMAT_PCM ) {
		// PCM�t�H�[�}�b�g����Ȃ�
		mmioClose( hmfr, 0 );
		return FALSE;
	}

	if ( mmioAscend( hmfr, &child, 0 ) ) {
		// �A�Z���h���s
		mmioClose(hmfr, 0);
		return FALSE;
	}

	child.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
	if ( mmioDescend( hmfr, &child, &parent, MMIO_FINDCHUNK ) ) {
		// �f�B�Z���h���s
		mmioClose( hmfr, 0 );
		return FALSE;
	}

	// WAVE�w�b�_�̕ۊ�
	memcpy( &pChannel[ nChannel ].wavFormatEx, &wfmtx, sizeof( wfmtx ) );

	// �f�[�^�T�C�Y�A�擪�ʒu�̕ۊ�
	pChannel[ nChannel ].dwDataSize = child.cksize;
	pChannel[ nChannel ].pDataTop = (void*)child.dwDataOffset;

	// HMMIO�̕ۊ�
	pChannel[ nChannel ].hmmio = hmfr;

	// �t�@�C�������
//	mmioClose( hmfr, 0 );

	// �Z�J���_���o�b�t�@�T�C�Y�̊m��
	pChannel[ nChannel ].dwSize = wfmtx.nAvgBytesPerSec * DDS_STREAM_BUFFER;

	// �Z�J���_���o�b�t�@�̍쐬
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
	BOOL bEndFlg = FALSE;	// ���t�I���t���O
    BOOL bPlaying  = FALSE;	// ���t��~�t���O

    DWORD dwBufPosition = 0;
	// DSBuffer��Lock���邽�߂ɕK�v
    LPVOID lplpvAudioPtr1, lplpvAudioPtr2;
    DWORD dwAudioBytes1, dwAudioBytes2;
	// �t�@�C���̓ǂݍ��݈ʒu
    DWORD dwFilePosition = 0;
	// �Đ��I���p( 0:�ʏ�, 1:�Ō�̍Đ��ʒu, 2:�Đ��I�� )
    DWORD dwEnding = 0;

	// �ŏ��̃f�[�^���o�b�t�@�ɏ������ށi�����O�o�b�t�@������ǃX�^�[�g����Ȃ̂ŕ����͂��蓾�Ȃ��j
    if( pChannel[nChannel].lpBuffer->Lock( 0,0, &lplpvAudioPtr1, &dwAudioBytes1, &lplpvAudioPtr2,&dwAudioBytes2,DSBLOCK_ENTIREBUFFER ) == DS_OK) {
		// �f�[�^�̐擪�ɃV�[�N
		mmioSeek( pChannel[nChannel].hmmio, (long)pChannel[nChannel].pDataTop, SEEK_SET );
		// �f�[�^��ǂݍ���
		mmioRead( pChannel[nChannel].hmmio, (char *)lplpvAudioPtr1, dwAudioBytes1 );
		pChannel[nChannel].lpBuffer->Unlock( lplpvAudioPtr1, dwAudioBytes1, lplpvAudioPtr2, dwAudioBytes2 );
		// ���̓ǂݍ��݈ʒu�܂Ői�߂�
		dwFilePosition += dwAudioBytes1;
    } else {
		// Lock���s�����̂ŏI��
		bEndFlg = TRUE;
	}

	// �Đ����[�v�ɓ���
	while( !bEndFlg ) {
		// �����C�x���g����������܂ő҂�
		DWORD ev = WaitForSingleObject( pChannel[nChannel].hEvent, 100 );
		// �C�x���g����
		if( ev == WAIT_OBJECT_0 ) {
			if( pChannel[nChannel].dwEvent == DSCTRL_EVENT_PLAY ) {
				// ���t�J�n
				pChannel[nChannel].lpBuffer->Play( 0, 0, DSBPLAY_LOOPING );
				bPlaying = TRUE;
			} else if( pChannel[nChannel].dwEvent == DSCTRL_EVENT_STOP ) {
				// ���t��~
				pChannel[nChannel].lpBuffer->Stop();
				bPlaying = FALSE;
			} else {
				// ���t�I��
				bEndFlg = TRUE;
				break;
			}
		} else if( bPlaying ) {
			// �Đ����̏���

			// dwPos�Ɍ��݂̍Đ��|�W�V�����𓾂�
			DWORD dwPos;
			pChannel[nChannel].lpBuffer->GetCurrentPosition( &dwPos, NULL );

			// �Đ��|�W�V������dwBufPosition��菬����������A�Đ��|�W�V�����Ƀo�b�t�@�̒�����������
			if( dwPos < dwBufPosition ) {
				dwPos += pChannel[nChannel].dwSize;
			}

			// �Đ��J�[�\���ʒu�������ȏ�(����̏ꍇ1.5�b)��ɂȂ���Ή������Ȃ�
			if( ( dwPos - dwBufPosition ) < ( pChannel[nChannel].wavFormatEx.nAvgBytesPerSec * DDS_STREAM_BUFFER / 2 ) ) {
				continue;
			}

			if( dwEnding == 0 ) {
				BOOL bMp3RetFlg=TRUE;
				// ���b�N����T�C�Y�����߂�i�o�b�t�@�̔������A�f�[�^�̎c�蕪���A��菭�Ȃ����j
				DWORD Len;
				Len = min( pChannel[ nChannel ].dwDataSize - dwFilePosition, pChannel[ nChannel ].wavFormatEx.nAvgBytesPerSec * DDS_STREAM_BUFFER / 2 );
				// �o�b�t�@�̃��b�N
				if( pChannel[nChannel].lpBuffer->Lock( dwBufPosition, Len, &lplpvAudioPtr1, &dwAudioBytes1, &lplpvAudioPtr2, &dwAudioBytes2, 0 ) == DS_OK ) {
					// ��P���b�N�o�b�t�@�ɓǂݍ���
					mmioRead(pChannel[nChannel].hmmio, (char*)lplpvAudioPtr1, dwAudioBytes1);
					// ��Q���b�N�o�b�t�@�͗L�����H
					if( lplpvAudioPtr2 ) {
						// ��Q���b�N�o�b�t�@�ɓǂݍ���
						mmioRead( pChannel[nChannel].hmmio, (char*)lplpvAudioPtr2, dwAudioBytes2 );
						dwBufPosition = dwAudioBytes2;
					} else {
						// ��Q���b�N�o�b�t�@�͕K�v�Ȃ�
						dwBufPosition += dwAudioBytes1;
						if( dwBufPosition == pChannel[nChannel].dwSize ) {
							dwBufPosition = 0;
						}
					}
					// �A�����b�N
					pChannel[nChannel].lpBuffer->Unlock( lplpvAudioPtr1, dwAudioBytes1, lplpvAudioPtr2, dwAudioBytes2 );
					// �ǂݍ��񂾕��i�߂�
					dwFilePosition += Len;
				}
				// �f�[�^�͍Ō�܂œǂݍ��񂾂��H
				if( dwFilePosition >= pChannel[nChannel].dwDataSize ) {
					// �f�[�^�̐擪�ɃV�[�N
					mmioSeek( pChannel[nChannel].hmmio, (long)pChannel[nChannel].pDataTop, SEEK_SET );
					dwFilePosition = 0;
					// ���[�v�Đ���v������Ă��邩�H
					if ( !pChannel[nChannel].bLoop ) {
						// ���[�v���Ȃ��̂ŉ��t�I��
						dwEnding = 1;
					}
				}
			} else if( dwEnding == 2 ) {
				// �Đ��I��
				pChannel[nChannel].lpBuffer->Stop();
				bPlaying = FALSE;
			} else {
				// �I��������
				if( pChannel[nChannel].lpBuffer->Lock( dwBufPosition, pChannel[nChannel].wavFormatEx.nAvgBytesPerSec * DDS_STREAM_BUFFER / 2,
						&lplpvAudioPtr1, &dwAudioBytes1, &lplpvAudioPtr2, &dwAudioBytes2, 0 ) == DS_OK ) {
					// �_�~�[�f�[�^�O�Ŗ��߂�i�������Ȃ��ƃ����O�o�b�t�@�̃S�~���Đ����Ă��܂�
					ZeroMemory( lplpvAudioPtr1, dwAudioBytes1 );
					// ��Q���b�N�o�b�t�@�͗L�����H
					if( lplpvAudioPtr2 ) {
						// �L���Ȃ̂łO�Ŗ��߂�
						ZeroMemory( lplpvAudioPtr2, dwAudioBytes2 );
						dwBufPosition  = dwAudioBytes2;
					} else {
						// �L���ł͂Ȃ�
						dwBufPosition += dwAudioBytes1;
						if( dwBufPosition == pChannel[nChannel].dwSize ) {
							dwBufPosition = 0;
						}
					}
					// �A�����b�N
					pChannel[nChannel].lpBuffer->Unlock( lplpvAudioPtr1, dwAudioBytes1, lplpvAudioPtr2, dwAudioBytes2 );
					// �Đ��I���ɐi�߂�
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
	LPSTR szFileName2,		// �t�@�C���� or ���\�[�X
	int nChannel,			// �ǂݍ��݃`�����l���ԍ�(-1�ŋ󂢂Ă�`�����l���ɒǉ�)
	DWORD dwFlg				// �t���O
) {
	if ( pDSound == NULL ) { return TRUE; }

	// EPS���Ꮘ��
	char szFileName[ MAX_PATH ];
	strcpy( szFileName, szFileName2 );

	// �g���`�����l����������
	nChannel = DSoundInitChannel( nChannel );
	if ( dwFlg == DSLF_FLG_DEFAULT ) {
		WAVEFORMATEX wfmtx;
		DWORD dwSize;
		dwSize = DSoundGetWaveHeader( szFileName, &wfmtx );
		// �X�g���[���o�b�t�@�̔{�ȉ��Ȃ�
		if ( wfmtx.nAvgBytesPerSec * DDS_STREAM_BUFFER * 2 > dwSize ) {
			dwFlg = DSLF_FLG_STATIC;
		} else {
			dwFlg = DSLF_FLG_STREAM;
		}
	}
	if ( dwFlg == DSLF_FLG_STATIC ) {
		// �X�^�e�B�b�N
		DSoundLoadFileStatic( szFileName, nChannel );
		pChannel[ nChannel ].bStream = FALSE;
	} else if ( dwFlg == DSLF_FLG_STREAM ) {
		// �X�g���[��
		// �C�x���g�쐬
		pChannel[ nChannel ].hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
		if ( !pChannel[ nChannel ].hEvent ) {
			// �쐬���s
			DSoundReleaseChannel( nChannel );
			return -1;
		}
		// �t�@�C���̓ǂݍ��݁i���ۂɂ̓o�b�t�@�쐬�܂łœǂݍ��݂͂��Ȃ��j
		DSoundLoadFileStream( szFileName, nChannel );
		// �X�g���[���Đ��p�X���b�h�̍쐬
		unsigned dummy;
		pChannel[ nChannel ].hThread = (HANDLE)_beginthreadex( NULL, 0, thStreamPlay, (void*)nChannel, 0, &dummy );
		if( !pChannel[ nChannel ].hThread ) {
			// ���s
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
		// �X�g���[��
		pChannel[nChannel].dwEvent = DSCTRL_EVENT_PLAY;
		SetEvent( pChannel[nChannel].hEvent );
		WaitForSingleObject( pChannel[nChannel].hThread, 100 );
	} else {
		// �X�^�e�B�b�N
		pChannel[nChannel].lpBuffer->Play( 0, 0, bLoop == TRUE ? DSBPLAY_LOOPING : 0 );
	}

	return TRUE;
}

void DSoundStop( int nChannel ) {
	if ( pDSound == NULL ) { return; }
	if ( nChannel < 0 || nChannel > nChannelMax ) { return; }
	
	if ( pChannel[nChannel].lpBuffer == NULL ) { return; }

	if( pChannel[nChannel].bStream ) {
		// �X�g���[��
		pChannel[nChannel].dwEvent = DSCTRL_EVENT_STOP;
		SetEvent( pChannel[nChannel].hEvent );
		WaitForSingleObject( pChannel[nChannel].hThread, 100 );
	} else {
		// �X�^�e�B�b�N
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

// �ő�P�O�O��
// DSBVOLUME_MAX��DSBVOLUME_MIN�̒l���ς������܂��������B
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

// ���ő�-100�`100�E�ő�
// DSBPAN_LEFT��DSBPAN_RIGHT�̒l���ς������܂���
BOOL DSoundSetPan( int nChannel, int nCmd ) {
	int nPan = nCmd * 100;
	if ( pDSound == NULL ) return TRUE;
	if ( nChannel < 0 || nChannel > nChannelMax ) { return TRUE; }
	if ( pChannel[nChannel].lpBuffer != NULL ) {
		pChannel[nChannel].lpBuffer->SetPan( nPan );
	}
	return TRUE;
}

// Stop���Ȃ�TRUE / Play���Ȃ�FALSE
BOOL DSoundIsStop( int nChannel ) {
	if ( pDSound == NULL ) return TRUE;
	BOOL bFlg = TRUE;
	if ( nChannel < 0 || nChannel > nChannelMax ) { return TRUE; }
	if ( pChannel[nChannel].lpBuffer != NULL ) {
		DWORD dwData;
		pChannel[nChannel].lpBuffer->GetStatus( &dwData );
		if ( dwData & DSBSTATUS_PLAYING ) {
			// �Đ���
			bFlg = FALSE;
		}
	}
	return bFlg;
}


