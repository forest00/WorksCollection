/**
 * @file  image_manager.cpp
 * 画像ファイル読み込み一括管理
 *
 * @date 2017-06-14
 * @version 3.0
 *
 * Copyright (C) Amusement Media Academy All rights Resved.
 */

#include "amgame.h"
#include "image_manager.h"
#include <stdio.h>
#include <string.h>

static int nowLoading = -1;

 // 使用する画像ファイル一覧(画像ファイル番号と合わせる)
static const BlockImage g_image_tble[IMAGE_MAX] = {
	{LOAD_TEXTURE, "", 0, 0, 1, 1, 1, 1},
{LOAD_BLK_TEXTURE, "image/斉藤君作/team_logo.png", 400, 200, 1, 2, 2, 60},

{LOAD_TEXTURE, "image/タイトル/title_back.png", 1280, 720, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/タイトル/red_eyes.png", 1280, 720, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/タイトル/red_eyes2.png", 1280, 720, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/タイトル/shadow_mist.png", 1024, 600, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/タイトル/shadow_mist2.png", 1024, 600, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/タイトル/title_logo.png", 1024, 600, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/タイトル/press any button.png", 300, 50, 1, 1, 1, 0},

{LOAD_TEXTURE, "image/スタッフロール/スタッフロール.png", 1024, 600, 1, 1, 1, 0},

{LOAD_TEXTURE, "image/モードセレクト/start.png", 146, 68, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/モードセレクト/continue.png", 146, 68, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/モードセレクト/option.png", 146, 68, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/モードセレクト/exit.png", 146, 68, 1, 1, 1, 1},

{LOAD_TEXTURE, "image/オプション/sadsf.png", 146, 68, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/オプション/Luminocity.png", 146, 68, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/オプション/輝度選択.png", 65, 65, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/オプション/輝度ゲージ.png", 65, 65, 1, 1, 1, 0},

{LOAD_TEXTURE, "image/背景/white_back.png", 1280, 720, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/背景/game_play_back.png", 1280, 720, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/背景/sea.png", 1280, 720, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/背景/pause_back.png", 1280, 720, 1, 1, 1, 1},

{LOAD_TEXTURE, "image/文字素材/gameclear.png", 1280, 720, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/文字素材/gameover.png", 1280, 720, 1, 1, 1, 1},

{LOAD_TEXTURE, "image/文字素材/OP③.png", 400, 640, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/文字素材/OP②.png", 400, 640, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/文字素材/OP①.png", 400, 640, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/文字素材/OP④.png", 400, 640, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/文字素材/OP⑤.png", 400, 640, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/文字素材/OP⑥.png", 400, 640, 1, 1, 1, 0},

{LOAD_TEXTURE, "image/文字素材/戻る.png", 400, 640, 1, 1, 1, 0},

{LOAD_TEXTURE, "image/チュートリアル画像/チュートリアル1.png", 600, 60, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/チュートリアル画像/チュートリアル2.png", 600, 60, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/チュートリアル画像/チュートリアル3.png", 600, 60, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/チュートリアル画像/チュートリアル4.png", 600, 60, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/チュートリアル画像/チュートリアル5.png", 600, 60, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/チュートリアル画像/チュートリアル6.png", 600, 60, 1, 1, 1, 0},

{LOAD_TEXTURE, "image/ステージセレクト/tutorial.png", 180, 32, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/ステージセレクト/stage_1.png", 180, 32, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/ステージセレクト/stage_2.png", 180, 32, 1, 1, 1, 1},

{LOAD_TEXTURE, "image/背景/title_1.png", 1040, 480, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/選択系UI/チュートリアル＆ステージセレクト.png", 800, 300, 1, 1, 1, 1},

{LOAD_TEXTURE, "image/文字素材/STAGE_SELECT_BACK.png", 3000, 500, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/文字素材/A_BACK.png", 1560, 500, 1, 1, 1, 0},

{LOAD_TEXTURE, "image/文字素材/OP2.png", 600, 48, 1, 1, 1, 0},

{LOAD_BLK_TEXTURE, "image/effect/enemy_hole.png", 200, 200, 10, 2, 20, 20 * 3},
{LOAD_TEXTURE, "image/effect/雲/雲単体白黒.png", 83, 39, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/effect/雲/雲集合体白黒α.png", 251, 172, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/effect/雲/雲集合体白黒β.png", 240, 163, 1, 1, 1, 1},

{LOAD_TEXTURE, "image/effect/黒い粒.png", 8, 8, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/effect/闇の玉.png", 16, 16, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/effect/炎.png", 160, 160, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/effect/炎エフェクト.png", 100, 120, 1, 1, 1, 1},

{LOAD_TEXTURE, "image/effect/dark.png", 1280, 720, 1, 1, 1, 1},
{LOAD_BLK_TEXTURE, "image/effect/爪エフェクト連番/clow_sheet.png", 400, 600, 9, 1, 9, 60},
{LOAD_TEXTURE, "image/斉藤君作/stonebrick1 (4).png", 3000, 128, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/斉藤君作/Charge.png", 300, 300, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/城/城のみ.png", 1280, 720, 1, 1, 1, 0},
{LOAD_BLK_TEXTURE, "image/城/赤　塔のみ　崩れる/castle_red_break.png", 1280, 720, 13, 1, 13, 60},
{LOAD_BLK_TEXTURE, "image/城/緑　塔のみ　崩れる/castle_green_break_sheet.png", 1280, 720, 16, 1, 16, 60},
{LOAD_BLK_TEXTURE, "image/城/青　塔のみ　崩れる/castle_blue_break_sheet.png", 1280, 720, 14, 1, 14, 60},

{LOAD_TEXTURE, "image/船/船通常.png", 1280, 720, 1, 1, 1, 60},
{LOAD_BLK_TEXTURE, "image/船/船壊れアニメ.png", 1280, 720, 5, 2, 10, 60},

{LOAD_TEXTURE, "image/斉藤君作/heart1.png", 50, 50, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/斉藤君作/kanban.png", 200, 200, 1, 1, 1, 1},

{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/jack/jack stand by_sheet.png", 256, 256, 6, 1, 6, 6 * 4},		//IMAGE_1P_IDLE
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/jack/jack run　_sheet.png", 256, 256, 12, 1, 12, 12 * 4},		//IMAGE_1P_WALK
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/jack/jack jump_sheet.png", 256, 256, 11, 1, 11, 60},			//IMAGE_1P_JUMP,
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/jack/jack atk_sheet.png", 256, 256, 15, 1, 15, 15 * 2},		//IMAGE_1P_MAIN_ATTACK1,
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/jack/jack atk_sheet.png", 256, 256, 15, 1, 15, 15 * 2},		//IMAGE_1P_MAIN_ATTACK2,
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/jack/jack atk_sheet.png", 256, 256, 15, 1, 15, 15 * 2},		//IMAGE_1P_MAIN_ATTACK3,
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/jack/jack sab 1.png", 256, 256, 13, 1, 13, 13},			//IMAGE_1P_SUB_ATTACK_BEFORE1,
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/jack/jack sab 2.png", 256, 256, 13, 1, 13, 13},			//IMAGE_1P_SUB_ATTACK_AFTER,
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/jack/jack gad_sheet.png", 256, 256, 2, 1, 2, 2 * 6},		//IMAGE_1P_GUARD,
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/jack/jack avoid_sheet.png", 256, 256, 20, 1, 20, 42},		//IMAGE_1P_AVOID,
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/jack/jack damage.png", 256, 256, 1, 1, 1, 32},			//IMAGE_1P_DAMAGE,
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/jack/jack down_sheet.png", 256, 256, 3, 1, 3, 3 * 12},		//IMAGE_1P_KNOCK_OUT,
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/jack/jack dow.png", 256, 256, 1, 1, 1, 30},			//IMAGE_1P_DEAD,
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/jack/jack sosei_sheet.png", 256, 256, 13, 1, 13, 13 * 3},		//IMAGE_1P_RESURRECTION,
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/jack/chain.png", 400, 50, 30, 1, 30, 30},
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/jack/jack_atk_effect.png", 512, 512, 5, 1, 5, 32},
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/jack/jack_sub_atk_effect.png", 200, 300, 3, 2, 6, 6 * 2},

{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/レイス/_2_reisu stanby.png", 256, 256, 19, 1, 19, 60},
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/レイス/_2_reisu run.png", 256, 256, 13, 1, 13, 50},
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/レイス/_2_reisu jump.png", 256, 256, 16, 1, 16, 60},
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/レイス/_2_reisu atk.png", 256, 256, 9, 1, 9, 9 * 4},
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/レイス/_2_reisu sab atk.png", 256, 256, 16, 1, 16, 16 * 4},
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/レイス/_2_reisu guard.png", 256, 256, 5, 1, 5, 30},
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/レイス/_2_reisu jump.png", 256, 256, 16, 1, 16, 32},
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/レイス/_2_reisu _dummy_damage.png", 256, 256, 4, 1, 4, 30},
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/レイス/_2_reisu knock down.png", 256, 256, 20, 1, 20, 40},
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/レイス/_2_reisu _lie_down.png", 256, 256, 4, 1, 4, 30},
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/レイス/_2_reisu sosei.png", 256, 256, 14, 1, 14, 14 * 3},
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/レイス/_2_reisu sab atk.png", 256, 256, 16, 1, 16, 16 * 4},
{LOAD_TEXTURE, "image/プレイヤーキャラ/レイス/reisu_atk_tama.png", 200, 100, 1, 1, 1, 1},
{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/レイス/reisu_sub_atk_effect.png", 350, 300, 9, 1, 9, 9 * 2},
{LOAD_TEXTURE, "image/プレイヤーキャラ/レイス/reisu_atk_tama.png", 200, 100, 1, 1, 1, 1},

{LOAD_BLK_TEXTURE, "image/敵キャラ/ゴブリン/nuru_wait_sheet.png", 130, 130, 4, 1, 4, 48},
{LOAD_BLK_TEXTURE, "image/敵キャラ/ゴブリン/nuru_walk_sheet.png", 130, 130, 4, 1, 4, 48},
{LOAD_BLK_TEXTURE, "image/敵キャラ/ゴブリン/nuru_damage_0000_sheet.png", 130, 130, 3, 1, 3, 24},
{LOAD_BLK_TEXTURE, "image/敵キャラ/ゴブリン/nuru_shin_attack.png", 160, 160, 20, 1, 19, 72},
{LOAD_BLK_TEXTURE, "image/敵キャラ/ワイバーン/nuri-wait.png", 130, 130, 4, 1, 4, 48},
{LOAD_BLK_TEXTURE, "image/敵キャラ/ワイバーン/nuri-wait.png", 130, 130, 4, 1, 4, 48},
{LOAD_BLK_TEXTURE, "image/敵キャラ/ワイバーン/nuri-dm.png", 130, 130, 3, 1, 3, 24},
{LOAD_BLK_TEXTURE, "image/敵キャラ/ワイバーン/nuri-at.png", 130, 130, 7, 1, 7, 48},
{LOAD_BLK_TEXTURE, "image/敵キャラ/スネーク/nira_walk3.png", 240, 240, 3, 1, 3, 48},
{LOAD_BLK_TEXTURE, "image/敵キャラ/スネーク/nira_walk3.png", 240, 240, 3, 1, 3, 48},
{LOAD_BLK_TEXTURE, "image/敵キャラ/スネーク/nira_dm3.png", 240, 240, 4, 1, 4, 24},
{LOAD_BLK_TEXTURE, "image/敵キャラ/スネーク/nira_at3_.png", 240, 240, 24, 1, 24, 48},
{LOAD_BLK_TEXTURE, "image/ボス/巨人/superwalk_nuru.png", 700, 700, 23, 1, 23, 60},
{LOAD_BLK_TEXTURE, "image/ボス/巨人/superwalk_nuru.png", 700, 700, 23, 1, 23, 60},
{LOAD_BLK_TEXTURE, "image/ボス/巨人/super.nuru.attack.png", 750, 700, 19, 2, 21, 66},
{LOAD_BLK_TEXTURE, "image/ボス/巨人/super.nuru_nagiat.png", 700, 700, 12, 1, 12, 60},
{LOAD_BLK_TEXTURE, "image/ボス/巨人/super.nuru_tkr.png", 700, 700, 13, 1, 13, 120},
{LOAD_BLK_TEXTURE, "image/ボス/巨人/superwalk_nuru.png", 700, 700, 23, 1, 23, 24},
{LOAD_BLK_TEXTURE, "image/ボス/ウミヘビ/_2_hebi stanby0000_sheet.png", 250, 250, 13, 1, 13, 13 * 4},
{LOAD_BLK_TEXTURE, "image/ボス/ウミヘビ/_2_hebi work0000_sheet.png", 250, 250, 22, 1, 21, 21 * 4},
{LOAD_BLK_TEXTURE, "image/ボス/ウミヘビ/_2_hebi eatk0000_sheet.png", 250, 250, 10, 8, 71, 71},
{LOAD_BLK_TEXTURE, "image/ボス/ウミヘビ/_2_hebi last sparet 20000_sheet.png", 250, 250, 17, 3, 51, 51},
{LOAD_BLK_TEXTURE, "image/ボス/ウミヘビ/_2_hebi geroatk0000_sheet.png", 250, 250, 31, 1, 31, 31},
{LOAD_BLK_TEXTURE, "image/ボス/ウミヘビ/_2_hebi _damage__sheet.png", 250, 250, 2, 1, 2, 2 * 4},
{LOAD_BLK_TEXTURE, "image/ボス/ウミヘビ/_2_hebi down0000_sheet.png", 250, 250, 14, 1, 14, 14 * 4},

{LOAD_TEXTURE, "image/effect/雷/雷落下位置示唆光柱.png", 500, 2000, 1, 1, 1, 1},
{LOAD_BLK_TEXTURE, "image/effect/雷/thunder_anim.png", 600, 720, 9, 1, 9, -1},

{LOAD_TEXTURE, "image/仮素材/1p_name_prate.png", 160, 100, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/仮素材/2p_name_prate.png", 160, 100, 1, 1, 1, 1},


{LOAD_TEXTURE, "image/リザルト画像/window_frame.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/リザルト画像/死亡数.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/リザルト画像/討伐数.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/リザルト画像/耐久率.png", -1, -1, 1, 1, 1, 1},

{LOAD_TEXTURE, "image/リザルト画像/数字/0.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/リザルト画像/数字/1.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/リザルト画像/数字/2.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/リザルト画像/数字/3.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/リザルト画像/数字/4.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/リザルト画像/数字/5.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/リザルト画像/数字/6.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/リザルト画像/数字/7.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/リザルト画像/数字/8.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/リザルト画像/数字/9.png", -1, -1, 1, 1, 1, 1},


{LOAD_BLK_TEXTURE, "image/プレイヤーキャラ/jack/blood_anim.png", 150, 150, 11, 1, 11, 22},


{LOAD_TEXTURE, "image/チュートリアル/枠のみ.png", 1280, 720, 1, 1, 1, 1},//IMAGE_TUTORIAL_F_
{LOAD_TEXTURE, "image/チュートリアル/ようこそ世界へ.png", 1280, 720, 1, 1, 1, 1},//IMAGE_TUTORIAL_0_
{LOAD_TEXTURE, "image/チュートリアル/城のもとまで.png", 1280, 720, 1, 1, 1, 1},//IMAGE_TUTORIAL_1_
{LOAD_TEXTURE, "image/チュートリアル/jumping!!.png", 1280, 720, 1, 1, 1, 1},//IMAGE_TUTORIAL_2_
{LOAD_TEXTURE, "image/チュートリアル/遭遇.png", 1280, 720, 1, 1, 1, 1},//IMAGE_TUTORIAL_3_
{LOAD_TEXTURE, "image/チュートリアル/let's蘇生.png", 1280, 720, 1, 1, 1, 1},//IMAGE_TUTORIAL_4_
{LOAD_TEXTURE, "image/チュートリアル/ガード説明.png", 1280, 720, 1, 1, 1, 1},//IMAGE_TUTORIAL_5_
{LOAD_TEXTURE, "image/チュートリアル/回避説明.png", 1280, 720, 1, 1, 1, 1},//IMAGE_TUTORIAL_6_
{LOAD_TEXTURE, "image/チュートリアル/後は適当に.png", 1280, 720, 1, 1, 1, 1},//IMAGE_TUTORIAL_7_

{LOAD_TEXTURE, "image/in_game_ui/hp_meter_player.png", -1, -1, 1, 1, 1, 1},//IMAGE_HP_METER,
{LOAD_TEXTURE, "image/in_game_ui/HPBAR1P.png", -1, -1, 1, 1, 1, 1},//IMAGE_HP_BAR_FRAME_1P,
{LOAD_TEXTURE, "image/in_game_ui/HPBAR2P.png", -1, -1, 1, 1, 1, 1},//IMAGE_HP_BAR_FRAME_2P,
{LOAD_TEXTURE, "image/in_game_ui/拠点ｈｐゲージ○.png", -1, -1, 1, 1, 1, 1},//IMAGE_BASE_HP_FRAME,
{LOAD_TEXTURE, "image/in_game_ui/拠点ｈｐゲージ針.png", -1, -1, 1, 1, 1, 1},//IMAGE_BASE_HP_NEEDLE,
{LOAD_TEXTURE, "image/in_game_ui/ジャックの顔.png", -1, -1, 1, 1, 1, 1},//IMAGE_1P_FACE_ICON,
{LOAD_TEXTURE, "image/in_game_ui/レイスの顔.png", -1, -1, 1, 1, 1, 1},//IMAGE_2P_FACE_ICON,

};

static BlockImage *image_table_ptr = nullptr;

static const int INDEX_BUFFER_MAX = 256;   ///< 読み込むファイルの最大数
static const int HANDLE_BUFFER_MAX = 65536;  ///< 読み込む画像ブロックの最大数

/**
* @file  pg_basic05.cpp
* プログラム技術基礎第５回（アニメーションその２、状態遷移）
*
* @date 2014-06-05
* @version 2.0
*
* Copyright (C) Amusement Media Academy All rights Resved.
*/

#include "amgame.h"
#include "image_manager.h"
#include "game_state.h"

/**
*  使用する画像を全て読み込む。
*/
static void imageLoadFromFile();

/**
*  画像ファイル情報の取得
*
*  画像ファイル一覧(image_table_ptr)にアクセスする場合に使用します。
*/

const BlockImage* getImageFile(ImageFileId file_id){
	if(file_id < IMAGE_NONE || file_id >= IMAGE_MAX){
		file_id = IMAGE_NONE;
	}
	return &image_table_ptr[file_id];
}

/*
*  画像サイズの取得
*
*  サイズのみを取得したい場合はこちらを使います
*/

int getImageSize(ImageFileId file_id, int *width, int *height){
	const BlockImage *p = &image_table_ptr[file_id];
	*width = p->x_siz;
	*height = p->y_siz;
	return 0;
}

/**
*  使用する画像を全て読み込む。
*
*  image_table_ptrに登録した画像を全て読み込みます。
*/
void imageLoadFromFile(){
	unsigned int iMax = (int)IMAGE_MAX;
	unsigned int allImageNum = 0;
	unsigned int loadedImageNum = 0;
	for(unsigned int i = 0; i < iMax; i++){
		allImageNum += image_table_ptr[i].a_num;
	}
	for(unsigned int i = 0; i < iMax; i++){
		const BlockImage *bi = &image_table_ptr[i];
		imageMgrLoadFromFile(bi);
		//進行度を表示
		double progress = (double)loadedImageNum / (double)(allImageNum - 1);
		char message[99] = {'\0'};
		snprintf(message, 99, "Now Loading...%5.1lf%%", progress * 100.0);		int l = strlen(message);
		int x = DISP_W / 2;
		int y = DISP_H / 2;
		int strX = x - l * 8;
		int strY = y - 24;
		int color = GetColor(255, 255, 255);
		char *paramS = message;
		DrawFBox(strX, strY, DISP_W, strY + 16, GetColor(0, 0, 0));
		DrawString(strX, strY, paramS, color);
		int boxW = 1024;
		int boxH = 16;
		int boxX = x - boxW / 2;
		int boxY = y + 8;
		int boxWP = (int)(boxW * progress);
		DrawFBox(boxX, boxY, boxX + boxW, boxY + boxH, GetColor(0, 0, 0));
		DrawTBox(boxX, boxY, boxX + boxW, boxY + boxH, color);
		DrawFBox(boxX, boxY, boxX + boxWP, boxY + boxH, color);
		loadedImageNum += bi->a_num;
		if(!AM_ProcessEvent()){
			exit(0);
		}
		AM_Flip();
		if(bi->kind == LOAD_END){
			break;
		}
		//ここまで
	}
}


/**
 *  ファイル数ブロック数をカウントする。
 *
 *  g_index_bufferにg_handle_bufferへの添え字を格納する。
 *  ハンドルの総数(g_index_count)をカウントする。
 *  @param a_num 追加したブロック数
 */
static void ImageMgrCountUp(int block_count);

static int g_index_buffer[INDEX_BUFFER_MAX];  ///< ファイル毎のハンドル格納位置バッファ
static int g_index_count = 0;                 ///< ハンドル格納位置のカウンタ
static int g_handle_buffer[HANDLE_BUFFER_MAX];///< 画像ハンドル用バッファ
static int g_handle_count = 0;                ///< 画像ハンドルのカウンタ

/**
 *  画像ファイル管理の初期化
 */
void imageMgrInit(){
	g_index_count = 0;
	g_handle_count = 0;
	image_table_ptr = (BlockImage*)g_image_tble;
	imageLoadFromFile();
}
/**
 *  画像ファイルの読み込み
 */
void imageMgrLoadFromFile(const BlockImage *bi){
	// 種別に合わせた画像の読み込み
	switch(bi->kind){
	case LOAD_TEXTURE:
		g_handle_buffer[g_handle_count] = LoadTexture(bi->file);
		ImageMgrCountUp(bi->a_num);
		break;
	case LOAD_BLK_TEXTURE:
		LoadBlkTexture(bi->file, bi->x_siz, bi->y_siz,
					   bi->x_num, bi->y_num, bi->a_num,
					   &g_handle_buffer[g_handle_count]);
		ImageMgrCountUp(bi->a_num);
		break;
	case LOAD_BLK_TEXTURE_LR:
		LoadBlkTextureLR(bi->file, bi->x_siz, bi->y_siz,
						 bi->x_num, bi->y_num, bi->a_num,
						 &g_handle_buffer[g_handle_count]);
		ImageMgrCountUp(bi->a_num * 2);
		break;
	default:
		break;
	}
}

/**
 *  画像ハンドルの取得
 */
int imageMgrGetHandle(int file_no, int block_no){

	int index = g_index_buffer[file_no] + block_no;

	int handle = g_handle_buffer[index];

	return handle;
}
/**
 *  ファイル数ブロック数をカウントする。
 */
void ImageMgrCountUp(int block_count){
	// ファイル毎のハンドル格納位置を記憶
	g_index_buffer[g_index_count] = g_handle_count;
	// 読み込んだブロックを数える
	g_handle_count += block_count;
	// 読み込んだファイルを数える
	g_index_count += 1;
}
