

static int stageClearTimeKeeper = 0;
static int stageElapsedTimeCounter = 0;

/*
�Q�[���J�n���Ɉ�񂾂��Ăяo��
*/
void stageTimeStart(){
	stageElapsedTimeCounter =
		stageClearTimeKeeper = 0;
}

/*
�Q�[���v���C���A�|�[�Y��ʈȊO�Ŗ��t���[���Ăяo��
*/
void stageTimeUpdate(){
	stageElapsedTimeCounter++;
}

/*
�Q�[���N���A�̏u�ԂɌĂяo��
*/
void stageTimeStageClear(){
	stageClearTimeKeeper = stageElapsedTimeCounter;
}

/*
�Q�[���̌o�ߎ��Ԃ𓾂�
*/
int stageElapsedTime(){
	return stageElapsedTimeCounter;
}

/*
�Q�[���̃N���A���Ԃ𓾂�
*/
int stageClearTime(){
	return stageClearTimeKeeper;
}

