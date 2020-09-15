// Jared Rand
// 500683609

#ifndef GROUND_H
#define GROUND_H

#include "Vector3D.h"
#include "Mountain.h"
#include "QuadMesh.h"

float getYCoordinateFromMountain(Vector3D* vertex, Mountain* mountain);

float getVertexHeight(Vector3D* vertex, MountainStack* mountainStack);

void getAllVertexHeights(QuadMesh* quadMesh, MountainStack* mountainStack);
#endif