#pragma once
//	
//	�~������uMagnet Hero�v
//	
//	�쐬��	�X	�m�u
//	
//	�ŏI�X�V		2018/12/13
//	

#define input_define_safe_release(p)\
{\
	if(p)\
	{\
		(p)->Release();\
		(p) = nullptr;\
	}\
}
