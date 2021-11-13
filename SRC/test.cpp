#include <iostream.h>
#include <stdio.h>
#include <conio.h>

//#define INT13H
#include "dosgl.h"
#include "dglm.h"
#include "smplShdr.h"

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 200     

int main() {
    //dglInit();

    float vertices[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
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

    int rotation = 0;
    float height = 0;

    //GAME LOOP
    while (kc != 0x1b) {
       
        if (kbhit()) {
            kc = getch();
            
            switch (kc) {
            case 'w':
                rotation += 10;
                break;
            case 's':
                rotation -= 10;
                break;
            case 'e':
                height += 0.01f;
                break;
            case 'q':
                height -= 0.01f;
                break;
            }
        }

        //TODO:
        //CHANGE INT TO SIZE_T IN FOR LOOPS

        mat4 mod = mat4(1.0f); // make sure to initialize matrix to identity matrix first
        mat4 v = mat4(1.0f);
        mat4 proj = mat4(1.0f);
        mod = rotate(mod, radians(rotation), vec3(1.0f, 0.0f, 0.0f));
        v = translate(v, vec3(0.0f, height, -3.0f));
        proj = perspective(radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        //printdglm(proj);

        unsigned int modLoc = dglGetUniformLocation("model");
        unsigned int vLoc = dglGetUniformLocation("view");
        unsigned int projLoc = dglGetUniformLocation("projection");

        dglUniformMatrix4fv(modLoc, &mod[0][0]);
        dglUniformMatrix4fv(vLoc, &v[0][0]);
        dglUniformMatrix4fv(projLoc, &proj[0][0]);
        

        dglDrawElements(DGL_TRIANGLES, 6);
        dglSwapBuffers();
    }

    dglTerminate();

	return 0;
}