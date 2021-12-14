/*/// pipeline.h
///	Handles basic graphics pipeline that assembles triangles and draws them on screen
/// 
/// Writen by Nikita Berezkov
//*/

#ifndef __PIPELINE_H
#define __PIPELINE_H

#include "dglm.h"
#include "vga.h"

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
	//Parameters that are set by dglViewPort()
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