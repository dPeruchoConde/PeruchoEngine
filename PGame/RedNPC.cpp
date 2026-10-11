#include "RedNPC.h"
#include "PMesh.h"

void RedNPC::Start() {
	AddComponent<PMesh>("Mesh", MeshShape::CUBE);
}