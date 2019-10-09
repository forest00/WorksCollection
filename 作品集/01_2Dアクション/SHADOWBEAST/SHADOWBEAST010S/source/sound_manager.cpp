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

//楽曲ファイル一覧
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

	{PLAY_BACK_GROUND,"se_bgm/SE/1P_斬撃.wav"},
	{PLAY_BACK_GROUND,"se_bgm/SE/1P・2Pダメージ.wav"},
	{PLAY_BACK_GROUND,"se_bgm/SE/2P_ライフル.wav"},
	{PLAY_BACK_GROUND,"se_bgm/SE/ゴブリン_パンチ.wav"},
	{PLAY_BACK_GROUND,"se_bgm/SE/巨人＿薙ぎ払い.wav"},
	{PLAY_BACK_GROUND,"se_bgm/SE/決定ボタン.wav"},
	{PLAY_BACK_GROUND,"se_bgm/SE/十字ボタン.wav"},
	{PLAY_BACK_GROUND,"se_bgm/SE/城完全爆発.wav"},
	{PLAY_BACK_GROUND,"se_bgm/SE/塔爆発.wav"},
    {PLAY_BACK_GROUND,"se_bgm/SE/タイトル画面 SE1 ボタン押した時の音.wav" },
    {PLAY_BACK_GROUND,"se_bgm/SE/タイトル画面 SE2 ライオン.wav" },
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
