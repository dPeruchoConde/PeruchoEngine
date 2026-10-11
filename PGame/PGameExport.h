#pragma once

#ifdef PGAME_EXPORTS
#define PGAME_API __declspec(dllexport)
#else
#define PGAME_API __declspec(dllimport)
#endif