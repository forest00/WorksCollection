#pragma once
//	
//	�~������uMagnet Hero�v
//	
//	�J�����̌^��`�ł�
//	
//	���Direct3D�̏����������Ă����K�v������܂�
//	
//	�쐬��	�X	�m�u
//	

//DirectX
#include <d3dx9.h>

//�J����
struct game_main_camera_t
{
	D3DXVECTOR3 _eye;							//���_�̈ʒu
	D3DXVECTOR3 _at;							//�����_�̈ʒu
	D3DXVECTOR3 _up;							//������̃x�N�g��(0,1,0)
	float _distance;							//�����_����̋���
	float _theta;								//�㉺�p�x
	float _phi;									//���E�p�x
	float _user_move_force;						//
	float _user_close_force;					//
	float _user_turning_force;					//
	float _user_move_weight;					//
	float _user_close_weight;					//
	float _user_turning_weight;					//
	D3DXVECTOR3 _user_move_result;				//
	float _user_close_result;					//
	float _user_theta_result;					//
	float _user_phi_result;						//
	D3DXVECTOR3 _user_move_accumulate;			//
	float _distance_upper;						//�����_���痣�������
	float _distance_lower;						//�����_���痣����鉺��
	float _theta_upper;							//�㉺�p�x�̏��
	float _theta_lower;							//�㉺�p�x�̉���
	float _shift_up;							//������ɏC�������
	float _move_time;							//
	float _move_power;							//
	D3DXVECTOR3 _true_eye;						//�{���̎��_�ʒu
	float _jump_point;							//�v���[���[���W�����v���Ȃ瑝���Ă����A��������Ȃ���Ό����Ă����l
};

//�J�����̃f�[�^�x�[�X
struct camera_database_t
{
	float _stage1_default_distance;
	float _stage1_default_phi;
	float _stage1_default_theta;
	float _user_move_force;
	float _user_close_force;
	float _user_turning_force;
	float _user_move_weight;
	float _user_close_weight;
	float _user_turning_weight;
	float _distance_upper;
	float _distance_lower;
	float _theta_upper;
	float _theta_lower;
	float _shift_up;
	float _move_time;
	float _move_power;
};