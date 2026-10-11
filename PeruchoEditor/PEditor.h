#pragma once

#include "Entity.h"

class PEditor
{
public:
    static PEditor& GetInstance();

    //Borro operandos que no quiero que se usen (cast, =)
    PEditor(const PEditor&) = delete;
    PEditor& operator=(const PEditor&) = delete;

    void Start();
    void Update();
    void Render();

    void CloseEditor();

    bool MovingViewPort = false;

private:
    PEditor() = default;
    ~PEditor() = default;

    void SetUpDocking();

    void DrawEditorDocks();
    void DrawEntitiesPanel();
    void DrawInspector();
    void DrawViewport();
    void RenderViewPort();

    bool firstFrame = true;

    RenderTexture2D ViewportTexture{};

    Entity* SelectedEntity = nullptr;

private:
    Entity* MainEditorCamera = nullptr;
};

