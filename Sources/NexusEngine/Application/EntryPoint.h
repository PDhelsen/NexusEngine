#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Application.h"

namespace NxEn
{
	namespace EntryPoint
	{
		NEXUS_ENGINE_API Application* CreateApplication(NxFr::StringView DllName);
		NEXUS_ENGINE_API void DestroyApplication(NxFr::StringView DllName, Application* Target);

		NEXUS_ENGINE_API int GetErrorCode();
		NEXUS_ENGINE_API void SetErrorCode(int8 Code);
		NEXUS_ENGINE_API bool ShouldRestart();
		NEXUS_ENGINE_API void ScheduleRestart();
		NEXUS_ENGINE_API void ResetRestart();
	}
}
