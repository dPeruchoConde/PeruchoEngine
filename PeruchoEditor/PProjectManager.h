#pragma once

class PProjectManager
{
public:
	static PProjectManager& GetInstance();

	//Borro operandos que no quiero que se usen (cast, =)
	PProjectManager(const PProjectManager&) = delete;
	PProjectManager& operator=(const PProjectManager&) = delete;

	bool RebuildGame();

private:
	PProjectManager() = default;
	~PProjectManager() = default;
};