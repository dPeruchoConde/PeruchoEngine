#pragma once

#include "ExportHeaders.h"
#include <string>
#include <vector>

using namespace std;

class PENGINE_API PSceneManager
{
public:
    static PSceneManager& GetInstance();

    //Borro operandos que no quiero que se usen (cast, =)
    PSceneManager(const PSceneManager&) = delete;
    PSceneManager& operator=(const PSceneManager&) = delete;

    //En el futuro hay que montar un hilo para esto
    void LoadScene(string sceneName);

    void SaveScene(string sceneName);

    void DeleteScene(string sceneName);

    vector<string> GetAllSceneNames();

private:
    PSceneManager() = default;
    ~PSceneManager() = default;
};