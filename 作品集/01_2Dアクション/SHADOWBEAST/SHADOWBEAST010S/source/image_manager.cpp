/**
 * @file  image_manager.cpp
 * �摜�t�@�C���ǂݍ��݈ꊇ�Ǘ�
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

 // �g�p����摜�t�@�C���ꗗ(�摜�t�@�C���ԍ��ƍ��킹��)
static const BlockImage g_image_tble[IMAGE_MAX] = {
	{LOAD_TEXTURE, "", 0, 0, 1, 1, 1, 1},
{LOAD_BLK_TEXTURE, "image/�ē��N��/team_logo.png", 400, 200, 1, 2, 2, 60},

{LOAD_TEXTURE, "image/�^�C�g��/title_back.png", 1280, 720, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/�^�C�g��/red_eyes.png", 1280, 720, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/�^�C�g��/red_eyes2.png", 1280, 720, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/�^�C�g��/shadow_mist.png", 1024, 600, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/�^�C�g��/shadow_mist2.png", 1024, 600, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/�^�C�g��/title_logo.png", 1024, 600, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/�^�C�g��/press any button.png", 300, 50, 1, 1, 1, 0},

{LOAD_TEXTURE, "image/�X�^�b�t���[��/�X�^�b�t���[��.png", 1024, 600, 1, 1, 1, 0},

{LOAD_TEXTURE, "image/���[�h�Z���N�g/start.png", 146, 68, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/���[�h�Z���N�g/continue.png", 146, 68, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/���[�h�Z���N�g/option.png", 146, 68, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/���[�h�Z���N�g/exit.png", 146, 68, 1, 1, 1, 1},

{LOAD_TEXTURE, "image/�I�v�V����/sadsf.png", 146, 68, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/�I�v�V����/Luminocity.png", 146, 68, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/�I�v�V����/�P�x�I��.png", 65, 65, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/�I�v�V����/�P�x�Q�[�W.png", 65, 65, 1, 1, 1, 0},

{LOAD_TEXTURE, "image/�w�i/white_back.png", 1280, 720, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/�w�i/game_play_back.png", 1280, 720, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/�w�i/sea.png", 1280, 720, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/�w�i/pause_back.png", 1280, 720, 1, 1, 1, 1},

{LOAD_TEXTURE, "image/�����f��/gameclear.png", 1280, 720, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/�����f��/gameover.png", 1280, 720, 1, 1, 1, 1},

{LOAD_TEXTURE, "image/�����f��/OP�B.png", 400, 640, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/�����f��/OP�A.png", 400, 640, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/�����f��/OP�@.png", 400, 640, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/�����f��/OP�C.png", 400, 640, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/�����f��/OP�D.png", 400, 640, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/�����f��/OP�E.png", 400, 640, 1, 1, 1, 0},

{LOAD_TEXTURE, "image/�����f��/�߂�.png", 400, 640, 1, 1, 1, 0},

{LOAD_TEXTURE, "image/�`���[�g���A���摜/�`���[�g���A��1.png", 600, 60, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/�`���[�g���A���摜/�`���[�g���A��2.png", 600, 60, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/�`���[�g���A���摜/�`���[�g���A��3.png", 600, 60, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/�`���[�g���A���摜/�`���[�g���A��4.png", 600, 60, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/�`���[�g���A���摜/�`���[�g���A��5.png", 600, 60, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/�`���[�g���A���摜/�`���[�g���A��6.png", 600, 60, 1, 1, 1, 0},

{LOAD_TEXTURE, "image/�X�e�[�W�Z���N�g/tutorial.png", 180, 32, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/�X�e�[�W�Z���N�g/stage_1.png", 180, 32, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/�X�e�[�W�Z���N�g/stage_2.png", 180, 32, 1, 1, 1, 1},

{LOAD_TEXTURE, "image/�w�i/title_1.png", 1040, 480, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/�I���nUI/�`���[�g���A�����X�e�[�W�Z���N�g.png", 800, 300, 1, 1, 1, 1},

{LOAD_TEXTURE, "image/�����f��/STAGE_SELECT_BACK.png", 3000, 500, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/�����f��/A_BACK.png", 1560, 500, 1, 1, 1, 0},

{LOAD_TEXTURE, "image/�����f��/OP2.png", 600, 48, 1, 1, 1, 0},

{LOAD_BLK_TEXTURE, "image/effect/enemy_hole.png", 200, 200, 10, 2, 20, 20 * 3},
{LOAD_TEXTURE, "image/effect/�_/�_�P�̔���.png", 83, 39, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/effect/�_/�_�W���̔�����.png", 251, 172, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/effect/�_/�_�W���̔�����.png", 240, 163, 1, 1, 1, 1},

{LOAD_TEXTURE, "image/effect/������.png", 8, 8, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/effect/�ł̋�.png", 16, 16, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/effect/��.png", 160, 160, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/effect/���G�t�F�N�g.png", 100, 120, 1, 1, 1, 1},

{LOAD_TEXTURE, "image/effect/dark.png", 1280, 720, 1, 1, 1, 1},
{LOAD_BLK_TEXTURE, "image/effect/�܃G�t�F�N�g�A��/clow_sheet.png", 400, 600, 9, 1, 9, 60},
{LOAD_TEXTURE, "image/�ē��N��/stonebrick1 (4).png", 3000, 128, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/�ē��N��/Charge.png", 300, 300, 1, 1, 1, 0},
{LOAD_TEXTURE, "image/��/��̂�.png", 1280, 720, 1, 1, 1, 0},
{LOAD_BLK_TEXTURE, "image/��/�ԁ@���̂݁@�����/castle_red_break.png", 1280, 720, 13, 1, 13, 60},
{LOAD_BLK_TEXTURE, "image/��/�΁@���̂݁@�����/castle_green_break_sheet.png", 1280, 720, 16, 1, 16, 60},
{LOAD_BLK_TEXTURE, "image/��/�@���̂݁@�����/castle_blue_break_sheet.png", 1280, 720, 14, 1, 14, 60},

{LOAD_TEXTURE, "image/�D/�D�ʏ�.png", 1280, 720, 1, 1, 1, 60},
{LOAD_BLK_TEXTURE, "image/�D/�D���A�j��.png", 1280, 720, 5, 2, 10, 60},

{LOAD_TEXTURE, "image/�ē��N��/heart1.png", 50, 50, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/�ē��N��/kanban.png", 200, 200, 1, 1, 1, 1},

{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/jack/jack stand by_sheet.png", 256, 256, 6, 1, 6, 6 * 4},		//IMAGE_1P_IDLE
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/jack/jack run�@_sheet.png", 256, 256, 12, 1, 12, 12 * 4},		//IMAGE_1P_WALK
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/jack/jack jump_sheet.png", 256, 256, 11, 1, 11, 60},			//IMAGE_1P_JUMP,
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/jack/jack atk_sheet.png", 256, 256, 15, 1, 15, 15 * 2},		//IMAGE_1P_MAIN_ATTACK1,
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/jack/jack atk_sheet.png", 256, 256, 15, 1, 15, 15 * 2},		//IMAGE_1P_MAIN_ATTACK2,
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/jack/jack atk_sheet.png", 256, 256, 15, 1, 15, 15 * 2},		//IMAGE_1P_MAIN_ATTACK3,
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/jack/jack sab 1.png", 256, 256, 13, 1, 13, 13},			//IMAGE_1P_SUB_ATTACK_BEFORE1,
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/jack/jack sab 2.png", 256, 256, 13, 1, 13, 13},			//IMAGE_1P_SUB_ATTACK_AFTER,
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/jack/jack gad_sheet.png", 256, 256, 2, 1, 2, 2 * 6},		//IMAGE_1P_GUARD,
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/jack/jack avoid_sheet.png", 256, 256, 20, 1, 20, 42},		//IMAGE_1P_AVOID,
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/jack/jack damage.png", 256, 256, 1, 1, 1, 32},			//IMAGE_1P_DAMAGE,
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/jack/jack down_sheet.png", 256, 256, 3, 1, 3, 3 * 12},		//IMAGE_1P_KNOCK_OUT,
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/jack/jack dow.png", 256, 256, 1, 1, 1, 30},			//IMAGE_1P_DEAD,
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/jack/jack sosei_sheet.png", 256, 256, 13, 1, 13, 13 * 3},		//IMAGE_1P_RESURRECTION,
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/jack/chain.png", 400, 50, 30, 1, 30, 30},
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/jack/jack_atk_effect.png", 512, 512, 5, 1, 5, 32},
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/jack/jack_sub_atk_effect.png", 200, 300, 3, 2, 6, 6 * 2},

{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/���C�X/_2_reisu stanby.png", 256, 256, 19, 1, 19, 60},
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/���C�X/_2_reisu run.png", 256, 256, 13, 1, 13, 50},
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/���C�X/_2_reisu jump.png", 256, 256, 16, 1, 16, 60},
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/���C�X/_2_reisu atk.png", 256, 256, 9, 1, 9, 9 * 4},
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/���C�X/_2_reisu sab atk.png", 256, 256, 16, 1, 16, 16 * 4},
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/���C�X/_2_reisu guard.png", 256, 256, 5, 1, 5, 30},
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/���C�X/_2_reisu jump.png", 256, 256, 16, 1, 16, 32},
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/���C�X/_2_reisu _dummy_damage.png", 256, 256, 4, 1, 4, 30},
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/���C�X/_2_reisu knock down.png", 256, 256, 20, 1, 20, 40},
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/���C�X/_2_reisu _lie_down.png", 256, 256, 4, 1, 4, 30},
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/���C�X/_2_reisu sosei.png", 256, 256, 14, 1, 14, 14 * 3},
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/���C�X/_2_reisu sab atk.png", 256, 256, 16, 1, 16, 16 * 4},
{LOAD_TEXTURE, "image/�v���C���[�L����/���C�X/reisu_atk_tama.png", 200, 100, 1, 1, 1, 1},
{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/���C�X/reisu_sub_atk_effect.png", 350, 300, 9, 1, 9, 9 * 2},
{LOAD_TEXTURE, "image/�v���C���[�L����/���C�X/reisu_atk_tama.png", 200, 100, 1, 1, 1, 1},

{LOAD_BLK_TEXTURE, "image/�G�L����/�S�u����/nuru_wait_sheet.png", 130, 130, 4, 1, 4, 48},
{LOAD_BLK_TEXTURE, "image/�G�L����/�S�u����/nuru_walk_sheet.png", 130, 130, 4, 1, 4, 48},
{LOAD_BLK_TEXTURE, "image/�G�L����/�S�u����/nuru_damage_0000_sheet.png", 130, 130, 3, 1, 3, 24},
{LOAD_BLK_TEXTURE, "image/�G�L����/�S�u����/nuru_shin_attack.png", 160, 160, 20, 1, 19, 72},
{LOAD_BLK_TEXTURE, "image/�G�L����/���C�o�[��/nuri-wait.png", 130, 130, 4, 1, 4, 48},
{LOAD_BLK_TEXTURE, "image/�G�L����/���C�o�[��/nuri-wait.png", 130, 130, 4, 1, 4, 48},
{LOAD_BLK_TEXTURE, "image/�G�L����/���C�o�[��/nuri-dm.png", 130, 130, 3, 1, 3, 24},
{LOAD_BLK_TEXTURE, "image/�G�L����/���C�o�[��/nuri-at.png", 130, 130, 7, 1, 7, 48},
{LOAD_BLK_TEXTURE, "image/�G�L����/�X�l�[�N/nira_walk3.png", 240, 240, 3, 1, 3, 48},
{LOAD_BLK_TEXTURE, "image/�G�L����/�X�l�[�N/nira_walk3.png", 240, 240, 3, 1, 3, 48},
{LOAD_BLK_TEXTURE, "image/�G�L����/�X�l�[�N/nira_dm3.png", 240, 240, 4, 1, 4, 24},
{LOAD_BLK_TEXTURE, "image/�G�L����/�X�l�[�N/nira_at3_.png", 240, 240, 24, 1, 24, 48},
{LOAD_BLK_TEXTURE, "image/�{�X/���l/superwalk_nuru.png", 700, 700, 23, 1, 23, 60},
{LOAD_BLK_TEXTURE, "image/�{�X/���l/superwalk_nuru.png", 700, 700, 23, 1, 23, 60},
{LOAD_BLK_TEXTURE, "image/�{�X/���l/super.nuru.attack.png", 750, 700, 19, 2, 21, 66},
{LOAD_BLK_TEXTURE, "image/�{�X/���l/super.nuru_nagiat.png", 700, 700, 12, 1, 12, 60},
{LOAD_BLK_TEXTURE, "image/�{�X/���l/super.nuru_tkr.png", 700, 700, 13, 1, 13, 120},
{LOAD_BLK_TEXTURE, "image/�{�X/���l/superwalk_nuru.png", 700, 700, 23, 1, 23, 24},
{LOAD_BLK_TEXTURE, "image/�{�X/�E�~�w�r/_2_hebi stanby0000_sheet.png", 250, 250, 13, 1, 13, 13 * 4},
{LOAD_BLK_TEXTURE, "image/�{�X/�E�~�w�r/_2_hebi work0000_sheet.png", 250, 250, 22, 1, 21, 21 * 4},
{LOAD_BLK_TEXTURE, "image/�{�X/�E�~�w�r/_2_hebi eatk0000_sheet.png", 250, 250, 10, 8, 71, 71},
{LOAD_BLK_TEXTURE, "image/�{�X/�E�~�w�r/_2_hebi last sparet 20000_sheet.png", 250, 250, 17, 3, 51, 51},
{LOAD_BLK_TEXTURE, "image/�{�X/�E�~�w�r/_2_hebi geroatk0000_sheet.png", 250, 250, 31, 1, 31, 31},
{LOAD_BLK_TEXTURE, "image/�{�X/�E�~�w�r/_2_hebi _damage__sheet.png", 250, 250, 2, 1, 2, 2 * 4},
{LOAD_BLK_TEXTURE, "image/�{�X/�E�~�w�r/_2_hebi down0000_sheet.png", 250, 250, 14, 1, 14, 14 * 4},

{LOAD_TEXTURE, "image/effect/��/�������ʒu��������.png", 500, 2000, 1, 1, 1, 1},
{LOAD_BLK_TEXTURE, "image/effect/��/thunder_anim.png", 600, 720, 9, 1, 9, -1},

{LOAD_TEXTURE, "image/���f��/1p_name_prate.png", 160, 100, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/���f��/2p_name_prate.png", 160, 100, 1, 1, 1, 1},


{LOAD_TEXTURE, "image/���U���g�摜/window_frame.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/���U���g�摜/���S��.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/���U���g�摜/������.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/���U���g�摜/�ϋv��.png", -1, -1, 1, 1, 1, 1},

{LOAD_TEXTURE, "image/���U���g�摜/����/0.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/���U���g�摜/����/1.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/���U���g�摜/����/2.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/���U���g�摜/����/3.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/���U���g�摜/����/4.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/���U���g�摜/����/5.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/���U���g�摜/����/6.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/���U���g�摜/����/7.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/���U���g�摜/����/8.png", -1, -1, 1, 1, 1, 1},
{LOAD_TEXTURE, "image/���U���g�摜/����/9.png", -1, -1, 1, 1, 1, 1},


{LOAD_BLK_TEXTURE, "image/�v���C���[�L����/jack/blood_anim.png", 150, 150, 11, 1, 11, 22},


{LOAD_TEXTURE, "image/�`���[�g���A��/�g�̂�.png", 1280, 720, 1, 1, 1, 1},//IMAGE_TUTORIAL_F_
{LOAD_TEXTURE, "image/�`���[�g���A��/�悤�������E��.png", 1280, 720, 1, 1, 1, 1},//IMAGE_TUTORIAL_0_
{LOAD_TEXTURE, "image/�`���[�g���A��/��̂��Ƃ܂�.png", 1280, 720, 1, 1, 1, 1},//IMAGE_TUTORIAL_1_
{LOAD_TEXTURE, "image/�`���[�g���A��/jumping!!.png", 1280, 720, 1, 1, 1, 1},//IMAGE_TUTORIAL_2_
{LOAD_TEXTURE, "image/�`���[�g���A��/����.png", 1280, 720, 1, 1, 1, 1},//IMAGE_TUTORIAL_3_
{LOAD_TEXTURE, "image/�`���[�g���A��/let's�h��.png", 1280, 720, 1, 1, 1, 1},//IMAGE_TUTORIAL_4_
{LOAD_TEXTURE, "image/�`���[�g���A��/�K�[�h����.png", 1280, 720, 1, 1, 1, 1},//IMAGE_TUTORIAL_5_
{LOAD_TEXTURE, "image/�`���[�g���A��/������.png", 1280, 720, 1, 1, 1, 1},//IMAGE_TUTORIAL_6_
{LOAD_TEXTURE, "image/�`���[�g���A��/��͓K����.png", 1280, 720, 1, 1, 1, 1},//IMAGE_TUTORIAL_7_

{LOAD_TEXTURE, "image/in_game_ui/hp_meter_player.png", -1, -1, 1, 1, 1, 1},//IMAGE_HP_METER,
{LOAD_TEXTURE, "image/in_game_ui/HPBAR1P.png", -1, -1, 1, 1, 1, 1},//IMAGE_HP_BAR_FRAME_1P,
{LOAD_TEXTURE, "image/in_game_ui/HPBAR2P.png", -1, -1, 1, 1, 1, 1},//IMAGE_HP_BAR_FRAME_2P,
{LOAD_TEXTURE, "image/in_game_ui/���_�����Q�[�W��.png", -1, -1, 1, 1, 1, 1},//IMAGE_BASE_HP_FRAME,
{LOAD_TEXTURE, "image/in_game_ui/���_�����Q�[�W�j.png", -1, -1, 1, 1, 1, 1},//IMAGE_BASE_HP_NEEDLE,
{LOAD_TEXTURE, "image/in_game_ui/�W���b�N�̊�.png", -1, -1, 1, 1, 1, 1},//IMAGE_1P_FACE_ICON,
{LOAD_TEXTURE, "image/in_game_ui/���C�X�̊�.png", -1, -1, 1, 1, 1, 1},//IMAGE_2P_FACE_ICON,

};

static BlockImage *image_table_ptr = nullptr;

static const int INDEX_BUFFER_MAX = 256;   ///< �ǂݍ��ރt�@�C���̍ő吔
static const int HANDLE_BUFFER_MAX = 65536;  ///< �ǂݍ��މ摜�u���b�N�̍ő吔

/**
* @file  pg_basic05.cpp
* �v���O�����Z�p��b��T��i�A�j���[�V�������̂Q�A��ԑJ�ځj
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
*  �g�p����摜��S�ēǂݍ��ށB
*/
static void imageLoadFromFile();

