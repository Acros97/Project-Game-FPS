#include "raylib.h"
#include "raymath.h"
#include "levels.h"
#include "core.h"
#include "menu.h"
#include "window.h"
#include <iostream>

typedef enum GameScreen { LOGO = 0, MENU = 1, GAMEPLAY = 2, ENDING = 3 } GameScreen;
// display options
typedef enum MenuOption { BEGIN_GAME = 0, OPTIONS = 1, EXIT = 2 } GameOptions;
int currentOption = BEGIN_GAME;
const int optionCount = 3;

Vector2 optionPositions[optionCount] = {
	{90,270}, // BEGIN_GAME
	{ 90,320 }, // OPTIONS
	{ 90,370 } // EXIT
};

void DrawCollisions() { // only for debugging
	for (int i = 0; i < 4; i++) {
		DrawBoundingBox(level01Collisions.walls[i], BLUE);
	}
}

// ================== load levels ======================
void loadLevels(GameLevels currentLevel) {
	switch (currentLevel) {
	case LEVEL01:
		DrawLevel01();
	default:
		break;
	}
}
// ========================== menu update ======================================= //
void UpdateMenu(Texture2D button) {
	if (IsKeyPressed(KEY_DOWN)) {
		currentOption++;
		if (currentOption >= optionCount) currentOption = 0;
	}
	else if (IsKeyPressed(KEY_UP)) {
		currentOption--;
		if (currentOption < 0) currentOption = optionCount - 1;
	}

	// draw icon in current position
	DrawTextureEx(button, optionPositions[currentOption], 0, 0.04f, SKYBLUE);
}

// ============================= render game ========================================= //
void RenderGame(Camera3D camera, GameLevels currentLevel, Texture2D hand1, float time) {
	BeginMode3D(camera);
	DrawGrid(10, 5.0f);
	loadLevels(currentLevel);
	DrawCollisions(); // debugging
	EndMode3D();
	getGraphicsFirstPersonPlayer(hand1, time);
}

// ==================== main function ====================
int main() {
	const int screenWidth = 800;
	const int screenHeight = 600; // 800x600 default resolution
	float sensitivity = 0.3f, moveSpeed = 0.20f, time = 0.0f;

	displaySmallWindow(); //  INIT WINDOW - this will be modified.
	int maximized = 0;
	GameScreen currentScreen = MENU;

	// ==== Set camera  ====
	Camera3D camera = { 0 };
	camera.position = { 0.0f, 0.0f, 0.0f };
	camera.target = { 0.0f, 2.0f, 0.0f };
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.fovy = 60.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	// ============================== load resources ==================================== //
	Font customFont = LoadFont("assets/fonts/Tiny5-Regular.ttf"); // Load custom font
	Texture2D button = LoadTexture("textures/select.png"); // select button
	Texture2D logoGame = LoadTexture("textures/logo.png"); // logo
	Texture2D hand1 = LoadTexture("textures/hand1.png");
	// ============================================================================= //
	SetTargetFPS(60); // 60 fps

	// player collision initialize
	Player player = { {0, 1.0f, 0}, 3.0f, 0.0f, true };
	UpdatePlayerBoundingBox(player);

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		// ======== screen mode ========
		if (IsKeyDown(KEY_F10)) {
			if (maximized == 1) {
				displaySmallWindow();
				maximized = 0;
			}
			if (maximized == 0) {
				displayFullScreenBox();
				maximized = 1;
			}
		}
		// ======================

		if (currentScreen == MENU) { // menu screen
			DrawMenu(customFont, logoGame);
			LoadVersion(customFont);
			UpdateMenu(button);

			if (currentOption == BEGIN_GAME && IsKeyPressed(KEY_ENTER)) currentScreen = GAMEPLAY; // it will be changed, because the player choose scenario before.
			if (currentOption == EXIT && IsKeyPressed(KEY_ENTER)) return 0;
		}
		else if (currentScreen == GAMEPLAY) { // current game
			GameLevels currentLevel = LEVEL01;

			UpdateGame(&camera, sensitivity, moveSpeed, player, currentLevel);
			time += GetFrameTime();
			RenderGame(camera, currentLevel, hand1, time);
		}

		EndDrawing();
	}

	// ============================
	UnloadFont(customFont);
	UnloadTexture(button);
	UnloadTexture(hand1);
	UnloadTexture(logoGame);
	// ============================
	CloseWindow();
	return 0;
}