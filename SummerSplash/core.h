#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <iostream>

#define GRAVITY 15.0f
#define JUMP_FORCE 7.5f
#define GROUND_Y 1.0f

// ==== Enums ====
typedef enum GameLevels { LEVEL01 = 0, LEVEL02 = 1 } GameLevels;

// ==== Structs ====
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
	std::vector<Collectable> playerInventory;
};

// ==== Variables globales ====
extern std::vector<Collectable> worldObjects;

// ==== Funciones ====
void loadModels01();
void UpdatePlayerBoundingBox(Player& player);
void getGraphicsFirstPersonPlayer(Texture2D hand1, float time);
void UpdateGame(Camera3D* camera, float sensitivity, float moveSpeed, Player& player, GameLevels currentLevel);
void CheckObjectCollection(Player& player);
void BoundingGravityObject(Model model);
