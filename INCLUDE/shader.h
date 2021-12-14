/*/// shader.h
///	Contains abstract class that desribes default shader.
/// Because there is no compiler for shader program, you have specify yourself all data that you need.
///	Also specify what location every field has how it handles it.
/// For example, investigate smplShdr.h
///
/// Written by Nikita Berezkov
//*/

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

	/***
	 *	Vertex shader 
	 */
	virtual vec4 vert() = 0;

	/***
	 *	Function has to get data pointer and offset where to start reading data
	 */
	virtual void setAttribute(unsigned int location, unsigned int offset, void* data) = 0;

	/***
	 *	Returns location of variables.
	 *		You determine location and variable names yourself.
	 */
	virtual int getUniformLocation(String name) = 0;

	/***
	 *	Has to set data to specified location.
	 *	You specify how data should be handled 
	 */
	virtual void setUniformMatrix4fv(int location, const float* value) = 0;
};

/***
 *	 Retrieves data from "value" and sets it to "vector"
 */
void shader::retrieveVec3(vec3* vector, const float* value) {
	float* ptr = &(*vector).x;

	for (int ind = 0; ind < 3; ind++)
	{
		float val = value[ind];
		ptr[ind] = val;
	}
}

/***
 *	 Retrieves data from "value" and sets it to "matrix"
 */
void shader::retrieveMatrix4(mat4* matrix, const float* value) {
	float* ptr = &(*matrix)[0].x;

	for (int ind = 0; ind < 16; ind++)
	{
		float val = value[ind];
		ptr[ind] = val;
	}
}

#endif