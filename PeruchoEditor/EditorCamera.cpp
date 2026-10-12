#include "EditorCamera.h"
#include "PEditor.h"
#include "PCamera.h"

EditorCamera::EditorCamera(std::string name, Vector3 position, Vector4 rotation, Vector3 scale, std::string tag
	) : Entity(name, position, rotation, scale, tag) 
{
	IsEditorOnly = true;

	AddComponent<PCamera>("CameraComponent", CAMERA_FREE, true);
}

void EditorCamera::Update(float deltaTime) {
	if (PEditor::GetInstance().MovingViewPort && IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		Entity::Update(deltaTime);
	}
}