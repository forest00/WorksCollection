//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	
//	最終更新		2018/12/13
//	

//親ヘッダー
#include "input_button.h"


/*------------------------------------------------------------------------------*/
extern void input_button_t_init(input_button_t *button)
{
	button->_count = button->_prev = 0;
}

extern time_t input_button_t_count(const input_button_t *button)
{
	return button->_count;
}

extern bool input_button_t_trigger(const input_button_t *button)
{
	return button->_count && !button->_prev;
}

extern bool input_button_t_repeat(const input_button_t *button)
{
	return button->_count && button->_prev;
}

extern bool input_button_t_release(const input_button_t *button)
{
	return !button->_count && button->_prev;
}

extern void input_button_t_count_up_if(input_button_t *button, bool pushed)
{
	button->_prev = button->_count;
	if (pushed)
	{
		button->_count++;
	}
	else
	{
		button->_count = 0;
	}
}
/*------------------------------------------------------------------------------*/
