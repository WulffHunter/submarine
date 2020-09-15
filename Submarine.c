// Jared Rand
// 500683609

#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
    #include <glut/glut.h>
#else
    #include <gl/glut.h>
#endif
#include <OpenGL/glu.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Submarine.h"
#include "Vector3D.h"
#include <math.h>
#include "Torpedo.h"
#include <stdio.h>
#include "Scenery.h"
#include "Util.h"

Submarine* InitializeSubmarine(float x, float y, float z, Texture* texture) {
    Submarine* submarine = malloc(sizeof(Submarine));

    submarine->subQuadric = gluNewQuadric();

    // The initial position, which is the origin
    submarine->position = NewVector3D(x, y, z);
    // The initial velocity, which is zero in all directions
    submarine->velocity = NewVector3D(0, 0, 0);
    // The initial rotation, which is zero in all directions
    submarine->rotation = NewVector3D(0, 0, 0);

    submarine->moveForward = false;
    submarine->forwardDirection = true;

    submarine->rotate = 0;
    submarine->rotationVelocity = 0;

    // Quadrics
    submarine->turretQuadric = gluNewQuadric();

    submarine->ballast = 0;

    // The propeller

    submarine->propeller.propellerQuadric = gluNewQuadric();
    // The propeller is initially not moving
    submarine->propeller.isSpinning = false;
    // The propeller's initial rotation is zero
    submarine->propeller.rotation = 0;
    // The propeller's initial spin velocity is zero
    submarine->propeller.spinVelocity = 0;

    submarine->periscope.rotation = 0;
    submarine->periscope.rotate = 0;
    submarine->periscope.rotateSubmarine = false;
    submarine->periscope.lookThroughPeriscope = false;
    submarine->periscope.zoom = 60;
    submarine->periscope.raise = false;
    submarine->periscope.height = 0;

    submarine->texture = texture;

    return submarine;
}

void RenderSubmarine(Submarine* submarine) {
    if (submarine != NULL) {
        // Recent Matrix = RM

        // Copy RM
        glPushMatrix();
            glTranslatef(
                submarine->position.x,
                submarine->position.y,
                submarine->position.z
            ); // M = RM * T1

            glRotatef(submarine->rotation.x, 1.0, 0.0, 0.0); // M = RM * R1
            glRotatef(submarine->rotation.y, 0.0, 1.0, 0.0); // M = RM * R1 * R2
            glRotatef(submarine->rotation.z, 0.0, 0.0, 1.0); // M = RM * R1 * R2 * R3

            // Render heirarchically using M
            RenderHull(submarine);
        glPopMatrix(); // M = RM
    }
}

void RenderHull(Submarine* submarine) {
    if (submarine != NULL) {
        if (submarine->subQuadric == NULL) {
            submarine->subQuadric = gluNewQuadric();
        }

        // Recent Matrix = RM
        // Copy RM
        glPushMatrix();
            // Copy RM
            glPushMatrix();

                glBindTexture(GL_TEXTURE_2D, submarine->texture->textureId);
                
                glScalef(SUBMARINE_LENGTH, 1.0, 1.0); // M = RM * S1
                gluSphere(submarine->subQuadric, 1.0, 50, 50); // Render using M

            glPopMatrix(); // M = RM

            // Copy RM
            glPushMatrix();
                glTranslatef(0.0, 0.25, 0.0); // M = RM * T1
                RenderTurret(submarine); // Render using M
            glPopMatrix(); // M = RM

            // Copy RM
            glPushMatrix();
                glTranslatef(6.0, 0.0, 0.0); // M = RM * T3
                RenderPropeller(&submarine->propeller, submarine->texture); // Render using M
            glPopMatrix(); // M = RM
        glPopMatrix(); // M = RM
    }
}

void RenderTurret(Submarine* submarine) {
    // If the submarine pointer is void,
    // initialize a new quadric and use that.
    // It's far less performant, but nobody
    // should be using this function with a
    // null submarine.
    GLUquadric* quadric = submarine != NULL
        ? submarine->turretQuadric
        : gluNewQuadric();

    // Recent Matrix = RM
    // Copy RM
    glPushMatrix();
        glTranslatef(0.0, 1.0, 0.0); // M = RM * T1
        RenderPeriscope(submarine);
        glPushMatrix();
            glRotatef(90, 1.0, 0.0, 0.0); // M = RM * T1 * R1
            // Widen the x axis a bit
            glScalef(2.0, 0.5, 0.5); // M = RM * T1 * R1 * S1
            gluSphere(quadric, 1.0, 50, 50); // Render using M
            gluCylinder(quadric, 1.0, 1.0, 1.0, 50, 50); // Render using M
        glPopMatrix();
    glPopMatrix(); // M = RM
}

