#pragma once

#include <time.h>




/*------------------------------------------------------------------------------*/
struct input_button_t
{
	time_t _count;		//押されている時間をカウント
	time_t _prev;		//1フレーム前のカウント
};
/*------------------------------------------------------------------------------*/





/*------------------------------------------------------------------------------*/
extern void input_button_t_init(input_button_t *button);

extern time_t input_button_t_count(const input_button_t *button);

extern bool input_button_t_trigger(const input_button_t *button);

extern bool input_button_t_repeat(const input_button_t *button);

extern bool input_button_t_release(const input_button_t *button);

extern void input_button_t_count_up_if(input_button_t *button, bool pushed);
/*------------------------------------------------------------------------------*/

