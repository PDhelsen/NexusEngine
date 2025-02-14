#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusFramework/Core/NexusFramework.h"

namespace NxEn
{
	namespace EntryPoint
	{
		NEXUS_ENGINE_API void Initialize();
		NEXUS_ENGINE_API void Shutdown();

		NEXUS_ENGINE_API int GetErrorCode();
		NEXUS_ENGINE_API void SetErrorCode(int8 Code);
		NEXUS_ENGINE_API bool ShouldRestart();
		NEXUS_ENGINE_API void ScheduleRestart();
		NEXUS_ENGINE_API void ResetRestart();
	}
}
