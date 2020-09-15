// Jared Rand
// 500683609

#ifndef SUB_AND_TORPEDO_H
#define SUB_AND_TORPEDO_H

#include "Vector3D.h"
#include "Propeller.h"
#include <stdbool.h>
#include <OpenGL/glu.h>
#include "Util.h"

typedef struct Periscope {
    int rotate;
    float rotation;

    bool rotateSubmarine;
    bool lookThroughPeriscope;

    bool raise;
    float height;

    float zoom;
} Periscope;

// Data structure for a submarine
typedef struct Submarine {
    Texture* texture;

    GLUquadric* subQuadric;

    Vector3D position;
    Vector3D velocity;
    Vector3D rotation;

    bool moveForward;
    bool forwardDirection;

    int rotate;
    float rotationVelocity;

    Periscope periscope;

    int ballast;

    GLUquadric* turretQuadric;

    Propeller propeller;
} Submarine;

static const float TORPEDO_LENGTH = 2;

// Data structure for a submarine
typedef struct Torpedo {
    GLUquadric* torpedoQuadric;

    Vector3D position;
    float yRotation;

    Propeller propeller;

    Submarine* creator;

    Texture* texture;
} Torpedo;

#endif