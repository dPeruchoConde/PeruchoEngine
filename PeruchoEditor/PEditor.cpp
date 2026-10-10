#include "PEditor.h"
#include "PEngine.h"
#include "rlImGui.h"
#include "imgui.h"
#include "imgui_internal.h"

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

    DrawEditorDocks();

    DrawEntitiesPanel();
    DrawInspector();

    RenderViewPort();
    DrawViewport();

    rlImGuiEnd();
}

void PEditor::RenderViewPort() {
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

    for (const auto& [name, entity] : engine.GetEntities())
    {
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

    ImVec2 viewportSize = ImGui::GetContentRegionAvail();

    MovingViewPort = ImGui::IsWindowHovered();

    rlImGuiImageRenderTexture(&ViewportTexture);

    ImGui::End();
}

void PEditor::CloseEditor() {
    UnloadRenderTexture(ViewportTexture);
    rlImGuiShutdown();
}