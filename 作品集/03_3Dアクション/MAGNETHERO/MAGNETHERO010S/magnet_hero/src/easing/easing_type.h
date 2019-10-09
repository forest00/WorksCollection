#pragma once

enum easing_type_t
{
	et_None,
	et_Linear,
	et_EaseInQuad,
	et_EaseOutQuad,
	et_EaseInOutQuad,
	et_EaseInCubic,
	et_EaseOutCubic,
	et_EaseInOutCubic,
	et_EaseInQuart,
	et_EaseOutQuart,
	et_EaseInOutQuart,
	et_EaseInQuint,
	et_EaseOutQuint,
	et_EaseInOutQuint,
	et_EaseInSine,
	et_EaseOutSine,
	et_EaseInOutSine,
	et_EaseInExpo,
	et_EaseOutExpo,
	et_EaseInOutExpo,
	et_EaseInCirc,
	et_EaseOutCirc,
	et_EaseInOutCirc,
	et_Max,
};

enum include_or_exclude_linear_t
{
	ioel_include_linear,
	ioel_exclude_linear,
};

/*
*@fn et_EaseInRandom
*@brief �����_���ȃC�[�Y�C��
*@param includion et_Linear���܂߂邩�ۂ�
*/
extern easing_type_t et_EaseInRandom(include_or_exclude_linear_t);

/*
*@fn et_EaseOutRandom
*@brief �����_���ȃC�[�Y�A�E�g
*@param includion et_Linear���܂߂邩�ۂ�
*/
extern easing_type_t et_EaseOutRandom(include_or_exclude_linear_t);

/*
*@fn et_EaseInOutRandom
*@brief �����_���ȃC�[�Y�C���A�E�g
*@param includion et_Linear���܂߂邩�ۂ�
*/
extern easing_type_t et_EaseInOutRandom(include_or_exclude_linear_t);

/*
*@fn call_easing
*@brief �C�[�W���O�^�C�v�ɉ����ăC�[�W���O�֐����Ăяo��
*@param type �C�[�W���O�^�C�v
*@param t �J�����g�^�C��
*@param b �J�n�l
*@param c �S�̂̕ω���
*@param d �f�����[�V����
*@return  �v�Z����
*/
extern double call_easing(easing_type_t type, double t, double b, double c, double d);
