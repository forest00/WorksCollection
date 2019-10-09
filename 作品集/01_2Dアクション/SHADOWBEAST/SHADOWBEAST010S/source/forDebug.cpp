
#include "amgame.h"
#include "forDebug.h"
#include <stdio.h>
#include <Windows.h>
#include "camera.h"
#include "game_state.h"
#include "image_manager.h"

#define WHITE GetColor(255,255,255)
#define BLACK GetColor(0,0,0)
#define RED   GetColor(255,0,0)
#define LIME  GetColor(0,255,0)
#define BLUE  GetColor(0,0,255)
#define COLOR_NUM 5
#define FONTSIZE 16
#define X_MAX 1024
#define NUL -1
#define KEYIN_DEBUG_L (1<<0)
#define KEYIN_DEBUG_K (1<<1)
#define KEYIN_DEBUG_J (1<<2)
#define LIMIT 128


typedef struct{
	bool active;
	int prev, next;
	bool flash;
	char *str;
}DebugInfoS;

typedef struct{
	bool active;
	bool flash;
	int x, y;
}DebugInfoP;

typedef struct{
	bool active;
	bool flash;
	int left, top, right, bottom;
}DebugInfoR;

static DebugInfoS noneStr = {false,NUL,NUL,false,nullptr};
static DebugInfoP nonePoint = {false,false,0,0};
static DebugInfoR noneRect = {false,false,0,0,0,0};
static BYTE cKeyBuf0[256] = {0};
static BYTE cKeyBuf1[256] = {0};

#ifdef _DBGOPT_KEYBOARD_
// キーボードの状態チェック
static int checkKeyBoard()
{

	for(int i = 0; i < 256; i++){
		cKeyBuf1[i] = cKeyBuf0[i];
	}
	GetKeyboardState(cKeyBuf0);
		
	return 0;
}
#endif

static const unsigned int infoSMax = 32;
static const unsigned int infoPMax = 12;
static const unsigned int infoRMax = 8;
static unsigned int infoCount = 0;
static unsigned int offSetX = 0;
static unsigned int colorIndex = 0;
static int color[COLOR_NUM] = {0};
static int nowKey = 0;

static DebugInfoS listS[infoSMax] = {noneStr};
static DebugInfoP listP[infoPMax] = {nonePoint};
static DebugInfoR listR[infoRMax] = {noneRect};
static int head = 0, tail = head;

static int connect_(int nHead, int nTail){
	listS[nHead].next = nTail;
	listS[nTail].prev = nHead;
	return 0;
}

static int add_(char *data){
	int nonActive = NUL;
	for(int i = 0; i < infoSMax; i++){
		if(!(listS[i].active)){
			nonActive = i;
			break;
		}
	}
	if(nonActive == NUL){
		return NUL;
	}
	connect_(tail, nonActive);
	tail = nonActive;
	DebugInfoS *p = &listS[nonActive];
	p->active = true;
	p->str = (char*)malloc(sizeof(char)*LIMIT);
	strcpy(p->str, data);
	infoCount++;
	return nonActive;
}

static int del_(int target){
	DebugInfoS *t = &listS[target];
	connect_(t->prev, t->next);
	if(target == tail){
		tail = t->prev;
	}
	free(t->str);
	listS[target] = noneStr;
	infoCount--;
	return 0;
}

static int addDebugInfo_(char *data, bool flash){
	if(infoCount >= infoSMax){
		return -1;
	}
	int creature = add_(data);
	if(creature == NUL){
		-1;
	}
	listS[creature].flash = flash;
	return 0;
}


int initDebugInfo(){
	infoCount = 0;
	offSetX = 0;
	colorIndex = 0;
	nowKey = 0;
	head = 0;
	tail = head;
	for(int i = 0; i < infoSMax; i++){
		listS[i] = noneStr;
	}
	listS[head].active = true;
	addDebugInfo();
	color[0] = WHITE;
	color[1] = BLACK;
	color[2] = LIME;
	color[3] = RED;
	color[4] = BLUE;
	return 0;
}

int addDebugInfo(const char *fmt, ...){
	// fmt 以下の引数から、文字列を作り出す
	constexpr int elemNum = 1 << 12;
	char data[elemNum];
	va_list list;
	va_start(list, fmt);
	vsprintf(data, fmt, list);
	va_end(list);
	return addDebugInfo_(data, false);
}
int flashDebugInfo(const char *fmt, ...){
	// fmt 以下の引数から、文字列を作り出す
	constexpr int elemNum = 1 << 12;
	char data[elemNum];
	va_list list;
	va_start(list, fmt);
	vsprintf(data, fmt, list);
	va_end(list);
	return addDebugInfo_(data, true);
}

int addDebugInfo(){
	return addDebugInfo_("", false);
}

