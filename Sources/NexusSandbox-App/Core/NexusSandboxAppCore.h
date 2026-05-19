#pragma once

#if NX_SANDBOX_APP_DLL
	#define NX_SANDBOX_APP_API __declspec(dllexport)
#else
	#define NX_SANDBOX_APP_API __declspec(dllimport)
#endif

#include "NexusFramework/Core/NexusFramework.h"
#include "NexusEngine/Core/NexusEngine.h"
#include "NexusApp/Core/NexusApp.h"
