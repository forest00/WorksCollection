
#include <stdio.h>
#include <string.h>
#include "stage_data_reader.h"
#include "stage_data_reader_assistant.h"
#include "chara_castle.h"
#include "chara_block.h"
#include "chara_enemy.h"
#include "game_state.h"
#include "chara_item.h"
#include "tree_sign.h"

#define READER_FILENAME "_data\\stage_data\\stage%d_data.txt"

//�E�F�[�u�̃m�[�h�̏��
#define READER_WAVE_NODE_CAPA 32
//�E�F�[�u�̓o�^���̏��
#define READER_WAVE_CAPA 32
//�X�e�[�W�̃X�N���v�g�f�[�^�̏��
#define READER_SCRIPT_CAPA 512
//�e�R�}���h�̖��O�̒����̏��
#define READER_CMD_NAME_MAX 16
//�G�̖��O�̒����̏��
#define READER_ENEMY_NAME_MAX 16
//����������̕������̏��
#define READER_ARG_CHARS_MAX 64
//���x���̌��̏��
#define READER_LABEL_CAPA 32

//extern�ϐ���
XYi initPosPlayer1 = { 0 };
XYi initPosPlayer2 = { 0 };
XYi initPosCastle = { 0 };
int WORLD_WIDTH = 0;
int WORLD_HEIGHT = 0;
int WORLD_CENTER_X = 0;
int WORLD_CENTER_Y = 0;
int GROUND_Y = 0;
//extern�ϐ���

typedef struct
{
	char name[READER_ARG_CHARS_MAX];	//���O
	int index;							//�Ή�����C���f�b�N�X
}Label;
int labelCount = 0;						//���x���̐�
Label labelDic[READER_LABEL_CAPA];		//���x����o�^���Ƃ��̈�
int  labelToLine(char *labelName)
{
	for (int i = 0; i < labelCount; i++)
	{
		if (!strcmp(labelDic[i].name, labelName))
		{
			return labelDic[i].index;
		}
	}
	return -1;
}

typedef struct
{
	int elapsed;		//�S�̂̌o�ߎ���
	int tmpCounter;		//�ʂ̃J�E���g
	int index;			//�i�݋
	int treeSignIndex;	//�Ŕ̐�
}Progress;
static Progress scriptProgress = { 0 };

struct WaveNode
{
	int delay;
	EnemyType enemyType;
	XYi pos;
};
struct Wave
{
	WaveNode node[READER_WAVE_NODE_CAPA];
	int amount;
};
static Wave waveTable[READER_WAVE_CAPA];

static const char enemyTypeName[ENEMY_TYPE_MAX][READER_ENEMY_NAME_MAX] = {
	"none",
	"goblin",
	"wyvern",
	"snake",
	"titan",
	"sea_snake",
};

enum ScriptCommand
{
	CMD_NONE,
	CMD_WORLD,
	CMD_PLAYER1,
	CMD_PLAYER2,
	CMD_CASTLE,
	CMD_BLOCK,
	CMD_RESISTER,
	CMD_APPEAR,
	CMD_ENEMY,
	CMD_WAIT_TIME,
	CMD_WAIT_ENEMY,
	CMD_IF,
	CMD_ELSE,
	CMD_ENDIF,
	CMD_WHILE,
	CMD_ENDWHILE,
	CMD_LABEL,
	CMD_JUMP,
	CMD_ITEM,
	CMD_FOG,
	CMD_DISPEL_FOG,
	CMD_LOAD_IMAGE,
	CMD_DRAW_IMAGE,
	CMD_CLEAR_IMAGE,
	CMD_MOVE_IMAGE,
	CMD_SET_TREE_SIGN,
	CMD_WAIT_TREE_SIGN,
	CMD_END,
	CMD_MAX,
};
typedef struct
{
	char cmdName[READER_CMD_NAME_MAX];		//�R�}���h��
	bool isLoop;							//���[�v����ׂ����ǂ���
}Command;
static const Command command[CMD_MAX] = {
	"none"			,false	,
	"world"			,false	,
	"1p"			,false	,
	"2p"			,false	,
	"object"		,false	,
	"block"			,false	,
	"register"		,false	,
	"appear"		,false	,
	"enemy"			,true	,
	"wait_time"		,false	,
	"wait_enemy"	,false	,
	"if"			,true	,
	"else"			,true	,
	"endif"			,true	,
	"while"			,true	,
	"endwhile"		,false	,
	"label"			,true	,
	"jump"			,false	,
	"item"			,false	,
	"fog"			,false	,
	"dispel_fog"	,false	,
	"load_image"	,true	,
	"draw_image"	,true	,
	"clear_image"	,true	,
	"move_image"	,true	,
	"sign"			,false	,
	"wait_sign"		,false	,
	"end"			,false	,
};

