#pragma once
#include "ExportHeaders.h"
#include <string>
#include <json.hpp>

class Entity;

using namespace std;
//using namespace nlohmann::json;

class PENGINE_API Component
{
public:
	Component(string name, Entity& e);
	virtual ~Component() {}

	virtual void Start() {};

	virtual void Update(float deltaTime) {};

	virtual void Render() {};

	virtual void Destroy() {};

	virtual void Draw() {};

	virtual void Serialize(nlohmann::json& j) const;

	virtual void Deserialize(const nlohmann::json& j);

	virtual string GetType() const { return ""; }

	Entity& RootEntity;

	string Name = "";

	bool Enabled = true;

	bool Render3D = true;
};
