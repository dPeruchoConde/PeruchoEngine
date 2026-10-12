#pragma once
#include "ExportHeaders.h"
#include "Entity.h"
#include <unordered_map>

#define P_ENTITY(T, PNAME)                                      \
    namespace {                                                 \
        struct T##Registration                                  \
        {                                                       \
            T##Registration()                                   \
            {                                                   \
                PRegistry::GetInstance().RegisterEntity(        \
                    PNAME,                                      \
                    [](const std::string& name)                 \
                    {                                           \
                        return std::make_unique<T>(name);       \
                    });                                         \
            }                                                   \
        };                                                      \
        static T##Registration g_##T##Registration;             \
    }

using namespace std;

class PENGINE_API PRegistry
{
public:
	static PRegistry& GetInstance();

	//Elimino funciones no deseadas cast e =
	PRegistry(const PRegistry&) = delete;
	PRegistry& operator=(const PRegistry&) = delete;

	Component* CreateComponent(string type, string name, Entity& e);
    void RegisterEntity(string name, function<unique_ptr<Entity>(const string)> constructor);
	unique_ptr<Entity> CreateEntity(string type, string name);

    vector<string> GetEntitiesTypes() const;

    void ClearRegistry();

private:
	PRegistry();
	~PRegistry() = default;

	unordered_map<string, function<Component*(string, Entity&)>> ComponentTypes;

	unordered_map<string, function<unique_ptr<Entity>(const string)>> EntitiesContructors;
};