#pragma once

#include <string>

class PSceneManager
{
public:
    static PSceneManager& GetInstance();

    //Borro operandos que no quiero que se usen (cast, =)
    PSceneManager(const PSceneManager&) = delete;
    PSceneManager& operator=(const PSceneManager&) = delete;

    //En el futuro hay que montar un hilo para esto
    void LoadScene(std::string sceneName);

    void SaveScene();

private:
    PSceneManager() = default;
    ~PSceneManager() = default;

    std::string CurrentScene = "";
};