enum ScriptIfConditions
{
	IF_CONDITIONS_NONE,
	IF_ENEMY_KILL_COUNT,
	IF_ENEMY_EXIST,
	IF_TIME_ELAPSED,
	IF_TIME_NOT_ELAPSED,
	IF_CONDITIONS_MAX,
};
static const char ifConditions[IF_CONDITIONS_MAX][READER_CMD_NAME_MAX] = {
	"none",
	"enemy_killed",
	"enemy_exist",
	"elapsed",
	"not_elapsed",
};

struct Script
{
	ScriptCommand cmd;						//�ǂ̃R�}���h��
	ScriptIfConditions ifConditions;		//�ǂ̏�����(if���̎��̂�)
	int scopeLevel;							//�X�R�[�v���x���͂ǂ̂��炢��
	int param1;								//����	����1
	int param2;								//����	����2
	int param3;								//����	����3
	EnemyType enemyType;					//����	�G�l�~�[�^�C�v
	char paramS[READER_ARG_CHARS_MAX];		//����	������
};
static Script scriptTable[READER_SCRIPT_CAPA];

/*
if���̏����`�F�b�N
�����𖞂����Ă����true��Ԃ�
*/
static bool conditionCheck(ScriptIfConditions conditions, int threshold)
{
	switch (conditions)
	{
	case IF_CONDITIONS_NONE:
		break;
	case IF_ENEMY_KILL_COUNT:
	{
		int count = getEnemyKillCount();
		return count >= (threshold);
	}
	break;
	case IF_ENEMY_EXIST:
	{
		int enemies = checkActiveEnemy();
		return enemies >= threshold;
	}
	break;
	case IF_TIME_ELAPSED:
	{
		int count = scriptProgress.elapsed;
		return count >= threshold;
	}
	break;
	case IF_TIME_NOT_ELAPSED:
	{
		int count = scriptProgress.elapsed;
		return count < threshold;
	}
	break;
	default:
		break;
	}
	return false;
}
/*
str�̒�����delim��T���A
������΃k�������ɒu�������A���̈��̕����ւ̃|�C���^��Ԃ�
������Ȃ���΁Anullptr��Ԃ�
*/
static char *my_strtok(char *str, char delim)
{
	int i = 0;
	for (i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == delim)
		{
			str[i] = '\0';
			return &str[i + 1];
		}
	}
	return nullptr;
}

static EnemyType enemyTypeJudge(char *str)
{
	for (int i = 0; i < ENEMY_TYPE_MAX; i++)
	{
		int diff = strcmp(str, enemyTypeName[i]);
		if (!diff)
		{
			return (EnemyType)(i);
		}
	}
	return ENEMY_NONE;
}

static ScriptIfConditions ifConditionsJudge(char *str, char **next)
{
	*next = my_strtok(str, ',');
	for (int i = 0; i < CMD_MAX; i++)
	{
		int diff = strcmp(str, ifConditions[i]);
		if (!diff)
		{
			return (ScriptIfConditions)(i);
		}
	}
	return IF_CONDITIONS_NONE;
}

static ScriptCommand cmdJudge(char *str, char **next)
{
	*next = my_strtok(str, ',');
	for (int i = 0; i < CMD_MAX; i++)
	{
		int diff = strcmp(str, command[i].cmdName);
		if (!diff)
		{
			return (ScriptCommand)(i);
		}
	}
	return CMD_NONE;
}


