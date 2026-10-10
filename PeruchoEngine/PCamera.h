#pragma once
#include "ExportHeaders.h"
#include "Component.h"
#include <raylib.h>

class PENGINE_API PCamera : public Component
{
public:
	PCamera(string name, Entity& e, CameraMode mode = CAMERA_THIRD_PERSON, bool currentCamera = false);

	void SetMainCamera();

	void Update(float deltaTime) override;

	void Start() override;

	string GetType() const override;

	Camera3D Camera;

	CameraMode Mode;

private:
	bool CurrentCamera = false;
};

