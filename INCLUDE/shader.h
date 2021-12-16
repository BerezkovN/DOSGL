/*/// shader.h
///	Описує абстрактний клас для шейдерної програми
/// Сучасні графічні конвеєри мають в собі спеціальні компілятори для компілювання шейдерних програм
/// Щоб не писати свій компілятор, було ствроено цей клас в якому можна самому описувати вершинний шейдер
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
	//Потрібно власноруч описувати кількість атрибутів, 
	//	так як в нас немає компілятора який це зробить самостійно
	int attributes;

	/***
	 *	Вершинний шейдер
	 */
	virtual vec4 vert() = 0;

	/***
	 *	Функція для задання значень в нашому шейдері
	 */
	virtual void setAttribute(unsigned int location, unsigned int offset, void* data) = 0;

	/***
	 *	Повертає номер знаходження атрибу
	 *		Так як немає компілятора, його потрібно самому описувати
	 */
	virtual int getUniformLocation(String name) = 0;

	/***
	 *	Задає значення матриці в шейдері в номері знаходження
	 */
	virtual void setUniformMatrix4fv(int location, const float* value) = 0;
};

/***
 *	 Переведення структури трьохвимірного вектора в звичайний масив
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
 *	 Переведення структури матриці 4-на-4 в звичайний масив
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