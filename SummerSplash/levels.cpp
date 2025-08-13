#include "raylib.h"
#include "raymath.h"
#include "core.h"

struct Level {
	BoundingBox walls[4];
};

void DrawLevel01() {
	DrawCube({ -25, 5, 0 }, 1, 10, 50, RED);
	DrawCube({ 25, 5, 0 }, 1, 10, 50, RED);
	DrawCube({ 0, 5, 25 }, 50, 10, 1, RED);
	DrawCube({ 0, 5,-25 }, 50, 10, 1, RED);

	for (auto& obj : worldObjects) {
		if (obj.active) BoundingGravityObject(obj.model);
	}
}

struct Level level01Collisions = {
	{
		{ Vector3{-25.5f, 0, -25.0f}, Vector3{-24.5f, 5, 25.0f} },
		{ Vector3{24.5f, 0, -25.0f}, Vector3{25.5f, 5, 25.0f} },
		{ Vector3{-25.0f, 0, 24.5f}, Vector3{25.0f, 5, 25.5f} },
		{ Vector3{-25.0f, 0, -25.5f}, Vector3{25.0f, 5, -24.5f} }
	}
};