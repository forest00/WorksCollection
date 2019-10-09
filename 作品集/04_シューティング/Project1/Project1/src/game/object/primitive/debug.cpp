#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "screen.h"

#define DebugInfoCapacity	(20)
#define TmpSize	FILENAME_MAX

static int DebugInfoNum = 0;

static int DebugInfoCount = 0;

static int DebugInfoStart = 0;

static char *DebugInfoList[DebugInfoCapacity];

static char tmp[TmpSize];

extern void RegisterDebug(const char *caption)
{
	int index = DebugInfoCount % DebugInfoCapacity;
	free(DebugInfoList[index]);
	DebugInfoList[index] = _strdup(caption);
	DebugInfoNum++;
	DebugInfoCount++;
	if (DebugInfoCount >= DebugInfoCapacity * 2)
	{
		DebugInfoCount = DebugInfoCapacity;
	}
	DebugInfoStart = DebugInfoCount - DebugInfoCapacity;
	if (DebugInfoStart < 0)
	{
		DebugInfoStart = 0;
	}
	if (DebugInfoNum >= DebugInfoCapacity)
	{
		DebugInfoNum = DebugInfoCapacity;
	}
}

extern void RegisterDebug(const char *caption, char n)
{
	snprintf(tmp, TmpSize, "%s : %c", caption, n);
	RegisterDebug(tmp);
}

extern void RegisterDebug(const char *caption, const char *n)
{
	snprintf(tmp, TmpSize, "%s : %s", caption, n);
	RegisterDebug(tmp);
}

extern void RegisterDebug(const char *caption, int n)
{
	snprintf(tmp, TmpSize, "%s : %d", caption, n);
	RegisterDebug(tmp);
}

extern void RegisterDebug(const char *caption, size_t n)
{
	snprintf(tmp, TmpSize, "%s : %u", caption, (unsigned int)(n));
	RegisterDebug(tmp);
}

extern void RegisterDebug(const char *caption, float n)
{
	snprintf(tmp, TmpSize, "%s : %f", caption, n);
	RegisterDebug(tmp);
}

extern void RegisterDebug(const char *caption, double n)
{
	snprintf(tmp, TmpSize, "%s : %lf", caption, n);
	RegisterDebug(tmp);
}

extern void ClearDebug()
{
	for (int i = 0; i < DebugInfoNum; i++)
	{
		int index = (DebugInfoStart + i) % DebugInfoCapacity;
		free(DebugInfoList[index]);
		DebugInfoList[index] = nullptr;
	}
	DebugInfoNum = 0;
	DebugInfoCount = 0;
	DebugInfoStart = 0;
}

extern void DrawDebug()
{
	for (int i = 0; i < DebugInfoNum; i++)
	{
		int index = (DebugInfoStart + i) % DebugInfoCapacity;
		WriteScreen(0, i, DebugInfoList[index]);
	}
}
