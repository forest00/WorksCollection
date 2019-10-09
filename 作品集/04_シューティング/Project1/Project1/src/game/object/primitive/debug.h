#pragma once

extern void RegisterDebug(const char *caption);
extern void RegisterDebug(const char *caption, char n);
extern void RegisterDebug(const char *caption, const char *n);
extern void RegisterDebug(const char *caption, int n);
extern void RegisterDebug(const char *caption, size_t n);
extern void RegisterDebug(const char *caption, float n);
extern void RegisterDebug(const char *caption, double n);
extern void ClearDebug();
extern void DrawDebug();
