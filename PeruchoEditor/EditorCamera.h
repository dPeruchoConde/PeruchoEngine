#pragma once

#include "Entity.h"

class EditorCamera : public Entity
{
public:
	EditorCamera(
		std::string name,
		Vector3 position = { 0, 0, 0 },
		Vector4 rotation = { 0, 0, 0, 0 },
		Vector3 scale = { 1, 1, 1 },
		std::string tag = ""
	) : Entity(name, position, rotation, scale, tag) {}

	void Update(float deltaTime) override;
};

