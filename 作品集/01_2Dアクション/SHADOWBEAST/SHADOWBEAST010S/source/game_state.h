#pragma once

/*
��ʂ̉���
*/
#define	DISP_W		1280

/*
��ʂ̏c��
*/
#define	DISP_H		720

/*
��ʂ̒���x
*/
#define	DISP_CENTER_X		(DISP_W/2)

/*
��ʂ̒���y
*/
#define	DISP_CENTER_Y		(DISP_H/2)

/*
�Q�[���̂��ꂼ��̏��
*/
typedef enum {
	TEAMLOGO,
    TITLE,
    MODESELECT,
    OPTION,
    STAGESELECT,
    GAMEPLAY,
    GAMECLEAR,
    GAMEOVER,
    GAMECOMPLETE,
    OPENING,
    OPENING2,
    ENDING,
	STAFFROLL,
    MOVING,
    STORY,
}GameState;


GameState getGameState();
void initGameState();
void changeGameState(GameState next);
void moveGameState();
void setSelectedStage(int stage);
void updateGameFrame();
void intoGameStart();
void intoGameClear();
void intoEnding();
void intoGameOver();

bool isChangingGameState();
int getSelectedStage();
unsigned int getGameFrame();
int isGameClear();
int isGameEnding();
int isGameOver();

extern void pause();

int countUpPlayer1Dead();
int countUpPlayer2Dead();
int countUpEnemyKill();
int addTowerDamege(int damage);
int setTowerMaxHp(int hp);

//���U���g��ʗp��
int getPlayer1DownCount();
int getPlayer2DownCount();
int getEnemyKillCount();
double getTopwerDurability();
