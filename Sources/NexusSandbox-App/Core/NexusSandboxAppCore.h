#pragma once

#if NEXUS_SANDBOX_APP_DLL
	#define NEXUS_SANDBOX_APP_API __declspec(dllexport)
#else
	#define NEXUS_SANDBOX_APP_API __declspec(dllimport)
#endif
