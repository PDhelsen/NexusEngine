#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class Bootstrapper
	{
	public:
		NEXUS_ENGINE_API Bootstrapper();
		NEXUS_ENGINE_API ~Bootstrapper();

		NEXUS_ENGINE_API void Boot();
		NEXUS_ENGINE_API void Unboot();

		NEXUS_ENGINE_API void AddStep(NxFr::Delegate<void()> Step);

	private:
		void ExecuteSteps();

		NxFr::Event<> Steps;
	};
}
