// Jared Rand
// 500683609

#include "Camera.h"
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
    #include <glut/glut.h>
#else
    #include <gl/glut.h>
#endif
#include <stdlib.h>

Camera* InitializeCamera() {
    Camera* camera = malloc(sizeof(Camera));

    camera->cameraPosition.x = 0;
    camera->cameraPosition.y = 0;
    camera->cameraPosition.z = 0;

    camera->targetPosition.x = 0;
    camera->targetPosition.y = 0;
    camera->targetPosition.z = 0;

    camera->zoom = 40;

    return camera;
}

void LookThroughCamera(Camera* camera, int w, int h) {
    // gluPerspective(60.0, (GLdouble)w / h, 0.2, camera->zoom);

    // Set up the camera at position (0, 6, 22) looking at the origin, up along positive y axis
    gluLookAt(
        camera->cameraPosition.x,
        camera->cameraPosition.y,
        camera->cameraPosition.z,
        camera->targetPosition.x,
        camera->targetPosition.y,
        camera->targetPosition.z,
        0.0, 1.0, 0.0
    ); // M = I * V1
}