//�t�@�C�����p�[�W���O
static int parse(char *str)
{
	int scopeLevel = 0;
	int index = 0;
	Script *sc = &scriptTable[index];
	char *line = nullptr;
	char *next = nullptr;
	char *context = nullptr;
	line = strtok_s(str, "\n", &context);
	do
	{
		line = strtok_s(nullptr, "\n", &context);
		ScriptCommand cmd = cmdJudge(line, &next);
		if (cmd == CMD_END)
		{
			sc->cmd = cmd;
			break;
		}
		sc->scopeLevel = scopeLevel;
		switch (cmd)
		{
		case CMD_NONE:
			break;
		case CMD_WORLD:
			line = strtok_s(nullptr, "\n", &context);
			sscanf_s(line, "%d,%d",
				&WORLD_WIDTH, &WORLD_HEIGHT);
			WORLD_CENTER_X = WORLD_WIDTH / 2;
			WORLD_CENTER_Y = WORLD_HEIGHT / 2;
			break;
		case CMD_PLAYER1:
			line = strtok_s(nullptr, "\n", &context);
			sscanf_s(line, "%d,%d", &initPosPlayer1.x, &initPosPlayer1.y);
			break;
		case CMD_PLAYER2:
			line = strtok_s(nullptr, "\n", &context);
			sscanf_s(line, "%d,%d", &initPosPlayer2.x, &initPosPlayer2.y);
			break;
		case CMD_CASTLE:
			line = strtok_s(nullptr, "\n", &context);
			sscanf_s(line, "%d,%d", &initPosCastle.x, &initPosCastle.y);
			setProtectTarget(initPosCastle.x, initPosCastle.y);
			break;
		case CMD_BLOCK:
			//�u���b�N���o���܂���
		{
			int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
			int enemyType = 0;
			do
			{
				line = strtok_s(nullptr, "\n", &context);
				sscanf_s(line, "%d,%d,%d,%d,%d",
					&x0, &y0, &x1, &y1, &enemyType);
				//�����ŃK�[�h
				if (enemyType < 0)
				{
					break;
				}
				//�K�[�h�����܂�
				if (x0 > x1)
				{
					swap(&x0, &x1);
				}
				if (y0 > y1)
				{
					swap(&y0, &y1);
				}
				GROUND_Y = max(GROUND_Y, y0);
				int w = x1 - x0;
				int h = y1 - y0;
				switch (enemyType)
				{
				case 0:
					setBlockU(x0, y0, w, h);
					break;
				case 1:
					setBlock(x0, y0, w, h);
					break;
				default:
					break;
				}
			} while (true);
		}
		break;
		case CMD_RESISTER:
			//�G�̃E�F�[�u����o�^���܂���
		{
			//�������`�F�b�N
			sscanf_s(next, "%d", &(sc->param1));
			int delay = 0, x = 0, y = 0;
			char name[READER_ENEMY_NAME_MAX];
			unsigned int loop = 0;
			do
			{
				line = strtok_s(nullptr, "\n", &context);
				sscanf_s(line, "%d,%d,%d,%s",
					&delay, &x, &y, name, (unsigned int)'\n');
				//�����ŃK�[�h
				if (delay < 0)
				{
					break;
				}
				//�K�[�h�����܂�
				EnemyType _type = enemyTypeJudge(name);
				WaveNode *node = &waveTable[sc->param1].node[loop];
				node->delay = delay;
				node->enemyType = _type;
				node->pos = { x,y };
				loop++;
			} while (true);
			waveTable[sc->param1].amount = loop;
		}
		break;
		case CMD_APPEAR:
			//�������`�F�b�N
			sc->cmd = cmd;
			sscanf_s(next, "%d", &sc->param1);
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_ENEMY:
		{
			char name[READER_ENEMY_NAME_MAX];
			//�������`�F�b�N
			sc->cmd = cmd;
			sscanf_s(next, "%d,%d,%s",
				&(sc->param1), &(sc->param2), name, (unsigned int)'\n');
			sc->enemyType = enemyTypeJudge(name);
			index++;
			sc = &scriptTable[index];
			break;
		}
		case CMD_WAIT_TIME:
			//�������`�F�b�N
			sc->cmd = cmd;
			sscanf_s(next, "%d", &sc->param1);
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_WAIT_ENEMY:
			//�������`�F�b�N
			sc->cmd = cmd;
			sscanf_s(next, "%d", &sc->param1);
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_IF:
		case CMD_WHILE:
			//�X�R�[�v���x���������̂ŏC��
			sc->scopeLevel++;
			//�X�R�[�v���x�����オ��
			scopeLevel++;
			//�������`�F�b�N
			sc->cmd = cmd;
			{
				ScriptIfConditions condition = ifConditionsJudge(next, &next);
				sc->ifConditions = condition;
				sscanf_s(next, "%d", &(sc->param1));
			}
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_ELSE:
			sc->cmd = cmd;
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_ENDIF:
		case CMD_ENDWHILE:
			//�X�R�[�v���x����������
			scopeLevel--;
			sc->cmd = cmd;
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_LABEL:
			//���x�����ƌ��݈ʒu���o���Ă���
			strncpy_s(labelDic[labelCount].name, READER_ARG_CHARS_MAX,
				next, READER_ARG_CHARS_MAX);
			labelDic[labelCount].index = index;
			labelCount++;
			break;
		case CMD_JUMP:
			//���x�������o���Ă���
			strncpy_s(sc->paramS, READER_ARG_CHARS_MAX,
				next, READER_ARG_CHARS_MAX);
			sc->cmd = cmd;
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_ITEM:
			sc->cmd = cmd;
			sscanf_s(next, "%d,%d",
				&(sc->param1), &(sc->param2));
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_FOG:
		case CMD_DISPEL_FOG:
			//�R�}���h�݂̂��o���Ă���
			sc->cmd = cmd;
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_LOAD_IMAGE:
			//�������`�F�b�N
		{
			char *c = my_strtok(next, ',');
			sc->param1 = atoi(next);
			strncpy_s(sc->paramS, READER_ARG_CHARS_MAX,
				c, READER_ARG_CHARS_MAX);
			loadImageByData(sc->param1, (char*)(sc->paramS));
		}
		break;
		case CMD_DRAW_IMAGE:
		case CMD_CLEAR_IMAGE:
			//�������`�F�b�N
			sc->cmd = cmd;
			sscanf_s(next, "%d", &sc->param1);
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_MOVE_IMAGE:
			//�������`�F�b�N
			sc->cmd = cmd;
			sscanf_s(next, "%d,%d,%d",
				&sc->param1, &sc->param2, &sc->param3);
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_SET_TREE_SIGN:
			//�R�}���h�ƈ������o���Ă���
			sc->cmd = cmd;
			sscanf_s(next, "%d,%d,%d",
				&sc->param1, &sc->param2, &sc->param3);
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_WAIT_TREE_SIGN:
			//�R�}���h�݂̂��o���Ă���
			sc->cmd = cmd;
			index++;
			sc = &scriptTable[index];
			break;
		default:
			break;
		}
	} while (true);
	return 0;
}

