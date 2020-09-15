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
#include "Propeller.h"
#include <math.h>

void RenderPropellerWing(Propeller* propeller, Texture* texture) {
    glBindTexture(GL_TEXTURE_2D, texture->textureId);
    // Recent Matrix = RM
    // Copy RM
    glPushMatrix();
        glTranslatef(0.0, 0.25, 0.0); // M = RM * T1
        glRotatef(45, 0.0, 1.0, 0.0); // M = RM * T1 * R1
        // Widen the x axis a bit
        glScalef(0.025, 2.0, 0.25); // M = RM * T1 * R1 * S1
        gluSphere(propeller->propellerQuadric, 0.25, 50, 50); // Render using M
    glPopMatrix(); // M = RM
}

void RenderAllPropellerWings(Propeller* propeller, Texture* texture) {
    glBindTexture(GL_TEXTURE_2D, texture->textureId);
    // Recent Matrix = RM
    // Copy RM
    glPushMatrix(); // M = RM
        RenderPropellerWing(propeller, texture); // Render using M
    glPopMatrix(); // M = RM
    // Copy RM
    glPushMatrix(); // M = RM
        glRotatef(90, 1.0, 0.0, 0.0); // M = RM * R1
        RenderPropellerWing(propeller, texture);
    glPopMatrix(); // M = RM
    // Copy RM
    glPushMatrix(); // M = RM
        glRotatef(180, 1.0, 0.0, 0.0); // M = RM * R2
        RenderPropellerWing(propeller, texture); // Render using M
    glPopMatrix(); // M = RM
    // Copy RM
    glPushMatrix(); // M = RM
        glRotatef(270, 1.0, 0.0, 0.0); // M = RM * R3
        RenderPropellerWing(propeller, texture); // Render using M
    glPopMatrix(); // M = RM
}

void RenderPropeller(Propeller* propeller, Texture* texture) {
    glBindTexture(GL_TEXTURE_2D, texture->textureId);
    if (propeller != NULL) {
        if (propeller->propellerQuadric == NULL) {
            propeller->propellerQuadric = gluNewQuadric();
        }

        // Recent Matrix = RM
        // Copy RM
        glPushMatrix(); // M = RM
            // Rotate the propeller by `rotation` degrees along
            // the x axis
            glRotatef(propeller->rotation, 1.0, 0.0, 0.0); // M = RM * R1
            RenderAllPropellerWings(propeller, texture); // Render using M
            // Since this is using M = RM * R1, the sphere rotates too
            gluSphere(propeller->propellerQuadric, 0.25, 50, 50); // Render using M
        glPopMatrix(); // M = RM
    }
}

void SpinPropeller(Propeller* propeller, int delta) {
    if (propeller != NULL) {
        // If the propeller is spinning
        if (propeller->isSpinning) {

            // If it's not spinning at full speed
            if (propeller->spinVelocity < PROPELLER_MAX_SPEED) {
                // If the propeller is stopped, give it a tiny bit of spin
                // so we can increase it
                if (propeller->spinVelocity == 0) {
                    propeller->spinVelocity = PROPELLER_MIN_SPEED_THRESHOLD;
                }
                // Increase the velocity
                propeller->spinVelocity *= PROPELLER_INCREASE_SPEED;
            }
        } else {
            // Else, if the propeller is no longer spinning

            // If the propeller is still moving
            if (propeller->spinVelocity > PROPELLER_MIN_SPEED_THRESHOLD) {
                // Gradually slow the propeller
                propeller->spinVelocity *= 0.95;
            } else {
                // Else, keep the propeller stopped
                propeller->spinVelocity = 0;
            }
        }
        

        // The propeller's current rotation is the old rotation plus
        // the velocity. Mod by 360 degrees to prevent overflow.
        // We have to use `fmod()` because we're working with
        // floats.
        propeller->rotation = fmod((
            propeller->rotation + (
                (propeller->spinVelocity / 1000) * delta
            )
        ), 360.0);   
    }
}

void ToggleSpinPropeller(Propeller* propeller) {
    if (propeller != NULL) {
        propeller->isSpinning = !propeller->isSpinning;
    }
}