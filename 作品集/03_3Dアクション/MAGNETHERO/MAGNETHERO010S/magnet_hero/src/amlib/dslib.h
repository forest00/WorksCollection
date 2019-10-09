#ifndef __DSLIB_H__
#define	__DSLIB_H__
/*
** DirectSound Lib.
*/


// ������
BOOL DSoundInit( HWND hWnd, LPGUID lpGuid );

// ���
BOOL DSoundRelease();

// wav�t�@�C���̓ǂݍ���
enum {
	DSLF_FLG_DEFAULT = 0,
	DSLF_FLG_STATIC,
	DSLF_FLG_STREAM,
};
int DSoundLoadFile(
	LPSTR szfilename,		// �t�@�C���� or ���\�[�X
	int nChannel,			// �ǂݍ��݃`�����l���ԍ�(-1�ŋ󂢂Ă�`�����l���ɒǉ�)
	DWORD dwFlg				// �t���O DSLF_FLG_xxxx
);

// wav�t�@�C���̉��
BOOL DSoundReleaseChannel( int nChannel );

// �Đ�
BOOL DSoundPlay( int nChannel, BOOL bLoop );

// ��~
void DSoundStop( int nChannel );

// �{�����[���Z�b�g
BOOL DSoundSetVolume( int nChannel,int nVolume );

// �p���Z�b�g
BOOL DSoundSetPan( int nChannel, int nCmd );

// �Đ������H
BOOL DSoundIsStop( int nChannel );



#endif

