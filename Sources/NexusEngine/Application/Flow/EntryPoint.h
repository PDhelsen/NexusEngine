#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace EntryPoint
	{
		NX_ENGINE_API int Main(int argc, char* argv[]);

		void SetErrorCode(int32 Code);
		void ScheduleRestart();
	}
}
