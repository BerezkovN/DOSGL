#include <iostream.h>
#include <assert.h>
#include <stdlib.h>
#include <dos.h>

//#define DEBUG

#include "shader.h"
#include "dosgl.h"
#include "pipeline.h"


unsigned int vertexArrayCount = 0;
unsigned int currentVertexArray = 0;

VAO* vertexArrays = new VAO[DGL_MAX_AMOUNT_OF_BUFFERS];

unsigned int bufferCount = 0;
unsigned int currentVertexBuffer = 0;
unsigned int currentElementBuffer = 0;

void** buffers;

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
		vertexArrays[vertexArrayCount].attributes = new attributePointer[DGL_MAX_VERTEX_ATTRIBS];
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
	attributePointer* attribs = vertexArrays[currentVertexArray].attributes;

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
	
	VAO& vao = vertexArrays[currentVertexArray];

	vec4* vertices = new vec4[3];

	for (int ind = 0; ind < count; ind += 3)
	{
		for (int attribInd0 = 0; attribInd0 < internalShader->attributes; attribInd0++)
		{
			int currentPos = (int)vao.attributes[attribInd0].pointer + vao.attributes[attribInd0].stride * ((unsigned int*)buffers[vao.elementBuffer])[ind];
			internalShader->setAttribute(attribInd0, currentPos, buffers[vao.attributes[attribInd0].vertexBuffer]);
		}
		vertices[0] = internalShader->vert();

		for (int attribInd1 = 0; attribInd1 < internalShader->attributes; attribInd1++)
		{
			int currentPos = (int)vao.attributes[attribInd1].pointer + vao.attributes[attribInd1].stride * ((unsigned int*)buffers[vao.elementBuffer])[ind + 1];
			internalShader->setAttribute(attribInd1, currentPos, buffers[vao.attributes[attribInd1].vertexBuffer]);
		}
		vertices[1] = internalShader->vert();

		for (int attribInd2 = 0; attribInd2 < internalShader->attributes; attribInd2++)
		{
			int currentPos = (int)vao.attributes[attribInd2].pointer + vao.attributes[attribInd2].stride * ((unsigned int*)buffers[vao.elementBuffer])[ind + 2];
			internalShader->setAttribute(attribInd2, currentPos, buffers[vao.attributes[attribInd2].vertexBuffer]);
		}
		vertices[2] = internalShader->vert();

		internalPipeline.AssembleTriangle(vertices);
	}

	delete vertices;
}

void dglSwapBuffers() {
	page_flip(&visual_page, &active_page);
	outpw(SC_INDEX, ALL_PLANES);
	memset(&VGA[active_page], 0, SCREEN_SIZE / 4);
}

void dglInit() {
	buffers = new void* [DGL_MAX_AMOUNT_OF_BUFFERS];

	set_unchained_mode();

	visual_page = 0;
	active_page = SCREEN_SIZE / 4;
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