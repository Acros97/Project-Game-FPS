#include "raylib.h"

void displayFullScreenBox() {
	ToggleFullscreen();
}

void displaySmallWindow() {
	const int width = 800;
	const int height = 600;
	InitWindow(width, height, "Proyect name");
}