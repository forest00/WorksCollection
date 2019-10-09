#pragma once

//標準の<math.h>にある、M_PIなどのマクロを使えるようにした上で、includeするためのヘッダーです

#define _USE_MATH_DEFINES
#include <math.h>
#undef _USE_MATH_DEFINES