int addDebugInfo(int x){
	char data[LIMIT] = "";
	snprintf(data, LIMIT, "X : % 6d", x);
	return addDebugInfo_(data, false);
}
int addDebugInfo(int x, int y){
	char data[LIMIT] = "";
	snprintf(data, LIMIT, "X : % 6d   Y : % 6d", x, y);
	return addDebugInfo_(data, false);
}
int addDebugInfo(double x){
	char data[LIMIT] = "";
	snprintf(data, LIMIT, "X : % 12f", x);
	return addDebugInfo_(data, false);
}
int addDebugInfo(double x, double y){
	char data[LIMIT] = "";
	snprintf(data, LIMIT, "X : % 12f   Y : % 12f", x, y);
	return addDebugInfo_(data, false);
}
int addDebugInfo(char *str){
	return addDebugInfo_(str, false);
}

int flashDebugInfo(){
	return addDebugInfo_("", true);
}
int flashDebugInfo(int x){
	char data[LIMIT] = "";
	snprintf(data, LIMIT, "X : % 6d", x);
	return addDebugInfo_(data, true);
}
int flashDebugInfo(int x, int y){
	char data[LIMIT] = "";
	snprintf(data, LIMIT, "X : % 6d   Y : % 6d", x, y);
	return addDebugInfo_(data, true);
}
int flashDebugInfo(double x){
	char data[LIMIT] = "";
	snprintf(data, LIMIT, "X : % 12f", x);
	return addDebugInfo_(data, true);
}
int flashDebugInfo(double x, double y){
	char data[LIMIT] = "";
	snprintf(data, LIMIT, "X : % 12f   Y : % 12f", x, y);
	return addDebugInfo_(data, true);
}
int flashDebugInfo(char *str){
	return addDebugInfo_(str, true);
}

static DebugInfoP *findNonActiveDebugInfoP(){
	for(int i = 0; i < infoPMax; i++){
		if(!listP[i].active){
			return &listP[i];
		}
	}
	return nullptr;
}

int addDebugInfoX(int x){
	return addDebugInfoP(x, -8);
}
int addDebugInfoX(double x){
	return addDebugInfoP((int)x, -8);
}
int addDebugInfoY(int y){
	return addDebugInfoP(-8, y);
}
int addDebugInfoY(double y){
	return addDebugInfoP(-8, (int)y);
}
int addDebugInfoP(int x, int y){
	DebugInfoP *p = findNonActiveDebugInfoP();
	if(p == nullptr){
		return -1;
	}
	p->active = true;
	p->flash = false;
	p->x = x;
	p->y = y;
	return 0;
}
int addDebugInfoP(double x, double y){
	addDebugInfoP((int)x, (int)y);
	return 0;
}

int flashDebugInfoX(int x){
	return flashDebugInfoP(x, -8);
}
int flashDebugInfoX(double x){
	return flashDebugInfoP((int)x, -8);
}
int flashDebugInfoY(int y){
	return flashDebugInfoP(-8, y);
}
int flashDebugInfoY(double y){
	return flashDebugInfoP(-8, (int)y);
}
int flashDebugInfoP(int x, int y){
	DebugInfoP *p = findNonActiveDebugInfoP();
	if(p == nullptr){
		return -1;
	}
	p->active = true;
	p->flash = true;
	p->x = x;
	p->y = y;
	return 0;
}
int flashDebugInfoP(double x, double y){
	flashDebugInfoP((int)x, (int)y);
	return 0;
}

static DebugInfoR *findNonActiveDebugInfoR(){
	for(int i = 0; i < infoRMax; i++){
		if(!listR[i].active){
			return &listR[i];
		}
	}
	return nullptr;
}

int addDebugInfoRect(int left, int top, int right, int bottom){
	DebugInfoR *p = findNonActiveDebugInfoR();
	if(p == nullptr){
		return -1;
	}
	p->active = true;
	p->flash = false;
	p->left = left;
	p->top = top;
	p->right = right;
	p->bottom = bottom;
	return 0;
}
int flashDebugInfoRect(int left, int top, int right, int bottom){
	DebugInfoR *p = findNonActiveDebugInfoR();
	if(p == nullptr){
		return -1;
	}
	p->active = true;
	p->flash = true;
	p->left = left;
	p->top = top;
	p->right = right;
	p->bottom = bottom;
	return 0;
}

int addDebugInfoBox(int x, int y, int width, int height){
	return addDebugInfoRect(x, y, x + width, y + height);
}
int flashDebugInfoBox(int x, int y, int width, int height){
	return flashDebugInfoRect(x, y, x + width, y + height);
}


