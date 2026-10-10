#pragma once
#include "ExportHeaders.h"
#include "Component.h"
#include "MeshShapes.h"
#include "raylib.h"

class PENGINE_API PMesh : public Component
{
public:
	PMesh(string name, Entity& e, MeshShape shape = MeshShape::COMPLEX_SHAPE, string route = "", string textureRoute = "", Color color = RED);
	~PMesh();

	void Render() override;

	string GetType() const override;

private:
	MeshShape Shape = MeshShape::COMPLEX_SHAPE;
	Color MeshColor = RED;
	string MeshRoute = "";
	string TextureRoute = "";
	Model PModel = { 0 };
	Texture2D PTexture = { 0 };
};

