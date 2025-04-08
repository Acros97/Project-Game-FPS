#include "raylib.h"
#include "raymath.h"
#include "levels.h"

#define GRAVITY 15.0f
#define JUMP_FORCE 7.5f
#define GROUND_Y 1.0f

Vector3 oldPosition;
float yaw = 0.0f, pitch = 0.0f;
bool isJumping = false;
typedef enum GameLevels { LEVEL01 = 0, LEVEL02 = 1 } GameLevels;

struct Player {
	Vector3 position;
	float speed;
	float velocityY;
	bool isGrounded;
	BoundingBox boundingBox;
};

// ==================== camera rotation ====================
void CameraRotation(Camera3D *camera, float sensitivity) {
	Vector2 mousePosition = GetMousePosition();
	Vector2 center = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

	float deltaX = -(mousePosition.x - center.x) * sensitivity;
	float deltaY = (mousePosition.y - center.y) * sensitivity;

	yaw += deltaX;
	pitch -= deltaY;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	Vector3 direction = {
		cos(DEG2RAD * pitch) * sin(DEG2RAD * yaw),
		sin(DEG2RAD * pitch),
		cos(DEG2RAD * pitch) * cos(DEG2RAD * yaw)
	};

	camera->target = Vector3Add(camera->position, direction);
	SetMousePosition(center.x, center.y);
	HideCursor();
}

// ==================== player movement ====================
void SetKeyboard(Camera3D *camera, float moveSpeed, Player &player) {
	Vector3 forward = Vector3Normalize(Vector3Subtract(camera->target, camera->position));
	forward.y = 0;
	Vector3 right = Vector3Normalize(Vector3CrossProduct(camera->up, forward));

	if (IsKeyDown(KEY_W)) player.position = Vector3Add(player.position, Vector3Scale(forward, moveSpeed));
	if (IsKeyDown(KEY_S)) player.position = Vector3Subtract(player.position, Vector3Scale(forward, moveSpeed));
	if (IsKeyDown(KEY_A)) player.position = Vector3Add(player.position, Vector3Scale(right, moveSpeed));
	if (IsKeyDown(KEY_D)) player.position = Vector3Subtract(player.position, Vector3Scale(right, moveSpeed));

	if (IsKeyPressed(KEY_SPACE) && player.isGrounded) {
		player.velocityY = JUMP_FORCE;  // apply jump force
		player.isGrounded = false;
	}
}

// ==================== collision ====================
void UpdatePlayerBoundingBox(Player &player) {
	player.boundingBox = {
		Vector3 {player.position.x - 0.5f, player.position.y, player.position.z - 0.5f},
		Vector3 {player.position.x + 0.5f, player.position.y + 2, player.position.z + 0.5f}
	};
}

// ==================== gravity ====================
void ApplyGravity(Player &player) {
	player.velocityY -= GRAVITY * GetFrameTime();
	player.position.y += player.velocityY * GetFrameTime();

	// if the player touches the floor
	if (player.position.y <= GROUND_Y) {
		player.position.y = GROUND_Y;
		player.velocityY = 0;
		player.isGrounded = true;
	}
}

// ===================== graphics ======================
void getGraphicsFirstPersonPlayer(Texture2D hand1, float time) {
	float offsetY = sin(time * 2.0f) * 10;
	DrawTextureEx(hand1, { 550, 330 + offsetY }, 4, 0.34f, WHITE);
}

// ==================== game logic ====================
void UpdateGame(Camera3D *camera, float sensitivity, float moveSpeed, Player &player, GameLevels currentLevel) {
	oldPosition = player.position;

	CameraRotation(camera, sensitivity); // player camera rotation
	SetKeyboard(camera, moveSpeed, player); // player movement
	ApplyGravity(player); // jump gravity
	UpdatePlayerBoundingBox(player); // player collision

	// Detect collision
	if (currentLevel == LEVEL01) {
		for (int i = 0; i < 4; i++) {
			if (CheckCollisionBoxes(player.boundingBox, level01Collisions.walls[i])) {
				player.position = oldPosition;
				UpdatePlayerBoundingBox(player);
			}
		}
	}

	camera->position.x = player.position.x;
	camera->position.y = player.position.y + 2;
	camera->position.z = player.position.z + 6;
}