// Jared Rand
// 500683609

#ifndef TORPEDO_H
#define TORPEDO_H

#include "Submarine.h"
#include "Vector3D.h"
#include "Propeller.h"
#include <OpenGL/glu.h>
#include "SubAndTorpedo.h"

static const float TORPEDO_SPEED = 20;

Torpedo* InitializeTorpedo(Submarine* submarine, Texture* texture);

void UpdateTorpedo(Torpedo* torpedo, int delta);

void RenderTorpedo(Torpedo* torpedo);

#endif