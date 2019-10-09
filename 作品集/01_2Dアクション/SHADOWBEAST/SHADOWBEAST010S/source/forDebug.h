#pragma once

//デバッグオプションを切り替えるとビルドの所要時間が伸びます
//#define _DBG_

#ifdef _DBG_
#define _DBGOPT_DRAW_FRAME_
#define _DBGOPT_DRAW_INPUT_
#define _DBGOPT_DRAW_DEBUGSTRING_
#define _DBGOPT_DRAW_RECT_
#define _DBGOPT_KEYBOARD_
#endif



int initDebugInfo();

int addDebugInfo(const char *fmt, ...);
int flashDebugInfo(const char *fmt, ...);

int addDebugInfo();
int addDebugInfo(int x);
int addDebugInfo(int x, int y);
int addDebugInfo(double x);
int addDebugInfo(double x, double y);
int addDebugInfo(char *str);

int flashDebugInfo();
int flashDebugInfo(int x);
int flashDebugInfo(int x, int y);
int flashDebugInfo(double x);
int flashDebugInfo(double x, double y);
int flashDebugInfo(char *str);

int addDebugInfoX(int x);
int addDebugInfoX(double x);
int addDebugInfoY(int y);
int addDebugInfoY(double y);
int addDebugInfoP(int x, int y);
int addDebugInfoP(double x, double y);

int flashDebugInfoX(int x);
int flashDebugInfoX(double x);
int flashDebugInfoY(int y);
int flashDebugInfoY(double y);
int flashDebugInfoP(int x, int y);
int flashDebugInfoP(double x, double y);

int addDebugInfoRect(int left, int top, int right, int bottom);
int flashDebugInfoRect(int left, int top, int right, int bottom);

int addDebugInfoBox(int x, int y, int width, int height);
int flashDebugInfoBox(int x, int y, int width, int height);




int renderDebugInfo();

int ForDebugCheckKey(char c);

bool ForDebugFreshKey(char c);

bool ForDebugUpKey(char c);

bool ForDebugDownKey(char c);
