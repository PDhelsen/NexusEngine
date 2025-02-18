#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace EntryPoint
	{
		NEXUS_ENGINE_API void ScheduleRestart();
		NEXUS_ENGINE_API void SetErrorCode(int8 Code);
		NEXUS_ENGINE_API int GetErrorCode();

		NEXUS_ENGINE_API int Main(int argc, char* argv[]);
	}
}
