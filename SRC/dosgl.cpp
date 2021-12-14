#include <iostream.h>
#include <assert.h>
#include <stdlib.h>
#include <dos.h>

#include "shader.h"
#include "dosgl.h"
#include "pipeline.h"

//**
//	Struct to hold information that 
//  comes from dglVertexAttribPointer
struct attributePointer {
	unsigned int vertexBuffer;
	unsigned int index;
	int normalized;
	unsigned int stride;
	void* pointer;
};

//**
//	Struct to hold information about all attributes
//  Also keeps track of what elemenetBuffer is bound
struct VAO {
	unsigned int elementBuffer;
	unsigned int length;
	attributePointer* attributes;

	VAO() {
		length = DGL_MAX_VERTEX_ATTRIBS;
	}
};

unsigned int bufferCount = 0;
unsigned int vertexArrayCount = 0;

unsigned int currentVertexArray = 0;
unsigned int currentVertexBuffer = 0;
unsigned int currentElementBuffer = 0;

//Contains array and element buffers data
//Every buffer gets an ID that can be later used as
//index in this array
void** buffers;

//Array of VAO with size of DGL_MAX_VERTEX_ATTRIBS
VAO* vertexArrays;

//Pointer to shader set in dglUseProgram
shader* internalShader;

pipeline internalPipeline;

/***
 * Generates IDs for VAO and EBO
 */
void dglGenBuffers(unsigned int size, unsigned int* buffs) {
	for (int ind = 0; ind < size; ind++)
	{
		buffs[ind] = ++bufferCount;
	}
}

/***
 *	Specifies what buffer is bound
 *	VAO doesn't memorize VBO yet
 */
void dglBindBuffer(unsigned int mode, unsigned int buffer) {
	if (mode == DGL_ARRAY_BUFFER) {
		currentVertexBuffer = buffer;
	}
	else if (mode == DGL_ELEMENT_ARRAY_BUFFER) {
		//Simple precaution
		if (currentVertexArray == 0)
		{
			dglTerminate("VAO is not bound\n");
			return;
		}

		currentElementBuffer = buffer;
		vertexArrays[currentVertexArray].elementBuffer = currentElementBuffer;
	}
}

/***
 * Sets data to "buffers"
 *		If mode is DGL_ARRAY_BUFFER, it tries to set data as float values
 *		If mode is DGL_ELEMENT_ARRAY_BUFFER, it tries to set data as uint values
 */
