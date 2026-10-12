#include "PEditor.h"
#include "PEngine.h"
#include "PCamera.h"
#include "EditorCamera.h"
#include "PLoader.h"
#include "PRegistry.h"
#include "PSceneManager.h"

int main() 
{
    PLoader moduleLoader;
    if (!moduleLoader.LoadModule("PGame.dll")) {
        return -1;
    }

    PEngine& engine = PEngine::GetInstance();
    engine.Start();
    engine.Grid = true;
    PEngine::GetInstance().PauseGame(true);

    PEditor& editor = PEditor::GetInstance();
    editor.Start();

    EditorCamera* camera = engine.AddEntity<EditorCamera>("EditorCamera", Vector3{ 5.0f, 5.0f, 5.0f });

    while (engine.WindowState())
    {
        editor.Update();

        editor.Render();
    }

    editor.CloseEditor();

    return 0;
}