#include "PMesh.h"
#include "Entity.h"
#include "rlgl.h"
#include "raymath.h"

PMesh::PMesh(string name, Entity& e, MeshShape shape, string route, string textureRoute, Color color) : Component(name, e) {
	Shape = shape;
	MeshRoute = route;
	MeshColor = color;
	TextureRoute = textureRoute;
	Render3D = true;

	PModel = LoadModel(MeshRoute.c_str());
	if (TextureRoute != "") {
		PTexture = LoadTexture(TextureRoute.c_str());
	}
	else {
		PTexture = { 0 };
	}
}

PMesh::~PMesh() {
	UnloadModel(PModel);
	if (TextureRoute != "") {
		UnloadTexture(PTexture);
	}
}

void PMesh::Render() {
	Vector3 position = RootEntity.GetEntityTransform().translation;
	Vector3 scale = RootEntity.GetEntityTransform().scale;
	Quaternion rotation = RootEntity.GetEntityTransform().rotation;

	rlPushMatrix();
	rlTranslatef(position.x, position.y, position.z);
	Matrix rotationMatrix = QuaternionToMatrix(rotation);
	rlMultMatrixf(MatrixToFloat(rotationMatrix));

	switch (Shape)
	{
	case MeshShape::CUBE:
		DrawCube(
			{ 0, 0, 0 },
			scale.x,
			scale.y,
			scale.z,
			MeshColor
		);
		DrawCubeWires(
			{ 0, 0, 0 },
			scale.x,
			scale.y,
			scale.z,
			BLACK
		);
		break;
	case MeshShape::SPHERE:
		DrawSphere(
			{ 0, 0, 0 },
			scale.x,
			MeshColor
		);
		break;
	case MeshShape::CAPSULE:
		DrawCapsule(
			{ 0, 0, 0 },
			{ 0, 0, 0 },
			scale.x,
			4,
			4,
			MeshColor
		);
		break;
	case MeshShape::COMPLEX_SHAPE:
		if (TextureRoute != "") {
			DrawModel(PModel, { 0, 0, 0 }, scale.x, WHITE);
			PModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = PTexture;
		}
		else {
			DrawModel(PModel, { 0, 0, 0 }, scale.x, MeshColor);
		}
		break;
	default:
		break;
	}

	rlPopMatrix();
}

string PMesh::GetType() const {
	return "PMesh";
}