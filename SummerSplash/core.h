#include "raylib.h"
#include "raymath.h"

struct  Player {
	Vector3 position;
	float speed;
	float velocityY;
	bool isGrounded;
	BoundingBox boundingBox;
};

typedef enum GameLevels { LEVEL01 = 0, LEVEL02 = 1 } GameLevels;

void CameraRotation(Camera3D *camera, float sensitivity);
void SetKeyboard(Camera3D *camera, float moveSpeed, Player &player);
void UpdatePlayerBoundingBox(Player &player);
void ApplyGravity(Player &player);
void UpdateGame(Camera3D *camera, float sensitivity, float moveSpeed, Player &player, GameLevels currentLevel);
