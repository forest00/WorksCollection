#pragma once


#define TREE_SIGN_MAX 64

//ŠÅ”Â

extern int initTreeSign();

extern int updateTreeSign();

extern int drawTreeSign();

extern int endTreeSign();


extern int setTreeSign(int index, double x, double y, int imageIndex);

extern int clearTreeSign(int index);

//ŠÅ”Â‚Ì”
extern int countSign();
//‚·‚Å‚É“Ç‚Ü‚ê‚Ä‚¢‚éŠÅ”Â‚Ì”
extern int countSignHasRead();
