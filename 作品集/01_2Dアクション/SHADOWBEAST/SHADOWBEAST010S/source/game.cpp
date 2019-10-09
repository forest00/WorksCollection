/*
** ゲーム本体
*/


// 使いたい機能や命令にあわせて include を追加する


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

// アプリの初期化
// 起動時に1回だけ実行される
void AppInit() {
	//ファイルからの読み込み
	loadCharacterDataFromFile();
	imageMgrInit();


	//初期化
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


	//ゲームの準備
	//setSelectedStage(6);//ウミヘビ戦
	//setSelectedStage(3);//巨人戦
	//setSelectedStage(0);//チュートリアルステージ
	setSelectedStage(1);//ステージ1
	//setSelectedStage(4);//ステージ2


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


// アプリの解放
// 終了時に1回だけ実行される
void AppRelease() {

	endCharacters();

}

//
// フレーム処理。1フレームに1回ずつ実行される
//

// フレーム処理：入力
void FrameInput() {

	keyUpdate();

}




// フレーム処理：計算
void FrameProcess() {

	gameSection(); //現在の状態からいろいろ分岐する

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


// フレーム処理：描画
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

