// Jared Rand
// 500683609

#include "Vector3D.h"
#include "Mountain.h"
#include <math.h>
#include "QuadMesh.h"
#include "Ground.h"
#include <stdio.h>
#include <stdlib.h>

// Gets the fraction of a float
float fract(float x) {
    return x - floorf(x);
}

// Gets the dot product of two 2D vectors
float dot(float x1, float y1, float x2, float y2) {
    return (x1 * x2) + (y1 * y2);
}

// Cap a float between a min and max
float capped(float input, float min, float max) {
    //If the min and max are the same, return that: else, return the random number
    return (min != max) ? (fmodf(input, ((max - min) + min))) : min;
}

// Get a "random" height from the x and z coordinate
float getRandomHeight(Vector3D* vertex) {
    return capped(
        fract(sinf(dot(vertex->x, vertex->z, 12.9898, 78.233))* 43758.5453123),
        -0.2,
        0.2
    );
}

float getYCoordinateFromMountain(Vector3D* vertex, Mountain* mountain) {
    return mountain->height * (expf(-(
        (
            powf(mountain->x - vertex->x, 2) +
            powf(mountain->z - vertex->z, 2)
        ) / mountain->width
    )));
}

float getVertexHeight(Vector3D* vertex, MountainStack* mountainStack) {
    if (mountainStack != NULL) {
        float y = 0;
        int i;

        // Sum the heights retrieved from all of the mountains
        // in the mountain array
        for (i = 0; i < mountainStack->used; i++) {
            y += getYCoordinateFromMountain(vertex, &mountainStack->stack[i]);
        }

        return y;
    }
    return 0;
}

void getAllVertexHeights(QuadMesh* quadMesh, MountainStack* mountainStack) {
    int i;
    Vector3D* currentVector;

    for (i = 0; i < quadMesh->numVertices; i++) {
        currentVector = &quadMesh->vertices[i].position;

        currentVector->y =
            getRandomHeight(currentVector) +
            getVertexHeight(currentVector, mountainStack);
    }

    ComputeNormalsQM(quadMesh);
}