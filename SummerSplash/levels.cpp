#include "raylib.h"
#include "player.h"
#include "levels.h"

LevelCollisions level01Collisions = {
	{
		BoundingBox{ Vector3{-25.5f, 0.0f, -25.0f}, Vector3{-24.5f, 10.0f, 25.0f} },
		BoundingBox{ Vector3{ 24.5f, 0.0f, -25.0f}, Vector3{ 25.5f, 10.0f, 25.0f} },
		BoundingBox{ Vector3{-25.0f, 0.0f,  24.5f}, Vector3{ 25.0f, 10.0f, 25.5f} },
		BoundingBox{ Vector3{-25.0f, 0.0f, -25.5f}, Vector3{ 25.0f, 10.0f, -24.5f} }
	}
};

void DrawLevel01() {
	DrawCube({ -25, 5, 0 }, 1, 10, 50, RED);
	DrawCube({ 25, 5, 0 }, 1, 10, 50, RED);
	DrawCube({ 0, 5, 25 }, 50, 10, 1, RED);
	DrawCube({ 0, 5, -25 }, 50, 10, 1, RED);

	for (const auto& obj : worldObjects) {
		if (obj.active) {
			BoundingGravityObject(obj.model);
		}
	}
}

Vector3 playerl01Position() {
	return  { 11.0f, 1.0f, 3.0f };
}