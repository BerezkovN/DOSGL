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
    DrawLine(trig.v0, trig.v1);
    DrawLine(trig.v1, trig.v2);
    DrawLine(trig.v2, trig.v0);
}

void pipeline::DrawLine(vec3& v0, vec3& v1) {
    float dx = v1.x - v0.x;
    float dy = v1.y - v0.y;

    float adx = abs(dx);
    float ady = abs(dy);

    float step;
    if (adx >= ady)
        step = adx;
    else
        step = ady;

    dx /= step;
    dy /= step;

    float x = v0.x;
    float y = v0.y;
    int i = 1;

    while (i <= step)
    {
        setpix(active_page, (int)x, (int)y, 15);
        x += dx;
        y += dy;
        i++;
    }
}

void pipeline::Draw(vec4* vertices, int count) {
	AssembleTriangles(vertices, count);
}