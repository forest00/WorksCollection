#pragma once

#include <stdio.h>
#include "sound_manager.h"
#include "amgame.h"

enum SoundPlayType{
	PLAY_NONE,
	PLAY_STREAM,
	PLAY_BACK_GROUND,
};

struct Sound{
	SoundPlayType playType;
	char fileName[FILENAME_MAX];
	int handle;
};

//�y�ȃt�@�C���ꗗ
static Sound soundFile[SOUND_MAX] = {
	{PLAY_NONE,""},

	{PLAY_STREAM,"se_bgm/bgm/tittle_bgm.wav"},
	{PLAY_STREAM,"se_bgm/bgm/op_bgm.wav"},
	{PLAY_STREAM,"se_bgm/bgm/stage1.wav"},
	{PLAY_STREAM,"se_bgm/bgm/stage2.wav"},
	{PLAY_STREAM,"se_bgm/bgm/boss.wav"},
	{PLAY_STREAM,"se_bgm/bgm/srtageselect.wav" },
	{PLAY_STREAM,"se_bgm/bgm/staffrole_bgm.wav"},
    {PLAY_STREAM,"se_bgm/bgm/modeselect.wav" },	

	{PLAY_BACK_GROUND,"se_bgm/SE/1P_�a��.wav"},
	{PLAY_BACK_GROUND,"se_bgm/SE/1P�E2P�_���[�W.wav"},
	{PLAY_BACK_GROUND,"se_bgm/SE/2P_���C�t��.wav"},
	{PLAY_BACK_GROUND,"se_bgm/SE/�S�u����_�p���`.wav"},
	{PLAY_BACK_GROUND,"se_bgm/SE/���l�Q�ガ����.wav"},
	{PLAY_BACK_GROUND,"se_bgm/SE/����{�^��.wav"},
	{PLAY_BACK_GROUND,"se_bgm/SE/�\���{�^��.wav"},
	{PLAY_BACK_GROUND,"se_bgm/SE/�銮�S����.wav"},
	{PLAY_BACK_GROUND,"se_bgm/SE/������.wav"},
    {PLAY_BACK_GROUND,"se_bgm/SE/�^�C�g����� SE1 �{�^�����������̉�.wav" },
    {PLAY_BACK_GROUND,"se_bgm/SE/�^�C�g����� SE2 ���C�I��.wav" },
};



int soundMgrLoad(){
	for(int i = 0; i < SOUND_MAX; i++){
		Sound *p = &soundFile[i];
		if(p->playType == PLAY_BACK_GROUND){
			p->handle = LoadWavMem(p->fileName);
		}
	}
	return 0;
}

int soundMgrPlay(SoundFileId id){
	Sound *p = &soundFile[id];
	switch(p->playType){
		case PLAY_NONE:
			return -1;
			break;
		case PLAY_STREAM:
			return PlayWavStreamBack(p->fileName);
			break;
		case PLAY_BACK_GROUND:
		{
			int handle = p->handle;
			return PlayMemBack(handle);
		}
		break;
		default:
			break;
	}
	return 0;
}
