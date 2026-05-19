#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace EntryPoint
	{
		void ScheduleRestart();
		void SetErrorCode(int8 Code);
		int GetErrorCode();

		NX_ENGINE_API int Main(int argc, char* argv[]);
	}
}
