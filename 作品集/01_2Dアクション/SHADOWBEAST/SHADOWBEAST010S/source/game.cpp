/*
** �Q�[���{��
*/


// �g�������@�\�▽�߂ɂ��킹�� include ��ǉ�����


#include	<Windows.h>
#include	"amgame.h"


#include "game.h"
#include "game_state.h"
#include "opening.h"
#include "team_logo.h"
#include "title.h"
#include "mode_select.h"
#include "option.h"
#include "stage_select.h"
#include "game_play.h"
#include "game_clear.h"
#include "game_complete.h"
#include "gameover.h"
#include "ending.h"
#include "camera.h"
#include "input_manager.h"
#include "image_manager.h"
#include "sound_manager.h"
#include "chara__manager.h"
#include "opening2.h"
#include "staffroll.h"

#include "forDebug.h"


int gameSection();

// �A�v���̏�����
// �N������1�񂾂����s�����
void AppInit() {
	//�t�@�C������̓ǂݍ���
	loadCharacterDataFromFile();
	imageMgrInit();


	//������
	initGameState();
	initTeamLogo();
	initOpening();
    initOpening2();
	initEnding();
	initTitle();
	initOption();
	initStageSelect();
	initModeSelect();
	initGamePlay();
	initGameClear();
	initGameComplete();
	initGameOver();
	initStaffRoll();
	initCharacters();
	initDebugInfo();


	//�Q�[���̏���
	//setSelectedStage(6);//�E�~�w�r��
	//setSelectedStage(3);//���l��
	//setSelectedStage(0);//�`���[�g���A���X�e�[�W
	setSelectedStage(1);//�X�e�[�W1
	//setSelectedStage(4);//�X�e�[�W2


	//changeGameState(GAMEPLAY);
	//changeGameState(TITLE);
	//changeGameState(GAMECLEAR);
    changeGameState(TEAMLOGO);
	//changeGameState(MODESELECT);
	//changeGameState(OPTION);
    //changeGameState(OPENING);
    //changeGameState(ENDING);
    //changeGameState(STAGESELECT);
	//changeGameState(STAGECLEAR);
	
	//effectInit();

}


// �A�v���̉��
// �I������1�񂾂����s�����
void AppRelease() {

	endCharacters();

}

//
// �t���[�������B1�t���[����1�񂸂��s�����
//

// �t���[�������F����
void FrameInput() {

	keyUpdate();

}




// �t���[�������F�v�Z
void FrameProcess() {

	gameSection(); //���݂̏�Ԃ��炢�낢�땪�򂷂�

}


int gameSection(){
	if(isChangingGameState()){
		moveGameState();
		switch(getGameState()){
			case TEAMLOGO:
				preTeamLogo();
				break;
			case TITLE:
				preTitle();
				break;
			case MODESELECT:
				preModeSelect();
				break;
			case OPTION:
				preOption();
				break;
			case STAGESELECT:
				preStageSelect();
				break;
			case GAMEPLAY:
				preGamePlay();
				break;
			case GAMECLEAR:
				preGameClear();
				break;
			case GAMEOVER:
				preGameOver();
				break;
			case GAMECOMPLETE:
				preGameComplete();
				break;
			case OPENING:
				preOpening();
				break;
            case OPENING2:
                preOpening2();
                break;
			case ENDING:
				preEnding();
				break;
			case STAFFROLL:
				preStaffRoll();
				break;
			default:
				break;
		}
	}
	switch(getGameState()){
		case TEAMLOGO:
			processTeamLogo();
			break;
		case TITLE:
			processTitle();
			break;
		case MODESELECT:
			processModeSelect();
			break;
		case OPTION:
			processOption();
			break;
		case STAGESELECT:
			processStageSelect();
			break;
		case GAMEPLAY:
			processGamePlay();
			break;
		case GAMECLEAR:
			processGameClear();
			break;
		case GAMEOVER:
			processGameOver();
			break;
		case GAMECOMPLETE:
			processGameComplete();
			break;
		case OPENING:
			processOpening();
			break;
        case OPENING2:
            processOpening2();
            break;
		case ENDING:
			processEnding();
			break;
		case STAFFROLL:
			processStaffRoll();
			break;
		default:
			break;
	}
	updateGameFrame();
	return 0;
}


// �t���[�������F�`��
void FrameDraw() {

	brightOption();

	SetDrawMode(AMDRAW_NOBLEND, 0);
	switch(getGameState()){
		case TEAMLOGO:
			drawTeamLogo();
			break;
		case TITLE:
			drawTitle();
			break;
		case MODESELECT:
			drawModeSelect();
			break;
		case OPTION:
			drawOption();
			break;
		case STAGESELECT:
			drawStageSelect();
			break;
		case GAMEPLAY:
			drawGamePlay();
			break;
		case GAMECLEAR:
			drawGameClear();
			break;
		case GAMEOVER:
			drawGameOver();
			break;
		case GAMECOMPLETE:
			drawGameComplete();
			break;
		case OPENING:
			drawOpening();
			break;
        case OPENING2:
            drawOpening2();
            break;
		case ENDING:
			drawEnding();
			break;
		case STAFFROLL:
			drawStaffRoll();
			break;
		default:
			break;
	}

	renderDebugInfo();


	AM_Flip();
}