/**
*  �摜�t�@�C�����̎擾
*
*  �摜�t�@�C���ꗗ(image_table_ptr)�ɃA�N�Z�X����ꍇ�Ɏg�p���܂��B
*/

const BlockImage* getImageFile(ImageFileId file_id){
	if(file_id < IMAGE_NONE || file_id >= IMAGE_MAX){
		file_id = IMAGE_NONE;
	}
	return &image_table_ptr[file_id];
}

/*
*  �摜�T�C�Y�̎擾
*
*  �T�C�Y�݂̂��擾�������ꍇ�͂�������g���܂�
*/

int getImageSize(ImageFileId file_id, int *width, int *height){
	const BlockImage *p = &image_table_ptr[file_id];
	*width = p->x_siz;
	*height = p->y_siz;
	return 0;
}

/**
*  �g�p����摜��S�ēǂݍ��ށB
*
*  image_table_ptr�ɓo�^�����摜��S�ēǂݍ��݂܂��B
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
		//�i�s�x��\��
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
		//�����܂�
	}
}


/**
 *  �t�@�C�����u���b�N�����J�E���g����B
 *
 *  g_index_buffer��g_handle_buffer�ւ̓Y�������i�[����B
 *  �n���h���̑���(g_index_count)���J�E���g����B
 *  @param a_num �ǉ������u���b�N��
 */
