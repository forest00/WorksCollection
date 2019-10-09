
#include	<Windows.h>
#include	"amgame.h"
#include	"AmHelper.h"
#include "game_state.h"
#include "game_play.h"
#include "pause.h"
#include "key.h"

static int alpha = 0;
static int alphaMax = 150;
static int count = 0;
static int hBack = -1;

//起動時に一回だけ実行される
void initPause() {
	hBack = LoadTexture("res/pause.png");
    //hBack = 0;
}

//ポーズ
void pause(){
    count = 0;
    do{
        drawGamePlay();
        count++;
        alpha = count * 3;
        if(alpha >= alphaMax){
            alpha = alphaMax;
        }
        SetDrawMode(AMDRAW_ALPHABLEND, alpha);
        DrawMem(0, 0, hBack);
        Flip();
        key_detail_1p();
        key_detail_2p();
        if(get_key2_1p() & KEYIN_RET){
            break;
        }
        HCls();
    } while(ProcessEvent());
    do{
        alpha -= 3;
        if(alpha <= 0){
            break;
        }
    } while(ProcessEvent());
    SetDrawMode(AMDRAW_NOBLEND, 0);
    return;
    
}
