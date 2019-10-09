//	
//	“~Šú§ìuMagnet Herov
//	
//	ì¬Ò	X	mu
//	
//	ÅIXV		2018/12/13
//	

//eƒwƒbƒ_[
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
