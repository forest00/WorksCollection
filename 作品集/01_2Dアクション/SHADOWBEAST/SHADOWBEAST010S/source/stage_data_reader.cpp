
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

//ウェーブのノードの上限
#define READER_WAVE_NODE_CAPA 32
//ウェーブの登録数の上限
#define READER_WAVE_CAPA 32
//ステージのスクリプトデータの上限
#define READER_SCRIPT_CAPA 512
//各コマンドの名前の長さの上限
#define READER_CMD_NAME_MAX 16
//敵の名前の長さの上限
#define READER_ENEMY_NAME_MAX 16
//引数文字列の文字数の上限
#define READER_ARG_CHARS_MAX 64
//ラベルの個数の上限
#define READER_LABEL_CAPA 32

//extern変数↓
XYi initPosPlayer1 = { 0 };
XYi initPosPlayer2 = { 0 };
XYi initPosCastle = { 0 };
int WORLD_WIDTH = 0;
int WORLD_HEIGHT = 0;
int WORLD_CENTER_X = 0;
int WORLD_CENTER_Y = 0;
int GROUND_Y = 0;
//extern変数↑

typedef struct
{
	char name[READER_ARG_CHARS_MAX];	//名前
	int index;							//対応するインデックス
}Label;
int labelCount = 0;						//ラベルの数
Label labelDic[READER_LABEL_CAPA];		//ラベルを登録しとく領域
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
	int elapsed;		//全体の経過時間
	int tmpCounter;		//個別のカウント
	int index;			//進み具合
	int treeSignIndex;	//看板の数
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
	char cmdName[READER_CMD_NAME_MAX];		//コマンド名
	bool isLoop;							//ループするべきかどうか
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
	ScriptCommand cmd;						//どのコマンドか
	ScriptIfConditions ifConditions;		//どの条件か(if文の時のみ)
	int scopeLevel;							//スコープレベルはどのくらいか
	int param1;								//引数	整数1
	int param2;								//引数	整数2
	int param3;								//引数	整数3
	EnemyType enemyType;					//引数	エネミータイプ
	char paramS[READER_ARG_CHARS_MAX];		//引数	文字列
};
static Script scriptTable[READER_SCRIPT_CAPA];

