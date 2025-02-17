#pragma once

#if NEXUS_EDITOR_DLL
	#define NEXUS_EDITOR_API __declspec(dllexport)
#else
	#define NEXUS_EDITOR_API __declspec(dllimport)
#endif

#include "NexusFramework/Core/NexusFramework.h"
#include "NexusEngine/Core/NexusEngine.h"
#include "NexusApp/Core/NexusApp.h"
