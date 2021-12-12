#include "pipeline.h"
#include "vga.h"

#include <conio.h>
#include <iostream.h>

//#define CULLBACK

void pipeline::AssembleTriangle(vec4* vertices) {
	vec4& v0 = vertices[0];
	vec4& v1 = vertices[1];
	vec4& v2 = vertices[2];

#if defined(CULLBACK)
	//CULLBACK
    vec3 v30 = vec3(v0.x, v0.y, v0.z);
    vec3 v31 = vec3(v1.x, v1.y, v1.z);
    vec3 v32 = vec3(v2.x, v2.y, v2.z);

    if ((v31 - v30) % (v32 - v30) * v30 <= 0.0f)
#endif
		ProcessTriangle(v0, v1, v2);

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
#if defined(DEBUG)
    cout << "\nTriangle being draw with coordinates:\n";
    cout << " " << trig.v0.x << " " << trig.v0.y << "\n";
    cout << " " << trig.v1.x << " " << trig.v1.y << "\n";
    cout << " " << trig.v2.x << " " << trig.v2.y << "\n";
#endif

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
#if defined(INT13H)
        double_buffer[(unsigned int)x * WIDTH + (unsigned int)y] = 15;
#else
        setpix(active_page, (int)x, (int)y, 15);
#endif
        
        x += dx;
        y += dy;
        i++;
    }
}