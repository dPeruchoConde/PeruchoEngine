#include "PEngine.h"
#include "PCamera.h"
#include "PRegistry.h"

PEngine& PEngine::GetInstance()
{
    static PEngine Instance;
    return Instance;
}

void PEngine::Start() {
    InitWindow(WindowWidth, WindowHeight, WindowName.c_str());

    SetTargetFPS(60);
}

void PEngine::StartGame() {
    for (const auto& [name, entity] : Entities) {
        if (Paused == false && entity->IsEditorOnly == false) {
            entity->Start();
        }
    }
}

void PEngine::Update() {
    float deltaTime = GetFrameTime();

    vector<string> entitiesToDestroy;
    for (const auto& [name, entity] : Entities) {
        if (Paused == false || entity->IsEditorOnly == true) {
            if (entity->ShouldDestroy()) {
                entitiesToDestroy.push_back(entity->Name);
            }
            else {
                entity->Update(deltaTime);
            }
        }
    }

    for (int i = 0; i < entitiesToDestroy.size(); i++) {
        Entities.at(entitiesToDestroy[i])->OnDestroy();
        RemoveEntity(entitiesToDestroy[i]);
    }
}

void PEngine::Render() {
    
    //BeginDrawing();
    ClearBackground(RAYWHITE);

    if (MainCamera) {
        BeginMode3D(*MainCamera);

        for (const auto& [name, entity] : Entities) {
            entity->Render(true);
        }

        if (Grid) {
            DrawGrid(100, 1.0f);
        }

        EndMode3D();

        for (const auto& [name, entity] : Entities) {
            entity->Render(false);
        }
    }
}

void PEngine::RenderGame() {
    BeginDrawing();
    Render();
    EndDrawing();
}

bool PEngine::WindowState() {
    return !WindowShouldClose();
}

void PEngine::RemoveEntity(string name) {
    Entities.erase(name);
}

void PEngine::ChangeMainCamera(Entity& newCamera) {
    if (PCamera* cameraEntity = newCamera.GetComponentByType<PCamera>()) {
        MainCamera = &cameraEntity->Camera;
    }
    EditorCamera = newCamera.IsEditorOnly;
}

const unordered_map<string, unique_ptr<Entity>>& PEngine::GetEntities() {
    return Entities;
}

void PEngine::Serialize(nlohmann::json& j) const {
    j["entities"] = nlohmann::json::array();

    for (const auto& [name, entity] : Entities) {
        if (entity->IsEditorOnly == false) {
            nlohmann::json entityJson;

            entity->Serialize(entityJson);

            j["entities"].push_back(entityJson);
        }
    }
}

void PEngine::Deserialize(const nlohmann::json& j) {
    for (const auto& entityJson : j["entities"])
    {
        std::string name = entityJson.at("name");
        std::string type = entityJson.at("type");

        if (unique_ptr<Entity> entity = PRegistry::GetInstance().CreateEntity(type, name)) {
            entity.get()->Deserialize(entityJson);
            Entities.emplace(name, std::move(entity));
        }
    }
}

void PEngine::ClearEntities() {
    for (auto it = Entities.begin(); it != Entities.end();) {
        if (!it->second->IsEditorOnly) {
            it = Entities.erase(it);
        }
        else {
            ++it;
        }
    }
    if (EditorCamera == false) {
        MainCamera = nullptr;
    }
    //Entities.clear();
    Cameras.clear();
}

Entity* PEngine::AddRegisteredEntity(string type, string name) {
    if (Entities.find(name) != Entities.end()) {
        return nullptr;
    }

    if (unique_ptr<Entity> e = PRegistry::GetInstance().CreateEntity(type, name)) {
        Entity* entityPtr = e.get();
        Entities.emplace(name, std::move(e));
        entityPtr->Start();
        return entityPtr;
    }

    return nullptr;
}

void PEngine::PauseGame(bool pause) {
    Paused = pause;
}