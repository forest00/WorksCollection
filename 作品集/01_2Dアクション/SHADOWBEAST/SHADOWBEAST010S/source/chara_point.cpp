
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
	//���Ԃ�i�߂�
	point->elapsed++;
	//���S����
	if(point->alpha <= 0){
		point->isDead = true;
	}
	//�`��p�̌v�Z��
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




//���X�g�̗v�f���Ȃ���
int pointListConnect(PointList *node0, PointList *node1){
	if(node0 != nullptr){
		node0->next = node1;
	}
	if(node1 != nullptr){
		node1->prev = node0;
	}
	return 0;
}

//���X�g�̖����𓾂�
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

//���X�g�̎w��ʒu�̌��ɑ}��
PointList *pointListInsert(PointList *node){
	PointList *creature = (PointList*)malloc(sizeof(PointList));
	if(creature == nullptr){
		assert("���X�g�F�������m�ێ��s");
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

//���X�g�̗v�f���폜
int pointListDelete(PointList **head, PointList *target){
	//null���n���ꂽ�牽�����Ȃ�
	if(target == nullptr){
		return -1;
	}
	//���X�g����O��
	pointListConnect(target->prev, target->next);
	//���X�g�̐擪���n���ꂽ�珑��������
	if(target == (*head)){
		*head = (*head)->next;
	}
	free(target);
	return 0;
}

//���X�g�̑S�v�f���X�V
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

//���X�g�̑S�v�f��`��
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

