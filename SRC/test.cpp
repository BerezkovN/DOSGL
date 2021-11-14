#include <iostream.h>
#include <stdio.h>
#include <conio.h>

//#define INT13H
#include "dosgl.h"
#include "dglm.h"
#include "smplShdr.h"
#include "vga.h"

#include "mesh.h"

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 200     

int main() {
    dglInit();

    float vertices[] = {
        // positions          // texture coords
        -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, // top right      0
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f, // bottom right   1
        -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, // bottom left    2
         0.5f,  0.5f, -0.5f,   0.0f, 1.0f,  // top left      3

        -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, // top right     4
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f, // bottom right  5
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, // bottom left   6
         0.5f,  0.5f,  0.5f,   0.0f, 1.0f  // top left      7
    };
    unsigned int indices[] = {
        0,2,1, 2,3,1,
        1,3,5, 3,7,5,
        2,6,3, 3,6,7,
        4,5,7, 4,7,6,
        0,4,2, 2,4,6,
        0,1,4, 1,5,4
    };

    dglViewPort(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    unsigned int vao, vbo, ebo;
    dglGenVertexArrays(1, &vao);
    dglGenBuffers(1, &vbo);
    dglGenBuffers(1, &ebo);

    dglBindVertexArray(vao);

    dglBindBuffer(DGL_ARRAY_BUFFER, vbo);
    dglBufferData(DGL_ARRAY_BUFFER, sizeof(vertices), vertices);

    dglBindBuffer(DGL_ELEMENT_ARRAY_BUFFER, ebo);
    dglBufferData(DGL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices);

    dglVertexAttribPointer(0, 0, 5 * sizeof(float), (void*)(0));

    simpleShader myShader;
    dglUseProgram(myShader);

    char kc = 0;

    float rotationX = 0;
    float rotationY = 0;
    float rotationZ = 0;

    //GAME LOOP
    while (kc != 0x1b) {
       
        if (kbhit()) {
            kc = getch();
            
            switch (kc) {
            case 'w':
                rotationX += 10;
                break;
            case 's':
                rotationX -= 10;
                break;
            case 'a':
                rotationY += 10;
                break;
            case 'd':
                rotationY -= 10;
                break;
            case 'e':
                rotationZ += 10;
                break;
            case 'q':
                rotationZ -= 10;
                break;
            }
        }

        //TODO:
        //CHANGE INT TO SIZE_T IN FOR LOOPS

        mat4 model = mat4(1.0f); // make sure to initialize matrix to identity matrix first
        mat4 v = mat4(1.0f);
        mat4 proj = mat4(1.0f);

        model = translate(model, vec3(0.0f, 0.0f, -3.0f));

        model = rotate(model, radians(rotationX), vec3(1.0f, 0.0f, 0.0f));
        model = rotate(model, radians(rotationY), vec3(0.0f, 1.0f, 0.0f));
        model = rotate(model, radians(rotationZ), vec3(0.0f, 0.0f, 1.0f));

        proj = perspective(radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        //printdglm(proj);

        unsigned int modLoc = dglGetUniformLocation("model");
        unsigned int vLoc = dglGetUniformLocation("view");
        unsigned int projLoc = dglGetUniformLocation("projection");

        dglUniformMatrix4fv(modLoc, &model[0][0]);
        dglUniformMatrix4fv(vLoc, &v[0][0]);
        dglUniformMatrix4fv(projLoc, &proj[0][0]);

        dglDrawElements(DGL_TRIANGLES, 36);
        dglSwapBuffers();
    }

    dglTerminate();

	return 0;
}