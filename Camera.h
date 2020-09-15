// Jared Rand
// 500683609

#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3D.h"

typedef struct Camera {
    Vector3D cameraPosition;
    Vector3D targetPosition;

    float zoom;
} Camera;

Camera* InitializeCamera();

void LookThroughCamera(Camera* camera, int w, int h);

#endif