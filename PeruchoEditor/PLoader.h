#pragma once
class PLoader
{
public:
	PLoader() = default;
	~PLoader();

	bool LoadModule(const char* path);

private:
	void* DllHandler = nullptr;
};