void dglBufferData(unsigned int mode, unsigned int size, const void* data) {
	if (mode == DGL_ARRAY_BUFFER) {
		//Simple precaution
		if (currentVertexBuffer == 0) {
			dglTerminate("Vertex buffer is not bound\n");
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
		//Simple precaution
		if (currentElementBuffer == 0) {
			dglTerminate("Element array buffer is not bound\n");
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

/***
 *	Generates IDs for VAO 
 */
void dglGenVertexArrays(unsigned int size, unsigned int* buffs) {
	for (int ind = 0; ind < size; ind++)
	{
		buffs[ind] = ++vertexArrayCount;
		vertexArrays[vertexArrayCount].attributes = new attributePointer[DGL_MAX_VERTEX_ATTRIBS];
	}
}

/***
 *	Simple binding of VAO
 */
void dglBindVertexArray(unsigned int vao) {
	currentVertexArray = vao;
}

/***
 *	Creates attribute pointer that is memorized by VAO
 */
void dglVertexAttribPointer(unsigned int index, int normalized, unsigned int stride, void* pointer) {
	//Simple precaution
	if (currentVertexArray == 0)
	{
		dglTerminate("VAO is not bound\n");
		return;
	}
	attributePointer* attribs = vertexArrays[currentVertexArray].attributes;

	attribs[index].vertexBuffer = currentVertexBuffer;
	attribs[index].index = index;
	attribs[index].normalized = normalized;
	attribs[index].stride = stride;
	attribs[index].pointer = pointer;
}

/***
 *	Returns location of uniform field
 */
int dglGetUniformLocation(String str) {
	//Simple precaution
	if (internalShader == NULL) {
		dglTerminate("Shader is null");
	}

	return internalShader->getUniformLocation(str);
}

/***
 *	Set matrix in shader in specified location 
 */
void dglUniformMatrix4fv(int location, const float* value) {
	internalShader->setUniformMatrix4fv(location, value);
}

int test = 0;

/***
 *	Draws using VBO and EBO of currently bound VAO 
 */
void dglDrawElements(unsigned int mode, const unsigned int count) {
	//We are dealing with triangles here
	assert(count % 3 == 0);

	if (currentVertexArray == 0)
	{
		dglTerminate("VAO is not bound\n");
		return;
	}
	
	VAO& vao = vertexArrays[currentVertexArray];

	vec4* vertices = new vec4[3];

	for (int ind = 0; ind < count; ind += 3)
	{
		//Process first vertex
		for (int attribInd0 = 0; attribInd0 < internalShader->attributes; attribInd0++)
		{
			int currentPos = (int)vao.attributes[attribInd0].pointer + vao.attributes[attribInd0].stride * ((unsigned int*)buffers[vao.elementBuffer])[ind];
			internalShader->setAttribute(attribInd0, currentPos, buffers[vao.attributes[attribInd0].vertexBuffer]);
		}
		vertices[0] = internalShader->vert();

		//Process second vertex
		for (int attribInd1 = 0; attribInd1 < internalShader->attributes; attribInd1++)
		{
			int currentPos = (int)vao.attributes[attribInd1].pointer + vao.attributes[attribInd1].stride * ((unsigned int*)buffers[vao.elementBuffer])[ind + 1];
			internalShader->setAttribute(attribInd1, currentPos, buffers[vao.attributes[attribInd1].vertexBuffer]);
		}
		vertices[1] = internalShader->vert();

		//Process third vertex
		for (int attribInd2 = 0; attribInd2 < internalShader->attributes; attribInd2++)
		{
			int currentPos = (int)vao.attributes[attribInd2].pointer + vao.attributes[attribInd2].stride * ((unsigned int*)buffers[vao.elementBuffer])[ind + 2];
			internalShader->setAttribute(attribInd2, currentPos, buffers[vao.attributes[attribInd2].vertexBuffer]);
		}
		vertices[2] = internalShader->vert();

		//Passing to the pipeline
		internalPipeline.AssembleTriangle(vertices);
	}

	delete vertices;
}

/***
 *	Flip VGA pages and clear not active page
 */
void dglSwapBuffers() {
	page_flip(&visual_page, &active_page);
	outpw(SC_INDEX, ALL_PLANES);
	memset(&VGA[active_page], 0, SCREEN_SIZE / 4);
}

/***
 *	Initialize all needed arrays and use VGA X mode
 */
void dglInit() {
	buffers = new void* [DGL_MAX_AMOUNT_OF_BUFFERS];
	vertexArrays = new VAO[DGL_MAX_AMOUNT_OF_BUFFERS];

	set_unchained_mode();

	visual_page = 0;
	active_page = SCREEN_SIZE / 4;
}

/***
 *	Immediatly terminates program 
 */
void dglTerminate() {
	delete buffers;
	delete vertexArrays;
	delete internalShader;

	set_mode(TEXT_MODE);
	exit(1);
}

/***
 *	Terminates program with message and waits for input
 */
void dglTerminate(String message) {
	delete buffers;
	delete vertexArrays;
	delete internalShader;

	set_mode(TEXT_MODE);
	cout << "Program was terminated\n"
		<< "	Error: " << message.char_ptr
		<< "\nPress ENTER to exit...";

	cin.get();
	exit(1);
}

/***
 *	Setup of screen properties 
 */
void dglViewPort(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
	internalPipeline.CORNERX = x;
	internalPipeline.CORNERY = y;
	internalPipeline.WIDTH = width;
	internalPipeline.HEIGHT = height;
}

/***
 *	Set shader that is used in dglDrawElements
 */
void dglUseProgram(shader& someShader) {
	internalShader = &someShader;
}