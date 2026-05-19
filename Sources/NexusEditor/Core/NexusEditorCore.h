#pragma once

#if NX_EDITOR_DLL
	#define NX_EDITOR_API __declspec(dllexport)
#else
	#define NX_EDITOR_API __declspec(dllimport)
#endif

#include "NexusFramework/Core/NexusFramework.h"
#include "NexusEngine/Core/NexusEngine.h"
#include "NexusApp/Core/NexusApp.h"
