#pragma once

#if NEXUS_APP_DLL
	#define NEXUS_APP_API __declspec(dllexport)
#else
	#define NEXUS_APP_API __declspec(dllimport)
#endif

#include "NexusFramework/Core/NexusFramework.h"
#include "NexusEngine/Core/NexusEngine.h"
