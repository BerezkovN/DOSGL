#include <iostream.h>
#include <assert.h>
#include <stdlib.h>
#include <dos.h>

#include "shader.h"
#include "dosgl.h"
#include "pipeline.h"

//**
//	Структура для зберігання атибутів
struct attributePointer {
	unsigned int vertexBuffer;
	unsigned int index;
	int normalized;
	unsigned int stride;
	void* pointer;
};

//**
//	Структура яка запам'ятовує всі VBO та EBO
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

//Зберігає в собі вершини та індекси
void** buffers;

//Масив який зберігає всі VAO
VAO* vertexArrays;

//Шейдерна програма яка є наслідником класу shader
shader* internalShader;

//Об'єкт класу конвеєра
pipeline internalPipeline;

/***
 * Створює id для VAO та EBO
 */
void dglGenBuffers(unsigned int size, unsigned int* buffs) {
	for (int ind = 0; ind < size; ind++)
	{
		buffs[ind] = ++bufferCount;
	}
}

/***
 *	Запам'ятовує буфер і готує до використання
 */
void dglBindBuffer(unsigned int mode, unsigned int buffer) {
	if (mode == DGL_ARRAY_BUFFER) {
		currentVertexBuffer = buffer;
	}
	else if (mode == DGL_ELEMENT_ARRAY_BUFFER) {
		//Перевірка, для уникнення помилки
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
 * Передає всі значення в buffers
 *		Якщо mode має значення DGL_ARRAY_BUFFER, то в пам'ять записуються значення типу float
 *		Якщо mode має значення DGL_ELEMENT_ARRAY_BUFFER, то в пам'ять записуються значення типу unsinged int
 */
void dglBufferData(unsigned int mode, unsigned int size, const void* data) {
	if (mode == DGL_ARRAY_BUFFER) {
		//Перевірка, для уникнення помилки
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
		//Перевірка, для уникнення помилки
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
 *	Створює атрибут і дає VAO його запам'ятати
 */
void dglVertexAttribPointer(unsigned int index, int normalized, unsigned int stride, void* pointer) {
	//Перевірка, для уникнення помилки
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
 *	Повертає номер локації певного поля в шейдері
 */
int dglGetUniformLocation(String str) {
	//Перевірка, для уникнення помилки
	if (internalShader == NULL) {
		dglTerminate("Shader is null");
	}

	return internalShader->getUniformLocation(str);
}

/***
 *	Задає значення певного поля в заданій локації в шейдері
 */
void dglUniformMatrix4fv(int location, const float* value) {
	internalShader->setUniformMatrix4fv(location, value);
}

/***
 *	Зображає модель на основі використовуємого VAO
 */
void dglDrawElements(unsigned int mode, const unsigned int count) {
	//Перевірка на трикутник
	assert(count % 3 == 0);

	if (currentVertexArray == 0)
	{
		dglTerminate("VAO is not bound\n");
		return;
	}
	
	VAO& vao = vertexArrays[currentVertexArray];
	
	//Початок графічного конвеєру

	vec4* vertices = new vec4[3];

	for (int ind = 0; ind < count; ind += 3)
	{
		//Перша вершина
		for (int attribInd0 = 0; attribInd0 < internalShader->attributes; attribInd0++)
		{
			int currentPos = (int)vao.attributes[attribInd0].pointer + vao.attributes[attribInd0].stride * ((unsigned int*)buffers[vao.elementBuffer])[ind];
			internalShader->setAttribute(attribInd0, currentPos, buffers[vao.attributes[attribInd0].vertexBuffer]);
		}
		vertices[0] = internalShader->vert();

		//Друга вершина
		for (int attribInd1 = 0; attribInd1 < internalShader->attributes; attribInd1++)
		{
			int currentPos = (int)vao.attributes[attribInd1].pointer + vao.attributes[attribInd1].stride * ((unsigned int*)buffers[vao.elementBuffer])[ind + 1];
			internalShader->setAttribute(attribInd1, currentPos, buffers[vao.attributes[attribInd1].vertexBuffer]);
		}
		vertices[1] = internalShader->vert();

		//Третя вершина
		for (int attribInd2 = 0; attribInd2 < internalShader->attributes; attribInd2++)
		{
			int currentPos = (int)vao.attributes[attribInd2].pointer + vao.attributes[attribInd2].stride * ((unsigned int*)buffers[vao.elementBuffer])[ind + 2];
			internalShader->setAttribute(attribInd2, currentPos, buffers[vao.attributes[attribInd2].vertexBuffer]);
		}
		vertices[2] = internalShader->vert();

		//Передаєм конвеєру вершини
		internalPipeline.AssembleTriangle(vertices);
	}

	delete[] vertices;
}

/***
 *	Поміняти буфери місцями
 */
void dglSwapBuffers() {
	page_flip(&visual_page, &active_page);
	outpw(SC_INDEX, ALL_PLANES);
	memset(&VGA[active_page], 0, SCREEN_SIZE / 4);
}

/***
 *	Ініціалізація mode X та деяких масивів
 */
void dglInit() {
	buffers = new void* [DGL_MAX_AMOUNT_OF_BUFFERS];
	vertexArrays = new VAO[DGL_MAX_AMOUNT_OF_BUFFERS];

	set_unchained_mode();

	visual_page = 0;
	active_page = SCREEN_SIZE / 4;
}

/***
 *	Безпечний вихід з програми
 */
void dglTerminate() {
	delete[] buffers;
	delete[] vertexArrays;
	delete[] internalShader;

	set_mode(TEXT_MODE);
	exit(1);
}

/***
 *	Безпечний вихід з програми та додаткове повідомлення
 */
void dglTerminate(String message) {
	delete[] buffers;
	delete[] vertexArrays;
	delete[] internalShader;

	set_mode(TEXT_MODE);
	cout << "Program was terminated\n"
		<< "	Error: " << message.char_ptr
		<< "\nPress ENTER to exit...";

	cin.get();
	exit(1);
}

/***
 *	Задання властивостей екрана
 */
void dglViewPort(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
	internalPipeline.CORNERX = x;
	internalPipeline.CORNERY = y;
	internalPipeline.WIDTH = width;
	internalPipeline.HEIGHT = height;
}

/***
 *	Задаєм шейдерну програму
 */
void dglUseProgram(shader& someShader) {
	internalShader = &someShader;
}