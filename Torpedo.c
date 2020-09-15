// Jared Rand
// 500683609

#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
    #include <glut/glut.h>
#else
    #include <gl/glut.h>
#endif
#include "Vector3D.h"
#include "Propeller.h"
#include <OpenGL/glu.h>
#include "Submarine.h"
#include "Torpedo.h"
#include "Propeller.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

Torpedo* InitializeTorpedo(Submarine* submarine, Texture* texture) {
    Torpedo* torpedo = malloc(sizeof(Torpedo));

    torpedo->torpedoQuadric = gluNewQuadric();

    torpedo->position.x = submarine->position.x;
    torpedo->position.y = submarine->position.y;
    torpedo->position.z = submarine->position.z;

    torpedo->yRotation = submarine->rotation.y;

    torpedo->propeller.isSpinning = true;
    torpedo->propeller.propellerQuadric = gluNewQuadric();
    torpedo->propeller.rotation = 0;
    torpedo->propeller.spinVelocity = PROPELLER_MAX_SPEED;

    torpedo->creator = submarine;

    torpedo->texture = texture;

    return torpedo;
}

void UpdateTorpedo(Torpedo* torpedo, int delta) {
    if (torpedo != NULL) {
        torpedo->propeller.isSpinning = true;
        // In C, trig functions are done using radians.
        // We multiply our theta by radians to produce the correct
        // answer.
        float radian = M_PI / 180.0;
        float angle = torpedo->yRotation * radian;

        // Update the x position: the angle it should move at,
        // multiplied by delta for smooth movement
        torpedo->position.x -= (
            (cosf(angle) * TORPEDO_SPEED) / 1000
            * delta
        );

        // Update the z position: the angle it should move at,
        // multiplied by delta for smooth movement
        torpedo->position.z += (
            (sinf(angle) * TORPEDO_SPEED) / 1000
            * delta
        );
    }
}

void RenderTorpedo(Torpedo* torpedo) {
    if (torpedo != NULL) {
        if (torpedo->torpedoQuadric == NULL) {
            torpedo->torpedoQuadric = gluNewQuadric();
        }

        glBindTexture(GL_TEXTURE_2D, torpedo->texture->textureId);

        // Recent Matrix = RM
        // Copy RM
        glPushMatrix();
            glTranslatef(
                torpedo->position.x,
                torpedo->position.y,
                torpedo->position.z
            );
            glRotatef(torpedo->yRotation, 0.0, 1.0, 0.0);
            // Copy RM
            glPushMatrix();
                glScalef(TORPEDO_LENGTH, 0.25, 0.25); // M = RM * S1
                gluSphere(torpedo->torpedoQuadric, 1.0, 50, 50); // Render using M
            glPopMatrix(); // M = RM

            // Copy RM
            glPushMatrix();
                // glScalef(0.25, 0.25, 0.25); // M = RM * S1
                glTranslatef(2.0, 0.0, 0.0); // M = RM * T3
                RenderPropeller(&torpedo->propeller, torpedo->texture); // Render using M
            glPopMatrix(); // M = RM
        glPopMatrix(); // M = RM
    }
}