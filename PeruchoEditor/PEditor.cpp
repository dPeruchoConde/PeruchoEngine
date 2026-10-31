#include "PEditor.h"
#include "PEngine.h"
#include "rlImGui.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "PRegistry.h"
#include "PSceneManager.h"
#include "EditorCamera.h"
#include "PCamera.h"

PEditor& PEditor::GetInstance()
{
    static PEditor Instance;
    return Instance;
}

void PEditor::Start() {
    rlImGuiSetup(true);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ViewportTexture = LoadRenderTexture(1280, 720);
}

void PEditor::Update() {
    PEngine::GetInstance().Update();
}

void PEditor::SetUpDocking() {
    ImGuiID dockspace_id = ImGui::GetID("PDockSpace");

    ImGui::DockBuilderRemoveNode(dockspace_id);

    ImGui::DockBuilderAddNode(
        dockspace_id,
        ImGuiDockNodeFlags_DockSpace
    );

    ImGui::DockBuilderSetNodeSize(
        dockspace_id,
        ImGui::GetMainViewport()->WorkSize
    );

    ImGuiID left;
    ImGuiID right;
    ImGuiID center;

    ImGui::DockBuilderSplitNode(
        dockspace_id,
        ImGuiDir_Left,
        0.20f,
        &left,
        &center
    );

    ImGui::DockBuilderSplitNode(
        center,
        ImGuiDir_Right,
        0.25f,
        &right,
        &center
    );

    ImGui::DockBuilderDockWindow("Entities", left);
    ImGui::DockBuilderDockWindow("Components", right);
    ImGui::DockBuilderDockWindow("Viewport", center);

    ImGui::DockBuilderFinish(dockspace_id);
}

void PEditor::Render() {
    rlImGuiBegin();

    RenderViewPort();

    DrawEditorDocks();

    DrawEntitiesPanel();
    DrawInspector();

    DrawViewport();

    rlImGuiEnd();
    EndDrawing();
}

void PEditor::RenderViewPort() {
    BeginDrawing();
    ClearBackground(DARKGRAY);
    BeginTextureMode(ViewportTexture);

    PEngine::GetInstance().Render();

    EndTextureMode();
}

void PEditor::DrawEditorDocks() {
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoDocking;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("Editor", nullptr, flags);

    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("PDockSpace");

    ImGui::DockSpace(
        dockspace_id,
        ImVec2(0, 0),
        ImGuiDockNodeFlags_None
        //ImGuiDockNodeFlags_PassthruCentralNode
    );

    if (firstFrame)
    {
        SetUpDocking();
        firstFrame = false;
    }

    ImGui::End();
}

void PEditor::DrawEntitiesPanel()
{
    ImGui::Begin("Entities");

    PEngine& engine = PEngine::GetInstance();
    PRegistry& registry = PRegistry::GetInstance();

    // Boton para crear entidades
    if (Simulating == false) {
        if (ImGui::Button("+ Add Entity"))
        {
            ImGui::OpenPopup("Add Entity");
        }

        if (SelectedEntity != nullptr) {
            ImGui::SameLine();

            // Boton para eliminar entidades
            if (ImGui::Button("- Remove Entity"))
            {
                SelectedEntity->Destroy();
                SelectedEntity = nullptr;
            }
        }
    }

    // Datos de la entidad a crear
    static string selectedType;
    static char entityName[128] = "NewEntity";
    static bool creationFailed = false;

    const vector<string> types = registry.GetEntitiesTypes();

    // Popup de creacion.
    if (ImGui::BeginPopup("Add Entity"))
    {
        ImGui::Text("New entity");
        ImGui::Separator();

        if (types.empty())
        {
            ImGui::TextWrapped(
                "There are no registered entities you need to create one first"
            );
        }
        else
        {
            if (find(types.begin(), types.end(), selectedType) == types.end()) {
                selectedType = types.front();
            }

            // Selector de tipo
            if (ImGui::BeginCombo("Entity Type", selectedType.c_str()))
            {
                for (const auto& type : types)
                {
                    bool selected = (selectedType == type);

                    if (ImGui::Selectable(type.c_str(), selected))
                    {
                        selectedType = type;
                    }

                    if (selected) {
                        ImGui::SetItemDefaultFocus();
                    } 
                }

                ImGui::EndCombo();
            }

            // Nombre de la entidad
            ImGui::InputText("Entity Name", entityName, IM_ARRAYSIZE(entityName));

            if (ImGui::Button("Add"))
            {
                if (entityName[0] != '\0' &&
                    !selectedType.empty())
                {
                    Entity* newEntity = engine.AddRegisteredEntity(selectedType, entityName);

                    if (newEntity)
                    {
                        SelectedEntity = newEntity;
                        creationFailed = false;

                        entityName[0] = '\0';

                        ImGui::CloseCurrentPopup();
                    }
                    else
                    {
                        creationFailed = true;
                    }
                }
                else
                {
                    creationFailed = true;
                }
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel"))
            {
                creationFailed = false;
                ImGui::CloseCurrentPopup();
            }

            if (creationFailed)
            {
                ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "Entity name is invalid");
            }
        }

        ImGui::EndPopup();
    }

    ImGui::Separator();

    // Lista de entidades
    for (const auto& [name, entity] : engine.GetEntities())
    {
        if (dynamic_cast<EditorCamera*>(entity.get())) {
            continue;
        }

        bool selected = SelectedEntity == entity.get();

        if (ImGui::Selectable(name.c_str(), selected))
        {
            SelectedEntity = entity.get();
        }
    }

    ImGui::End();
}

