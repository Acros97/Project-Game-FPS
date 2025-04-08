#include "raylib.h"

void displayFullScreenBox() {
	int current_monitor = 0;
	const int MaxWidth = GetMonitorWidth(current_monitor);
	const int MaxHeight = GetMonitorHeight(current_monitor);
	InitWindow(MaxWidth, MaxHeight, "Proyect name");
	ToggleFullscreen();
}

void displaySmallWindow() {
	const int width = 800;
	const int height = 600;
	InitWindow(width, height, "Proyect name");
}