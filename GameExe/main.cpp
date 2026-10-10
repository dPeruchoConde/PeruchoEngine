#include "PEngine.h"
#include "GameCamera.h"
#include "PCamera.h"

int main() 
{
	PEngine& engine = PEngine::GetInstance();
	engine.Start();

	GameCamera* camera = engine.AddEntity<GameCamera>("GameCamera", Vector3{ 5.0f, 5.0f, 5.0f });
	camera->AddComponent<PCamera>("CameraComponent", CAMERA_ORBITAL, true);

	while (engine.WindowState()) {
		engine.Update();

		engine.Render();
	}

	return 0;
}