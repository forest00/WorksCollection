#pragma once


#define TREE_SIGN_MAX 64

//看板

extern int initTreeSign();

extern int updateTreeSign();

extern int drawTreeSign();

extern int endTreeSign();


extern int setTreeSign(int index, double x, double y, int imageIndex);

extern int clearTreeSign(int index);

//看板の数
extern int countSign();
//すでに読まれている看板の数
extern int countSignHasRead();