void RenderPeriscope(Submarine* submarine) {
    if (
        submarine != NULL && (
            submarine->periscope.raise ||
            submarine->periscope.height > 0
        )
    ) {
        // If the submarine pointer is void,
        // initialize a new quadric and use that.
        // It's far less performant, but nobody
        // should be using this function with a
        // null submarine.
        GLUquadric* quadric = submarine != NULL
            ? submarine->turretQuadric
            : gluNewQuadric();

        // Recent Matrix = RM
        // Copy RM
        glPushMatrix();
        glTranslatef(0.0, submarine->periscope.height, 0.0); // M = RM * T1
            glPushMatrix();
                glTranslatef(0.0, -0.25, 0.0); // M = RM * T1
                glRotatef(-90, 0.0, 1.0, 0.0); // M = RM * T1 * R1
                glPushMatrix();
                    glRotatef(submarine->rotation.y * -1, 0.0, 1.0, 0.0); // M = RM * T1 * R1
                    glRotatef(fmodf(submarine->periscope.rotation, 360), 0.0, 1.0, 0.0); // M = RM * T1 * R1
                    glScalef(0.25, 0.25, 1); // M = RM * T1 * R1 * S1
                    gluCylinder(quadric, 1.0, 1.0, 1.0, 50, 50); // Render using M
                glPopMatrix();
            glPopMatrix();

            glRotatef(90, 1.0, 0.0, 0.0); // M = RM * T1 * R1
            // Widen the y axis a bit
            glScalef(0.25, 0.25, 1.25); // M = RM * T1 * R1 * S1
            gluCylinder(quadric, 1.0, 1.0, 1.0, 50, 50); // Render using M
        glPopMatrix(); // M = RM
    }
}

void SubmarineHandleKeyPress(unsigned char key, Submarine* submarine) {
    if (submarine != NULL) {
        // Instead of strictly turning on/off booleans,
        // we add/subtract to int's to allow for simultaneous
        // keypresses

        bool tempMoveForward = false;
        bool tempForwardDirection = submarine->moveForward;
        int tempRotate = 0;
        bool tempRotatePropeller = false;
        float tempZoom = submarine->periscope.zoom;

        switch (key) {
            case 'w':
                tempMoveForward = true;
                tempForwardDirection = true;
                tempRotatePropeller = true;
            break;
            case 'a':
                tempRotate += 1;
            break;
            case 's':
                tempMoveForward = true;
                tempForwardDirection = false;
                tempRotatePropeller = true;
            break;
            case 'd':
                tempRotate -= 1;
            break;
            case 'z':
                tempZoom = tempZoom > 10
                    ? tempZoom - 10
                    : tempZoom;
            break;
            case 'x':
                tempZoom = tempZoom < 100
                    ? tempZoom + 10
                    : tempZoom;
            break;
        }

        submarine->moveForward = tempMoveForward;
        submarine->forwardDirection = tempForwardDirection;
        submarine->rotate = tempRotate;
        submarine->propeller.isSpinning = tempRotatePropeller;
        submarine->periscope.zoom = tempZoom;
    }
}

void SubmarineHandleFunctionKeyPress(int key, Submarine* submarine) {
    // Instead of strictly turning on/off booleans,
    // we add/subtract to int's to allow for simultaneous
    // keypresses
    int tempBallast = 0;
    float tempRotatePeriscope = 0;

    switch (key) {
        case GLUT_KEY_UP:
            tempBallast += 1;
        break;
        case GLUT_KEY_DOWN:
            tempBallast -= 1;
        break;
        case GLUT_KEY_LEFT:
            tempRotatePeriscope += 1;
        break;
        case GLUT_KEY_RIGHT:
            tempRotatePeriscope -= 1;
        break;
    }

    if (submarine != NULL) {
        submarine->ballast = tempBallast;
        submarine->periscope.rotate = tempRotatePeriscope;
    }
}

