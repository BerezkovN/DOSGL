#pragma once
#include "str.h"

#define DGL_ARRAY_BUFFER 0x01
#define DGL_ELEMENT_ARRAY_BUFFER 0x02

#define DGL_MAX_AMOUNT_OF_BUFFERS 50
#define DGL_MAX_VERTEX_ATTRIBS 20

#define DGL_TRIANGLES 0x13

void dglInit();
void dglTerminate();

void dglGenBuffers(unsigned int size, unsigned int* buffs);
void dglGenVertexArrays(unsigned int size, unsigned int* buffs);

void dglBindBuffer(unsigned int mode, unsigned int buffer);
void dglBindVertexArray(unsigned int vao);

void dglBufferData(unsigned int mode, unsigned int size, void* data);

void dglVertexAttribPointer(unsigned int index, int normalized, unsigned int stride, void* pointer);

int dglGetUniformLocation(String str);
void dglUniformMatrix4fv(int location, const float* value);

void dglDrawElements(unsigned int mode, unsigned int count);
void dglSwapBuffers();

void dglViewPort(unsigned int x, unsigned int y, unsigned int width, unsigned int height);