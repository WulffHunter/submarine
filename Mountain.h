// Jared Rand
// 500683609

#ifndef MOUNTAIN_H
#define MOUNTAIN_H

#include <stdlib.h>

static const float MOUNTAIN_MAX_HEIGHT = 10;
static const float MOUNTAIN_MIN_HEIGHT = -10;

typedef struct Mountain {
    float x;
    float z;
    float height;
    float width;
} Mountain;

typedef struct MountainStack {
    Mountain* stack;
    size_t used;
    size_t size;
} MountainStack;

void moveMountain(Mountain* mountain, float x, float z);

void resizeMountain(Mountain* mountain, float height, float width);

MountainStack* initMountainStack(size_t initialSize);

void pushMountainStack(MountainStack* ms, Mountain mountain);

void popMountainStack(MountainStack* ms);

void clearMountainStack(MountainStack* ms);

Mountain* peekMountainStack(MountainStack* ms);

void freeMountainStack(MountainStack* ms);

void mountainHandleKeyRelease(MountainStack* ms, unsigned char key);

void mountainHandleFunctionKeyRelease(MountainStack* ms, int key);
#endif