Torpedo* SubmarineHandleKeyRelease(unsigned char key, Submarine* submarine, Texture* texture) {
    Torpedo* torpedo = NULL;

    if (submarine != NULL) {

        // Instead of strictly turning on/off booleans,
        // we add/subtract to int's to allow for simultaneous
        // keypresses

        bool tempMoveForward = submarine->moveForward;
        int tempRotate = submarine->rotate;

        switch (key) {
            case 'w':
                tempMoveForward = false;
            break;
            case 'a':
                tempRotate -= 1;
            break;
            case 's':
                tempMoveForward = false;
            break;
            case 'd':
                tempRotate += 1;
            break;
            case 'p':
                ToggleSpinPropeller(&submarine->propeller);
            break;
            case ' ':
                torpedo = InitializeTorpedo(submarine, texture);
            break;
            case 'r':
                submarine->periscope.rotateSubmarine = true;
            break;
            case 'c':
                submarine->periscope.lookThroughPeriscope =
                    submarine->periscope.raise &&
                    !submarine->periscope.lookThroughPeriscope;
            break;
            case 'e':
                submarine->periscope.raise = !submarine->periscope.raise;
                submarine->periscope.lookThroughPeriscope = false;
            break;
        }

        submarine->moveForward = tempMoveForward;
        submarine->rotate = tempRotate;
    }

    return torpedo;
}

void SubmarineHandleFunctionKeyRelease(int key, Submarine* submarine) {
    if (submarine != NULL) {
        int tempBallast = submarine->ballast;
        int tempRotatePeriscope = submarine->periscope.rotate;

        switch (key) {
            case GLUT_KEY_UP:
                tempBallast -= 1;
            break;
            case GLUT_KEY_DOWN:
                tempBallast += 1;
            break;
            case GLUT_KEY_LEFT:
                tempRotatePeriscope -= 1;
            break;
            case GLUT_KEY_RIGHT:
                tempRotatePeriscope += 1;
            break;
        }

        submarine->ballast = tempBallast;
        submarine->periscope.rotate = tempRotatePeriscope;
    }
    
}

void FloatSubmarine(Submarine* submarine, int delta) {
    if (submarine != NULL) {
        // Store the submarine's ballast modifier and the velocity
        // of rotation to limit heap reads.
        int ballast = submarine->ballast;
        float velocityY = submarine->velocity.y;

        // If the submarine is no longer rotating
        if (ballast == 0) {
            // If the sub still has movement on the rotation axis
            if (velocityY > SUBMARINE_MIN_SPEED_THRESHOLD) {
                // Gradually slow movement
                velocityY *= SUBMARINE_DECREASE_SPEED_BALLAST;
            } else {
                // Else, fix the sub's rotation ballast at zero
                velocityY = 0;
            }
        } else {
            // If the sub is still
            if (velocityY < SUBMARINE_MIN_SPEED_THRESHOLD) {
                // Give it some initial ballast velocity
                velocityY = SUBMARINE_MIN_SPEED_THRESHOLD;
            }

            // If the sub has not yet reached the maximum ballast speed
            if (velocityY < SUBMARINE_MAX_SPEED_BALLAST) {
                // Gradually increase movement
                velocityY *= SUBMARINE_INCREASE_SPEED_BALLAST;
            }
        }
        // Store the new y velocity for future use
        submarine->velocity.y = velocityY;

        int directionModifier = (ballast != 0)
            ? ballast
            : 1;

        // If the sub is moving on the y axis
        if (velocityY > 0) {
            // Move smoothly using the delta
            float newY = submarine->position.y + (
                ((velocityY * directionModifier) / 1000)
                * delta
            );

            if (newY < 25) {
                submarine->position.y = newY;
            }
        }
    }
}

