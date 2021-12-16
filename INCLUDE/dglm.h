/*/// dglm.h
///	Бібліотека для роботи з матрицями та векторами
//*/

#ifndef __DGLM_H
#define __DGLM_H

#include "math.h"

//**
//	Струкутура трьохвимірного вектора
//
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

	/*Індексатор*/
	float& operator[](int ind);

	vec3 operator+ (vec3 vec) {
		return vec3(x + vec.x, y + vec.y, z + vec.z);
	}

	vec3 operator- (vec3 vec) {
		return vec3(x - vec.x, y - vec.y, z - vec.z);
	}

	/*Скалярне множення векторів*/
	float operator* (vec3 vec) {
		return x * vec.x + y * vec.y + z * vec.z;
	}

	/*Довжина вектора*/
	float length() {
		return sqrt(x * x + y * y + z * z);
	}

	/*Нормалізування вектора*/
	vec3 normilize() {
		return vec3(x / this->length(), y / this->length(), z / this->length());
	}

	void print();
};

//**
//	Струкутура чотирьохвимірного вектора
//
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

	/*Індексатор*/
	float& operator[](int ind);

	void print();
};

//**
//	Струкутура 4-на-4 матриці
//
struct mat4 {
public:
	mat4();

	mat4(float x);

	/*Множення матриці на вектор*/
	vec4 operator* (vec4 vec) {
		vec4 A0 = matrix[0];
		vec4 A1 = matrix[1];
		vec4 A2 = matrix[2];
		vec4 A3 = matrix[3];

		return (A0 * vec[0] + A1 * vec[1] + A2 * vec[2] + A3 * vec[3]);
	}

	/*Множення матриці на матрицю*/
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

	/*Індексатор*/
	vec4& operator[](int ind) {
		return matrix[ind];
	}

private:
	vec4 matrix[4];
};

mat4 translate(mat4 matrix, vec3 vector);

mat4 scale(mat4 matrix, vec3 vector);

mat4 rotate(mat4 matrix, float t, vec3 r);

mat4 perspective(float fovY, float aspect, float farVal, float nearVal);

mat4 perspective(float left, float right, float bottom, float top, float nearVal, float farVal);

float radians(float degrees);

#endif