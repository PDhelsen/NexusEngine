#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class DebugManager
	{
		friend class Ticker;

	public :
		NEXUS_ENGINE_API static DebugManager* GetInstance();
		NEXUS_ENGINE_API static void Initialize();
		NEXUS_ENGINE_API static void Shutdown();

	private:
		void Flush();

	private:
		NxFr::Logger* Logger;
		NxFr::Stats* Stats;
		NxFr::Instruments* Instrumentor;
	};
}

