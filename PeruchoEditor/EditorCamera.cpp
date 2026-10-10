#include "EditorCamera.h"
#include "PEditor.h"

void EditorCamera::Update(float deltaTime) {
	if (PEditor::GetInstance().MovingViewPort && IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		Entity::Update(deltaTime);
	}
}