
#include <math.h>
#include "stage_data_reader_assistant.h"
#include "image_manager.h"
#include "chara__common.h"
#include "chara_player1.h"
#include "chara_player2.h"
#include "chara_enemy.h"
#include "game_state.h"
#include "stage_data_reader.h"
#include "chara_boss_stage2_thunder.h"


static constexpr int TRANS_TIME = 60;
static constexpr int TUTORIAL_FIRST_TIME = 96;
static constexpr int TUTORIAL_5_TIME = 300;
static constexpr int TUTORIAL_6_TIME = 300;
static constexpr int TUTORIAL_LAST_TIME = 256;
enum Progress{
	TUTORIAL_PROGRESS_0,
	TUTORIAL_PROGRESS_1,
	TUTORIAL_PROGRESS_2,
	TUTORIAL_PROGRESS_3,
	TUTORIAL_PROGRESS_4,
	TUTORIAL_PROGRESS_5,
	TUTORIAL_PROGRESS_6,
	TUTORIAL_PROGRESS_7,
	TUTORIAL_PROGRESS_END,
};
static constexpr int IMAGE_NUM = TUTORIAL_PROGRESS_END;


struct Conditions0{
	int count;
};
struct Conditions1{
	bool pl1Move;
	bool pl2Move;
};
struct Conditions2{
	bool pl1Jump;
	bool pl2Jump;
};
struct Conditions3{
	int count;
};
struct Conditions4{
	bool alreadyKillPl2;
};
struct Conditions5{
	int count;
};
struct Conditions6{
	int count;
};
struct Conditions7{
	int count;
};
struct Image{
	int index;
	int handle;
};
struct Tutorial{
	Image images[IMAGE_NUM];
	Conditions0 condition0;
	Conditions1 condition1;
	Conditions2 condition2;
	Conditions3 condition3;
	Conditions4 condition4;
	Conditions5 condition5;
	Conditions6 condition6;
	Conditions7 condition7;
	Progress progress;
	int nextTiming;			//次への条件を満たした瞬間
	int transTiming;		//切り替わった瞬間
	int count;
	bool clearNnextCondition;//条件達成
	bool shoudNext;
};
static Tutorial tutorial;






int initTutorial(){
	Tutorial zero = {};
	tutorial = zero;
	for(int i = 0; i < IMAGE_NUM; i++){
		int handle = imageMgrGetHandle(IMAGE_TUTORIAL_0_ + i, 0);
		SetImage(i, handle);
		tutorial.images[i].index = i;
		tutorial.images[i].handle = handle;
	}
	return 0;
}

