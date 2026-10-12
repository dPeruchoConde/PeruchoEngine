#include "PSceneManager.h"
#include "PPath.h"
#include "PEngine.h"
#include <fstream>

PSceneManager& PSceneManager::GetInstance() {
	static PSceneManager Instance;
	return Instance;
}

void PSceneManager::LoadScene(string sceneName) {
	PEngine::GetInstance().ClearEntities();

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

void PSceneManager::SaveScene(string sceneName) {
	fs::path assetsPath = PPath::GetInstance().GetAssetsPath();
	fs::path scenePath = assetsPath / "Scenes" / (sceneName + ".json");

	nlohmann::json sceneJson;

	PEngine::GetInstance().Serialize(sceneJson);

	std::ofstream file(scenePath);
	if (file.is_open() == false) {
		return;
	}

	file << sceneJson.dump(4);
}

void PSceneManager::DeleteScene(string sceneName) {
	PEngine::GetInstance().ClearEntities();

	fs::path assetsPath = PPath::GetInstance().GetAssetsPath();
	fs::path scenePath = assetsPath / "Scenes" / (sceneName + ".json");
	if (fs::exists(scenePath)) {
		fs::remove(scenePath);
	}
}

vector<string> PSceneManager::GetAllSceneNames() {
	vector<string> sceneNames;

	fs::path assetsPath = PPath::GetInstance().GetAssetsPath();
	fs::path scenePath = assetsPath / "Scenes";

	if (fs::exists(scenePath)) {
		for (const auto& entry : fs::directory_iterator(scenePath))
		{
			if (!entry.is_regular_file()) {
				continue;
			}

			if (entry.path().extension() != ".json") {
				continue;
			}

			string sceneName = entry.path().stem().string();

			if (sceneName == "temp") {
				continue;
			}

			sceneNames.push_back(sceneName);
		}
	}

	return sceneNames;
}