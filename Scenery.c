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
#include "Scenery.h"
#include "Util.h"

Scenery* InitializeScenery(Texture* texture) {
    Scenery* scenery = malloc(sizeof(Scenery));

    scenery->quadric = gluNewQuadric();

    scenery->texture = texture;

    return scenery;
}

void RenderScenery(Scenery* scenery) {
    // Either load the quadric from the scenery
    // struct or create a new one (slower)
    GLUquadric* quadric = scenery != NULL
        ? scenery->quadric
        : gluNewQuadric();

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glBindTexture(GL_TEXTURE_2D, scenery->texture->textureId);
    gluQuadricDrawStyle(quadric, GLU_FILL); 
    glPolygonMode(GL_FRONT, GL_FILL); 
    gluQuadricNormals(quadric, GLU_SMOOTH);
    glBindTexture(GL_TEXTURE_2D, scenery->texture->textureId);

    // Render the first column

    // Recent Matrix = RM
    // Copy RM
    glPushMatrix(); // M = RM
        glPushMatrix(); // M = RM
            glTranslatef(
                COLUMN_1_POSITION.x,
                COLUMN_1_POSITION.y,
                COLUMN_1_POSITION.z
            ); // M = RM * T1
            glRotatef(20, 1.0, 0.0, 0.0); // M = RM * T1 * R1
            RenderColumn(quadric); // Render using M
        glPopMatrix(); // M = RM
        glPushMatrix(); // M = RM
            glTranslatef(
                COLUMN_2_POSITION.x,
                COLUMN_2_POSITION.y,
                COLUMN_2_POSITION.z
            ); // M = RM * T1
            glRotatef(20, 1.0, 0.0, 0.0); // M = RM * T1 * R1
            glRotatef(120, 0.0, 1.0, 0.0); // M = RM * T1 * R1
            RenderColumn(quadric); // Render using M
        glPopMatrix(); // M = RM
    glPopMatrix(); // M = RM
}

void RenderColumn(GLUquadric* quadric) {
    if (quadric == NULL) {
        quadric = gluNewQuadric();
    }

    // Recent Matrix = RM
    // Copy RM
    glPushMatrix(); // M = RM
        glScalef(1.0, 1.0, COLUMN_LENGTH); // M = RM * S1
        gluCylinder(quadric, 1.0, 1.0, 1.0, 50, 50); // Render using M
    glPopMatrix(); // M = RM
}