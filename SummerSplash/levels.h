#include "raylib.h"
struct  Level {
	BoundingBox walls[4];
};
extern Level level01Collisions;

void DrawLevel01();
