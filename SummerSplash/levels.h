#include "raylib.h"
#include <vector>
struct  Level {
	BoundingBox walls[4];
};
extern Level level01Collisions;

void DrawLevel01(std::vector<Model> models);
