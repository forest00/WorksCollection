#pragma once
//	
//	“~Šú§ìuMagnet Herov
//	
//	ì¬ŽÒ	X	mŽu
//	
//	ÅIXV		2018/12/13
//	

#define input_define_safe_release(p)\
{\
	if(p)\
	{\
		(p)->Release();\
		(p) = nullptr;\
	}\
}
