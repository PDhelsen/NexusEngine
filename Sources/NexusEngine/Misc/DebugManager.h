#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class DebugManager
	{
	public:
		NEXUS_ENGINE_API DebugManager(NxFr::StringView Folder, bool AutoStart = false);
		NEXUS_ENGINE_API ~DebugManager();

		NEXUS_ENGINE_API void Flush();

		NxFr::Logger* GetLogger() const { return Logger; }
		NxFr::Stats* GetStats() const { return Stats; };
		NxFr::Instruments* GetInstrumentor() const { return Instrumentor; }

	private:
		NxFr::Logger* Logger;
		NxFr::Stats* Stats;
		NxFr::Instruments* Instrumentor;
	};
}

