// levels.h
#pragma once
#include "raylib.h"
#include "core.h"

struct LevelCollisions {
	BoundingBox walls[4];
};

extern LevelCollisions level01Collisions;
void DrawLevel01();
Vector3 playerl01Position();