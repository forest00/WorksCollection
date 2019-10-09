#pragma once
#include "primitive/vector2d.h"
#include "primitive/rect.h"

//一行の限界
#define LooksLineMax	256
//ファイルの数
#define LooksFileNum	3
//ファイル一覧↓
#define LooksFilename_Player	"res/looks/player.txt"
#define LooksFilename_Enemy		"res/looks/enemy.txt"
#define LooksFilename_Bullet	"res/looks/bullet.txt"	
//ファイル一覧↑

enum LooksID
{
	LooksID_None,
	LooksID_Player,
	LooksID_Enemy,
	LooksID_Bullet,
	LooksID_Max,
};

struct Looks
{
	char **body;
	int *length;
	int max_length;
	int lines;
};

//初期化
extern void InitLooks();
//終了
extern void EndLooks();
//取得
extern const Looks *GetLooks(LooksID id);
//矩形計算
extern Rect CalcLooksRect(const Looks *looks, Vector2D pos);
//スクリーンに描画
extern void DrawLooks(const Looks *looks, Vector2D pos);
