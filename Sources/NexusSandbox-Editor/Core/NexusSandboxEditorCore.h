#pragma once

#if NEXUS_SANDBOX_EDITOR_DLL
	#define NEXUS_SANDBOX_EDITOR_API __declspec(dllexport)
#else
	#define NEXUS_SANDBOX_EDITOR_API __declspec(dllimport)
#endif
