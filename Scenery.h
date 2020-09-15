// Jared Rand
// 500683609

#include <OpenGL/glu.h>
#include "Vector3D.h"
#include "Util.h"

static const Vector3D COLUMN_1_POSITION = { 7, 1, 0.0 };
static const Vector3D COLUMN_2_POSITION = { -7, -0.5, -5 };

static const float COLUMN_LENGTH = 6.0f;

typedef struct {
    Texture* texture;

    GLUquadric* quadric;
} Scenery;

Scenery* InitializeScenery(Texture* texture);

void RenderScenery(Scenery* scenery);

void RenderColumn(GLUquadric* quadric);