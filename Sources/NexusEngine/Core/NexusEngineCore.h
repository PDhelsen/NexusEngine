#pragma once

#if NEXUS_ENGINE_DLL
	#define NEXUS_ENGINE_API __declspec(dllexport)
#else
	#define NEXUS_ENGINE_API __declspec(dllimport)
#endif
