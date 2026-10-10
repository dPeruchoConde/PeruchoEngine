#pragma once

#ifdef PENGINE_EXPORTS
#define PENGINE_API __declspec(dllexport)
#else
#define PENGINE_API __declspec(dllimport)
#endif