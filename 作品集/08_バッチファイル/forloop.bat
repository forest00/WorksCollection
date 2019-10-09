@echo off
set /p loopCount="Input loop count : "
for /l %%i in (0,1,%loopCount%) do call argview.exe %%i
timeout 10
exit /b 0
