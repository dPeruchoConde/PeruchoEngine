#pragma once
class PLoader
{
public:
	static PLoader& GetInstance();

	//Borro operandos que no quiero que se usen (cast, =)
	PLoader(const PLoader&) = delete;
	PLoader& operator=(const PLoader&) = delete;

	bool LoadModule(const char* path);

	void UnloadModule();

private:
	PLoader() = default;
	~PLoader();

private:
	void* DllHandler = nullptr;
};

