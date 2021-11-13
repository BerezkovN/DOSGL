#include "dglm.h"

#include <iostream.h>

float& vec3::operator[](int ind) {
	if (ind == 0)
		return x;
	if (ind == 1)
		return y;
	if (ind == 2)
		return z;
}

void vec3::print() {
	cout << "(";
	cout << x;
	cout << ", " << y << ", " << z << ")   \n";
}

float& vec4::operator[](int ind) {
	if (ind == 0)
		return x;
	if (ind == 1)
		return y;
	if (ind == 2)
		return z;
	if (ind == 3)
		return w;
}

void vec4::print() {
	cout << "(" << x << ", " << y << ", " << z << ", " << w << ")   \n";
}

mat4::mat4()
{
	for (int ind = 0; ind < 4; ind++)
	{
		matrix[ind] = vec4();
	}
}

mat4::mat4(float x)
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
	result[0][0] = (2 * nearVal) / (right - left);
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