#include "core.h"
#include "levels.h"
#include "raymath.h"
#include "player.h"
#include <iostream>

std::vector<Collectable> worldObjects;
Vector3 oldPosition;
float yaw = 0.0f, pitch = 0.0f;

void CameraRotation(Camera3D* camera, float sensitivity) {
	Vector2 mousePosition = GetMousePosition();
	Vector2 center = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };

	float deltaX = -(mousePosition.x - center.x) * sensitivity;
	float deltaY = (mousePosition.y - center.y) * sensitivity;

	yaw += deltaX;
	pitch -= deltaY;
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	Vector3 direction = {
		cosf(DEG2RAD * pitch) * sinf(DEG2RAD * yaw),
		sinf(DEG2RAD * pitch),
		cosf(DEG2RAD * pitch) * cosf(DEG2RAD * yaw)
	};

	camera->target = Vector3Add(camera->position, direction);
	SetMousePosition((int)center.x, (int)center.y);
	HideCursor();
}

void SetKeyboard(Camera3D* camera, float moveSpeed, Player& player) {
	Vector3 forward = Vector3Subtract(camera->target, camera->position);
	forward.y = 0;
	forward = Vector3Normalize(forward);
	Vector3 right = Vector3CrossProduct(camera->up, forward);
	right = Vector3Normalize(right);

	float deltaTime = GetFrameTime();

	if (IsKeyDown(KEY_W)) player.position = Vector3Add(player.position, Vector3Scale(forward, moveSpeed * deltaTime));
	if (IsKeyDown(KEY_S)) player.position = Vector3Subtract(player.position, Vector3Scale(forward, moveSpeed * deltaTime));
	if (IsKeyDown(KEY_A)) player.position = Vector3Add(player.position, Vector3Scale(right, moveSpeed * deltaTime));
	if (IsKeyDown(KEY_D)) player.position = Vector3Subtract(player.position, Vector3Scale(right, moveSpeed * deltaTime));

	if (IsKeyPressed(KEY_SPACE) && player.isGrounded) {
		player.velocityY = JUMP_FORCE;
		player.isGrounded = false;
	}
}

void UpdatePlayerBoundingBox(Player& player) {
	player.boundingBox.min = { player.position.x - 0.5f, player.position.y, player.position.z - 0.5f };
	player.boundingBox.max = { player.position.x + 0.5f, player.position.y + 2.0f, player.position.z + 0.5f };
}

void CheckObjectCollection(Player& player) {
	for (auto& obj : worldObjects) {
		if (!obj.active) continue;

		if (Vector3Distance(player.position, obj.position) < 2.0f) {
			obj.active = false;
			
			player.playerInventory.push_back(obj);
			std::cout << "¡Objeto recolectado!\n";
			
		}
	}
}

void ApplyGravity(Player& player) {
	if (!player.isGrounded) {
		player.velocityY -= GRAVITY * GetFrameTime();
	}
	player.position.y += player.velocityY * GetFrameTime();

	if (player.position.y <= GROUND_Y) {
		player.position.y = GROUND_Y;
		player.velocityY = 0.0f;
		player.isGrounded = true;
	}
}

void BoundingGravityObject(Model model) {
	float time = GetTime();
	float verticalOffset = sinf(time * 2.0f) * 0.25f;
	float rotationY = time * 45.0f;
	Vector3 pos = { 2.0f, 1.0f + verticalOffset, 0.0f };
	DrawModelEx(model, pos, { 0,1,0 }, rotationY, { 1,1,1 }, WHITE);
}

void getGraphicsFirstPersonPlayer(Texture2D hand1, float time) {
	float offsetY = sinf(time * 2.0f) * 10;
	DrawTextureEx(hand1, { 550, 330 + offsetY }, 0, 0.34f, WHITE);
}

void loadModels01() {
	static bool loaded = false;
	if (loaded) return;

	Collectable item;
	item.model = LoadModel("models/test.glb");
	item.position = { 0, 1, 0 };
	item.box = GetMeshBoundingBox(item.model.meshes[0]);
	item.box.min = Vector3Add(item.box.min, item.position);
	item.box.max = Vector3Add(item.box.max, item.position);
	item.active = true;
	worldObjects.push_back(item);

	loaded = true;
}

void UpdateGame(Camera3D* camera, float sensitivity, float moveSpeed, Player& player, GameLevels currentLevel) {
	oldPosition = player.position;

	CameraRotation(camera, sensitivity);
	SetKeyboard(camera, moveSpeed, player);
	ApplyGravity(player);
	UpdatePlayerBoundingBox(player);
	CheckObjectCollection(player);

	// Colisiones con paredes
	if (currentLevel == LEVEL01) {
		for (int i = 0; i < 4; i++) {
			if (CheckCollisionBoxes(player.boundingBox, level01Collisions.walls[i])) {
				player.position = oldPosition;
				UpdatePlayerBoundingBox(player);
				break;
			}
		}
	}

	// Cámara en tercera persona (ajustable)
	camera->position = Vector3Add(player.position, { 0, 2, 6 });
}