#pragma once
//�Ə����킹������
//�쐬�ҁ@�X�@�m�u

//����̈ʒu
extern float default_aiming_cursor_pos();

//�����Ȉʒu
extern float invalid_aiming_cursor_pos();

//�Ə��J�[�\���̈ʒu������(�v���[���[���Ă�)
extern void aiming_cursor_pos_init();

//�X�V(�v���[���[���Ă�)
extern void aiming_cursor_pos_update(float target_x, float target_y, float target_z);

//�Ə��J�[�\���̃X�N���[����̈ʒu�𓾂�
extern void aiming_cursor_get_pos(float *x, float *y, float *z);

//�Ə��J�[�\���̃^�[�Q�b�g�̃X�N���[����̈ʒu�𓾂�
extern void aiming_cursor_get_target_pos(float *x, float *y, float *z);
