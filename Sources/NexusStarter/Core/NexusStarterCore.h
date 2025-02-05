#pragma once

#if NEXUS_STARTER_DLL
	#define NEXUS_STARTER_API __declspec(dllexport)
#else
	#define NEXUS_STARTER_API __declspec(dllimport)
#endif
