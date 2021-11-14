#include <iostream.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

//#define INT13H
#include "dosgl.h"
#include "dglm.h"
#include "smplShdr.h"
#include "vga.h"

#include "mesh.h"

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 200     

void extractFromOBJ(char* string, float** vertices, unsigned int& vertCount, unsigned int** indices, unsigned int& indCount) {
    FILE* objFile = fopen(string, "r");

    if (!objFile)
        dglTerminate();

    char* input = new char[100];

    vertCount = 0;
    indCount = 0;

    while (fscanf(objFile, "%s", input) != EOF) {
        if (input[0] == 'v' && input[1] == '\0')
            vertCount++;
        if (input[0] == 'f' && input[1] == '\0')
            indCount++;
    }
    vertCount *= 3;
    indCount *= 3;

    

    *vertices = new float[vertCount];
    *indices = new unsigned int[indCount];

    objFile = fopen(string, "r");

    unsigned int currVert = 0;
    unsigned int currInd = 0;

    while (fscanf(objFile, "%s", input) != EOF) {
        if (input[0] == 'v' && input[1] == '\0') {
            fscanf(objFile, "%f", &(vertices[0][currVert++]));
            fscanf(objFile, "%f", &(vertices[0][currVert++]));
            fscanf(objFile, "%f", &(vertices[0][currVert++]));
        }
        if (input[0] == 'f' && input[1] == '\0') {
            fscanf(objFile, "%s", input);
            sscanf(input, "%d%[^/]", &(indices[0][currInd++]));
            indices[0][currInd - 1]--;
            
            fscanf(objFile, "%s", input);
            sscanf(input, "%d%[^/]", &(indices[0][currInd++]));
            indices[0][currInd - 1]--;

            fscanf(objFile, "%s", input);
            sscanf(input, "%d%[^/]", &(indices[0][currInd++]));
            indices[0][currInd - 1]--;

        }
    }

}

int main() {
    dglInit();

    float* vertices = 0;
    unsigned int* indices = 0;

    unsigned int vertCount = 0;
    unsigned int indCount = 0;

    extractFromOBJ("MODELS\\cube.obj", &vertices, vertCount, &indices, indCount);

    dglViewPort(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


    Mesh cube(vertices, vertCount, indices, indCount);

    simpleShader myShader;
    dglUseProgram(myShader);

    char kc = 0;

    float rotationX = 0;
    float rotationY = 0;
    float rotationZ = 0;

    float scaleF = 1.0f;

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
            case 't':
                scaleF += 0.1f;
                break;
            case 'g':
                scaleF -= 0.1f;
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

        model = scale(model, vec3(scaleF, scaleF, scaleF));

        proj = perspective(radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        //printdglm(proj);

        unsigned int modLoc = dglGetUniformLocation("model");
        unsigned int vLoc = dglGetUniformLocation("view");
        unsigned int projLoc = dglGetUniformLocation("projection");

        dglUniformMatrix4fv(modLoc, &model[0][0]);
        dglUniformMatrix4fv(vLoc, &v[0][0]);
        dglUniformMatrix4fv(projLoc, &proj[0][0]);

        cube.Draw();
        dglSwapBuffers();
    }

    dglTerminate();

    return 0;
}