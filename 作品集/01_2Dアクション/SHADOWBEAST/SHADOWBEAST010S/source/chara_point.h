#pragma once

struct pointVec{
	double x, y;
};

struct pointRGB{
	int red, green, blue;
};

struct pointDrawInfo{
	pointVec pos;
	int alpha;
	pointRGB bright;
};

struct Point{
	bool isDead;
	int elapsed;
	pointVec pos;
	pointVec velocity;
	int alpha;
	pointRGB color;
	pointDrawInfo draw;
};

int pointInit();

int pointUpdate(Point *point);

int pointDraw(const pointDrawInfo *const draw);

int toRGB(int colorCode32bit, pointRGB *argb);


struct PointList{
	PointList *next;
	PointList *prev;
	Point data;
};




//リストの要素をつなげる
int pointListConnect(PointList *node0, PointList *node1);

//リストの末尾を得る
PointList *pointListGetTail(PointList *head);

//リストの指定位置の後ろに挿入
PointList *pointListInsert(PointList *node);

//リストの要素を削除
int pointListDelete(PointList **head, PointList *target);

//リストの全要素を更新
int pointUpdateList(PointList **head);

//リストの全要素を描画
int pointDrawList(PointList *head);

