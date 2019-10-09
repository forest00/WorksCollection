#pragma once

// 楽曲ファイル番号(使用する楽曲ファイル一覧と合わせる)
typedef enum {
	SOUND_NONE,

	BGM_TITLE,
	BGM_OP,
	BGM_STAGE1,
	BGM_STAGE2,
	BGM_BOSS,
    BGM_STAGE_SELECT,
	BGM_STAFF_ROLE,
    BGM_MODE_SELECT,
    
	SE_1P_SLASH,
	SE_PLAYER_DAMAGE,
	SE_2P_RIFLE,
	SE_GOBLIN_PUNCH,
	SE_TITAN_ATTACK,
	SE_DECISION_BUTTON,
	SE_CROSS_BUTTON,
	SE_CASTLE_EXPLOSION,
	SE_TOWER_EXPLOSION,
    SE_TITLE_DECISION_BUTTON,
    SE_TITLE_ROAR,

	SOUND_MAX,
}SoundFileId;


//なんか関数
int soundMgrLoad();

int soundMgrPlay(SoundFileId);



//soundMgrPlay(SE_1P_SLASH);
//soundMgrPlay(SE_PLAYER_DAMAGE);
//soundMgrPlay(SE_2P_RIFLE);
//soundMgrPlay(SE_GOBLIN_PUNCH);
//soundMgrPlay(SE_TITAN_ATTACK);
//soundMgrPlay(SE_DECISION_BUTTON);
//soundMgrPlay(SE_CROSS_BUTTON);
//soundMgrPlay(SE_CASTLE_EXPLOSION);
//soundMgrPlay(SE_TOWER_EXPLOSION);

