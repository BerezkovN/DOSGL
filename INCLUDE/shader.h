#ifndef __SHADER_H
#define __SHADER_H

#include "dglm.h"
#include "str.h"

class shader {
protected:

	void retrieveVec3(vec3* vector, const float* value);

	void retrieveMatrix4(mat4* matrix, const float* value);
public:
	//You have to manually specify how many attributes are in this shader
	int attributes;

	//Vertex shader
	virtual vec4 vert() = 0;

	virtual void setAttribute(unsigned int index, unsigned int position, void* data) = 0;

	//You have to manually specify which location your uniform variable has
	virtual int getUniformLocation(String name) = 0;

	virtual void setUniformMatrix4fv(int location, const float* value) = 0;
};

void shader::retrieveVec3(vec3* vector, const float* value) {
	float* ptr = &(*vector).x;

	for (int ind = 0; ind < 3; ind++)
	{
		float val = value[ind];
		ptr[ind] = val;
	}
}

void shader::retrieveMatrix4(mat4* matrix, const float* value) {
	float* ptr = &(*matrix)[0].x;

	for (int ind = 0; ind < 16; ind++)
	{
		float val = value[ind];
		ptr[ind] = val;
	}
}

#endif