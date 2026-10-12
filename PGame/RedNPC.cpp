#include "RedNPC.h"
#include "PMesh.h"

RedNPC::RedNPC(std::string name,
	Vector3 position,
	Vector4 rotation,
	Vector3 scale,
	std::string tag) : Entity(name, position, rotation, scale, tag) {

	AddComponent<PMesh>("Mesh", MeshShape::CUBE);
}

void RedNPC::Start() {
	Entity::Start();
	
}

void RedNPC::Update(float deltaTime) {
	Entity::Update(deltaTime);

	GetEntityTransform().translation.y += 2 * deltaTime;
}