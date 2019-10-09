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




//���X�g�̗v�f���Ȃ���
int pointListConnect(PointList *node0, PointList *node1);

//���X�g�̖����𓾂�
PointList *pointListGetTail(PointList *head);

//���X�g�̎w��ʒu�̌��ɑ}��
PointList *pointListInsert(PointList *node);

//���X�g�̗v�f���폜
int pointListDelete(PointList **head, PointList *target);

//���X�g�̑S�v�f���X�V
int pointUpdateList(PointList **head);

//���X�g�̑S�v�f��`��
int pointDrawList(PointList *head);

