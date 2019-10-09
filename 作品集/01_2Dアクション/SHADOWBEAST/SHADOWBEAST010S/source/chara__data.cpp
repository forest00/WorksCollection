
#include "chara__data.h"

#define charaBase true,0,0,0,0,0,0,0,0,{0,0,0,0},dispInfoDefault,dummyFunc

static int dummyFunc(pCharacter){
    return 0;
}

static const DispInfo dispInfoDefault = {
0,0,            //左上座標
0,0,            //回転拡大の原点
1,1,            //拡大倍率
0,              //回転角度
NOBLEND,        //ブレンド方法
255,            //不透明度
255,255,255,    //RGB
-1,             //画像ハンドル
false           //横反転フラグ
};

static const Player1Att player1AttBase = {
    0, 0,       //現在移動速度
    0.6,        //重力
    RIGHT,      //向き

    100,        //移動力
    -24,        //ジャンプ力
    300,        //攻撃力
    128,        //回避力
};




static const Character dataBase[] = {
    {charaBase,player1AttBase},
    {charaBase,0},
};


const pCharacter getCharacterBase(CharacterData index){
    return (const pCharacter)&dataBase[index];
}
