#include "PEditor.h"
#include "PEngine.h"
#include "PCamera.h"
#include "EditorCamera.h"

int main() 
{
    PEngine& engine = PEngine::GetInstance();
    engine.Start();
    engine.Grid = true;

    PEditor& editor = PEditor::GetInstance();
    editor.Start();

    EditorCamera* camera = engine.AddEntity<EditorCamera>("EditorCamera", Vector3{ 5.0f, 5.0f, 5.0f });
    camera->AddComponent<PCamera>("CameraComponent", CAMERA_FREE, true);

    while (engine.WindowState())
    {
        editor.Update();

        editor.Render();
    }

    return 0;
}