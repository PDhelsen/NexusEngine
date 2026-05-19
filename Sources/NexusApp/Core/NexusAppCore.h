#pragma once

#if NX_APP_DLL
	#define NX_APP_API __declspec(dllexport)
#else
	#define NX_APP_API __declspec(dllimport)
#endif

#include "NexusFramework/Core/NexusFramework.h"
#include "NexusEngine/Core/NexusEngine.h"
