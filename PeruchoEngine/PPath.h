#pragma once

#include <filesystem>

namespace fs = std::filesystem;

class path;

class PPath
{
public:
    static PPath& GetInstance();

    //Borro operandos que no quiero que se usen (cast, =)
    PPath(const PPath&) = delete;
    PPath& operator=(const PPath&) = delete;

    fs::path GetAssetsPath();

private:
    PPath() = default;
    ~PPath() = default;
};

