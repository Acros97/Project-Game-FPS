#include "raylib.h"
#include "raymath.h"
// levels must be designed

struct Level {
	BoundingBox walls[4]; // four walls
};

void DrawLevel01() {
	DrawCube({ -25, 5, 0 }, 1, 10, 50, RED);
	DrawCube({ 25, 5, 0 }, 1, 10, 50, RED); // put length over X position
	DrawCube({ 0,  5, 25 }, 50, 10, 1, RED); // put length over Y position
	DrawCube({ 0, 5,-25 }, 50, 10, 1, RED);
}

struct Level level01Collisions = {
	{
		{ Vector3{-25.5f, 0, -25.0f}, Vector3{-24.5f, 5, 25.0f} }, // left wall
		{ Vector3{24.5f, 0, -25.0f}, Vector3{25.5f, 5, 25.0f} },  // right wall
		{ Vector3{-25.0f, 0, 24.5f}, Vector3{25.0f, 5, 25.5f} },  // behind wall
		{ Vector3{-25.0f, 0, -25.5f}, Vector3{25.0f, 5, -24.5f} }  // front wall
	}
};