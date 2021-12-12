#include <iostream.h>
#include <assert.h>
#include <stdlib.h>
#include <dos.h>

//#define DEBUG

#include "shader.h"
#include "dosgl.h"
#include "pipeline.h"

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

void** buffers = new void* [DGL_MAX_AMOUNT_OF_BUFFERS];

shader* internalShader;

pipeline internalPipeline;

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
			dglTerminate();
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
			dglTerminate();
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
			dglTerminate();
			cout << "No buffers are bound\n";
			return;
		}



		int length = size / (sizeof(unsigned int));
		buffers[currentElementBuffer] = new unsigned int[length];

#if defined(DEBUG)
		cout << "\n buffer data\n = " << currentElementBuffer;
#endif

		for (int ind = 0; ind < length; ind++)
		{
#if defined(DEBUG)
			cout << ((unsigned int*)data)[ind] << " ";
#endif

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

//We don't specify size in argument list, because we do it manually in shader.h
void dglVertexAttribPointer(unsigned int index, int normalized, unsigned int stride, void* pointer) {
	if (currentVertexArray == 0)
	{
		dglTerminate();
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
	return internalShader->getUniformLocation(str);
}

void dglUniformMatrix4fv(int location, const float* value) {
	internalShader->setUniformMatrix4fv(location, value);
}

void dglDrawElements(unsigned int mode, const unsigned int count) {
	//We are dealing with triangles here
	assert(count % 3 == 0);

	if (currentVertexArray == 0)
	{
		dglTerminate();
		cout << "VAO is not bound\n";
		return;
	}

	VAO vao = vertexArrays[currentVertexArray];

#if defined(DEBUG)
	cout << "\nvao.elementBuffer = " << vao.elementBuffer;
	cout << "\nElement buffer: " << ((unsigned int*)buffers[vao.elementBuffer])[1] << "\n";
#endif

	vec4* vertices = new vec4[count];

	//VERTEX SPECIFICATION STAGE
	for (int ind = 0; ind < count; ind++)
	{
		for (int attribInd = 0; attribInd < internalShader->attributes; attribInd++)
		{
			int currentPos = (int)vao.attribs[attribInd].pointer + vao.attribs[attribInd].stride * ((unsigned int*)buffers[vao.elementBuffer])[ind];
			internalShader->setAttribute(attribInd, currentPos, buffers[vao.attribs[attribInd].vertexBuffer]);
		}

		//So instead of copying vec4 to the vertices array, i just store their addresses
		//Those vec4 are stored in the stack and we don't end function here so we should not be affraid to pass those addresses to a pipeline
		vertices[ind] = internalShader->vert();
	}



	internalPipeline.Draw(vertices, count);
	delete vertices;
}

void dglSwapBuffers() {
#if defined(INT13H)
	show_buffer(double_buffer);
	memset(double_buffer, 0, SCREEN_SIZE);
#else
	page_flip(&visual_page, &active_page);
	outpw(SC_INDEX, ALL_PLANES);
	memset(&VGA[active_page], 0, SCREEN_SIZE / 4);

#endif
}

void dglInit() {
#if defined(INT13H)
	if ((double_buffer = (byte*)malloc(SCREEN_SIZE)) == NULL)
	{
		cout << "Not enough memory for double buffer.\n";
		exit(1);
	}
	set_mode(VGA_256_COLOR_MODE);
#else
	set_unchained_mode();

	visual_page = 0;
	active_page = SCREEN_SIZE / 4;
#endif
}

void dglTerminate() {
	set_mode(TEXT_MODE);
	exit(1);
}

void dglViewPort(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
	internalPipeline.CORNERX = x;
	internalPipeline.CORNERY = y;
	internalPipeline.WIDTH = width;
	internalPipeline.HEIGHT = height;
}

void dglUseProgram(shader& someShader) {
	internalShader = &someShader;
}