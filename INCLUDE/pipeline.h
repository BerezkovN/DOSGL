/*/// pipeline.h
///	Клас графічного конвеєру
//*/

#ifndef __PIPELINE_H
#define __PIPELINE_H

#include "dglm.h"
#include "vga.h"

//**
//	Структура трикутника
//
struct triangle {
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
	//Параметри які задаються в dlgViewPort
	unsigned int WIDTH;
	unsigned int HEIGHT;
	unsigned int CORNERX;
	unsigned int CORNERY;

private:
	void DrawTriangle(const triangle& triangle);
	void DrawLine(vec3& v0, vec3& v1);

	vec3 toScreenSpace(vec4& vert);
public:
	void AssembleTriangle(vec4* vertices);
};

#endif 