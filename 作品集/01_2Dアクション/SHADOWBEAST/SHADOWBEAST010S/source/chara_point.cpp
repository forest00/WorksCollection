
#include <cassert>
#include "amgame.h"
#include "chara_point.h"
#include "camera.h"

static int iLerp(double x, double y, double t){
	return (int)(x + (y - x) * t);
}

static int hPoint = 0;

int pointInit(){
	hPoint = LoadTexture("image/effect/_point.png");
	if(hPoint < 0){
		return -1;
	}
	return 0;
}

int pointUpdate(Point *point){
	double t = (double)point->elapsed / 20.0;
	if(t > 1.0){
		t = 1.0;
	}
	point->pos.x += t * (point->velocity.x);
	point->pos.y += t * (point->velocity.y);
	point->velocity.x *= 0.975;
	point->velocity.y *= 0.975;
	point->velocity.y -= 0.2;
	point->alpha -= 3;
	//時間を進める
	point->elapsed++;
	//死亡判定
	if(point->alpha <= 0){
		point->isDead = true;
	}
	//描画用の計算↓
	pointDrawInfo *draw = &(point->draw);
	draw->pos = {point->pos.x - 1,point->pos.y - 1};
	correctByCamera(&draw->pos.x, &draw->pos.y);
	draw->alpha = point->alpha;
	draw->bright.red = iLerp(0, 1000, point->color.red / 255.0);
	draw->bright.green = iLerp(0, 1000, point->color.green / 255.0);
	draw->bright.blue = iLerp(0, 1000, point->color.blue / 255.0);
	return 0;
}

int pointDraw(const pointDrawInfo *const draw){
	SetDrawMode(AMDRAW_ALPHABLEND, draw->alpha);
	SetDrawBright(draw->bright.red, draw->bright.green, draw->bright.blue);
	DrawMemTh((int)(draw->pos.x), (int)(draw->pos.y), hPoint);
	return 0;
}

int toRGB(int colorCode32bit, pointRGB *rgb){
	if(rgb == nullptr){
		return -1;
	}
	rgb->red = (colorCode32bit >> 16) & 0xff;
	rgb->green = (colorCode32bit >> 8) & 0xff;
	rgb->blue = (colorCode32bit >> 0) & 0xff;
	return 0;
}

/*--------------------------------------*/




//リストの要素をつなげる
int pointListConnect(PointList *node0, PointList *node1){
	if(node0 != nullptr){
		node0->next = node1;
	}
	if(node1 != nullptr){
		node1->prev = node0;
	}
	return 0;
}

//リストの末尾を得る
PointList *pointListGetTail(PointList *head){
	if(head == nullptr){
		return nullptr;
	}
	PointList *current = head;
	while(current->next != nullptr){
		current = current->next;
	}
	return current;
}

//リストの指定位置の後ろに挿入
PointList *pointListInsert(PointList *node){
	PointList *creature = (PointList*)malloc(sizeof(PointList));
	if(creature == nullptr){
		assert("リスト：メモリ確保失敗");
		exit(0);
	}
	if(node == nullptr){
		creature->next = nullptr;
	} else{
		pointListConnect(creature, node->next);
	}
	pointListConnect(node, creature);
	return creature;
}

//リストの要素を削除
int pointListDelete(PointList **head, PointList *target){
	//nullが渡されたら何もしない
	if(target == nullptr){
		return -1;
	}
	//リストから外す
	pointListConnect(target->prev, target->next);
	//リストの先頭が渡されたら書き換える
	if(target == (*head)){
		*head = (*head)->next;
	}
	free(target);
	return 0;
}

//リストの全要素を更新
int pointUpdateList(PointList **head){
	if(head == nullptr){
		return -1;
	}
	PointList *current = *head;
	while(current != nullptr){
		PointList *nextBuf = current->next;
		pointUpdate(&current->data);
		if(current->data.isDead){
			pointListDelete(head, current);
		}
		current = nextBuf;
	}
	return 0;
}

//リストの全要素を描画
int pointDrawList(PointList *head){
	if(head == nullptr){
		return -1;
	}
	PointList *current = head;
	while(current != nullptr){
		pointDraw(&current->data.draw);
		current = current->next;
	}
	SetDrawMode(AMDRAW_ALPHABLEND, 255);
	SetDrawBright(1000, 1000, 100);
	return 0;
}

