

static int stageClearTimeKeeper = 0;
static int stageElapsedTimeCounter = 0;

/*
ゲーム開始時に一回だけ呼び出す
*/
void stageTimeStart(){
	stageElapsedTimeCounter =
		stageClearTimeKeeper = 0;
}

/*
ゲームプレイ中、ポーズ画面以外で毎フレーム呼び出す
*/
void stageTimeUpdate(){
	stageElapsedTimeCounter++;
}

/*
ゲームクリアの瞬間に呼び出す
*/
void stageTimeStageClear(){
	stageClearTimeKeeper = stageElapsedTimeCounter;
}

/*
ゲームの経過時間を得る
*/
int stageElapsedTime(){
	return stageElapsedTimeCounter;
}

/*
ゲームのクリア時間を得る
*/
int stageClearTime(){
	return stageClearTimeKeeper;
}