/*
���낢�돉��������
��������Ȃ���
���̊֐��͎g���Ȃ�
*/
int initStageData()
{
	///���x���̌��̓[����
	labelCount = 0;
	///�X�N���v�g�p�ϐ��̏�����
	scriptProgress = { 0 };
	///�v���C���[���W��������
	initPosPlayer1 = { 0 };
	initPosPlayer2 = { 0 };
	///����̈ʒu��������
	initPosCastle = { 0 };
	///���[���h�T�C�Y�̏�����
	WORLD_WIDTH = 0;
	WORLD_HEIGHT = 0;
	WORLD_CENTER_X = 0;
	WORLD_CENTER_Y = 0;
	///��ԉ��̃u���b�N�̈ʒu�����擾
	GROUND_Y = INT_MIN;
	///�E�F�[�u�̕���������
	Wave waveZero = { 0 };
	for (unsigned int i = 0; i < READER_WAVE_CAPA; i++)
	{
		waveTable[i] = { waveZero };
	}
	///�X�N���v�g�̕���������
	Script scriptZero = { CMD_NONE };
	for (unsigned int i = 0; i < READER_SCRIPT_CAPA; i++)
	{
		scriptTable[i] = { scriptZero };
	}
	return 0;
}
/*
�t�@�C������X�e�[�W�f�[�^��ǂݍ���
���łɃu���b�N���o��
*/
int loadFromFileStageData(int stage)
{
	///�t�@�C�����J��
	FILE *in = nullptr;
	char fileName[FILENAME_MAX] = { "\0" };
	sprintf_s(fileName, READER_FILENAME, stage);
	int error = fopen_s(&in, fileName, "rb");
	if (error)
	{
		///�t�@�C�����J���Ȃ������B�����I��
		exit(0);
	}
	else
	{
		///�J�����t�@�C���̒��g��ǂݍ���
		fseek(in, 0, SEEK_END);

		unsigned int fileLength = ftell(in);

		fseek(in, 0, SEEK_SET);

		char *original = nullptr;
		char *corrected = nullptr;
		unsigned int memSize = sizeof(char) * fileLength;
		///�������m��
		original = (char*)malloc(memSize);
		corrected = (char*)malloc(memSize);
		if (original == nullptr || corrected == nullptr)
		{
			exit(0);
		}
		///�t�@�C���ǂݍ���
		fread(original, fileLength, 1, in);
		///�ꕶ���Â`�F�b�N�A�󔒂ƃ^�u��(\r)������
		{
			unsigned int i = 0;
			unsigned int k = 0;
			char *removalTarget = " \t\r";
			for (i = 0; i < fileLength; i++)
			{
				char c = original[i];
				char *p = strchr(removalTarget, c);
				if (p != nullptr)
				{
					continue;
				}
				corrected[k] = c;
				k++;
			}
		}
		free(original);
		///�p�[�W���O����
		parse(corrected);
		free(corrected);
		fclose(in);
	}
	return 0;
}
/*
�ǂݍ��񂾃f�[�^�𔽉f������
*/
int reflectStageData()
{
	const Script *sc = nullptr;
	do
	{
		sc = &scriptTable[scriptProgress.index];
		switch (sc->cmd)
		{
		case CMD_NONE:
			break;
		case CMD_APPEAR:
		{
			//�G���o��
			Wave *w = &waveTable[sc->param1];
			int doneCount = 0;
			for (int i = 0; i < w->amount; i++)
			{
				WaveNode *node = &w->node[i];
				if (scriptProgress.tmpCounter >= node->delay)
				{
					doneCount++;
					if (scriptProgress.tmpCounter == node->delay)
					{
						setEnemy(node->enemyType, node->pos.x, node->pos.y);
					}
				}
			}
			//�o���؂��Ă���Ύ���
			if (doneCount >= w->amount)
			{
				scriptProgress.index++;
				scriptProgress.tmpCounter = -1;
			}
		}
		break;
		case CMD_ENEMY:
			//�G���o���Ď���
			setEnemy(sc->enemyType, sc->param1, sc->param2);
			scriptProgress.index++;
			scriptProgress.tmpCounter = -1;
			break;
		case CMD_WAIT_TIME:
			//�J�E���g�A�b�v���I���̂�҂�
			if (scriptProgress.tmpCounter >= (sc->param1))
			{
				scriptProgress.index++;
				scriptProgress.tmpCounter = -1;
			}
			break;
		case CMD_WAIT_ENEMY:
			//�G�̐��������ȉ��Ȃ玟��
			if (checkActiveEnemy() <= (sc->param1))
			{
				scriptProgress.index++;
				scriptProgress.tmpCounter = -1;
			}
			break;
		case CMD_IF:
		case CMD_WHILE:
		{
			//�����`�F�b�N
			bool isReach = conditionCheck(sc->ifConditions, sc->param1);
			if (!isReach)
			{
				//�����𖞂����Ă��Ȃ�
				//else,endif,endwhile��T��(�X�R�[�v���x�����������̂�)
				Script *_sc = nullptr;
				bool isSameLevel = false;
				bool isTargetCmd = false;
				do
				{
					scriptProgress.index++;
					_sc = &scriptTable[scriptProgress.index];
					isSameLevel = ((sc->scopeLevel) == (_sc->scopeLevel));
					ScriptCommand c = _sc->cmd;
					isTargetCmd = (c == CMD_ELSE ||
						c == CMD_ENDIF ||
						c == CMD_ENDWHILE);
				} while (!(isSameLevel && isTargetCmd));
			}
			//�����
			//�����𖞂����Ă��Ă��A���Ȃ��Ă��i�ޕK�v������
			scriptProgress.index++;
			scriptProgress.tmpCounter = -1;
		}
		break;
		case CMD_ELSE:
			//endif��T��(�X�R�[�v���x�����������̂�)
		{
			Script *_sc = nullptr;
			do
			{
				scriptProgress.index++;
				_sc = &scriptTable[scriptProgress.index];
				if (sc->scopeLevel == _sc->scopeLevel)
				{
					continue;
				}
			} while (_sc->cmd != CMD_ENDIF);
			scriptProgress.index++;
			scriptProgress.tmpCounter = -1;
		}
		break;
		case CMD_ENDIF:
			//�Ƃ肠���������
			scriptProgress.index++;
			scriptProgress.tmpCounter = -1;
			break;
		case CMD_ENDWHILE:
			//while��T��(�X�R�[�v���x�����������̂�)
		{
			Script *_sc = nullptr;
			do
			{
				scriptProgress.index--;
				_sc = &scriptTable[scriptProgress.index];
				if (sc->scopeLevel == _sc->scopeLevel)
				{
					continue;
				}
			} while (_sc->cmd != CMD_WHILE);
			scriptProgress.tmpCounter = -1;
		}
		break;
		case CMD_LABEL:
			scriptProgress.index++;
			scriptProgress.tmpCounter = -1;
			break;
		case CMD_JUMP:
		{
			//���x���W�����v
			int labelLine = labelToLine((char*)(sc->paramS));
			//���x���͑��݂��Ă��邩
			if (labelLine >= 0)
			{
				//�Y���ӏ��֍s��
				scriptProgress.index = labelLine;
			}
			else
			{
				//����֍s��
				scriptProgress.index++;
			}
			scriptProgress.tmpCounter = -1;
		}
		break;
		case CMD_ITEM:
			//�A�C�e�����o��
		{
			XYd v = { 0,0 };
			double theta = toRadian((double)(sc->param1));
			double power = (double)(sc->param2) / 10.0;
			v.x = cos(theta) * power;
			v.y = sin(theta) * power;
			setItemOnCastle(v.x, v.y);
		}
		scriptProgress.index++;
		scriptProgress.tmpCounter = -1;
		break;
		case CMD_FOG:
			//�����o��
			fogSet();
			scriptProgress.index++;
			scriptProgress.tmpCounter = -1;
			break;
		case CMD_DISPEL_FOG:
			//��������
			fogDispel();
			scriptProgress.index++;
			scriptProgress.tmpCounter = -1;
			break;
		case CMD_DRAW_IMAGE:
			dispImageInGame(sc->param1);
			scriptProgress.index++;
			scriptProgress.tmpCounter = -1;
			break;
		case CMD_CLEAR_IMAGE:
			clearImageInGame(sc->param1);
			scriptProgress.index++;
			scriptProgress.tmpCounter = -1;
			break;
		case CMD_MOVE_IMAGE:
			moveImageInGame(sc->param1, sc->param2, sc->param3);
			scriptProgress.index++;
			scriptProgress.tmpCounter = -1;
			break;
		case CMD_SET_TREE_SIGN:
			setTreeSign(scriptProgress.treeSignIndex, sc->param1, sc->param2, sc->param3);
			scriptProgress.treeSignIndex++;
			scriptProgress.index++;
			scriptProgress.tmpCounter = -1;
			break;
		case CMD_WAIT_TREE_SIGN:
			//���ׂĂ̊Ŕ���x�ł��ǂ܂�Ă���Ύ���
			if (countSign() == countSignHasRead())
			{
				scriptProgress.index++;
				scriptProgress.tmpCounter = -1;
			}
			break;
		case CMD_END:
			intoGameClear();
			break;
		default:
			break;
		}
	} while (command[sc->cmd].isLoop);
	scriptProgress.tmpCounter++;
	scriptProgress.elapsed++;
	return 0;
}