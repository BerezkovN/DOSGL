echo ///////////////////  > LOGS.TXT
echo //COMPILING TEST.EXE >> LOGS.TXT
echo ///////////////////  >> LOGS.TXT
echo /                 /  >> LOGS.TXT

REM -ml - uses large memory model (IMPORTANT)
REM -I 	- include folder

tcc -I..\INCLUDE -ml test.cpp ..\BIN\DOSGL.LIB >> LOGS.TXT