void PEditor::DrawInspector()
{
    ImGui::Begin("Components");

    if (SelectedEntity) {
        ImGui::Text("%s", SelectedEntity->Name.c_str());
        Transform& t = SelectedEntity->GetEntityTransform();

        if (ImGui::CollapsingHeader(
            "Transform",
            ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat3(
                "Position",
                &t.translation.x,
                0.1f
            );

            ImGui::DragFloat3(
                "Rotation",
                &t.rotation.x,
                0.1f
            );

            ImGui::DragFloat3(
                "Scale",
                &t.scale.x,
                0.1f
            );
        }

        for (auto& component : SelectedEntity->GetComponents())
        {
            Component* c = component.get();
            ImGui::Text("%s", c->Name.c_str());
        }
    }

    ImGui::End();
}

void PEditor::DrawViewport() {
    ImGui::Begin("Viewport");

    // Simular
    if (ImGui::Button("Save scene")) {
        if (CurrentScene != "") {
            PSceneManager::GetInstance().SaveScene("temp");
        }
        else {
            ImGui::OpenPopup("Create scene");
        }
    }

    ImGui::SameLine();

    const vector<string> sceneNames = PSceneManager::GetInstance().GetAllSceneNames();
    // Detener simulación
    if (ImGui::Button("Load scene")) {
        if (sceneNames.size() <= 0) {
            ImGui::OpenPopup("No scenes");        
        }
        else {
            ImGui::OpenPopup("Select scene");
        }
    }

    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();

    // Simular
    if (Simulating == false) {
        if (ImGui::Button("-> Simulate")) {
            PSceneManager::GetInstance().SaveScene("temp");
            PEngine::GetInstance().PauseGame(false);
            PEngine::GetInstance().StartGame();
            Simulating = true;
        }
    }
    else {
        if (ImGui::Button("-> Continue")) {
            PEngine::GetInstance().PauseGame(false);
        }

        ImGui::SameLine();

        // Pausar simulación
        if (ImGui::Button("|| Pause")) {
            PEngine::GetInstance().PauseGame(true);
        }

        ImGui::SameLine();

        // Detener simulación
        if (ImGui::Button("[] Stop")) {
            SelectedEntity = nullptr;
            PSceneManager::GetInstance().LoadScene("temp");
            PEngine::GetInstance().PauseGame(true);
            Simulating = false;
        }
    }

    ImGui::Separator();

    ImVec2 viewportSize = ImGui::GetContentRegionAvail();

    MovingViewPort = ImGui::IsWindowHovered();

    rlImGuiImageRenderTexture(&ViewportTexture);

    // Popup para cuando no hay escenas
    if (ImGui::BeginPopup("No scenes"))
    {
        ImGui::Text("No scenes");
        ImGui::Separator();

        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "There are no scenes to load");

        if (ImGui::Button("Accept"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    static string selectedScene = "";

    // Popup de para seleccionar escenas
    if (ImGui::BeginPopup("Select scene"))
    {
        ImGui::Text("Select a scene");
        ImGui::Separator();

        if (find(sceneNames.begin(), sceneNames.end(), selectedScene) == sceneNames.end()) {
            selectedScene = sceneNames.front();
        }

        // Selector de escena
        if (ImGui::BeginCombo("Entity Type", selectedScene.c_str()))
        {
            for (const auto& scene : sceneNames)
            {
                bool selected = (selectedScene == scene);

                if (ImGui::Selectable(scene.c_str(), selected))
                {
                    selectedScene = scene;
                }

                if (selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }

        if (ImGui::Button("Select"))
        {
            if (selectedScene[0] != '\0')
            {
                PSceneManager::GetInstance().LoadScene(selectedScene);
                CurrentScene = selectedScene;
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    static char sceneName[128] = "NewScene";

    // Popup de para guardar scenas
    if (ImGui::BeginPopup("Create scene"))
    {
        ImGui::Text("New scene");
        ImGui::Separator();

        // Nombre de la escena
        ImGui::InputText("Scene Name", sceneName, IM_ARRAYSIZE(sceneName));

        if (ImGui::Button("Save"))
        {
            if (sceneName[0] != '\0')
            {
                vector<string> existingScenes = PSceneManager::GetInstance().GetAllSceneNames();
                if (find(existingScenes.begin(), existingScenes.end(), sceneName) != existingScenes.end() ||
                    sceneName == "temp") {
                    ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "The scene name is invalid");
                }
                else {
                    PSceneManager::GetInstance().SaveScene(sceneName);
                    CurrentScene = sceneName;
                    ImGui::CloseCurrentPopup();
                }
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::End();
}

void PEditor::CloseEditor() {
    SelectedEntity = nullptr;
    PRegistry::GetInstance().ClearRegistry();
    PEngine::GetInstance().ClearEntities();

    UnloadRenderTexture(ViewportTexture);
    rlImGuiShutdown();

    PSceneManager::GetInstance().DeleteScene("temp");
}