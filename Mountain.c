// Jared Rand
// 500683609

#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
	#include <glut/glut.h>
#else
	#include <gl/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "Mountain.h"

void moveMountain(Mountain* mountain, float x, float z) {
    mountain->x = x;
    mountain->z = z;
}

void resizeMountain(Mountain* mountain, float height, float width) {
    if (height < MOUNTAIN_MAX_HEIGHT && height > MOUNTAIN_MIN_HEIGHT) {
        mountain->height = height;
    }
    mountain->width = width;
}

MountainStack* initMountainStack(size_t initialSize) {
    MountainStack*  ms = malloc(sizeof(MountainStack));

    ms->stack = (Mountain*) malloc(initialSize * sizeof(Mountain));
    ms->used = 0;
    ms->size = initialSize;

    return ms;
}

void pushMountainStack(MountainStack* ms, Mountain mountain) {
    if (ms != NULL) {
        if (ms->used == ms->size) {
            ms->size *= 2;
            ms->stack = (Mountain *) realloc(
                ms->stack,
                ms->size * sizeof(Mountain)
            );
        }

        ms->stack[ms->used++] = mountain;
    }
}

void popMountainStack(MountainStack* ms) {
    // We don't need to delete any information in the last mountain:
    // it will be overwritten if we push another mountain onto the stack
    ms->used--;
}

void clearMountainStack(MountainStack* ms) {
    // We don't need to delete any information in the last mountain:
    // it will be overwritten if we push another mountain onto the stack
    ms->used = 0;
}

Mountain* peekMountainStack(MountainStack* ms) {
    return &ms->stack[ms->used - 1];
}

void freeMountainStack(MountainStack* ms) {
    free(ms->stack);
    ms->stack = NULL;
    ms->used = ms->size = 0;
}

void mountainHandleKeyRelease(MountainStack* ms, unsigned char key) {
    switch (key) {
        case 'u':
            popMountainStack(ms);
            break;
        case 'r':
            clearMountainStack(ms);
            break;
        default:
            break;
    }
}

void mountainHandleFunctionKeyRelease(MountainStack* ms, int key) {
    Mountain* mountain = peekMountainStack(ms);

    switch (key) {
        case GLUT_KEY_UP:
            resizeMountain(mountain, mountain->height + 1, mountain->width);
        break;
        case GLUT_KEY_DOWN:
            resizeMountain(mountain, mountain->height - 1, mountain->width);
        break;
        case GLUT_KEY_RIGHT:
            resizeMountain(mountain, mountain->height, mountain->width * 2);
        break;
        case GLUT_KEY_LEFT:
            resizeMountain(mountain, mountain->height, mountain->width / 2);
        break;
    }
}