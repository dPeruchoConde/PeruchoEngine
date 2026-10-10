#include "PRegistry.h"
#include "PCamera.h"
#include "PMesh.h"

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

unique_ptr<Entity> PRegistry::CreateEntity(string name) {
	auto e = EntitiesContructors.find(name);
	if (e == EntitiesContructors.end()) {
		return nullptr;
	}

	return e->second(name);
}