#pragma once
#include "ExportHeaders.h"
#include <vector>
#include <memory>
#include <raylib.h>
#include <unordered_map>
#include "Entity.h"

class PSceneManager;

class PENGINE_API PEngine
{
public:
	static PEngine& GetInstance();

	//Borro operandos que no quiero que se usen (cast, =)
	PEngine(const PEngine&) = delete;
	PEngine& operator=(const PEngine&) = delete;

	void Start();
	void Update();
	void Render();

	bool WindowState();

	void Serialize(nlohmann::json& j) const;
	void Deserialize(const nlohmann::json& j);

	void ChangeMainCamera(Entity &newCamera);

	const unordered_map<string, unique_ptr<Entity>>& GetEntities();

	template<typename T, typename... Args>
		requires derived_from<T, Entity>
	T* AddEntity(string name, Args&&... args)
	{
		if (Entities.find(name) != Entities.end()) {
			return nullptr;
		}

		auto e = std::make_unique<T>(
			name,
			std::forward<Args>(args)...
		);

		T* ePtr = e.get();

		Entities.emplace(name, std::move(e));

		ePtr->Start();

		return ePtr;
	}

	Entity* AddRegisteredEntity(string type, string name);

	void ClearEntities();

	bool Grid = false;
	string WindowName = "Perucho engine";
	int WindowHeight = 720;
	int WindowWidth = 1280;

private:
	friend class PSceneManager;

	PEngine() = default;
	~PEngine() = default;

	void RemoveEntity(string name);

	vector<Camera> Cameras;

	Camera3D* MainCamera = nullptr;

	//vector<unique_ptr<Entity>> Entities;

	unordered_map<string, unique_ptr<Entity>> Entities;
};

