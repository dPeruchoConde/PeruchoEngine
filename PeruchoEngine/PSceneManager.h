#pragma once

#include "ExportHeaders.h"
#include <string>

class PENGINE_API PSceneManager
{
public:
    static PSceneManager& GetInstance();

    //Borro operandos que no quiero que se usen (cast, =)
    PSceneManager(const PSceneManager&) = delete;
    PSceneManager& operator=(const PSceneManager&) = delete;

    //En el futuro hay que montar un hilo para esto
    void LoadScene(std::string sceneName);

    void SaveScene(std::string sceneName);

    void DeleteScene(std::string sceneName);

private:
    PSceneManager() = default;
    ~PSceneManager() = default;

    std::string CurrentScene = "";
};