#include "raylib.h"
#include "raymath.h"
#include "levels.h"
#include "core.h"
#include "menu.h"
#include "window.h"
#include <iostream>

typedef enum GameScreen { LOGO = 0, MENU = 1, GAMEPLAY = 2, ENDING = 3 } GameScreen;
typedef enum MenuOption { BEGIN_GAME = 0, OPTIONS = 1, EXIT = 2 } GameOptions;
int currentOption = BEGIN_GAME;
const int optionCount = 3;

Vector2 optionPositions[optionCount] = {
	{90,270},
	{90,320},
	{90,370}
};

void DrawCollisions() {
	for (int i = 0; i < 4; i++) {
		DrawBoundingBox(level01Collisions.walls[i], BLUE);
	}
}

void loadLevels(GameLevels currentLevel) {
	switch (currentLevel) {
	case LEVEL01:
		DrawLevel01();
		break;
	default:
		break;
	}
}

void UpdateMenu(Texture2D button) {
	if (IsKeyPressed(KEY_DOWN)) {
		currentOption++;
		if (currentOption >= optionCount) currentOption = 0;
	}
	else if (IsKeyPressed(KEY_UP)) {
		currentOption--;
		if (currentOption < 0) currentOption = optionCount - 1;
	}
	DrawTextureEx(button, optionPositions[currentOption], 0, 0.04f, SKYBLUE);
}

void RenderGame(Camera3D camera, GameLevels currentLevel, Texture2D hand1, float time) {
	BeginMode3D(camera);
	DrawGrid(10, 5.0f);
	loadLevels(currentLevel);
	DrawCollisions();
	EndMode3D();
	getGraphicsFirstPersonPlayer(hand1, time);
}

int main() {
	const int screenWidth = 800;
	const int screenHeight = 600;
	float sensitivity = 0.3f, moveSpeed = 0.20f, time = 0.0f;

	displaySmallWindow();
	int maximized = 0;
	GameScreen currentScreen = MENU;

	Camera3D camera = { 0 };
	camera.position = { 0.0f, 0.0f, 0.0f };
	camera.target = { 0.0f, 2.0f, 0.0f };
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.fovy = 60.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	Font customFont = LoadFont("assets/fonts/Tiny5-Regular.ttf");
	Texture2D button = LoadTexture("textures/select.png");
	Texture2D logoGame = LoadTexture("textures/logo.png");
	Texture2D hand1 = LoadTexture("textures/hand1.png");

	SetTargetFPS(60);

	Player player = { {0, 1.0f, 0}, 3.0f, 0.0f, true };
	UpdatePlayerBoundingBox(player);

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		if (IsKeyDown(KEY_F10)) {
			if (maximized == 1) {
				displaySmallWindow();
				maximized = 0;
			}
			else {
				displayFullScreenBox();
				maximized = 1;
			}
		}

		if (currentScreen == MENU) {
			DrawMenu(customFont, logoGame);
			LoadVersion(customFont);
			UpdateMenu(button);

			if (currentOption == BEGIN_GAME && IsKeyPressed(KEY_ENTER)) {
				currentScreen = GAMEPLAY;
			}
			if (currentOption == EXIT && IsKeyPressed(KEY_ENTER)) return 0;
		}
		else if (currentScreen == GAMEPLAY) {
			GameLevels currentLevel = LEVEL01;
			loadModels01(); // cargamos solo una vez
			UpdateGame(&camera, sensitivity, moveSpeed, player, currentLevel);
			time += GetFrameTime();
			RenderGame(camera, currentLevel, hand1, time);
		}

		EndDrawing();
	}

	UnloadFont(customFont);
	UnloadTexture(button);
	UnloadTexture(hand1);
	UnloadTexture(logoGame);
	CloseWindow();
	return 0;
}