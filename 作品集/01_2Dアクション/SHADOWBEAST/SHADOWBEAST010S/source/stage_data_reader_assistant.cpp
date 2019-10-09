
#include <stdlib.h>
#include "amgame.h"
#include "game_state.h"


#define IMAGE_MAX 64
#define COUNT_LIMIT 24
#define ALPHA_MAX 255

typedef struct{
	bool isActive;
	bool isAppear;
	bool isFocus;
	int count;
	int countLimit;
	int x, y;
	int handle;
	int drawX, drawY;
	int drawW, drawH;
	int alpha;
	int alphaMax;
	int *arrayPtr;
	int elemNum;
}Image;

static int updateImage(Image *ip){
	if(ip->isAppear){
		ip->count++;
		if((ip->count) >= (ip->countLimit)){
			ip->count = (ip->countLimit);
		}
	} else{
		ip->count--;
		if((ip->count) < 0){
			ip->count = 0;
			ip->isFocus = false;
		}
	}
	double t = (double)(ip->count) / (double)(COUNT_LIMIT);
	ip->alpha = (int)(t * (ip->alphaMax));
	return 0;
}

static int drawImage(Image *ip){
	double t = (double)(ip->count) / (double)(COUNT_LIMIT);
	int iMax = (int)(t * (ip->elemNum));
	int x = (ip->x) + (ip->drawX);
	int y = (ip->y) + (ip->drawY);
	SetDrawMode(AMDRAW_ALPHABLEND, ip->alpha);
	for(int i = 0; i < iMax; i++){
		int drawX = x;
		int drawY = y + ip->arrayPtr[i];
		SetDrawArea(drawX, drawY, drawX + (ip->drawW), drawY + 1);
		DrawMemTh(x, y, ip->handle);
	}
	SetDrawMode(AMDRAW_ALPHABLEND, 255);
	SetDrawArea(0, 0, DISP_W, DISP_H);
	return 0;
}

Image images[IMAGE_MAX] = {0};


static int substArray(int *arrayPtr, int elemNum){
	for(int i = 0; i < elemNum; i++){
		arrayPtr[i] = i;
	}
	return 0;
}

static int shuffleArray(int *arrayPtr, int elemNum){
	for(int i = elemNum - 1; i >= 0; i--){
		int s = rand() % (i + 1);
		//swap
		int tmp = arrayPtr[i];
		arrayPtr[i] = arrayPtr[s];
		arrayPtr[s] = tmp;
	}
	return 0;
}

int SetImage(int index, int handle){
	if(index < 0 || index >= IMAGE_MAX){
		return -1;
	}
	Image *ip = &images[index];
	ip->isActive = true;
	ip->handle = handle;
	if(ip->handle < 0){
		return -2;
	}
	GetPictureSize(ip->handle, &(ip->drawW), &(ip->drawH));
	ip->x = DISP_CENTER_X;
	ip->y = DISP_CENTER_Y;
	ip->drawX = -(ip->drawW) / 2;
	ip->drawY = -(ip->drawH) / 2;
	ip->elemNum = ip->drawH;
	ip->arrayPtr = (int*)malloc(sizeof(int) * (ip->elemNum));
	ip->alphaMax = ALPHA_MAX;
	ip->countLimit = COUNT_LIMIT;
	substArray(ip->arrayPtr, ip->elemNum);
	shuffleArray(ip->arrayPtr, ip->elemNum);
	return 0;
}

int loadImageByData(int index, char *filePathAndName){
	return SetImage(index, LoadTexture(filePathAndName));
}

int dispImageInGame(int index){
	if(index < 0 || index >= IMAGE_MAX){
		return -1;
	}
	Image *ip = &images[index];
	ip->isAppear = true;
	ip->isFocus = true;
	ip->count = 0;
	return 0;
}

int clearImageInGame(int index){
	if(index < 0 || index >= IMAGE_MAX){
		return -1;
	}
	Image *ip = &images[index];
	ip->isAppear = false;
	ip->isFocus = true;
	return 0;
}

int moveImageInGame(int index, int x, int y){
	if(index < 0 || index >= IMAGE_MAX){
		return -1;
	}
	Image *ip = &images[index];
	ip->x = x;
	ip->y = y;
	return 0;
}


int initStageDataReaderAssistant(){
	for(int i = 0; i < IMAGE_MAX; i++){
		images[i] = {false};
	}
	return 0;
}

int updateStageDataReaderAssistant(){
	for(int i = 0; i < IMAGE_MAX; i++){
		if(images[i].isActive){
			if(images[i].isFocus){
				updateImage(&images[i]);
			}
		}
	}
	return 0;
}

int drawStageDataReaderAssistant(){
	for(int i = 0; i < IMAGE_MAX; i++){
		if(images[i].isActive){
			if(images[i].isFocus){
				drawImage(&images[i]);
			}
		}
	}
	return 0;
}