static void ImageMgrCountUp(int block_count);

static int g_index_buffer[INDEX_BUFFER_MAX];  ///< �t�@�C�����̃n���h���i�[�ʒu�o�b�t�@
static int g_index_count = 0;                 ///< �n���h���i�[�ʒu�̃J�E���^
static int g_handle_buffer[HANDLE_BUFFER_MAX];///< �摜�n���h���p�o�b�t�@
static int g_handle_count = 0;                ///< �摜�n���h���̃J�E���^

/**
 *  �摜�t�@�C���Ǘ��̏�����
 */
void imageMgrInit(){
	g_index_count = 0;
	g_handle_count = 0;
	image_table_ptr = (BlockImage*)g_image_tble;
	imageLoadFromFile();
}
/**
 *  �摜�t�@�C���̓ǂݍ���
 */
void imageMgrLoadFromFile(const BlockImage *bi){
	// ��ʂɍ��킹���摜�̓ǂݍ���
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
 *  �摜�n���h���̎擾
 */
int imageMgrGetHandle(int file_no, int block_no){

	int index = g_index_buffer[file_no] + block_no;

	int handle = g_handle_buffer[index];

	return handle;
}
/**
 *  �t�@�C�����u���b�N�����J�E���g����B
 */
void ImageMgrCountUp(int block_count){
	// �t�@�C�����̃n���h���i�[�ʒu���L��
	g_index_buffer[g_index_count] = g_handle_count;
	// �ǂݍ��񂾃u���b�N�𐔂���
	g_handle_count += block_count;
	// �ǂݍ��񂾃t�@�C���𐔂���
	g_index_count += 1;
}
