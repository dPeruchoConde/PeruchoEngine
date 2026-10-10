#include "PPath.h"
#include <filesystem>

PPath& PPath::GetInstance() {
	static PPath Instance;
	return Instance;
}

fs::path PPath::GetAssetsPath() {
    fs::path current = fs::current_path();

    while (!current.empty())
    {
        fs::path passets = current / "PGame/Assets";

        if (fs::exists(passets))
            return passets;

        fs::path parent = current.parent_path();

        if (parent == current)
            break;

        current = parent;
    }

    return {};
}