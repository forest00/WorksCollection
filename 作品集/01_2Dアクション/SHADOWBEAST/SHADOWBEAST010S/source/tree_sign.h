#pragma once


#define TREE_SIGN_MAX 64

//�Ŕ�

extern int initTreeSign();

extern int updateTreeSign();

extern int drawTreeSign();

extern int endTreeSign();


extern int setTreeSign(int index, double x, double y, int imageIndex);

extern int clearTreeSign(int index);

//�Ŕ̐�
extern int countSign();
//���łɓǂ܂�Ă���Ŕ̐�
extern int countSignHasRead();
