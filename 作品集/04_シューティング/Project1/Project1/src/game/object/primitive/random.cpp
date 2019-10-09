#include <stdlib.h>
#include <time.h>

static void InitRandom()
{
	static bool initialized = false;
	if (initialized == false)
	{
		srand((unsigned int)time(nullptr));
		initialized = true;
	}
}

extern int RandomMax()
{
	return RAND_MAX;
}

extern int Random()
{
	InitRandom();
	return rand();
}

extern double UniformRandom()
{
	return (double)(Random()) / (double)(RandomMax());
}
