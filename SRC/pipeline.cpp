#include "pipeline.h"
#include "vga.h"

/***
 *  toScreenSpace
 *      Converts clip space coordinates to screen coordinates
 */
vec3 pipeline::toScreenSpace(vec4& vert) {
    vec3 ndc = vec3(vert.x / vert.w, vert.y / vert.w, vert.z / vert.w);

    vec3 screen = vec3(WIDTH / 2 * ndc.x + (CORNERX + WIDTH / 2), HEIGHT / 2 * ndc.y + (CORNERY + HEIGHT / 2), 0);

    return screen;
}

/***
 *  AssembleTriange
 *      Creates triangle struct with screen space coordinates
*/
void pipeline::AssembleTriangle(vec4* vertices) {
	vec4& v0 = vertices[0];
	vec4& v1 = vertices[1];
	vec4& v2 = vertices[2];

    triangle trig(toScreenSpace(v0), toScreenSpace(v1), toScreenSpace(v2));
    DrawTriangle(trig);
}

/***
 *  DrawTriangle
 *      Doesn't use rasterization and just draws lines
 */
void pipeline::DrawTriangle(const triangle& trig) {
    DrawLine(trig.v0, trig.v1);
    DrawLine(trig.v1, trig.v2);
    DrawLine(trig.v2, trig.v0);
}

/***
 *  DrawLinw
 *      DDA line algorithm
 */
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
        //Performance killer
        if (x >= 0 && x <= SCREEN_WIDTH && y >= 0 && y <= SCREEN_HEIGHT)
        setpix(active_page, (int)x, (int)y, 15);
        
        x += dx;
        y += dy;
        i++;
    }
}