/*
if文の条件チェック
条件を満たしていればtrueを返す
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
strの中からdelimを探し、
見つかればヌル文字に置き換え、その一つ先の文字へのポインタを返す
見つからなければ、nullptrを返す
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


//ファイルをパージング
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
			//ブロックを出しまくる
		{
			int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
			int enemyType = 0;
			do
			{
				line = strtok_s(nullptr, "\n", &context);
				sscanf_s(line, "%d,%d,%d,%d,%d",
					&x0, &y0, &x1, &y1, &enemyType);
				//ここでガード
				if (enemyType < 0)
				{
					break;
				}
				//ガードここまで
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
			//敵のウェーブ情報を登録しまくる
		{
			//引数をチェック
			sscanf_s(next, "%d", &(sc->param1));
			int delay = 0, x = 0, y = 0;
			char name[READER_ENEMY_NAME_MAX];
			unsigned int loop = 0;
			do
			{
				line = strtok_s(nullptr, "\n", &context);
				sscanf_s(line, "%d,%d,%d,%s",
					&delay, &x, &y, name, (unsigned int)'\n');
				//ここでガード
				if (delay < 0)
				{
					break;
				}
				//ガードここまで
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
			//引数をチェック
			sc->cmd = cmd;
			sscanf_s(next, "%d", &sc->param1);
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_ENEMY:
		{
			char name[READER_ENEMY_NAME_MAX];
			//引数をチェック
			sc->cmd = cmd;
			sscanf_s(next, "%d,%d,%s",
				&(sc->param1), &(sc->param2), name, (unsigned int)'\n');
			sc->enemyType = enemyTypeJudge(name);
			index++;
			sc = &scriptTable[index];
			break;
		}
		case CMD_WAIT_TIME:
			//引数をチェック
			sc->cmd = cmd;
			sscanf_s(next, "%d", &sc->param1);
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_WAIT_ENEMY:
			//引数をチェック
			sc->cmd = cmd;
			sscanf_s(next, "%d", &sc->param1);
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_IF:
		case CMD_WHILE:
			//スコープレベルがずれるので修正
			sc->scopeLevel++;
			//スコープレベルが上がる
			scopeLevel++;
			//引数をチェック
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
			//スコープレベルが下がる
			scopeLevel--;
			sc->cmd = cmd;
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_LABEL:
			//ラベル名と現在位置を覚えておく
			strncpy_s(labelDic[labelCount].name, READER_ARG_CHARS_MAX,
				next, READER_ARG_CHARS_MAX);
			labelDic[labelCount].index = index;
			labelCount++;
			break;
		case CMD_JUMP:
			//ラベル名を覚えておく
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
			//コマンドのみを覚えておく
			sc->cmd = cmd;
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_LOAD_IMAGE:
			//引数をチェック
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
			//引数をチェック
			sc->cmd = cmd;
			sscanf_s(next, "%d", &sc->param1);
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_MOVE_IMAGE:
			//引数をチェック
			sc->cmd = cmd;
			sscanf_s(next, "%d,%d,%d",
				&sc->param1, &sc->param2, &sc->param3);
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_SET_TREE_SIGN:
			//コマンドと引数を覚えておく
			sc->cmd = cmd;
			sscanf_s(next, "%d,%d,%d",
				&sc->param1, &sc->param2, &sc->param3);
			index++;
			sc = &scriptTable[index];
			break;
		case CMD_WAIT_TREE_SIGN:
			//コマンドのみを覚えておく
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
いろいろ初期化する
これをやらないと
他の関数は使えない
*/
int initStageData()
{
	///ラベルの個数はゼロ個
	labelCount = 0;
	///スクリプト用変数の初期化
	scriptProgress = { 0 };
	///プレイヤー座標を初期化
	initPosPlayer1 = { 0 };
	initPosPlayer2 = { 0 };
	///お城の位置を初期化
	initPosCastle = { 0 };
	///ワールドサイズの初期化
	WORLD_WIDTH = 0;
	WORLD_HEIGHT = 0;
	WORLD_CENTER_X = 0;
	WORLD_CENTER_Y = 0;
	///一番下のブロックの位置情報を取得
	GROUND_Y = INT_MIN;
	///ウェーブの方を初期化
	Wave waveZero = { 0 };
	for (unsigned int i = 0; i < READER_WAVE_CAPA; i++)
	{
		waveTable[i] = { waveZero };
	}
	///スクリプトの方を初期化
	Script scriptZero = { CMD_NONE };
	for (unsigned int i = 0; i < READER_SCRIPT_CAPA; i++)
	{
		scriptTable[i] = { scriptZero };
	}
	return 0;
}
/*
ファイルからステージデータを読み込む
ついでにブロックも出す
*/
int loadFromFileStageData(int stage)
{
	///ファイルを開く
	FILE *in = nullptr;
	char fileName[FILENAME_MAX] = { "\0" };
	sprintf_s(fileName, READER_FILENAME, stage);
	int error = fopen_s(&in, fileName, "rb");
	if (error)
	{
		///ファイルを開けなかった。強制終了
		exit(0);
	}
	else
	{
		///開いたファイルの中身を読み込む
		fseek(in, 0, SEEK_END);

		unsigned int fileLength = ftell(in);

		fseek(in, 0, SEEK_SET);

		char *original = nullptr;
		char *corrected = nullptr;
		unsigned int memSize = sizeof(char) * fileLength;
		///メモリ確保
		original = (char*)malloc(memSize);
		corrected = (char*)malloc(memSize);
		if (original == nullptr || corrected == nullptr)
		{
			exit(0);
		}
		///ファイル読み込み
		fread(original, fileLength, 1, in);
		///一文字づつチェック、空白とタブと(\r)を消す
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
		///パージングする
		parse(corrected);
		free(corrected);
		fclose(in);
	}
	return 0;
}
/*
読み込んだデータを反映させる
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
			//敵を出す
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
			//出し切っていれば次へ
			if (doneCount >= w->amount)
			{
				scriptProgress.index++;
				scriptProgress.tmpCounter = -1;
			}
		}
		break;
		case CMD_ENEMY:
			//敵を出して次へ
			setEnemy(sc->enemyType, sc->param1, sc->param2);
			scriptProgress.index++;
			scriptProgress.tmpCounter = -1;
			break;
		case CMD_WAIT_TIME:
			//カウントアップが終わるのを待つ
			if (scriptProgress.tmpCounter >= (sc->param1))
			{
				scriptProgress.index++;
				scriptProgress.tmpCounter = -1;
			}
			break;
		case CMD_WAIT_ENEMY:
			//敵の数が引数以下なら次へ
			if (checkActiveEnemy() <= (sc->param1))
			{
				scriptProgress.index++;
				scriptProgress.tmpCounter = -1;
			}
			break;
		case CMD_IF:
		case CMD_WHILE:
		{
			//条件チェック
			bool isReach = conditionCheck(sc->ifConditions, sc->param1);
			if (!isReach)
			{
				//条件を満たしていない
				//else,endif,endwhileを探す(スコープレベルが同じものを)
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
			//一個次へ
			//条件を満たしていても、いなくても進む必要がある
			scriptProgress.index++;
			scriptProgress.tmpCounter = -1;
		}
		break;
		case CMD_ELSE:
			//endifを探す(スコープレベルが同じものを)
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
			//とりあえず一個次へ
			scriptProgress.index++;
			scriptProgress.tmpCounter = -1;
			break;
		case CMD_ENDWHILE:
			//whileを探す(スコープレベルが同じものを)
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
			//ラベルジャンプ
			int labelLine = labelToLine((char*)(sc->paramS));
			//ラベルは存在しているか
			if (labelLine >= 0)
			{
				//該当箇所へ行く
				scriptProgress.index = labelLine;
			}
			else
			{
				//一つ次へ行く
				scriptProgress.index++;
			}
			scriptProgress.tmpCounter = -1;
		}
		break;
		case CMD_ITEM:
			//アイテムを出す
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
			//霧を出す
			fogSet();
			scriptProgress.index++;
			scriptProgress.tmpCounter = -1;
			break;
		case CMD_DISPEL_FOG:
			//霧を消す
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
			//すべての看板が一度でも読まれていれば次へ
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