#include "Entity.h"
#include "PRegistry.h"

Entity::Entity(
	std::string name,
	Vector3 position,
	Vector4 rotation,
	Vector3 scale,
	std::string tag
)
	: Name(name),
	Tag(tag)
{
	EntityTransform.translation = position;
	EntityTransform.rotation = rotation;
	EntityTransform.scale = scale;
}

Component* Entity::GetComponentByName(string name) {
	for (int i = 0; i < Components.size(); i++) {
		if (Components[i]->Name == name) {
			return Components[i].get();
		}
	}

	return nullptr;
}

vector<Component*> Entity::GetAllComponentsByName(string name) {
	vector<Component*> lComponents;

	for (int i = 0; i < Components.size(); i++) {
		if (Components[i]->Name == name) {
			lComponents.push_back(Components[i].get());
		}
	}

	return lComponents;
}

void Entity::Update(float deltaTime) {
	for (int i = 0; i < Components.size(); i++) {
		Components[i]->Update(deltaTime);
	}
}

void Entity::Destroy() {
	DestroyEntity = true;
}

bool Entity::ShouldDestroy() {
	return DestroyEntity;
}

void Entity::Render(bool render3D) {
	for (int i = 0; i < Components.size(); i++) {
		if (Components[i]->Render3D == render3D) {
			Components[i]->Render();
		}
	}
}

Transform& Entity::GetEntityTransform() {
	return EntityTransform;
}

vector<unique_ptr<Component>>& Entity::GetComponents() {
	return Components;
}

void Entity::Serialize(nlohmann::json& j) const {
	j["name"] = Name;
	j["enabled"] = Enabled;
	j["tag"] = Tag;
	j["position"] = {
		EntityTransform.translation.x,
		EntityTransform.translation.y,
		EntityTransform.translation.z
	};
	j["scale"] = {
		EntityTransform.scale.x,
		EntityTransform.scale.y,
		EntityTransform.scale.z
	};
	j["rotation"] = {
		EntityTransform.rotation.x,
		EntityTransform.rotation.y,
		EntityTransform.rotation.z,
		EntityTransform.rotation.w
	};

	j["components"] = nlohmann::json::array();

	for (const auto& component : Components)
	{
		nlohmann::json componentJson;

		componentJson["type"] = component->GetType();

		component->Serialize(componentJson);

		j["components"].push_back(componentJson);
	}
}

void Entity::Deserialize(const nlohmann::json& j) {
	Name = j.value("name", "");
	Enabled = j.value("enabled", true);
	Tag = j.value("tag", "");

	const auto& position = j.at("position");
	EntityTransform.translation.x = position[0].get<float>();
	EntityTransform.translation.y = position[1].get<float>();
	EntityTransform.translation.z = position[2].get<float>();

	const auto& scale = j.at("scale");
	EntityTransform.scale.x = scale[0].get<float>();
	EntityTransform.scale.y = scale[1].get<float>();
	EntityTransform.scale.z = scale[2].get<float>();

	const auto& rotation = j.at("rotation");
	EntityTransform.rotation.x = rotation[0].get<float>();
	EntityTransform.rotation.y = rotation[1].get<float>();
	EntityTransform.rotation.z = rotation[2].get<float>();
	EntityTransform.rotation.w = rotation[3].get<float>();

	for (const auto& componentJson : j["components"])
	{
		std::string type = componentJson.at("type");
		std::string name = componentJson.at("name");

		if (Component* component = PRegistry::GetInstance().CreateCompoent(type, name, *this)) {
			component->Deserialize(componentJson);
			Components.emplace_back(component);
		}
	}
}