void RotateSubmarine(Submarine* submarine, int delta) {
    if (submarine != NULL) {
        // Store the submarine's rotation modifier and the velocity
        // of rotation to limit heap reads.
        int rotate = submarine->rotate;
        float rotationVelocity = submarine->rotationVelocity;

        // If the submarine is no longer rotating
        if (rotate == 0) {
            // If the sub still has movement on the rotation axis
            if (rotationVelocity > SUBMARINE_MIN_SPEED_THRESHOLD) {
                // Gradually slow movement
                rotationVelocity *= SUBMARINE_DECREASE_SPEED_ROTATION;
            } else {
                // Else, fix the sub's rotation velocity at zero
                rotationVelocity = 0;
            }
        } else {
            // If the sub is still
            if (rotationVelocity < SUBMARINE_MIN_SPEED_THRESHOLD) {
                // Give it some initial rotation velocity
                rotationVelocity = SUBMARINE_MIN_SPEED_THRESHOLD;
            }

            // If the sub has not yet reached the maximum rotation speed
            if (rotationVelocity < SUBMARINE_MAX_SPEED_ROTATION) {
                // Gradually increase movement
                rotationVelocity *= SUBMARINE_INCREASE_SPEED_ROTATION;
            }
        }
        // Store the new rotation velocity for future use
        submarine->rotationVelocity = rotationVelocity;

        // If the sub is rotating
        if (rotationVelocity > 0) {
            // Rotate smoothly using the delta
            submarine->rotation.y += (
                ((rotationVelocity * rotate) / 1000)
                * delta
            );
        }
    }
}

void MoveSubmarine(Submarine* submarine, int delta, int xMin, int xMax, int zMin, int zMax) {
    if (submarine != NULL) {
        // In C, trig functions are done using radians.
        // We multiply our theta by radians to produce the correct
        // answer.
        float radian = M_PI / 180.0;
        float angle = submarine->rotation.y * radian;

        // Store our current sub's velocity on the x-z plane
        float velocityX = submarine->velocity.x;

        // Store the variable temporarily to prevent too many reads
        int moveForward = submarine->moveForward;

        // If the sub is no longer propelling itself forwards
        if (!moveForward) {
            // If the sub still has some speed
            if (velocityX > SUBMARINE_MIN_SPEED_THRESHOLD) {
                // Gradually slow movement
                velocityX *= SUBMARINE_DECREASE_SPEED_MOVEMENT;
            } else {
                // Else, stop the sub completely
                velocityX = 0;
            }
        } else {
            // If the sub has no speed
            if (velocityX < SUBMARINE_MIN_SPEED_THRESHOLD) {
                // Give it some initial speed
                velocityX = SUBMARINE_MIN_SPEED_THRESHOLD;
            }

            // If the sub has not yet reached its max speed
            if (velocityX < SUBMARINE_MAX_SPEED_MOVEMENT) {
                // Gradually increase movement
                velocityX *= SUBMARINE_INCREASE_SPEED_MOVEMENT;
            }
        }

        // Store the sub's velocity for future use
        submarine->velocity.x = velocityX;

        // If the sub has some velocity
        if (velocityX != 0) {
            // Determine the direction
            int modifyDirection = submarine->forwardDirection
                ? 1
                : -1;

            // Update the x position: the angle it should move at,
            // multiplied by delta for smooth movement
            float newX = submarine->position.x - (
                (((cosf(angle) * velocityX) * modifyDirection)) / 1000
                * delta
            );

            // Update the z position: the angle it should move at,
            // multiplied by delta for smooth movement
            float newZ = submarine->position.z + (
                (((sinf(angle) * velocityX) * modifyDirection)) / 1000
                * delta
            );

            
            if (newX < xMax && newX > xMin) {
                submarine->position.x = newX;
            }

            if (newZ < zMax && newZ > zMin) {
                submarine->position.z = newZ;
            }
        }
    }
}

void RotatePeriscope(Submarine* submarine, int delta) {
    submarine->periscope.rotation += (
        submarine->periscope.rotate * PERISCOPE_ROTATION_SPEED
    ) / 10 * delta;

    if (submarine->periscope.raise) {
        if (submarine->periscope.height < 1.25) {
            submarine->periscope.height += (((float) delta) / 1000);
        } else {
            submarine->periscope.height = 1.25;
        }
    } else {
        if (submarine->periscope.height > 0) {
            submarine->periscope.height -= (((float) delta) / 1000);
        } else {
            submarine->periscope.height = 0;
        }
    }
}

void RotateToPeriscope(Submarine* submarine) {
    if (submarine != NULL) {
        if (submarine->periscope.rotateSubmarine) {
            float difference = (fmodf(submarine->rotation.y, 360.0f)) -
                (fmodf(submarine->periscope.rotation, 360.0f));

            if (difference > 3) {
                submarine->rotate = -1;
            } else if (difference < -3) {
                submarine->rotate = 1;
            } else {
                submarine->rotate = 0;
                submarine->periscope.rotateSubmarine = false;
            }
        }
    }
}

