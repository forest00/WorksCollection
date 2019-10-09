
#include "chara__data.h"

#define charaBase true,0,0,0,0,0,0,0,0,{0,0,0,0},dispInfoDefault,dummyFunc

static int dummyFunc(pCharacter){
    return 0;
}

static const DispInfo dispInfoDefault = {
0,0,            //������W
0,0,            //��]�g��̌��_
1,1,            //�g��{��
0,              //��]�p�x
NOBLEND,        //�u�����h���@
255,            //�s�����x
255,255,255,    //RGB
-1,             //�摜�n���h��
false           //�����]�t���O
};

static const Player1Att player1AttBase = {
    0, 0,       //���݈ړ����x
    0.6,        //�d��
    RIGHT,      //����

    100,        //�ړ���
    -24,        //�W�����v��
    300,        //�U����
    128,        //����
};




static const Character dataBase[] = {
    {charaBase,player1AttBase},
    {charaBase,0},
};


const pCharacter getCharacterBase(CharacterData index){
    return (const pCharacter)&dataBase[index];
}
