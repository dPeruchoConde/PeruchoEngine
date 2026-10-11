#include "PRegistry.h"
#include "PCamera.h"
#include "PMesh.h"
#include <algorithm>

PRegistry& PRegistry::GetInstance() {
	static PRegistry Instance;
	return Instance;
}

PRegistry::PRegistry() {
	ComponentTypes["PCamera"] =
		[](string name, Entity& e) -> Component* {
		return e.AddComponent<PCamera>(name);
		};

	ComponentTypes["PMesh"] =
		[](string name, Entity& e) -> Component* {
		return e.AddComponent<PMesh>(name);
		};
}

Component* PRegistry::CreateCompoent(string type, string name, Entity& e) {
	return ComponentTypes.at(type)(name, e);
}

void PRegistry::RegisterEntity(string name, function<unique_ptr<Entity>(const string)> constructor) {
	EntitiesContructors[name] = constructor;
}

unique_ptr<Entity> PRegistry::CreateEntity(string type, string name) {
	auto e = EntitiesContructors.find(type);
	if (e == EntitiesContructors.end()) {
		return nullptr;
	}

	unique_ptr<Entity> entity = e->second(name);
	entity->SetType(type);

	return entity;
}

vector<string> PRegistry::GetEntitiesTypes() const {
	vector<string> entitiesTypes;
	entitiesTypes.reserve(EntitiesContructors.size());

	for (const auto& [type, constructor] : EntitiesContructors) {
		entitiesTypes.push_back(type);
	}
	std::sort(entitiesTypes.begin(), entitiesTypes.end());

	return entitiesTypes;
}

void PRegistry::ClearRegistry() {
	EntitiesContructors.clear();
	ComponentTypes.clear();
}