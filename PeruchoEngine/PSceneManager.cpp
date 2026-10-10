#include "PSceneManager.h"
#include "PPath.h"
#include "PEngine.h"
#include <fstream>

PSceneManager& PSceneManager::GetInstance() {
	static PSceneManager Instance;
	return Instance;
}

void PSceneManager::LoadScene(std::string sceneName) {
	if (CurrentScene != "") {
		PEngine::GetInstance().ClearEntities();
	}

	fs::path assetsPath = PPath::GetInstance().GetAssetsPath();
	fs::path scenePath = assetsPath / "Scenes" / (sceneName + ".json");
	if (fs::exists(scenePath)) {
		std::ifstream file(scenePath);

		if (file.is_open() == false)
			return;

		nlohmann::json sceneJson;
		file >> sceneJson;

		PEngine::GetInstance().Deserialize(sceneJson);
	}
}

void PSceneManager::SaveScene() {
	fs::path assetsPath = PPath::GetInstance().GetAssetsPath();
	fs::path scenePath = assetsPath / "Scenes" / (CurrentScene + ".json");

	nlohmann::json sceneJson;

	PEngine::GetInstance().Serialize(sceneJson);

	std::ofstream file(scenePath);
	if (file.is_open() == false) {
		return;
	}

	file << sceneJson.dump(4);
}