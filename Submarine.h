// Jared Rand
// 500683609

#ifndef SUBMARINE_H
#define SUBMARINE_H

#include "Vector3D.h"
#include "Propeller.h"
#include <stdbool.h>
#include <OpenGL/glu.h>
#include "SubAndTorpedo.h"
#include "QuadMesh.h"
#include "Camera.h"

static const float SUBMARINE_LENGTH = 6;

static const float SUBMARINE_MIN_SPEED_THRESHOLD = 0.1f;

static const float SUBMARINE_MAX_SPEED_MOVEMENT = 15;
static const float SUBMARINE_INCREASE_SPEED_MOVEMENT = 1.25f;
static const float SUBMARINE_DECREASE_SPEED_MOVEMENT = 0.95f;

static const float SUBMARINE_MAX_SPEED_ROTATION = 75;
static const float SUBMARINE_INCREASE_SPEED_ROTATION = 1.5f;
static const float SUBMARINE_DECREASE_SPEED_ROTATION = 0.95f;

static const float SUBMARINE_MAX_SPEED_BALLAST = 10;
static const float SUBMARINE_INCREASE_SPEED_BALLAST = 1.25f;
static const float SUBMARINE_DECREASE_SPEED_BALLAST = 0.75f;

static const float PERISCOPE_ROTATION_SPEED = 2;

Submarine* InitializeSubmarine(float x, float y, float z, Texture* texture);

void RenderSubmarine(Submarine* submarine);

void RenderHull(Submarine* submarine);

// We only pass submarine in so we can access our
// saved quadric object.
void RenderTurret(Submarine* submarine);

void RenderPeriscope(Submarine* submarine);

void SubmarineHandleKeyPress(unsigned char key, Submarine* submarine);

Torpedo* SubmarineHandleKeyRelease(unsigned char key, Submarine* submarine, Texture* texture);

void SubmarineHandleFunctionKeyPress(int key, Submarine* submarine);

void SubmarineHandleFunctionKeyRelease(int key, Submarine* submarine);

void FloatSubmarine(Submarine* submarine, int delta);

void RotateSubmarine(Submarine* submarine, int delta);

void MoveSubmarine(Submarine* submarine, int delta, int xMin, int xMax, int zMin, int zMax);

void RotatePeriscope(Submarine* submarine, int delta);

void RotateToPeriscope(Submarine* submarine);

void UpdateSubmarine(Submarine* submarine, int delta, int xMin, int xMax, int zMin, int zMax);

bool CollideWithTorpedo(Submarine* submarine, Torpedo* torpedo);

bool CollideWithGround(Submarine* submarine, QuadMesh* qm);

Submarine* SubmarineCheckCollisions(Submarine* submarine, Torpedo* torpedo, QuadMesh* qm);

void GetCamera(Camera* camera, Submarine* submarine);

void RandomlyControlSubmarine(Submarine* submarine);

#endif