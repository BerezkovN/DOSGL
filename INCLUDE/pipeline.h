#ifndef __PIPELINE_H
#define __PIPELINE_H

//#define INT13H

#include "dglm.h"
#include "vga.h"

#define DGL_MAX_VERTEX_ATTRIBS 20

struct attributePointer {
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
	attributePointer* attributes;

	VAO() {
		length = DGL_MAX_VERTEX_ATTRIBS;
	}
};

class triangle {
public:
	vec3 v0;
	vec3 v1;
	vec3 v2;

	triangle(vec3& v0in, vec3& v1in, vec3& v2in) {
		v0 = v0in;
		v1 = v1in;
		v2 = v2in;
	}
};

class pipeline {
public:
	unsigned int WIDTH;
	unsigned int HEIGHT;
	unsigned int CORNERX;
	unsigned int CORNERY;

private:
	void ProcessTriangle(vec4& v0, vec4& v1, vec4& v2);
	void DrawTriangle(const triangle& triangle);
	void DrawLine(vec3& v0, vec3& v1);

	vec3 toScreenSpace(vec4& vert);
public:
	void AssembleTriangle(vec4* vertices);
};

#endif 