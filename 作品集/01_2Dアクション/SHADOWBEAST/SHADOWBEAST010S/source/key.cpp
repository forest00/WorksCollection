
#include	<Windows.h>
#include	"amgame.h"
#include	"AmHelper.h"

static int keybuf_1p = -1;
static int nowkey_1p = -1;
static int key0_1p = 0;
static int key1_1p = 0;
static int key2_1p = 0;
static int key3_1p = 0;
static int key_counters_1p[16];

static int keybuf_2p = -1;
static int nowkey_2p = -1;
static int key0_2p = 0;
static int key1_2p = 0;
static int key2_2p = 0;
static int key3_2p = 0;
static int key_counters_2p[16];


static int _start = 12;
static int _interval = 4;


void key_detail_1p() {
	keybuf_1p = nowkey_1p;
	nowkey_1p = CheckKey(AMINPUT_MULT);
	key0_1p = nowkey_1p;
	key1_1p = 0;
	key2_1p = 0;
	for (int i = 0; i < 16; i++) {
		int b = 1 << i;
		int c = key_counters_1p[i];
		if (key0_1p & b) {
			c++;
			if (c == 1) {
				key1_1p |= b;
				key2_1p |= b;
			} else if (c >= _start && (c - _start) % _interval == 0) {
				key1_1p |= b;
			}
		} else {
			c = 0;
		}
		key_counters_1p[i] = c;
	}
	key3_1p = (nowkey_1p ^ keybuf_1p) & keybuf_1p;
}

int get_key0_1p() {
    return key0_1p;
}
int get_key1_1p() {
    return key1_1p;
}
int get_key2_1p() {
    return key2_1p;
}
int get_key3_1p() {
    return key3_1p;
}


void key_detail_2p(){
    keybuf_2p = nowkey_2p;
    nowkey_2p = CheckKey(AMINPUT_PAD2);
    key0_2p = nowkey_2p;
    key1_2p = 0;
    key2_2p = 0;
    for(int i = 0; i < 16; i++){
        int b = 1 << i;
        int c = key_counters_2p[i];
        if(key0_2p & b){
            c++;
            if(c == 1){
                key1_2p |= b;
                key2_2p |= b;
            } else if(c >= _start && (c - _start) % _interval == 0){
                key1_2p |= b;
            }
        } else{
            c = 0;
        }
        key_counters_2p[i] = c;
    }
    key3_1p = (nowkey_2p ^ keybuf_2p) & keybuf_2p;
}

int get_key0_2p() {
    return key0_2p;
}
int get_key1_2p() {
    return key1_2p;
}
int get_key2_2p() {
    return key2_2p;
}
int get_key3_2p() {
    return key3_2p;
}


void key_repeat(int start, int interval) {
	_start = start;
	_interval = interval;
}

