#include "RedNPC.h"
#include "PMesh.h"

void RedNPC::Start() {
	Entity::Start();

	AddComponent<PMesh>("Mesh", MeshShape::CUBE);
}

void RedNPC::Update(float deltaTime) {
	Entity::Update(deltaTime);

	GetEntityTransform().translation.y += 2 * deltaTime;
}