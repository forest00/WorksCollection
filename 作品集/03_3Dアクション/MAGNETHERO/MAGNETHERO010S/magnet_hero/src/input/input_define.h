#pragma once
//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	
//	最終更新		2018/12/13
//	

#define input_define_safe_release(p)\
{\
	if(p)\
	{\
		(p)->Release();\
		(p) = nullptr;\
	}\
}
