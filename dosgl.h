#pragma once
#include<iostream.h>
#include<stdlib.h>

#include"dgraph.h"
#include"shader.h"

#define DGL_ARRAY_BUFFER 0x01
#define DGL_ELEMENT_ARRAY_BUFFER 0x02

#define DGL_MAX_AMOUNT_OF_BUFFERS 50
#define DGL_MAX_VERTEX_ATTRIBS 20

#define DGL_TRIANGLES 0x13

unsigned int WIDTH;
unsigned int HEIGHT;
unsigned int CORNERX;
unsigned int CORNERY;

struct attribPointer {
	unsigned int vertexBuffer;
	//bool enabled = false;
	unsigned int index;
	int normalized;
	unsigned int stride;
	void* pointer;
};

struct VAO {
	unsigned int elementBuffer;
	unsigned int length;
	attribPointer* attribs;

	VAO() {
		length = DGL_MAX_VERTEX_ATTRIBS;
	}
};

unsigned int vertexArrayCount = 0;
unsigned int currentVertexArray = 0;

VAO* vertexArrays = new VAO[DGL_MAX_AMOUNT_OF_BUFFERS];

unsigned int bufferCount = 0;
unsigned int currentVertexBuffer = 0;
unsigned int currentElementBuffer = 0;

void** buffers = new void*[DGL_MAX_AMOUNT_OF_BUFFERS];

shader ourShader;

void dglGenBuffers(unsigned int size, unsigned int* buffs) {
	for (int ind = 0; ind < size; ind++)
	{
		buffs[ind] = ++bufferCount;
	}
}

void dglBindBuffer(unsigned int mode, unsigned int buffer) {
	if (mode == DGL_ARRAY_BUFFER) {
		currentVertexBuffer = buffer;
	}
	else if (mode == DGL_ELEMENT_ARRAY_BUFFER) {
		if (currentVertexArray == 0)
		{
			cout << "VAO is not bound\n";
			return;
		}

		currentElementBuffer = buffer;
		vertexArrays[currentVertexArray].elementBuffer = currentElementBuffer;
	}
}

void dglBufferData(unsigned int mode, unsigned int size, void* data) {
	if (mode == DGL_ARRAY_BUFFER) {
		if (currentVertexBuffer == 0) {
			cout << "No buffers are bound\n";
			return;
		}

		int length = size / (sizeof(float));
		buffers[currentVertexBuffer] = new float[length];

		for (int ind = 0; ind < length; ind++)
		{
			((float*)buffers[currentVertexBuffer])[ind] = ((float*)data)[ind];
		}
	}
	else if (mode == DGL_ELEMENT_ARRAY_BUFFER) {
		if (currentElementBuffer == 0) {
			cout << "No buffers are bound\n";
			return;
		}
		
		int length = size / (sizeof(unsigned int));
		buffers[currentElementBuffer] = new unsigned int[length];

		for (int ind = 0; ind < length; ind++)
		{
			((unsigned int*)buffers[currentElementBuffer])[ind] = ((unsigned int*)data)[ind];
		}
	}
}

void dglGenVertexArrays(unsigned int size, unsigned int* buffs) {
	for (int ind = 0; ind < size; ind++)
	{
		buffs[ind] = ++vertexArrayCount;
		vertexArrays[vertexArrayCount].attribs = new attribPointer[DGL_MAX_VERTEX_ATTRIBS];
	}
}

void dglBindVertexArray(unsigned int vao) {
	currentVertexArray = vao;
}

void dglVertexAttribPointer(unsigned int index, int normalized, unsigned int stride, void* pointer) {
	if (currentVertexArray == 0)
	{
		cout << "VAO is not bound\n";
		return;
	}
	attribPointer* attribs = vertexArrays[currentVertexArray].attribs;

	attribs[index].vertexBuffer = currentVertexBuffer;
	attribs[index].index = index;
	attribs[index].normalized = normalized;
	attribs[index].stride = stride;
	attribs[index].pointer = pointer;
}

//void dglEnableVertexAttribArray(unsigned int index) {
//	if (currentVertexArray == 0)
//	{
//		std::cout << "VAO is not bound\n";
//		return;
//	}
//	vertexArrays[currentVertexArray].attribs[index].enabled = true;
//}

int dglGetUniformLocation(String str) {
	return ourShader.getUniformLocation(str);
}

void dglUniformMatrix4fv(int location, const float* value) {
	ourShader.setUniformMatrix4fv(location, value);
}

void dglDrawArrays(unsigned int mode, unsigned int first, unsigned int count) {
	if (currentVertexArray == 0)
	{
		cout << "VAO is not bound\n";
		return;
	}
	
	VAO vao = vertexArrays[currentVertexArray];
	

	int length = count + first;
	for (int ind = first; ind < length; ind++)
	{
		for (int attribInd = 0; attribInd < ourShader.attributes; attribInd++)
		{
			int currentPos = (int)vao.attribs[attribInd].pointer + vao.attribs[attribInd].stride * ind;
			ourShader.setAttribute(attribInd, currentPos, buffers[vao.attribs[attribInd].vertexBuffer]);
		}

		vec4 vert = ourShader.vert();

		//std::cout << vert.x << " " << vert.y << " " << vert.z << " " << vert.w << std::endl;

		vec3 ndc = vec3(vert.x / vert.w, vert.y / vert.w, vert.z / vert.w);

		vec3 windows = vec3(WIDTH / 2 * ndc.x + (CORNERX + WIDTH / 2), HEIGHT / 2 * ndc.y + (CORNERY + HEIGHT / 2), 0);

		cout << "Window:\n" << windows.x << " " << windows.y << " " << windows.z << endl;
	}
}

void dglDrawElements(unsigned int mode, unsigned int count) {
	if (currentVertexArray == 0)
	{
		cout << "VAO is not bound\n";
		return;
	}

	VAO vao = vertexArrays[currentVertexArray];

	for (int ind = 0; ind < count; ind++)
	{
		for (int attribInd = 0; attribInd < ourShader.attributes; attribInd++)
		{
			int currentPos = (int)vao.attribs[attribInd].pointer + vao.attribs[attribInd].stride * ((unsigned int*)buffers[vao.elementBuffer])[ind];
			ourShader.setAttribute(attribInd, currentPos, buffers[vao.attribs[attribInd].vertexBuffer]);
		}

		vec4 vert = ourShader.vert();

		//std::cout << vert.x << " " << vert.y << " " << vert.z << " " << vert.w << std::endl;

		vec3 ndc = vec3(vert.x / vert.w, vert.y / vert.w, vert.z / vert.w);

		vec3 windows = vec3(WIDTH / 2 * ndc.x + (CORNERX + WIDTH / 2), HEIGHT / 2 * ndc.y + (CORNERY + HEIGHT / 2), 0);

		SETPIX((int)windows.x, (int)windows.y, 15);

		//cout << "Window:\n" << windows.x << " " << windows.y << " " << windows.z << endl;
	}
}

void dglSwapBuffers() {
	swap_vgabuffer();
}

void dglInit() {
	set_mode(VGA_256_COLOR_MODE);
	init_vgabuffer();
}

void dglTerminate() {
	set_mode(TEXT_MODE);
}

void dglViewPort(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
	CORNERX = x;
	CORNERY = y;
	WIDTH = width;
	HEIGHT = height;
}