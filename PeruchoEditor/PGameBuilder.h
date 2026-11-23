#pragma once

#include <string>

class PGameBuilder
{
public:
	static bool BuildGame();

private:
	static std::string GetBuildCommand();
};