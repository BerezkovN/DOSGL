echo /////////////////// > LOGS.TXT
echo //COMPILING DOSGL.LIB >> LOGS.TXT
echo /////////////////// >> LOGS.TXT
echo /                 / >> LOGS.TXT

tcc -IINCLUDE -ml -c -nBIN SRC\dosgl.cpp SRC\dglm.cpp SRC\vga.cpp SRC\pipeline.cpp >> LOGS.TXT
del BIN\DOSGL.LIB
tlib /C BIN\DOSGL.LIB +BIN\DOSGL.OBJ+BIN\DGLM.OBJ+BIN\VGA.OBJ+BIN\PIPELINE.OBJ >> LOGS.TXT