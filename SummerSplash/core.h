#pragma once
#include "raylib.h"
#include "player.h"
#include <vector>



extern std::vector<Collectable> worldObjects;

void CameraRotation(Camera3D* camera, float sensitivity);
void SetKeyboard(Camera3D* camera, float moveSpeed, Player& player);
void UpdatePlayerBoundingBox(Player& player);
void ApplyGravity(Player& player);
void CheckObjectCollection(Player& player);
void BoundingGravityObject(Model model);
void getGraphicsFirstPersonPlayer(Texture2D hand1, float time);
void loadModels01();
void UpdateGame(Camera3D* camera, float sensitivity, float moveSpeed, Player& player, GameLevels currentLevel);