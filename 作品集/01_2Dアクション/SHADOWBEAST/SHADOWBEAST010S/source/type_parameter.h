#pragma once

//�p�[�e�B�N���Ǘ�

//�C�[�W���O���
typedef enum {
	EASE_NONE,
	EASE_LINEAR,
	EASE_IN_QUAD,
	EASE_OUT_QUAD,
	EASE_INOUT_QUAD,
	EASE_IN_CUBIC,
	EASE_OUT_CUBIC,
	EASE_INOUT_CUBIC,
	EASE_IN_QUART,
	EASE_OUT_QUART,
	EASE_INOUT_QUART,
	EASE_IN_QUINT,
	EASE_OUT_QUINT,
	EASE_INOUT_QUINT,
	EASE_IN_SINE,
	EASE_OUT_SINE,
	EASE_INOUT_SINE,
	EASE_IN_EXPO,
	EASE_OUT_EXPO,
	EASE_INOUT_EXPO,
	EASE_IN_CIRC,
	EASE_OUT_CIRC,
	EASE_INOUT_CIRC,
} EasingType;

//�p�����[�^
typedef struct{
	EasingType easing;///< �C�[�W���O���
	double current;	    ///< ���݂̒l
	double start;	    ///< �J�n�̒l
	double stop;	    ///< �I���̒l
}Parameter;

void ParameterUpdate(Parameter* prm, unsigned int current, unsigned int lifetime);