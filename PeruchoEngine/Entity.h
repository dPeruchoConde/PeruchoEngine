#pragma once
#include "ExportHeaders.h"
#include <vector>
#include <string>
#include <memory>
#include <concepts>
#include "Component.h"
#include <raylib.h>

using namespace std;

class PENGINE_API Entity
{
public:
	Entity(
		std::string name,
		Vector3 position = { 0, 0, 0 },
		Vector4 rotation = { 0, 0, 0, 0 },
		Vector3 scale = { 1, 1, 1 },
		std::string tag = ""
	);
	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;
	Entity(Entity&&) = delete;
	Entity& operator=(Entity&&) = delete;
	virtual ~Entity() = default;

	string Name = "";

	string Tag = "";
	
	bool Enabled = true;

	virtual void Start() {};

	virtual void Update(float deltaTime);

	virtual void OnDestroy() {};

	virtual void OnCollision(Entity& e) {};

	virtual void Render(bool render3D);

	virtual void Serialize(nlohmann::json& j) const;

	virtual void Deserialize(const nlohmann::json& j);

	Component* GetComponentByName(string name);

	template<typename T>
		requires derived_from<T, Component>
	T* GetComponentByType() {
		for (int i = 0; i < Compoments.size(); i++) {
			if (T* c = dynamic_cast<T*>(Compoments[i].get())) {
				return c;
			}
		}

		return nullptr;
	}

	vector<Component*> GetAllComponentsByName(string name);

	Transform& GetEntityTransform();

	template<typename T>
		requires derived_from<T, Component>
	vector<T*> GetAllComponentsByType() {
		vector<T*> lComponents;

		for (int i = 0; i < Compoments.size(); i++) {
			if (T* c = dynamic_cast<T*>(Compoments[i].get())) {
				lComponents.push_back(c);
			}
		}

		return lComponents;
	}

	template<typename T, typename... Args>
		requires derived_from<T, Component>
	T* AddComponent(std::string name, Args&&... args)
	{
		string type = "";
		auto component = std::make_unique<T>(
			type,
			name,
			*this,
			std::forward<Args>(args)...
		);

		T* ComponentPtr = component.get();

		Compoments.push_back(std::move(component));

		ComponentPtr->Start();

		return *ComponentPtr;
	}

	void Destroy();

	bool ShouldDestroy();

	vector<unique_ptr<Component>>& GetComponents();

private:
	Transform EntityTransform;

	vector<unique_ptr<Component>> Components;

	bool DestroyEntity = false;
};

