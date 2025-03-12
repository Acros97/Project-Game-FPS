#include "raylib.h"
#include "raymath.h"
#include <iostream>

#define GRAVITY 15.0f
#define JUMP_FORCE 5.5f
#define GROUND_Y 1.0f

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;

BoundingBox wall1, wall2;
Vector3 oldPosition;
float yaw = 0.0f, pitch = 0.0f;
bool isJumping = false;

struct Player {
	Vector3 position;
	float speed;
	float velocityY;
	bool isGrounded;
	BoundingBox boundingBox;
};

// ==================== design wallS (test) ====================
void InitWalls() {
	wall1 = { Vector3{-4.5f, 0, -5}, Vector3{-3.5f, 5, 5} };
	wall2 = { Vector3{3.5f, 0, -5}, Vector3{4.5f, 5, 5} };
}

void DrawWallsTest() {
	DrawCube({ -4, 2.5f, 0 }, 1, 5, 10, YELLOW);
	DrawCube({ 4, 2.5f, 0 }, 1, 5, 10, YELLOW);
}

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

// ==================== game logic ====================
void UpdateGame(Camera3D *camera, float sensitivity, float moveSpeed, Player &player) {
	oldPosition = player.position;
	CameraRotation(camera, sensitivity);
	SetKeyboard(camera, moveSpeed, player);
	ApplyGravity(player);
	UpdatePlayerBoundingBox(player);

	// Detect collision
	if (CheckCollisionBoxes(player.boundingBox, wall1) || CheckCollisionBoxes(player.boundingBox, wall2)) {
		player.position = oldPosition;
		UpdatePlayerBoundingBox(player);
	}

	camera->position.x = player.position.x;
	camera->position.y = player.position.y + 2;
	camera->position.z = player.position.z + 6;
}

// ==================== Renderización ====================
void RenderGame(Camera3D camera) {
	BeginMode3D(camera);
	DrawGrid(10, 2.0f);
	DrawWallsTest();
	EndMode3D();
}

// ==================== main function ====================
int main() {
	const int screenWidth = 800;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "Summer Splash! 3D");

	Camera3D camera = { 0 };
	camera.position = { 0.0f, 2.0f, 4.0f };
	camera.target = { 0.0f, 2.0f, 0.0f };
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.fovy = 60.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	float sensitivity = 0.3f;
	float moveSpeed = 0.18f;

	GameScreen currentScreen = TITLE;
	SetTargetFPS(60);

	InitWalls();

	// player collision initialize
	Player player = { {0, 1.0f, 0}, 3.0f, 0.0f, true };
	UpdatePlayerBoundingBox(player);

	while (!WindowShouldClose()) {
		// ** Debugging: **
		char debugText[100];
		sprintf_s(debugText, "Y: %.2f | VelY: %.2f | Suelo: %s",
			player.position.y, player.velocityY, player.isGrounded ? "SI" : "NO");

		BeginDrawing();
		ClearBackground(BLACK);

		if (currentScreen == TITLE) {
			DrawText("Summer Splash! Test Alpha 0.0.1", 20, 20, 40, WHITE);
			DrawText("Presiona ENTER para iniciar", 120, 220, 20, WHITE);
			if (IsKeyPressed(KEY_ENTER)) currentScreen = GAMEPLAY;
		}
		else if (currentScreen == GAMEPLAY) {
			DrawText(debugText, 10, 10, 20, RED);
			UpdateGame(&camera, sensitivity, moveSpeed, player);
			RenderGame(camera);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}