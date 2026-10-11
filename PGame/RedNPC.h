#pragma once

#include "Entity.h"
#include "PRegistry.h"
#include "PGameExport.h"

class PGAME_API RedNPC : public Entity
{
public:
	RedNPC(
		std::string name,
		Vector3 position = { 0, 0, 0 },
		Vector4 rotation = { 0, 0, 0, 0 },
		Vector3 scale = { 1, 1, 1 },
		std::string tag = ""
	) : Entity(name, position, rotation, scale, tag) {
	}

	virtual void Start();
};

P_ENTITY(RedNPC, "RedNPC")