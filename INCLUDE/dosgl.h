/*/// dosgl.h
///	Інтерфейс до графічного конвеєру
//*/

#pragma once
#include "str.h"
#include "shader.h"

//Максимальна кількість атрибутів яка може міститись в VAO
#define DGL_MAX_VERTEX_ATTRIBS 20

//Значення для створення VBO або EBO
#define DGL_ARRAY_BUFFER 0x01
#define DGL_ELEMENT_ARRAY_BUFFER 0x02

//Максимальна кількість можливих VBO та EBO загалом
#define DGL_MAX_AMOUNT_OF_BUFFERS 50

//Значення просто для пародіювання OpenGL
#define DGL_TRIANGLES 0x13

void dglInit();
void dglTerminate();
void dglTerminate(String message);

void dglGenBuffers(unsigned int size, unsigned int* buffs);
void dglGenVertexArrays(unsigned int size, unsigned int* buffs);

void dglBindBuffer(unsigned int mode, unsigned int buffer);
void dglBindVertexArray(unsigned int vao);

void dglBufferData(unsigned int mode, unsigned int size, const void* data);

void dglVertexAttribPointer(unsigned int index, int normalized, unsigned int stride, void* pointer);

int dglGetUniformLocation(String str);
void dglUniformMatrix4fv(int location, const float* value);

void dglDrawElements(unsigned int mode, const unsigned int count);
void dglSwapBuffers();

void dglViewPort(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

void dglUseProgram(shader& someShader);