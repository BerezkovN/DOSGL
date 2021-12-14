echo ///////////////////  > LOGS.TXT
echo //COMPILING TEST.EXE >> LOGS.TXT
echo ///////////////////  >> LOGS.TXT
echo /                 /  >> LOGS.TXT

tcc -I..\INCLUDE -ml test.cpp ..\BIN\DOSGL.LIB