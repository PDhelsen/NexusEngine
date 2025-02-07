#pragma once

#if NEXUS_EDITOR_DLL
	#define NEXUS_EDITOR_API __declspec(dllexport)
#else
	#define NEXUS_EDITOR_API __declspec(dllimport)
#endif
