#include "PProjectManager.h"
#include "PLoader.h"
#include "PSceneManager.h"
#include "PEngine.h"
#include "PRegistry.h"
#include "PGameBuilder.h"

using namespace std;

PProjectManager& PProjectManager::GetInstance() {
	static PProjectManager Instance;
	return Instance;
}

bool PProjectManager::RebuildGame() {
	//Guardo la escena y descargo las entidades y la dll
	PSceneManager::GetInstance().SaveScene("temp");

	PEngine::GetInstance().ClearEntities();
	PRegistry::GetInstance().ClearRegistry();

	PLoader::GetInstance().UnloadModule();

	//Compilo la dll
	if (!PGameBuilder::BuildGame()) {
		return false;
	}

	//Recargo la dll y la escena
	if (!PLoader::GetInstance().LoadModule("PGame.dll")) {
		return false;
	}

	PSceneManager::GetInstance().LoadScene("temp");

	return true;
}