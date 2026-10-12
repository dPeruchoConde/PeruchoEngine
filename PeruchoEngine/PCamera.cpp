#include "PCamera.h"
#include "PEngine.h"

PCamera::PCamera(string name, Entity& e, CameraMode mode, bool currentCamera) : Component (name, e) {
	Mode = mode;
	Camera = { 0 };
	CurrentCamera = currentCamera;

	Transform& transform = RootEntity.GetEntityTransform();
	Camera.position = transform.translation;
	Camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
	Camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
	Camera.fovy = 45.0f;
	Camera.projection = CAMERA_PERSPECTIVE;
}

void PCamera::Start() {
	if (CurrentCamera) {
		PEngine::GetInstance().ChangeMainCamera(RootEntity);
	}
}

void PCamera::SetMainCamera() {
	PEngine::GetInstance().ChangeMainCamera(this->RootEntity);
}

void PCamera::Update(float deltaTime) {
	UpdateCamera(&Camera, Mode);
}

string PCamera::GetType() const {
	return "PCamera";
}