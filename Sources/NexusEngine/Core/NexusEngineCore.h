#pragma once

#if NX_ENGINE_DLL
	#define NX_ENGINE_API __declspec(dllexport)
#else
	#define NX_ENGINE_API __declspec(dllimport)
#endif

#include "NexusFramework/Core/NexusFramework.h"
