echo /////////////////// > LOGS.TXT
echo //COMPILING DOSGL.LIB >> LOGS.TXT
echo /////////////////// >> LOGS.TXT
echo /                 / >> LOGS.TXT

tcc -IINCLUDE -ml -c -nBIN SRC\dosgl.cpp SRC\vga.cpp >> LOGS.TXT
tlib /C BIN\DOSGL.LIB -+BIN\DOSGL.OBJ-+BIN\VGA.OBJ >> LOGS.TXT

echo /////////////////// >> LOGS.TXT
echo //COMPILING TEST.EXE >> LOGS.TXT
echo /////////////////// >> LOGS.TXT
echo /                 / >> LOGS.TXT

tcc -IINCLUDE -ml SRC\test.cpp BIN\DOSGL.LIB