#include "PLoader.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

PLoader::~PLoader() {
	if (DllHandler) {
		FreeLibrary(static_cast<HMODULE>(DllHandler));
		DllHandler = nullptr;
	}
}

bool PLoader::LoadModule(const char* path) {
	if (DllHandler) {
		return true;
	}

	DllHandler = LoadLibraryA(path);
	if (!DllHandler) {
		DllHandler = nullptr;
		return false;
	}

	return true;
}