int renderDebugInfo(){
	#ifdef _DBGOPT_KEYBOARD_
	checkKeyBoard();
	#endif
	static bool drawFlag = true;
	static int count = 0;
	int current = listS[head].next;
	int c = color[colorIndex];
	bool f0 = ForDebugFreshKey('U');
	bool f1 = ForDebugFreshKey('Y');
	bool f2 = ForDebugFreshKey('T');
	bool f3 = ForDebugFreshKey('I');
	if(f0 || f1 || f2 || f3){
		count++;
	} else{
		count = 0;
	}
	if(count != 1 && ((count % 4) || count <= 12)){
		f0 = f1 = f2 = f3 = false;
	}
	if(f0){
		offSetX += FONTSIZE * 4;
		offSetX %= X_MAX;
	}
	if(f1){
		offSetX -= FONTSIZE * 4;
		offSetX %= X_MAX;
	}
	if(f2){
		colorIndex++;
		colorIndex %= COLOR_NUM;
	}
	if(f3){
		drawFlag = !drawFlag;
	}

	int y = 0;
	#ifdef _DBGOPT_DRAW_DEBUGSTRING_
	{
		if(drawFlag){
			static unsigned int nowFrame = 0;
			int drawX = offSetX;
			int drawY = y * FONTSIZE;
			static char str[LIMIT] = {'\0'};
			#ifdef _DBGOPT_DRAW_FRAME_
			sprintf(str, "Frame:%ld", nowFrame);
			#endif
			DrawString(drawX + 1, drawY + 1, str, 0xff000000 | ~c);
			DrawString(drawX, drawY, str, c);
			nowFrame++;
		}
	}
	while(current != NUL){
		int nextBuf = listS[current].next;
		if(drawFlag){
			int drawX = offSetX;
			int drawY = y * FONTSIZE;
			DrawString(drawX + 1, drawY + 1, listS[current].str, 0xff000000 | ~c);
			DrawString(drawX, drawY, listS[current].str, c);
		}
		if(listS[current].flash){
			del_(current);
		}
		current = nextBuf;
		y++;
	};
	#endif // _DBGOPT_DRAW_DEBUGSTRING_
	for(int i = 0; i < infoPMax; i++){
		DebugInfoP *p = &listP[i];
		if(p->active){
			int drawX = p->x;
			int drawY = p->y;
			if(drawFlag){
				DrawFBox(drawX, 0, drawX + 1, 2048, c);
				DrawFBox(0, drawY, 2048, drawY + 1, c);
				DrawFBox(drawX - 1, drawY - 2, drawX + 2, drawY + 3, c);
				DrawFBox(drawX - 2, drawY - 1, drawX + 3, drawY + 2, c);
			}
			if(p->flash){
				p->active = false;
			}
		}
	}
	for(int i = 0; i < infoRMax; i++){
		DebugInfoR *p = &listR[i];
		if(p->active){
			double left = p->left;
			double top = p->top;
			double right = p->right;
			double bottom = p->bottom;
			correctByCamera(&left, &top);
			correctByCamera(&right, &bottom);
			int l = (int)left;
			int t = (int)top;
			int r = (int)right;
			int b = (int)bottom;
			if(drawFlag){
				int col = c;
				DrawFBox(l, t, r, t + 1, col);
				DrawFBox(l, b, r, b - 1, col);
				DrawFBox(l, t, l + 1, b, col);
				DrawFBox(r, t, r - 1, b, col);
				col = ~col;
				l++, t++, r--, b--;
				DrawFBox(l, t, r, t + 1, col);
				DrawFBox(l, b, r, b - 1, col);
				DrawFBox(l, t, l + 1, b, col);
				DrawFBox(r, t, r - 1, b, col);
				l--, t--, r++, b++;
				l--, t--, r++, b++;
				DrawFBox(l, t, r, t + 1, col);
				DrawFBox(l, b, r, b - 1, col);
				DrawFBox(l, t, l + 1, b, col);
				DrawFBox(r, t, r - 1, b, col);
			}
			if(p->flash){
				p->active = false;
			}
		}
	}
	static bool forDebugPause = false;
	bool ___buf = false;
	___buf = forDebugPause;
	if(ForDebugDownKey('O')){
		forDebugPause = true;
	}
	//while(forDebugPause){
	//	AM_ProcessEvent();
	//	AM_Flip();
	//	checkKeyBoard();
	//	if(___buf == forDebugPause && ForDebugDownKey('O')){
	//		forDebugPause = false;
	//	}
	//	if(ForDebugDownKey('I')){
	//		drawFlag = !drawFlag;
	//		break;
	//	}
	//	if(ForDebugDownKey('P')){
	//		break;
	//	}
	//	___buf = forDebugPause;
	//}
	return 0;
}

int ForDebugCheckKey(char c){
	int i = (int)c;
	return cKeyBuf0[i];
}

bool ForDebugFreshKey(char c){
	int i = (int)c;
	return (bool)(cKeyBuf0[i] & 0x80);
}

bool ForDebugUpKey(char c){
	int i = (int)c;
	return (bool)(!(cKeyBuf0[i] & 0x80) && ((cKeyBuf1[i] & 0x80)));
}

bool ForDebugDownKey(char c){
	int i = (int)c;
	return (bool)((cKeyBuf0[i] & 0x80) && !((cKeyBuf1[i] & 0x80)));
}
