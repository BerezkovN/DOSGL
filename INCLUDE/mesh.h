#ifndef __MESH_H
#define __MESH_H

#include "dglm.h"
#include "dosgl.h"

class Mesh {
public:
    // mesh Data
    float*           vertices;
    size_t          vertCount;
    unsigned int*   indices;
    size_t          indCount;
    unsigned int    VAO;

    // constructor
    Mesh(float* vertices, size_t vertCount, unsigned int* indices, size_t indCount)
    {
        this->vertices = vertices;
        this->vertCount = vertCount;
        this->indices = indices;
        this->indCount = indCount;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    // render the mesh
    void Draw()
    {
        // draw mesh
        dglBindVertexArray(VAO);
        dglDrawElements(DGL_TRIANGLES, indCount);
        dglBindVertexArray(0);
    }

private:
    // render data 
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh()
    {
        // create buffers/arrays
        dglGenVertexArrays(1, &VAO);
        dglGenBuffers(1, &VBO);
        dglGenBuffers(1, &EBO);

        dglBindVertexArray(VAO);
        // load data into vertex buffers
        dglBindBuffer(DGL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        dglBufferData(DGL_ARRAY_BUFFER, vertCount * sizeof(float), vertices);

        dglBindBuffer(DGL_ELEMENT_ARRAY_BUFFER, EBO);
        dglBufferData(DGL_ELEMENT_ARRAY_BUFFER, indCount * sizeof(unsigned int), indices);

        // set the vertex attribute pointers
        // vertex Positions
        //dglEnableVertexAttribArray(0);
        dglVertexAttribPointer(0, 0, 3 * sizeof(float), (void*)0);
        dglBindVertexArray(0);
    }
};
#endif