int updateTutorial(){
	switch(tutorial.progress){
		case TUTORIAL_PROGRESS_0:
			if(tutorial.shoudNext){
				//画像消す
				clearImageInGame(tutorial.images[tutorial.progress].index);
				//次へ
				if(tutorial.count - tutorial.nextTiming >= TRANS_TIME){
					tutorial.progress = (Progress)(tutorial.progress + 1);
					tutorial.transTiming = tutorial.count + 1;
					tutorial.shoudNext = false;
				}
			} else{
				//画像出す
				if(tutorial.count == tutorial.transTiming){
					dispImageInGame(tutorial.images[tutorial.progress].index);
				}
				//ここで条件チェック
				tutorial.clearNnextCondition = tutorial.condition0.count >= TUTORIAL_FIRST_TIME;
				if(tutorial.clearNnextCondition){
					//条件を満たしたら、タイミングを記録し、フラグを切り替える
					tutorial.nextTiming = tutorial.count + 1;
					tutorial.clearNnextCondition = false;
					tutorial.shoudNext = true;
				}
			}
			tutorial.condition0.count++;
			break;
		case TUTORIAL_PROGRESS_1:
			if(tutorial.shoudNext){
				//画像消す
				clearImageInGame(tutorial.images[tutorial.progress].index);
				//次へ
				if(tutorial.count - tutorial.nextTiming >= TRANS_TIME){
					tutorial.progress = (Progress)(tutorial.progress + 1);
					tutorial.transTiming = tutorial.count + 1;
					tutorial.shoudNext = false;
				}
			} else{
				//画像出す
				if(tutorial.count == tutorial.transTiming){
					dispImageInGame(tutorial.images[tutorial.progress].index);
				}
				//ここで条件チェック
				{
					//城へ行く
					//1P城まで移動判定
					if(getPlayer1X() >= initPosCastle.x - 100){
						tutorial.condition1.pl1Move = true;
					}
					//2P城まで移動判定
					if(getPlayer2X() >= initPosCastle.x - 100){
						tutorial.condition1.pl2Move = true;
					}
				}
				tutorial.clearNnextCondition =
					tutorial.condition1.pl1Move && tutorial.condition1.pl2Move;
				if(tutorial.clearNnextCondition){
					//条件を満たしたら、タイミングを記録し、フラグを切り替える
					tutorial.nextTiming = tutorial.count + 1;
					tutorial.clearNnextCondition = false;
					tutorial.shoudNext = true;
				}
			}
			break;
		case TUTORIAL_PROGRESS_2:
			if(tutorial.shoudNext){
				//画像消す
				clearImageInGame(tutorial.images[tutorial.progress].index);
				//次へ
				if(tutorial.count - tutorial.nextTiming >= TRANS_TIME){
					tutorial.progress = (Progress)(tutorial.progress + 1);
					tutorial.transTiming = tutorial.count + 1;
					tutorial.shoudNext = false;
				}
			} else{
				//画像出す
				if(tutorial.count == tutorial.transTiming){
					dispImageInGame(tutorial.images[tutorial.progress].index);
				}
				//ここで条件チェック
				//ジャンプ
				{
					double moveX = 0.0, moveY = 0.0;
					//1Pジャンプ判定
					getPlayer1FactMove(&moveX, &moveY);
					if(moveY < -1){
						tutorial.condition2.pl1Jump = true;
					}
					//2Pジャンプ判定
					getPlayer2FactMove(&moveX, &moveY);
					if(moveY < -1){
						tutorial.condition2.pl2Jump = true;
					}
				}
				tutorial.clearNnextCondition =
					tutorial.condition2.pl1Jump && tutorial.condition2.pl2Jump;
				if(tutorial.clearNnextCondition){
					//条件を満たしたら、タイミングを記録し、フラグを切り替える
					tutorial.nextTiming = tutorial.count + 1;
					tutorial.clearNnextCondition = false;
					tutorial.shoudNext = true;
				}
			}
			break;
		case TUTORIAL_PROGRESS_3:
			if(tutorial.shoudNext){
				//画像消す
				clearImageInGame(tutorial.images[tutorial.progress].index);
				//次へ
				if(tutorial.count - tutorial.nextTiming >= TRANS_TIME){
					tutorial.progress = (Progress)(tutorial.progress + 1);
					tutorial.transTiming = tutorial.count + 1;
					tutorial.shoudNext = false;
				}
			} else{
				//画像出す
				if(tutorial.count == tutorial.transTiming){
					dispImageInGame(tutorial.images[tutorial.progress].index);
				}
				//ここで条件チェック
				tutorial.clearNnextCondition = false;
				{
					if(tutorial.condition3.count == 0){
						//敵だす
						setEnemy(ENEMY_GOBLIN, 300, 400);
					}
					if(tutorial.condition3.count == 300){
						//敵だす
						setEnemy(ENEMY_GOBLIN, 300, 400);
					}
					tutorial.condition3.count++;
					//倒したか判定
					if(getEnemyKillCount() >= 2){
						tutorial.clearNnextCondition = true;
					}
				}
				if(tutorial.clearNnextCondition){
					//条件を満たしたら、タイミングを記録し、フラグを切り替える
					tutorial.nextTiming = tutorial.count + 1;
					tutorial.clearNnextCondition = false;
					tutorial.shoudNext = true;
				}
			}
			break;
		case TUTORIAL_PROGRESS_4:
			if(tutorial.shoudNext){
				//画像消す
				clearImageInGame(tutorial.images[tutorial.progress].index);
				//次へ
				if(tutorial.count - tutorial.nextTiming >= TRANS_TIME){
					tutorial.progress = (Progress)(tutorial.progress + 1);
					tutorial.transTiming = tutorial.count + 1;
					tutorial.shoudNext = false;
				}
			} else{
				//画像出す
				if(tutorial.count == tutorial.transTiming){
					dispImageInGame(tutorial.images[tutorial.progress].index);
				}
				//ここで条件チェック
				tutorial.clearNnextCondition = false;
				{
					if(tutorial.condition4.alreadyKillPl2 == false){
						tutorial.condition4.alreadyKillPl2 = true;
						//2P殺す
						//この殺しは改良する必要あり(雷の高さが合わない)
						set2PKiller();
					}
					//復活したか判定
					if(isPlayer2Resurrection()){
						tutorial.clearNnextCondition = true;
					}
				}
				if(tutorial.clearNnextCondition){
					//条件を満たしたら、タイミングを記録し、フラグを切り替える
					tutorial.nextTiming = tutorial.count + 1;
					tutorial.clearNnextCondition = false;
					tutorial.shoudNext = true;
				}
			}
			break;
		case TUTORIAL_PROGRESS_5:
			if(tutorial.shoudNext){
				//画像消す
				clearImageInGame(tutorial.images[tutorial.progress].index);
				//次へ
				if(tutorial.count - tutorial.nextTiming >= TRANS_TIME){
					tutorial.progress = (Progress)(tutorial.progress + 1);
					tutorial.transTiming = tutorial.count + 1;
					tutorial.shoudNext = false;
				}
			} else{
				//画像出す
				if(tutorial.count == tutorial.transTiming){
					dispImageInGame(tutorial.images[tutorial.progress].index);
				}
				//ここで条件チェック
				tutorial.clearNnextCondition = tutorial.condition5.count >= TUTORIAL_5_TIME;
				if(tutorial.clearNnextCondition){
					//条件を満たしたら、タイミングを記録し、フラグを切り替える
					tutorial.nextTiming = tutorial.count + 1;
					tutorial.clearNnextCondition = false;
					tutorial.shoudNext = true;
				}
			}
			tutorial.condition5.count++;
			break;
		case TUTORIAL_PROGRESS_6:
			if(tutorial.shoudNext){
				//画像消す
				clearImageInGame(tutorial.images[tutorial.progress].index);
				//次へ
				if(tutorial.count - tutorial.nextTiming >= TRANS_TIME){
					tutorial.progress = (Progress)(tutorial.progress + 1);
					tutorial.transTiming = tutorial.count + 1;
					tutorial.shoudNext = false;
				}
			} else{
				//画像出す
				if(tutorial.count == tutorial.transTiming){
					dispImageInGame(tutorial.images[tutorial.progress].index);
				}
				//ここで条件チェック
				tutorial.clearNnextCondition = tutorial.condition6.count >= TUTORIAL_6_TIME;
				if(tutorial.clearNnextCondition){
					//条件を満たしたら、タイミングを記録し、フラグを切り替える
					tutorial.nextTiming = tutorial.count + 1;
					tutorial.clearNnextCondition = false;
					tutorial.shoudNext = true;
				}
			}
			tutorial.condition6.count++;
			break;
		case TUTORIAL_PROGRESS_7:
			if(tutorial.shoudNext){
				//画像消す
				clearImageInGame(tutorial.images[tutorial.progress].index);
				//次へ
				if(tutorial.count - tutorial.nextTiming >= TRANS_TIME){
					tutorial.progress = (Progress)(tutorial.progress + 1);
					tutorial.transTiming = tutorial.count + 1;
					tutorial.shoudNext = false;
				}
			} else{
				//画像出す
				if(tutorial.count == tutorial.transTiming){
					dispImageInGame(tutorial.images[tutorial.progress].index);
				}
				//ここで条件チェック
				tutorial.clearNnextCondition = tutorial.condition7.count >= TUTORIAL_LAST_TIME;
				if(tutorial.clearNnextCondition){
					//条件を満たしたら、タイミングを記録し、フラグを切り替える
					tutorial.nextTiming = tutorial.count + 1;
					tutorial.clearNnextCondition = false;
					tutorial.shoudNext = true;
				}
			}
			tutorial.condition7.count++;
			break;
		default:
			break;
	}
	tutorial.count++;
	if(tutorial.progress == TUTORIAL_PROGRESS_END){
		return -1;
	} else{
		return 0;
	}
}


