#include "raylib.h"
#include <iostream>

void DrawMenu(Font customFont, Texture2D logo) {
	DrawTextEx(customFont, "Choose scenario", { 150,270 }, 40, 2, SKYBLUE);
	DrawTextEx(customFont, "Options", { 150, 320 }, 40, 2, SKYBLUE);
	DrawTextEx(customFont, "Go to desktop", { 150, 370 }, 40, 2, SKYBLUE);
	// upload images
	DrawTextureEx(logo, { 250,30 }, 0, 0.22f, SKYBLUE);
	//const char* workingDirectory = GetWorkingDirectory();
}

void LoadVersion(Font customFont) {
	const char* _VERSION = "V0.0.0 (alpha)";
	DrawTextEx(customFont, _VERSION, { 700, 550 }, 20, 2, SKYBLUE);
}