// `delta` refers to the time elapsed since the last frame
// to enable smooth movement.
void UpdateSubmarine(Submarine* submarine, int delta, int xMin, int xMax, int zMin, int zMax) {
    if (submarine != NULL) {
        // If we're rotating to the position of the periscope, do that
        RotateToPeriscope(submarine);
        
        // First, we calculate the ballast (bouyancy) of the submarine
        FloatSubmarine(submarine, delta);

        // Second, we calculate the rotation of the submarine
        RotateSubmarine(submarine, delta);

        // Finally, we calculate the forward (x-z plane) movement
        MoveSubmarine(submarine, delta, xMin, xMax, zMin, zMax);

        SpinPropeller(&submarine->propeller, delta);

        RotatePeriscope(submarine, delta);
    }
}

bool CollideWithTorpedo(Submarine* submarine, Torpedo* torpedo) {
    return (
        torpedo != NULL &&
        submarine != NULL &&
        Distance(
            &submarine->position,
            &torpedo->position
        ) <= (
            SUBMARINE_LENGTH + TORPEDO_LENGTH
        )
    ) && torpedo->creator != submarine;
}

bool CollideWithGround(Submarine* submarine, QuadMesh* qm) {
    return submarine != NULL &&
        submarine->position.y <= 1;
}

bool CollideWithScenery(Submarine* submarine, Vector3D column) {
    return (
        submarine != NULL &&
        Distance(
            &submarine->position,
            &column
        ) <= (
            SUBMARINE_LENGTH + (COLUMN_LENGTH / 8)
        )
    );
}

Submarine* SubmarineCheckCollisions(Submarine* submarine, Torpedo* torpedo, QuadMesh* qm) {
    if (
        CollideWithTorpedo(submarine, torpedo) ||
        CollideWithGround(submarine, qm) ||
        CollideWithScenery(submarine, COLUMN_1_POSITION) ||
        CollideWithScenery(submarine, COLUMN_2_POSITION)
    ) {
        free(submarine);

        return NULL;
    }
    return submarine;
}

void GetCamera(Camera* camera, Submarine* submarine) {
    if (camera != NULL) {
        if (submarine != NULL) {
            float radian = M_PI / 180.0;

            camera->targetPosition.x = submarine->position.x;
            camera->targetPosition.y = submarine->position.y;
            camera->targetPosition.z = submarine->position.z;

            camera->zoom = 40;

            if (submarine->periscope.lookThroughPeriscope) {
                float angle = (fmodf(submarine->periscope.rotation, 360)) * radian;

                float x = -cosf(angle);
                float z = sinf(angle);

                camera->cameraPosition.x = submarine->position.x + x * 0.65;
                camera->cameraPosition.y = submarine->position.y + 2.25;
                camera->cameraPosition.z = submarine->position.z + z * 0.65;

                camera->targetPosition.x = submarine->position.x + (x * submarine->periscope.zoom);
                camera->targetPosition.y = camera->cameraPosition.y;
                camera->targetPosition.z = submarine->position.z + (z * submarine->periscope.zoom);

                camera->zoom = submarine->periscope.zoom;
            } else {
                float angle = (fmodf(submarine->rotation.y, 360)) * radian;

                camera->cameraPosition.x = submarine->position.x + -cosf(angle) * -12;
                camera->cameraPosition.y = submarine->position.y + 5;
                camera->cameraPosition.z = submarine->position.z + sinf(angle) * -12;
            }
        }
    }
}

void RandomlyControlSubmarine(Submarine* submarine) {
    float newMove = randomRange(0, 150);

    submarine->moveForward = false;
    submarine->rotate = 0;
    submarine->propeller.isSpinning = true;
    submarine->ballast = 0;

    if (newMove < 40) {
        submarine->rotate = 1;
    } else if (newMove < 80) {
        submarine->rotate = -1;
    } else if (newMove < 95) {
        submarine->ballast = 1;
    } else if (newMove < 105) {
        submarine->ballast = -1;
    } else if (newMove < 130) {
        submarine->forwardDirection = true;
        submarine->moveForward = true;
    } else if (newMove < 140) {
        submarine->forwardDirection = false;
        submarine->moveForward = true;
    } 
}