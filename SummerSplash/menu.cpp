#include "raylib.h"
#include <iostream>

void DrawMenu(Font customFont, Texture2D button, Texture2D logo) {
	DrawTextEx(customFont, "Seleccionar escenario", { 150,270 }, 40, 2, SKYBLUE);
	DrawTextEx(customFont, "Opciones", { 150, 320 }, 40, 2, SKYBLUE);
	DrawTextEx(customFont, "Volver al escritorio", { 150, 370 }, 40, 2, SKYBLUE);
	// upload images
	DrawTextureEx(button, { 90,250 }, 0, 0.04f, SKYBLUE);
	DrawTextureEx(logo, { 250,30 }, 0, 0.22f, SKYBLUE);
	//const char* workingDirectory = GetWorkingDirectory();
}

void LoadVersion(Font customFont) {
	const char* _VERSION = "V0.0.1";
	DrawTextEx(customFont, _VERSION, { 700, 550 }, 20, 2, SKYBLUE);
}