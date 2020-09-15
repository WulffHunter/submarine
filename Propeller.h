// Jared Rand
// 500683609

#ifndef PROPELLER_H
#define PROPELLER_H

#include <stdbool.h>
#include "Util.h"

static const float PROPELLER_MIN_SPEED_THRESHOLD = 0.1f;
static const float PROPELLER_MAX_SPEED = 500;
static const float PROPELLER_INCREASE_SPEED = 1.25f;
static const float PROPELLER_DECREASE_SPEED = 0.95f;

// Data structure for a propeller
typedef struct Propeller {
    GLUquadric* propellerQuadric;
    bool isSpinning;

    float rotation;
    float spinVelocity;
} Propeller;

void RenderPropeller(Propeller* propeller, Texture* texture);

void RenderPropellerWing(Propeller* propeller, Texture* texture);

void RenderAllPropellerWings(Propeller* propeller, Texture* texture);

void SpinPropeller(Propeller* propeller, int delta);

void ToggleSpinPropeller(Propeller* propeller);

#endif