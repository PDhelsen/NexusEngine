#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace EntryPoint
	{
		NX_ENGINE_API void ScheduleRestart();
		NX_ENGINE_API void SetErrorCode(int8 Code);
		NX_ENGINE_API int GetErrorCode();

		NX_ENGINE_API int Main(int argc, char* argv[]);
	}
}
