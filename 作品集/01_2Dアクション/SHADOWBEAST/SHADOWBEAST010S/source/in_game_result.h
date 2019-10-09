#pragma once


//リザルト出す直前に一回呼ぶ
int resultInfoSetUp();
//リザルトを出したい間ずっと呼ぶ
int resultInfoUpdate();
int resultInfoDraw();
//リザルトが不要になったら呼ぶ
//一瞬でリザルト情報が消えます
int resultInfoDestroy();

