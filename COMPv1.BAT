echo /////////////////// > LOGS.TXT
echo //COMPILING VGA.CPP >> LOGS.TXT
echo /////////////////// >> LOGS.TXT
echo /                 / >> LOGS.TXT

tcc -IINCLUDE -ml -c -nBIN SRC\vga.cpp >> LOGS.TXT


echo /////////////////// >> LOGS.TXT
echo //COMPILING MYSHADER.CPP >> LOGS.TXT
echo /////////////////// >> LOGS.TXT
echo /                 / >> LOGS.TXT

tcc -IINCLUDE -ml -c -nBIN SRC\myShader.cpp >> LOGS.TXT

echo /////////////////// >> LOGS.TXT
echo //COMPILING DOSGL.CPP >> LOGS.TXT
echo /////////////////// >> LOGS.TXT
echo /                 / >> LOGS.TXT

tcc -IINCLUDE -ml -c -nBIN SRC\dosgl.cpp >> LOGS.TXT