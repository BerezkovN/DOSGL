#include "shader.h"
#include <iostream.h>

class simpleShader : public shader {
private:
	//ATRIBUTES
	//	location 0
	vec3 aPos;

	//UNIFORMS
	//	location 0
	mat4 model;
	//	location 1
	mat4 view;
	//	location 2
	mat4 projection;

public:
	simpleShader() {
		attributes = 1;

		//Default values for attributes
		aPos = vec3(0.0f, 0.0f, 0.0f);

		//Default values for uniforms
		model = mat4(1.0f);
		view = mat4(1.0f);
		projection = mat4(1.0f);
	}

	vec4 vert() {
#if defined(DEBUG)
		cout << "\nPoint in vertex shader:\n";
		cout << " " << aPos.x << " " << aPos.y << " " << aPos.z << "\n";
#endif
		vec4 pos(aPos.x, aPos.y, aPos.z, 1.0f);
		return projection * view * model * pos;
	}

	void setAttribute(unsigned int index, unsigned int position, void* data) {
		if (index == 0) {
			retrieveVec3(&aPos, (float*)((char*)data + position));
		}
	}

	int getUniformLocation(String name) {
		if (strcmp(name.char_ptr, "model") == 0)
			return 0;
		else if (strcmp(name.char_ptr, "view") == 0)
			return 1;
		else if (strcmp(name.char_ptr, "projection") == 0)
			return 2;

		cout << "Uniform location was not found\n";
		return -1;
	}

	void setUniformMatrix4fv(int location, const float* value) {
		if (location == 0) {
			retrieveMatrix4(&model, value);
		}
		else if (location == 1) {
			retrieveMatrix4(&view, value);
		}
		else if (location == 2) {
			retrieveMatrix4(&projection, value);
		}
	}
};