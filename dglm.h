#pragma once
#include "math.h"

struct vec3 {
public:
	float x, y, z;

	vec3() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	vec3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	float& operator[](int ind) {
		if (ind == 0)
			return x;
		if (ind == 1)
			return y;
		if (ind == 2)
			return z;
	}

	float length() {
		return sqrt(x * x + y * y + z * z);
	}

	vec3 normilize() {
		return vec3(x / this->length(), y / this->length(), z / this->length());
	}
};

struct vec4 {
public:
	float x, y, z, w;

	vec4() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	}

	vec4(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	vec4 operator+ (vec4 vec) {
		return vec4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
	}

	vec4 operator* (float val) {
		return vec4(x * val, y * val, z * val, w * val);
	}

	float& operator[](int ind) {
		if (ind == 0)
			return x;
		if (ind == 1)
			return y;
		if (ind == 2)
			return z;
		if (ind == 3)
			return w;
	}
};

struct mat4 {
public:
	mat4()
	{
		for (int ind = 0; ind < 4; ind++)
		{
			matrix[ind] = vec4();
		}
	}

	mat4(float x) 
	{
		for (int ind = 0; ind < 4; ind++)
		{
			matrix[ind] = vec4();
		}

		matrix[0].x = x;
		matrix[1].y = x;
		matrix[2].z = x;
		matrix[3].w = x;
	}

	//A(mat4) * B(vec4) = C(vec4) 
	vec4 operator* (vec4 vec) {
		vec4 A0 = matrix[0];
		vec4 A1 = matrix[1];
		vec4 A2 = matrix[2];
		vec4 A3 = matrix[3];

		return (A0 * vec[0] + A1 * vec[1] + A2 * vec[2] + A3 * vec[3]);
	}

	//A(mat4) * B(mat4) = C(mat4)
	mat4 operator* (mat4 mat) {
		vec4 A0 = matrix[0];
		vec4 A1 = matrix[1];
		vec4 A2 = matrix[2];
		vec4 A3 = matrix[3];

		vec4 B0 = mat[0];
		vec4 B1 = mat[1];
		vec4 B2 = mat[2];
		vec4 B3 = mat[3];

		mat4 C;
		C[0] = A0 * B0[0] + A1 * B0[1] + A2 * B0[2] + A3 * B0[3];
		C[1] = A0 * B1[0] + A1 * B1[1] + A2 * B1[2] + A3 * B1[3];
		C[2] = A0 * B2[0] + A1 * B2[1] + A2 * B2[2] + A3 * B2[3];
		C[3] = A0 * B3[0] + A1 * B3[1] + A2 * B3[2] + A3 * B3[3];

		return C;
	}

	vec4& operator[](int ind) {
		return matrix[ind];
	}

private:
	vec4 matrix[4];
};

mat4 translate(mat4 matrix, vec3 vector) {
	mat4 transform(1.0f);

	transform[3] = vec4(vector.x, vector.y, vector.z, 1.0f);

	return transform * matrix;
}

mat4 scale(mat4 matrix, vec3 vector) {
	mat4 scaler(1.0f);

	//We don't simply multiply because that will affect our position
	scaler[0] = matrix[0] * vector[0];
	scaler[1] = matrix[1] * vector[1];
	scaler[2] = matrix[2] * vector[2];
	scaler[3] = matrix[3];

	return scaler;
}

mat4 rotate(mat4 matrix, float t, vec3 r) {
	mat4 rotation(1.0f);

	r = r.normilize();
	float cost = cos(t);
	float sint = sin(t);

	rotation[0][0] = cost + r.x * r.x * (1 - cost);
	rotation[1][0] = r.x * r.y * (1 - cost) - r.z * sint;
	rotation[2][0] = r.x * r.z * (1 - cost) + r.y * sint;

	rotation[0][1] = r.y * r.x * (1 - cost) + r.z * sint;
	rotation[1][1] = cost + r.y * r.y * (1 - cost);
	rotation[2][1] = r.y * r.z * (1 - cost) - r.x * sint;

	rotation[0][2] = r.z * r.x * (1 - cost) - r.y * sint;
	rotation[1][2] = r.z * r.y * (1 - cost) + r.x * sint;
	rotation[2][2] = cost + r.z * r.z * (1 - cost);

	return matrix * rotation;
}

mat4 perspective(float fovY, float aspect, float farVal, float nearVal) {
	float tanHalfFovY = tan(fovY / 2);

	mat4 result;
	result[0][0] = 1 / (aspect * tanHalfFovY);
	result[1][1] = 1 / tanHalfFovY;
	result[2][2] = 1 * (farVal + nearVal) / (farVal - nearVal);
	result[3][2] = 2 * (farVal * nearVal) / (farVal - nearVal);
	result[2][3] = -1;

	return result;
}

mat4 perspective(float left, float right, float bottom, float top, float nearVal, float farVal) {
	mat4 result;
	result[0][0] = (2*nearVal) / (right - left);
	result[2][0] = (right + left) / (right - left);
		
	result[1][1] = (2 * nearVal) / (top - bottom);
	result[2][1] = (top + bottom) / (top - bottom);

	result[2][2] = -1 * (farVal + nearVal) / (farVal - nearVal);
	result[3][2] = -2 * (farVal * nearVal) / (farVal - nearVal);
		
	result[2][3] = -1;

	return result;
}

float radians(float degrees) {
	return degrees * 0.01745329251f;
}