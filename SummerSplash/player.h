#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>
using namespace std;

#define GRAVITY 15.0f
#define JUMP_FORCE 7.5f
#define GROUND_Y 1.0f

typedef enum { LEVEL01 = 0, LEVEL02 = 1 } GameLevels;

struct Collectable {
	Model model;
	Vector3 position;
	BoundingBox box;
	bool active;
};

struct Player {
	Vector3 position;
	float speed;
	float velocityY;
	bool isGrounded;
	BoundingBox boundingBox;
	vector<Collectable> playerInventory;
};


