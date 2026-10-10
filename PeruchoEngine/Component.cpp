#include "Component.h"
#include "Entity.h"

Component::Component(string name, Entity& e) : RootEntity(e), Name(name) {

}

void Component::Serialize(nlohmann::json& j) const {
	j["name"] = Name;
	j["enabled"] = Enabled;
	j["render3D"] = Render3D;
}

void Component::Deserialize(const nlohmann::json& j) {
	Name = j.value("name", "");
	Enabled = j.value("enabled", true);
	Render3D = j.value("render3D", true);
}