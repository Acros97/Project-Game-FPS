#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include "levels.h"
#include "core.h"

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;

void DrawWallsTest() { // never used
	DrawCube({ -16, 2.5f, 0 }, 1, 5, 10, YELLOW);
	DrawCube({ 16, 2.5f, 0 }, 1, 5, 10, YELLOW);
}

/// only for debugging
void DrawCollisions() {
	for (int i = 0; i < 4; i++) {
		DrawBoundingBox(level01Collisions.walls[i], BLUE);
	}
}

// ================== design levels ======================
void loadLevels(GameLevels currentLevel) {
	if (currentLevel == LEVEL01) {
		DrawLevel01();
	}
}

// ==================== render ====================
void RenderGame(Camera3D camera, GameLevels currentLevel) {
	BeginMode3D(camera);
	DrawGrid(10, 5.0f);
	loadLevels(currentLevel);
	DrawCollisions(); // debugging
	EndMode3D();
}

// ==================== main function ====================
int main() {
	const int screenWidth = 800;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "Proyect game");

	// ==== Set camera  ====
	Camera3D camera = { 0 };
	camera.position = { 0.0f, 0.0f, 0.0f };
	camera.target = { 0.0f, 2.0f, 0.0f };
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.fovy = 60.0f;
	camera.projection = CAMERA_PERSPECTIVE;
	// =====================================
	float sensitivity = 0.3f;
	float moveSpeed = 0.20f;
	// ====================================

	GameScreen currentScreen = TITLE;
	SetTargetFPS(60);

	// player collision initialize
	Player player = { {0, 1.0f, 0}, 3.0f, 0.0f, true };
	UpdatePlayerBoundingBox(player);

	while (!WindowShouldClose()) {
		char debugText[100];
		sprintf_s(debugText, "Y: %.2f | VelY: %.2f | Floor: %s",
			player.position.y, player.velocityY, player.isGrounded ? "YES" : "NO");

		BeginDrawing();
		ClearBackground(BLACK);

		if (currentScreen == TITLE) {
			DrawText("Game FPS Test Alpha 0.0.1", 20, 20, 40, WHITE);
			DrawText("Press ENTER to play...", 120, 220, 20, WHITE);
			if (IsKeyPressed(KEY_ENTER)) currentScreen = GAMEPLAY;
		}
		else if (currentScreen == GAMEPLAY) { // Current game
			GameLevels currentLevel = LEVEL01;
			DrawText(debugText, 10, 10, 20, RED); // ?? Debugging

			UpdateGame(&camera, sensitivity, moveSpeed, player, currentLevel);
			RenderGame(camera, currentLevel);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}