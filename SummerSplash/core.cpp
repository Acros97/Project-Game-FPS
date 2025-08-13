#include "core.h"
#include "levels.h"

std::vector<Collectable> worldObjects;

Vector3 oldPosition;
float yaw = 0.0f, pitch = 0.0f;

void CameraRotation(Camera3D* camera, float sensitivity) {
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

void SetKeyboard(Camera3D* camera, float moveSpeed, Player& player) {
	Vector3 forward = Vector3Normalize(Vector3Subtract(camera->target, camera->position));
	forward.y = 0;
	Vector3 right = Vector3Normalize(Vector3CrossProduct(camera->up, forward));

	if (IsKeyDown(KEY_W)) player.position = Vector3Add(player.position, Vector3Scale(forward, moveSpeed));
	if (IsKeyDown(KEY_S)) player.position = Vector3Subtract(player.position, Vector3Scale(forward, moveSpeed));
	if (IsKeyDown(KEY_A)) player.position = Vector3Add(player.position, Vector3Scale(right, moveSpeed));
	if (IsKeyDown(KEY_D)) player.position = Vector3Subtract(player.position, Vector3Scale(right, moveSpeed));

	if (IsKeyPressed(KEY_SPACE) && player.isGrounded) {
		player.velocityY = JUMP_FORCE;
		player.isGrounded = false;
	}
}

void UpdatePlayerBoundingBox(Player& player) {
	player.boundingBox = {
		Vector3 {player.position.x - 0.5f, player.position.y, player.position.z - 0.5f},
		Vector3 {player.position.x + 0.5f, player.position.y + 2, player.position.z + 0.5f}
	};
}

void CheckObjectCollection(Player& player) {
	for (auto& obj : worldObjects) {
		if (!obj.active) continue;
		float dist = Vector3Distance(player.position, obj.position);
		if (dist < 2.0f) {
			obj.active = false;
			player.playerInventory.push_back(obj);
			std::cout << "Objeto recolectado\n";
		}
	}
}

void ApplyGravity(Player& player) {
	player.velocityY -= GRAVITY * GetFrameTime();
	player.position.y += player.velocityY * GetFrameTime();

	if (player.position.y <= GROUND_Y) {
		player.position.y = GROUND_Y;
		player.velocityY = 0;
		player.isGrounded = true;
	}
}

void BoundingGravityObject(Model model) {
	float time = GetTime();
	float verticalOffset = sinf(time * 2.0f) * 0.25f;
	float rotationY = time * 45.0f;
	Vector3 itemBasePosition = { 2.0f,1.0f + verticalOffset,0.0f };
	DrawModelEx(model, itemBasePosition, { 0,1,0 }, rotationY, { 1.0f,1.0f,1.0f }, WHITE);
}

void getGraphicsFirstPersonPlayer(Texture2D hand1, float time) {
	float offsetY = sin(time * 2.0f) * 10;
	DrawTextureEx(hand1, { 550, 330 + offsetY }, 4, 0.34f, WHITE);
}

void loadModels01() {
	Collectable item;
	item.model = LoadModel("models/test.glb");
	item.position = { 0,1,0 };
	item.box = GetMeshBoundingBox(item.model.meshes[0]);
	item.box.min = Vector3Add(item.box.min, item.position);
	item.box.max = Vector3Add(item.box.max, item.position);
	item.active = true;
	worldObjects.push_back(item);
}

void UpdateGame(Camera3D* camera, float sensitivity, float moveSpeed, Player& player, GameLevels currentLevel) {
	oldPosition = player.position;

	CameraRotation(camera, sensitivity);
	SetKeyboard(camera, moveSpeed, player);
	ApplyGravity(player);
	UpdatePlayerBoundingBox(player);
	CheckObjectCollection(player);

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