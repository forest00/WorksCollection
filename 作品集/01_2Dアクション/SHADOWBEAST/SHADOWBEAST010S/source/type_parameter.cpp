
#include "easing.h"
#include "type_parameter.h"

//パラメータの更新
void ParameterUpdate(Parameter* prm, unsigned int current, unsigned int lifetime) {
	double start = prm->start;
	double goal = prm->stop - start;
	switch(prm->easing){
		case EASE_NONE:
			break;
		case EASE_LINEAR:
			prm->current = Linear(current, start, goal, lifetime);
			break;
		case EASE_IN_QUAD:
			prm->current = EaseInQuad(current, start, goal, lifetime);
			break;
		case EASE_OUT_QUAD:
			prm->current = EaseOutQuad(current, start, goal, lifetime);

			break;
		case EASE_INOUT_QUAD:
			prm->current = EaseInOutQuad(current, start, goal, lifetime);

			break;
		case EASE_IN_CUBIC:
			prm->current = EaseInCubic(current, start, goal, lifetime);

			break;
		case EASE_OUT_CUBIC:
			prm->current = EaseOutCubic(current, start, goal, lifetime);

			break;

		case EASE_INOUT_CUBIC:
			prm->current = EaseInOutCubic(current, start, goal, lifetime);

			break;
		case EASE_IN_QUART:
			prm->current = EaseInQuart(current, start, goal, lifetime);

			break;
		case EASE_OUT_QUART:
			prm->current = EaseOutQuart(current, start, goal, lifetime);

			break;
		case EASE_INOUT_QUART:
			prm->current = EaseInOutQuart(current, start, goal, lifetime);

			break;
		case EASE_IN_QUINT:
			prm->current = EaseInQuint(current, start, goal, lifetime);

			break;
		case EASE_OUT_QUINT:
			prm->current = EaseOutQuint(current, start, goal, lifetime);

			break;
		case EASE_INOUT_QUINT:
			prm->current = EaseInOutQuint(current, start, goal, lifetime);

			break;
		case EASE_IN_SINE:
			prm->current = EaseInSine(current, start, goal, lifetime);

			break;
		case EASE_OUT_SINE:
			prm->current = EaseOutSine(current, start, goal, lifetime);

			break;
		case EASE_INOUT_SINE:
			prm->current = EaseInOutSine(current, start, goal, lifetime);

			break;
		case EASE_IN_EXPO:
			prm->current = EaseInExpo(current, start, goal, lifetime);

			break;
		case EASE_OUT_EXPO:
			prm->current = EaseOutExpo(current, start, goal, lifetime);
			break;
		case EASE_INOUT_EXPO:
			prm->current = EaseInOutExpo(current, start, goal, lifetime);
			break;
		case EASE_IN_CIRC:
			prm->current = EaseInCirc(current, start, goal, lifetime);

			break;
		case EASE_OUT_CIRC:
			prm->current = EaseOutCirc(current, start, goal, lifetime);

			break;
		case EASE_INOUT_CIRC:
			prm->current = EaseInOutCirc(current, start, goal, lifetime);
			break;
		default:
			break;
	}
}