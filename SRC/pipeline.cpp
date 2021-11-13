#include "pipeline.h"
#include "vga.h"

#include <conio.h>
#include <iostream.h>

void pipeline::AssembleTriangles(vec4* vertices, int count) {
	for (int i = 0; i < count; i += 3)
	{
		vec4& v0 = vertices[i];
		vec4& v1 = vertices[i + 1];
		vec4& v2 = vertices[i + 2];

		//CULLBACK

		ProcessTriangle(v0, v1, v2);
	}
}

vec3 pipeline::toScreenSpace(vec4& vert) {
	vec3 ndc = vec3(vert.x / vert.w, vert.y / vert.w, vert.z / vert.w);

	vec3 screen = vec3(WIDTH / 2 * ndc.x + (CORNERX + WIDTH / 2), HEIGHT / 2 * ndc.y + (CORNERY + HEIGHT / 2), 0);

	return screen;
}

void pipeline::ProcessTriangle(vec4& v0, vec4& v1, vec4& v2) {
	triangle trig (toScreenSpace(v0), toScreenSpace(v1), toScreenSpace(v2));
	DrawTriangle(trig);
}

void pipeline::DrawTriangle(const triangle& trig) {
	setpix(active_page, (int)trig.v0.x, (int)trig.v0.y, 15);
	setpix(active_page, (int)trig.v1.x, (int)trig.v1.y, 15);
	setpix(active_page, (int)trig.v2.x, (int)trig.v2.y, 15);

}

void pipeline::Draw(vec4* vertices, int count) {
	AssembleTriangles(vertices, count);
}