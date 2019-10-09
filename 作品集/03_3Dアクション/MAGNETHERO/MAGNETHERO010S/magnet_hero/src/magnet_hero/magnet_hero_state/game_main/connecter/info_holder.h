#pragma once
//�v���[���[�̏�����x�Ƃ��Ă����ĕʂ̏ꏊ�ɓn�����߂ɑ��݂���w�b�_�[�ł�
//�K�v�ł���΃v���[���[�ȊO�̏��������ł���肵�܂�
//�����Œ񋟂����@�\�͂��ׂăQ�[���̃��C���V�[���݂̂Ŏg���܂�
//�쐬�ҁ@�X�m�u

//DirectX�̃w�b�_�[
#include <d3dx9.h>


//�v���[���[�̈ʒu��ۑ�
extern void holder_player_position_set(const D3DXVECTOR3 *position);
//�v���[���[�̈ʒu���擾
extern void holder_player_position_get(D3DXVECTOR3 *position);


//�v���[���[�̔�΂���̈ʒu�E�p�x��ۑ�
extern void holder_player_magnet_hand_position_set(bool do_draw, time_t frame, const D3DXVECTOR3 *position);
//�v���[���[�̔�΂���̈ʒu�E�p�x���擾
extern void holder_player_magnet_hand_position_get(bool *do_draw, time_t *frame, D3DXVECTOR3 *position);


//�v���[���[�̉e�̈ʒu��ۑ�
extern void holder_player_shadow_position_set(const D3DXVECTOR3 *position);
//�v���[���[�̉e�̈ʒu���擾
extern void holder_player_shadow_position_get(D3DXVECTOR3 *position);


//�v���[���[�̉e�̖@����ۑ�
extern void holder_player_shadow_normal_set(const D3DXVECTOR3 *normal);
//�v���[���[�̉e�̖@�����擾
extern void holder_player_shadow_normal_get(D3DXVECTOR3 *normal);


//�v���[���[�̐L�΂��r�[���ƃI�u�W�F�N�g�������������Ԃ�ۑ�(�J�����p)
extern void player_beam_hit_record();
//�v���[���[�̐L�΂��r�[���ƃI�u�W�F�N�g�������������Ԃ��擾(�J�����p)
extern DWORD player_beam_hit_get();


//�v���[���[�������������Ԃ�ۑ�
extern void holder_falled_player_record();
//�v���[���[�������������Ԃ��擾
extern DWORD holder_falled_player_get();


//�v���[���[���r�[����ł����瓖���邩�ǂ�������ς݂̒l��n��
extern void player_beam_hit_object_set(bool flag);
//�v���[���[���r�[����ł����瓖���邩�ǂ�������ς݂̒l��Ԃ�
extern bool player_beam_hit_object_get();


//���̊֐��͖��t���[���Ăяo���A�v���[���[�����͈ڍs��ԂȂ�true��n���A�����łȂ����false��n���Ȃ���΂����Ȃ�
extern void ask_player_state_is_sticking_migration(bool is);
//�v���[���[�����͈ڍs��ԂȂ�true��Ԃ�
extern bool player_state_is_sticking_migration();


//�v���[���[���W�����v���Ȃ�true�ŌĂԁA�����łȂ����false�ŌĂ�
extern void player_is_jumping_set(bool is);
//�v���[���[���W�����v���Ȃ�true���Ԃ�
extern bool player_is_jumping_get();


//�v���[���[Y���W�ړ��ʂ𖈃t���[���n��
extern void player_jump_move_y_set(float y);
//�v���[���[Y���W�ړ���
extern float player_jump_move_y_get();


//�v���[���[���W�����v�����u�ԂɌĂ�(���Ԃ�b�P�ʂœn��)
extern void player_do_jump_moment_set(float moment);
//�v���[���[���W�����v�����u�Ԃ̎��Ԃ�b�P�ʂŕԂ�
extern float player_do_jump_moment_get();
